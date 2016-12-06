#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>
#include <QMessageBox>
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
    connect(server,&QIODevice::readyRead,this,&MainWindow::on_readyRead);
    server->bind(serverPort);

    qsrand(QTime::currentTime().msec());
    A=new PMAgility(Epic);
//    A->gainExp(200);
    QString info_A=A->getInfomation();
    ui->labelPM_1->setText(info_A);
    ui->comboBox_Rarity_1->setCurrentIndex(0);
    ui->comboBox_Type_1->setCurrentIndex(0);

    initUsersTable();
    initMonsTable();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUsersTable()
{
    //每次选中整行
    ui->tableWidget_users->setSelectionBehavior(QAbstractItemView::SelectRows);
    //把选中模式设为单选，即每次只选中一行，而不能选中多行
    ui->tableWidget_users->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_users->setSortingEnabled(true);
    ui->tableWidget_users->sortByColumn(0, Qt::AscendingOrder);
    ui->tableWidget_users->horizontalHeader()->setStretchLastSection(true);

    int nOldRowCount = ui->tableWidget_users->rowCount();
    ui->tableWidget_users->setSortingEnabled(false);

    QStringList allUsers=database->allUsers();
    for(QString& user:allUsers)
    {
        ui->tableWidget_users->insertRow(nOldRowCount);
        QTableWidgetItem *userName = new QTableWidgetItem(user);
        userName->setForeground(QBrush(QColor("gray")));
        ui->tableWidget_users->setItem(nOldRowCount, 0, userName);
        QTableWidgetItem *ipAddr = new QTableWidgetItem("-");
        ipAddr->setForeground(QBrush(QColor("gray")));
        ui->tableWidget_users->setItem(nOldRowCount, 1, ipAddr);
        QTableWidgetItem *userPort = new QTableWidgetItem("-");
        userPort->setForeground(QBrush(QColor("gray")));
        ui->tableWidget_users->setItem(nOldRowCount, 2, userPort);
        userName->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        ipAddr->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        userPort->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    }
    ui->tableWidget_users->setSortingEnabled(true);
    ui->tableWidget_users->sortByColumn(0, Qt::AscendingOrder);
}

void MainWindow::initMonsTable()
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

PokeMon *MainWindow::getRandomPM()
{
    PokeMon *newPM;
    int roll_r=qrand()%100;
    PMRarity rarity;
    if(roll_r>90)
        rarity=Legendary;
    else if(roll_r>70)
        rarity=Epic;
    else if(roll_r>40)
        rarity=Rare;
    else
        rarity=Common;

    PMType type=(PMType)(roll_r%4);
    switch (type) {
    case Strength:
        newPM=new PMStrength(rarity);
        break;
    case Defense:
        newPM=new PMDefense(rarity);
        break;
    case Shield:
        newPM=new PMShield(rarity);
        break;
    case Agility:
        newPM=new PMAgility(rarity);
        break;
    default:
        break;
    }
    return newPM;

}

void MainWindow::onLogin(QHostAddress senderAddr, quint16 port, QString name)
{

    QList<QTableWidgetItem*>itemsMatched=
            ui->tableWidget_users->findItems(name,Qt::MatchExactly);
    if(!itemsMatched.isEmpty())
    {
        int loginRow = ui->tableWidget_users->row(itemsMatched[0]);

        QTableWidgetItem *userName = ui->tableWidget_users->item(loginRow,0);
        userName->setForeground(QBrush(QColor("white")));

        QTableWidgetItem *ipAddr = ui->tableWidget_users->item(loginRow,1);
        ipAddr->setText(QHostAddress(senderAddr.toIPv4Address()).toString());
        ipAddr->setForeground(QBrush(QColor("white")));

        QTableWidgetItem *userPort = ui->tableWidget_users->item(loginRow,2);
        userPort->setText(QString("%1").arg(port));
        userPort->setForeground(QBrush(QColor("white")));

        ui->tableWidget_users->setSortingEnabled(true);
        ui->tableWidget_users->sortByColumn(0, Qt::AscendingOrder);

        ui->tableWidget_users->setCurrentItem(ipAddr);
        ui->tableWidget_users->scrollToItem(ipAddr);
    }
    else
    {
        QMessageBox::critical(this,"error","can't find user "+name);
    }

}

