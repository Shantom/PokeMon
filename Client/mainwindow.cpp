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

    battle=new Battle(this);

    socket=new QUdpSocket(this);
    socket->bind(selfPort);
    connect(socket,&QIODevice::readyRead,this,&MainWindow::on_readyRead);

    qsrand(QTime::currentTime().msec());
    A=new PMAgility(Epic);
//    A->gainExp(200);
    QString info_A=A->getInfomation();


    ui->labelPM_1->setText(info_A);
    ui->comboBox_Rarity_1->setCurrentIndex(0);
    ui->comboBox_Type_1->setCurrentIndex(0);

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

    allPM.clear();
    for(int i=0; i<nCount; i++)
    {
        PokeMon *tmpPM;
        inStream>>tmpPM;
        allPM.append(tmpPM);
    }
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

        ui->tableWidget_monsters->setColumnWidth(1,60);
        ui->tableWidget_monsters->setColumnWidth(2,80);

        ui->tableWidget_monsters->setSortingEnabled(true);
        ui->tableWidget_monsters->sortByColumn(0, Qt::AscendingOrder);
    }
}

void MainWindow::on_pushButton_LvUp_1_clicked()
{
    A->gainExp(expToLvUp[A->level]-expToLvUp[A->level-1]);

    if(A->level==15)
        ui->pushButton_LvUp_1->setEnabled(false);

    QString info_A=A->getInfomation();
    ui->labelPM_1->setText(info_A);
}

void MainWindow::on_pushButton_Attack_1_clicked()
{
    int movement=A->move();
    QString moveInfo;
    if(movement==ordAttack)
        moveInfo+=tr("PokeMon A gives a hit on B.");
    else
        moveInfo+= tr("PokeMon A uses his limitbreak ")+
                LimitBreak_toString[movement]+".";
    ui->label_moveInfo_1->setText(moveInfo);
}

void MainWindow::on_pushButton_LvMax_1_clicked()
{
    A->gainExp(expToLvUp[14]);
    ui->pushButton_LvMax_1->setEnabled(false);
    ui->pushButton_LvUp_1->setEnabled(false);
    QString info_A=A->getInfomation();
    ui->labelPM_1->setText(info_A);

}

void MainWindow::on_pushButton_Create_1_clicked()
{
    delete A;
    PMType type=(PMType)ui->comboBox_Type_1->currentIndex();
    PMRarity rarity=(PMRarity)ui->comboBox_Rarity_1->currentIndex();
    switch (type) {
    case Strength:
        A=new PMStrength(rarity);
        break;
    case Defense:
        A=new PMDefense(rarity);
        break;
    case Shield:
        A=new PMShield(rarity);
        break;
    case Agility:
        A=new PMAgility(rarity);
        break;
    default:
        break;
    }
    QString info_A=A->getInfomation();
    ui->labelPM_1->setText(info_A);

    ui->pushButton_LvMax_1->setEnabled(true);
    ui->pushButton_LvUp_1->setEnabled(true);
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
    return in;
}

QDataStream &operator<<(QDataStream &out, const PokeMon *&pm)
{
    out<<(int)pm->type<<pm->name<<pm->level<<pm->attack<<pm->defence<<pm->maxHealth
      <<pm->speed<<pm->exp<<(int)pm->rarity<<(int)pm->limitBreak;
    return out;
}

void MainWindow::on_tableWidget_monsters_currentCellChanged(int currentRow, int currentColumn,
                                                            int previousRow, int previousColumn)
{
    QString name = ui->tableWidget_monsters->item(currentRow,0)->text();
    for(auto pm:allPM)
    {
        if(name==pm->name)
        {
            QString info=pm->getInfomation();
            ui->labelPM_1->setText(info);
            ui->pushButton_LvMax_1->setEnabled(false);
            ui->pushButton_LvUp_1->setEnabled(false);
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
}
