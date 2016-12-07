#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pokemon.h"
#include "pmstrength.h"
#include "pmagility.h"
#include "pmshield.h"
#include "pmdefense.h"
#include "logindialog.h"
#include <QTime>

MainWindow::MainWindow(QString name,quint16 port,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    serverAddress("127.0.0.1"),
    userName(name),
    selfPort(port)
{
    ui->setupUi(this);

    QIcon icon(QPixmap(":/new/prefix1/image/pokemon_colored.png"));
    setWindowIcon(icon);


    battle=new Battle(name,port,allPM,this);
    connect(battle,SIGNAL(initMonsTable()),this,SLOT(initMonsTable()));
    connect(this,SIGNAL(sendPM(PokeMon*)),battle,SLOT(recvPM(PokeMon*)));

    socket=new QUdpSocket(this);
    socket->bind(selfPort);
    connect(socket,&QIODevice::readyRead,this,&MainWindow::on_readyRead);

    qsrand(QTime::currentTime().msec());
    A=new PMAgility(Epic);
//    A->gainExp(200);
    QString info_A=A->getInfomation();


    ui->labelPM_1->setText(info_A);

    ui->pushButton_Battle->setDisabled(true);


    initMonsTable();
}

MainWindow::~MainWindow()
{
    delete ui;
    /*log out*/
    QByteArray datagram;//datagram to send
    QDataStream outStream(&datagram,QIODevice::ReadWrite);
    datagramType type=EXIT;
    outStream<<type<<userName;
    qDebug()<<"EXIT";
    socket->writeDatagram(datagram,serverAddress,serverPort);

}

void MainWindow::initMonsTable()
{
    //每次选中整行
    ui->tableWidget_monsters->setSelectionBehavior(QAbstractItemView::SelectRows);
    //把选中模式设为单选，即每次只选中一行，而不能选中多行
    ui->tableWidget_monsters->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_monsters->horizontalHeader()->setStretchLastSection(true);


    QByteArray datagram;//datagram to send
    QDataStream outStream(&datagram,QIODevice::ReadWrite);
    datagramType type=GETSELFMONS;
    outStream<<type<<userName;
    socket->writeDatagram(datagram,serverAddress,serverPort);

}

void MainWindow::on_readyRead()
{
    QByteArray datagram;//received datagram
    datagram.resize(socket->pendingDatagramSize());
    QHostAddress senderAddr;
    socket->readDatagram(datagram.data(),datagram.size(),&senderAddr);
    QDataStream inStream(datagram);
    datagramType type;//type of this datagram
    inStream>>type;
    switch (type) {
    case GETSELFMONS:
        qDebug()<<"GETSELFMONS_";
        on_getSelfMons(inStream);
        break;
    default:
        break;
    }

}

void MainWindow::on_getSelfMons(QDataStream &inStream)
{
    int nCount;//number of mons
    inStream>>nCount;

    for(auto pm :allPM)
        delete pm;

    int advanceNum=0;

    allPM.clear();
    for(int i=0; i<nCount; i++)
    {
        PokeMon *tmpPM;
        inStream>>tmpPM;
        allPM.append(tmpPM);
        if(tmpPM->level==15)
            advanceNum++;
    }
    QImage *numBadge;
    if(nCount<=3)
        numBadge=new QImage(":/new/prefix1/image/Medal_Bronze.png");
    else if(nCount<=5)
        numBadge=new QImage(":/new/prefix1/image/Medal_Silver.png");
    else
        numBadge=new QImage(":/new/prefix1/image/Medal_Gold.png");
    ui->label_numBadge->setPixmap(QPixmap::fromImage(*numBadge));

    QImage *advancesBadge;
    if(advanceNum<=3)
        advancesBadge=new QImage(":/new/prefix1/image/Medal_Bronze.png");
    else if(advanceNum<=5)
        advancesBadge=new QImage(":/new/prefix1/image/Medal_Silver.png");
    else
        advancesBadge=new QImage(":/new/prefix1/image/Medal_Gold.png");
    ui->label_advancesBadge->setPixmap(QPixmap::fromImage(*advancesBadge));


    battle->refreshAllPM(allPM);
    refreshMonsTable();
}

void MainWindow::refreshMonsTable()
{
    ui->tableWidget_monsters->setSortingEnabled(false);

    int nCountRow=ui->tableWidget_monsters->rowCount();
    for(int i=0;i<nCountRow;i++)
        ui->tableWidget_monsters->removeRow(0);


    for (auto pm:allPM)
    {
        int nOldRowCount = ui->tableWidget_monsters->rowCount();
        ui->tableWidget_monsters->insertRow(nOldRowCount);
        QTableWidgetItem *name = new QTableWidgetItem(pm->name);
        ui->tableWidget_monsters->setItem(nOldRowCount, 0, name);
        QTableWidgetItem *level = new QTableWidgetItem(QString::number(pm->level));
        ui->tableWidget_monsters->setItem(nOldRowCount, 1, level);
        QTableWidgetItem *type = new QTableWidgetItem(PMType_toString[pm->type]);
        ui->tableWidget_monsters->setItem(nOldRowCount, 2, type);
        QTableWidgetItem *rarity = new QTableWidgetItem(PMRarity_toString[pm->rarity]);
        ui->tableWidget_monsters->setItem(nOldRowCount, 3, rarity);
        switch (pm->rarity) {
        case Common:
            rarity->setForeground(QBrush(QColor("gray")));
            break;
        case Rare:
            rarity->setForeground(QBrush(QColor("skyblue")));
            break;
        case Epic:
            rarity->setForeground(QBrush(QColor("purple")));
            break;
        case Legendary:
            rarity->setForeground(QBrush(QColor("gold")));
            break;
        default:
            break;
        }
        name->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        level->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        type->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        rarity->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    }
    ui->tableWidget_monsters->setColumnWidth(1,60);
    ui->tableWidget_monsters->setColumnWidth(2,80);

    ui->tableWidget_monsters->setSortingEnabled(true);
    ui->tableWidget_monsters->sortByColumn(0, Qt::AscendingOrder);
}


QDataStream &operator>>(QDataStream &in, PokeMon *&pm)
{
    int type;
    in>>type;
    switch (type) {
    case Strength:
        pm=new PMStrength();
        break;
    case Defense:
        pm=new PMDefense();
        break;
    case Shield:
        pm=new PMShield();
        break;
    case Agility:
        pm=new PMAgility();
        break;
    default:
        break;
    }
    pm->type=(PMType)type;
    in>>pm->name>>pm->level>>pm->attack>>pm->defence>>pm->maxHealth>>pm->speed>>pm->exp;
    int rarity,limitBreak;
    in>>rarity>>limitBreak;
    pm->rarity=(PMRarity)rarity;
    pm->limitBreak=(LimitBreak)limitBreak;
    in>>pm->id;
    return in;
}

QDataStream &operator<<(QDataStream &out, const PokeMon *&pm)
{
    out<<(int)pm->type<<pm->name<<pm->level<<pm->attack<<pm->defence<<pm->maxHealth
      <<pm->speed<<pm->exp<<(int)pm->rarity<<(int)pm->limitBreak<<pm->id;
    qDebug()<<"qwer";
    return out;
}

void MainWindow::on_tableWidget_monsters_currentCellChanged(int currentRow, int currentColumn,
                                                            int previousRow, int previousColumn)
{
    ui->pushButton_Battle->setEnabled(true);
    if(currentRow==-1)
        return;
    QString name = ui->tableWidget_monsters->item(currentRow,0)->text();
    for(auto pm:allPM)
    {
        if(name==pm->name)
        {
            QString info=pm->getInfomation();
            ui->labelPM_1->setText(info);
            break;
        }
    }
}

void MainWindow::on_pushButton_allUsers_clicked()
{
    emit openUsers();
}

void MainWindow::on_pushButton_Battle_clicked()
{
    battle->show();
    int curRow=ui->tableWidget_monsters->currentRow();
    if(curRow==-1)
        curRow=0;
    QString name = ui->tableWidget_monsters->item(curRow,0)->text();
    PokeMon * self;
    for(auto pm:allPM)
    {
        if(name==pm->name)
        {
            self=pm;
            break;
        }
    }
    emit sendPM(self);
}