void MainWindow::onLogout(QString name)
{
    QList<QTableWidgetItem*>itemsMatched=
            ui->tableWidget_users->findItems(name,Qt::MatchExactly);
    if(!itemsMatched.isEmpty())
    {
        int logoutRow = ui->tableWidget_users->row(itemsMatched[0]);
        QTableWidgetItem *userName = ui->tableWidget_users->item(logoutRow,0);
        userName->setForeground(QBrush(QColor("gray")));

        QTableWidgetItem *ipAddr = ui->tableWidget_users->item(logoutRow,1);
        ipAddr->setText("-");
        ipAddr->setForeground(QBrush(QColor("gray")));

        QTableWidgetItem *userPort = ui->tableWidget_users->item(logoutRow,2);
        userPort->setText("-");
        userPort->setForeground(QBrush(QColor("gray")));
    }
}

void MainWindow::onSignup(QString name)
{
    ui->tableWidget_users->setSortingEnabled(false);

    int nOldRowCount = ui->tableWidget_users->rowCount();
    ui->tableWidget_users->insertRow(nOldRowCount);
    QTableWidgetItem *userName = new QTableWidgetItem(name);
    userName->setForeground(QBrush(QColor("gray")));
    ui->tableWidget_users->setItem(nOldRowCount, 0, userName);
    QTableWidgetItem *ipAddr = new QTableWidgetItem("-");
    ipAddr->setForeground(QBrush(QColor("gray")));
    ui->tableWidget_users->setItem(nOldRowCount, 1, ipAddr);
    QTableWidgetItem *userPort = new QTableWidgetItem("-");
    userPort->setForeground(QBrush(QColor("gray")));
    ui->tableWidget_users->setItem(nOldRowCount, 2, userPort);

    userName->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    ipAddr->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    userPort->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    ui->tableWidget_users->setSortingEnabled(true);
    ui->tableWidget_users->sortByColumn(0, Qt::AscendingOrder);

    /*send every online users a datagram to tell them someone has just signed up*/

    /*randomly create 3 PM to a new user*/
    for(int i=0;i<3;i++)
    {
        qsrand(QTime::currentTime().msec()+i*i);
        PokeMon *newPM=getRandomPM();
        newPM->setName(name+QString("_%1").arg(i));
        database->addPokeMon(name,newPM);
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

void MainWindow::on_readyRead()
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

    switch (type) {
    case LOGIN:
    {
        inStream>>port>>name>>word;
        qDebug()<<port<<name<<word<<"login";
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
        qDebug()<<port<<name<<word<<"signup";
        bool isOk=database->signup(name,word);
        if(isOk)
        {
            msg="Yes";
            onSignup(name);
        }
        else
            msg="whatever";
        outStream<<msg;
        server->writeDatagram(msgba,senderAddr,port);
    }
        break;
    case EXIT:
    {
        inStream>>name;
        qDebug()<<name<<"logout";
        onLogout(name);
    }
        break;
    case GETSELFMONS:
    {
        inStream>>name;
        qDebug()<<name<<"GETSELFMONS";
        on_getSelfMons(name);
    }
        break;
    case GETUSERS:
    {
        inStream>>port;
        qDebug()<<port<<"GETUSERS";
        on_getUsers(port,senderAddr);
    }
        break;
    case GETCERTAINMONS:
    {
        inStream>>port>>name;
        qDebug()<<port<<name<<"GETCERTAINMONS";
        on_getCertainMons(port,name,senderAddr);
    }
        break;
    case GETOPPONENT:
    {
        int level;
        int tmpRarity;

        inStream>>port>>name>>level>>tmpRarity;
        qDebug()<<"GETOPPONENT";
        on_getOpponent(port,name,level,(PMRarity)tmpRarity,senderAddr);
        break;
    }
    case SENDRESULT:
    {
        qDebug()<<"SENDRESULT";
        on_sendResult(inStream);
        break;
    }
    case GETTROPHY:
    {
        qDebug()<<name<<"GETTROPHY";
        on_getTrophy(inStream);
        break;
    }
    case KILLPM:
    {
        qDebug()<<"KILLPM";
        on_killPM(inStream);
        break;
    }

    default:
        break;
    }

}

void MainWindow::on_getSelfMons(QString name)
{
    QByteArray msgba;//message to send
    QDataStream outStream(&msgba,QIODevice::ReadWrite);

    datagramType type=GETSELFMONS;//type of this datagram
    outStream<<type;
    QList<QTableWidgetItem*>itemsMatched=
            ui->tableWidget_users->findItems(name,Qt::MatchExactly);
    if(!itemsMatched.isEmpty())
    {
        int row=itemsMatched[0]->row();
        int port=ui->tableWidget_users->item(row,2)->text().toInt();
        QHostAddress senderAddr(ui->tableWidget_users->item(row,1)->text());
        QList<PokeMon *> allPM=database->pmsOfUser(name);
        outStream<<allPM.length();
        for(PokeMon * pm:allPM)
        {
            outStream<<pm;
            delete pm;
        }
        server->writeDatagram(msgba,senderAddr,port);
    }
}

void MainWindow::on_getUsers(quint16 port,QHostAddress &senderAddr)
{
    QByteArray msgba;//message to send
    QDataStream outStream(&msgba,QIODevice::ReadWrite);

    datagramType type=GETUSERS;//type of this datagram
    outStream<<type;
    int nCountRow=ui->tableWidget_users->rowCount();
    outStream<<nCountRow;
    for(int i=0;i<nCountRow;i++)
    {
        QTableWidgetItem *user=ui->tableWidget_users->item(i,0);
        outStream<<*user;
    }
    server->writeDatagram(msgba,senderAddr,port);
}

void MainWindow::on_getCertainMons(quint16 port, QString name, QHostAddress &senderAddr)
{
    QByteArray msgba;//message to send
    QDataStream outStream(&msgba,QIODevice::ReadWrite);

    datagramType type=GETCERTAINMONS;//type of this datagram
    outStream<<type;
    auto allPM=database->pmsOfUser(name);
    outStream<<allPM.length();
    for(auto pm:allPM)
    {
        outStream<<pm;
        delete pm;
    }

    server->writeDatagram(msgba,senderAddr,port);
}

void MainWindow::on_getOpponent(quint16 port, QString name, int level,PMRarity rarity, QHostAddress &senderAddr)
{
    QByteArray msgba;//message to send
    QDataStream outStream(&msgba,QIODevice::ReadWrite);

//    datagramType type=GETOPPONENT;//type of this datagram
//    outStream<<type;

    qsrand(QTime::currentTime().msec());
    PMType pmType=(PMType)(qrand()%4);
    PokeMon *opponent;
    switch (pmType) {
    case Strength:
        opponent=new PMStrength(rarity);
        break;
    case Shield:
        opponent=new PMShield(rarity);
        break;
    case Defense:
        opponent=new PMDefense(rarity);
        break;
    case Agility:
        opponent=new PMAgility(rarity);
        break;
    default:
        break;
    }
    opponent->gainExp(expToLvUp[level-1]);
    outStream<<opponent;


    server->writeDatagram(msgba,senderAddr,port);

}

void MainWindow::on_sendResult(QDataStream &inStream)
{
    QString name;
    int battleType;
    int id;
    bool result;
    inStream>>name>>battleType>>id>>result;
    if(result)
    {
        //胜场数+1

        int exp;
        inStream>>exp;
        PokeMon *pm=database->pmAt(id);
        pm->gainExp(exp);
        database->updatePM(id,pm);

    }
    if(battleType==2)
    {
        ;
    }

}

void MainWindow::on_getTrophy(QDataStream &inStream)
{
    QString name;
    PokeMon *pm;
    inStream>>name>>pm;
    database->addPokeMon(name,pm);

}

void MainWindow::on_killPM(QDataStream &inStream)
{
    int id;
    inStream>>id;
    database->deletePokeMon(id);

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

void MainWindow::on_tableWidget_users_currentCellChanged(int currentRow, int currentColumn,
                                                         int previousRow, int previousColumn)
{
    ui->tableWidget_monsters->setSortingEnabled(false);
    int rowCount=ui->tableWidget_monsters->rowCount();
    for(int i=0;i<rowCount;++i)
    {
        ui->tableWidget_monsters->removeRow(0);
    }


    QString currentName=ui->tableWidget_users->item(currentRow,0)->text();

    QList<PokeMon *> pmList=database->pmsOfUser(currentName);
    for (auto pm:pmList)
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
        delete pm;
    }

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

QDataStream &operator<<(QDataStream &out, const PokeMon *pm)
{
    out<<(int)pm->type<<pm->name<<pm->level<<pm->attack<<pm->defence<<pm->maxHealth
      <<pm->speed<<pm->exp<<(int)pm->rarity<<(int)pm->limitBreak<<pm->id;
    return out;
}
