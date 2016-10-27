#include "users.h"
#include "ui_users.h"
#include "logindialog.h"

Users::Users(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Users),
    serverAddress("127.0.0.1")
{
    ui->setupUi(this);
    socket = new QUdpSocket(this);
    connect(socket,&QIODevice::readyRead,this,&Users::on_readyRead);

    while(!socket->bind(selfPort))
    {
        selfPort++;
    }
    initUsersTable();
    initMonsTable();
}

Users::~Users()
{
    delete ui;
}

void Users::initUsersTable()
{
    //每次选中整行
    ui->tableWidget_users->setSelectionBehavior(QAbstractItemView::SelectRows);
    //把选中模式设为单选，即每次只选中一行，而不能选中多行
    ui->tableWidget_users->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_users->setSortingEnabled(true);
    ui->tableWidget_users->sortByColumn(0, Qt::AscendingOrder);
    ui->tableWidget_users->horizontalHeader()->setStretchLastSection(true);

    refreshTableUsers();

}

void Users::initMonsTable()
{
    //每次选中整行
    ui->tableWidget_monsters->setSelectionBehavior(QAbstractItemView::SelectRows);
    //把选中模式设为单选，即每次只选中一行，而不能选中多行
    ui->tableWidget_monsters->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_monsters->setSortingEnabled(true);
    ui->tableWidget_monsters->sortByColumn(0, Qt::AscendingOrder);
    ui->tableWidget_monsters->setColumnWidth(1,60);
    ui->tableWidget_monsters->setColumnWidth(3,80);
    ui->tableWidget_monsters->setColumnWidth(4,80);
    ui->tableWidget_monsters->setColumnWidth(5,80);
    ui->tableWidget_monsters->setColumnWidth(6,80);
    ui->tableWidget_monsters->setColumnWidth(7,80);
    ui->tableWidget_monsters->horizontalHeader()->setStretchLastSection(true);

}

void Users::on_getUsers(QDataStream &inStream)
{
    int nCount;//number of users
    inStream>>nCount;

    for(auto user :users)
    {
        ui->tableWidget_users->removeRow(0);
    }
    users.clear();

    ui->tableWidget_users->setSortingEnabled(false);
    for (int i=0;i<nCount;i++)
    {
        QTableWidgetItem *user=new QTableWidgetItem;
        inStream>>*user;//not sure it is of this form?
        users.append(user);

        int nOldRowCount = ui->tableWidget_users->rowCount();
        ui->tableWidget_users->insertRow(nOldRowCount);
        ui->tableWidget_users->setItem(nOldRowCount, 0, user);

    }

    ui->tableWidget_users->setSortingEnabled(true);
    ui->tableWidget_users->sortByColumn(0, Qt::AscendingOrder);
}

void Users::refreshTableUsers()
{
    QByteArray datagram;//datagram to send
    QDataStream outStream(&datagram,QIODevice::ReadWrite);
    datagramType type=GETUSERS;
    outStream<<type<<selfPort;
    socket->writeDatagram(datagram,serverAddress,serverPort);

}

void Users::on_OpenUsers()
{
    this->show();
}

void Users::on_readyRead()
{
    QByteArray datagram;//received datagram
    datagram.resize(socket->pendingDatagramSize());
    QHostAddress senderAddr;//not used yet
    socket->readDatagram(datagram.data(),datagram.size(),&senderAddr);
    QDataStream inStream(datagram);
    datagramType type;//type of this datagram
    inStream>>type;
    switch (type) {
    case GETUSERS:
        qDebug()<<"GETUSERS";
        on_getUsers(inStream);
        break;
    default:
        break;
    }

}

void Users::on_pushButton_refresh_clicked()
{
    refreshTableUsers();
}
