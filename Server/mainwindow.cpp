#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>
#include "pokemon.h"
#include "pmstrength.h"
#include "pmagility.h"
#include "pmshield.h"
#include "pmdefense.h"
#include "database.h"
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    database=new Database();


    server=new QUdpSocket(this);
    connect(server,SIGNAL(readyRead()),this,SLOT(on_readyread()));
    server->bind(serverPort);

    qsrand(QTime::currentTime().msec());
    A=new PMAgility(Epic);
//    A->gainExp(200);
    QString info_A=A->getInfomation();
    ui->labelPM_1->setText(info_A);
    ui->comboBox_Rarity_1->setCurrentIndex(0);
    ui->comboBox_Type_1->setCurrentIndex(0);

    //每次选中整行
    ui->tableWidget_users->setSelectionBehavior(QAbstractItemView::SelectRows);
    //为方便删除按钮操作，把选中模式设为单选，即每次只选中一行，而不能选中多行
    ui->tableWidget_users->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_users->setSortingEnabled(true);
    ui->tableWidget_users->sortByColumn(0, Qt::AscendingOrder);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLogin(QHostAddress senderAddr, quint16 port, QString name)
{
    int nOldRowCount = ui->tableWidget_users->rowCount();
    ui->tableWidget_users->insertRow(nOldRowCount);

    ui->tableWidget_users->setSortingEnabled(false);

    QTableWidgetItem *ipAddr = new QTableWidgetItem
            (QHostAddress(senderAddr.toIPv4Address()).toString());
    ipAddr->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    ui->tableWidget_users->setItem(nOldRowCount, 0, ipAddr);
    QTableWidgetItem *userName = new QTableWidgetItem(name);
    userName->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    ui->tableWidget_users->setItem(nOldRowCount, 1, userName);
    QTableWidgetItem *userPort = new QTableWidgetItem(QString("%1").arg(port));
    userPort->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    ui->tableWidget_users->setItem(nOldRowCount, 2, userPort);

    ui->tableWidget_users->setSortingEnabled(true);
    ui->tableWidget_users->sortByColumn(0, Qt::AscendingOrder);
    ui->tableWidget_users->horizontalHeader()->setStretchLastSection(true);

    ui->tableWidget_users->setCurrentItem(ipAddr);
    ui->tableWidget_users->scrollToItem(ipAddr);

}

void MainWindow::onLogout(QString name)
{
    QList<QTableWidgetItem*>itemsMatched=
            ui->tableWidget_users->findItems(name,Qt::MatchExactly);
    if(!itemsMatched.isEmpty())
    {
        ui->tableWidget_users->removeRow(ui->tableWidget_users->row(itemsMatched[0]));
    }
}

QDataStream &operator>>(QDataStream &in, datagramType &type)
{
    int number=0;
    in>>number;
    type=(datagramType)number;
    return in;
}

QDataStream &operator<<(QDataStream &out, const datagramType type)
{
    out<<(int)type;
    return out;
}

void MainWindow::on_readyread()
{
    QByteArray datagram;//received datagram
    datagram.resize(server->pendingDatagramSize());
    QHostAddress senderAddr;
    server->readDatagram(datagram.data(),datagram.size(),&senderAddr);
    QString name,word;//sender's name and word
    QDataStream inStream(datagram);
    quint16 port;//port of this client
    datagramType type;//type of this datagram
    inStream>>type;

    QString msg;
    QByteArray msgba;//message to send
    QDataStream outStream(&msgba,QIODevice::ReadWrite);

    qDebug()<<type<<senderAddr;
    switch (type) {
    case LOGIN:
    {
        inStream>>port>>name>>word;
        qDebug()<<port<<name<<word;
        bool isOk=database->login(name,word);
        if(isOk)
        {
            msg="Yes";
            onLogin(senderAddr,port,name);
        }
        else
            msg="whatever";
        outStream<<msg;
        server->writeDatagram(msgba,senderAddr,port);
    }
        break;
    case SIGNUP:
    {
        inStream>>port>>name>>word;
        qDebug()<<port<<name<<word;
        bool isOk=database->signup(name,word);
        if(isOk)
            msg="Yes";
        else
            msg="whatever";
        outStream<<msg;
        server->writeDatagram(msgba,senderAddr,port);
    }
        break;
    case EXIT:
    {
        inStream>>name;
        qDebug()<<name;
        onLogout(name);
    }
    default:
        break;
    }

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

void MainWindow::on_pushButton_LvUp_1_clicked()
{
    A->gainExp(expToLvUp[A->level]-expToLvUp[A->level-1]);

    if(A->level==15)
        ui->pushButton_LvUp_1->setEnabled(false);

    QString info_A=A->getInfomation();
    ui->labelPM_1->setText(info_A);
}

