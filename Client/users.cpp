#include "users.h"
#include "ui_users.h"
#include "logindialog.h"
#include "mainwindow.h"
#include <QMessageBox>

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
        user->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    }

    ui->tableWidget_users->setSortingEnabled(true);
    ui->tableWidget_users->sortByColumn(0, Qt::AscendingOrder);
}

void Users::on_getCertainMons(QDataStream &inStream)
{
    int nCount;//number of users
    inStream>>nCount;

    for(auto pm:allPM)
    {
        delete pm;
    }
    allPM.clear();

    for(int i=0;i<nCount;i++)
    {
        PokeMon * tmpPM;
        inStream>>tmpPM;
        allPM.append(tmpPM);
    }
    refreshTableMons();

}

void Users::refreshTableUsers()
{
    QByteArray datagram;//datagram to send
    QDataStream outStream(&datagram,QIODevice::ReadWrite);
    datagramType type=GETUSERS;
    outStream<<type<<selfPort;
    socket->writeDatagram(datagram,serverAddress,serverPort);

}

void Users::refreshTableMons()
{
    int nCountRow=ui->tableWidget_monsters->rowCount();
    for(int i=0;i<nCountRow;i++)
    {
        ui->tableWidget_monsters->removeRow(0);
    }

    ui->tableWidget_monsters->setSortingEnabled(false);

    for (auto pm:allPM)
    {
        int nOldRowCount = ui->tableWidget_monsters->rowCount();
        ui->tableWidget_monsters->insertRow(nOldRowCount);
        QTableWidgetItem *name = new QTableWidgetItem(pm->name);
        QTableWidgetItem *level = new QTableWidgetItem(QString::number(pm->level));
        QTableWidgetItem *type = new QTableWidgetItem(PMType_toString[pm->type]);
        QTableWidgetItem *attack = new QTableWidgetItem(QString::number(pm->attack));
        QTableWidgetItem *defence = new QTableWidgetItem(QString::number(pm->defence));
        QTableWidgetItem *maxHealth = new QTableWidgetItem(QString::number(pm->maxHealth));
        QTableWidgetItem *speed = new QTableWidgetItem(QString::number(pm->speed));
        QTableWidgetItem *exp = new QTableWidgetItem(QString::number(pm->exp));
        QTableWidgetItem *limitBreak = new QTableWidgetItem(LimitBreak_toString[pm->limitBreak]);
        QTableWidgetItem *rarity = new QTableWidgetItem(PMRarity_toString[pm->rarity]);
        ui->tableWidget_monsters->setItem(nOldRowCount, 0, name);
        ui->tableWidget_monsters->setItem(nOldRowCount, 1, level);
        ui->tableWidget_monsters->setItem(nOldRowCount, 2, type);
        ui->tableWidget_monsters->setItem(nOldRowCount, 3, attack);
        ui->tableWidget_monsters->setItem(nOldRowCount, 4, defence);
        ui->tableWidget_monsters->setItem(nOldRowCount, 5, maxHealth);
        ui->tableWidget_monsters->setItem(nOldRowCount, 6, speed);
        ui->tableWidget_monsters->setItem(nOldRowCount, 7, exp);
        ui->tableWidget_monsters->setItem(nOldRowCount, 8, limitBreak);
        ui->tableWidget_monsters->setItem(nOldRowCount, 9, rarity);
        name->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        level->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        type->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        attack->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        defence->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        maxHealth->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        speed->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        exp->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        limitBreak->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        rarity->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
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
    }

    ui->tableWidget_monsters->setSortingEnabled(true);
    ui->tableWidget_monsters->sortByColumn(0, Qt::AscendingOrder);


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
        qDebug()<<"GETUSERS_";
        on_getUsers(inStream);
        break;
    case GETCERTAINMONS:
        qDebug()<<"GETCERTAINMONS_";
        on_getCertainMons(inStream);
        break;
    default:
        break;
    }

}

void Users::on_pushButton_refresh_clicked()
{
    refreshTableUsers();
}


void Users::on_pushButton_showPM_clicked()
{
    int current=ui->tableWidget_users->currentRow();

    if(current!=-1)
    {
        QString name=ui->tableWidget_users->item(current,0)->text();
        refreshTableUsers();
        if(socket->waitForReadyRead(500))
        {
            QList<QTableWidgetItem*>itemsMatched=
                    ui->tableWidget_users->findItems(name,Qt::MatchExactly);
            if(!itemsMatched.isEmpty())
            {
                QByteArray datagram;//datagram to send
                QDataStream outStream(&datagram,QIODevice::ReadWrite);
                datagramType type=GETCERTAINMONS;
                outStream<<type<<selfPort<<name;
                socket->writeDatagram(datagram,serverAddress,serverPort);
            }
        }
        else
            QMessageBox::warning(this,"警告","连接超时");
    }

}

void Users::on_tableWidget_users_doubleClicked(const QModelIndex &index)
{
    on_pushButton_showPM_clicked();
}
