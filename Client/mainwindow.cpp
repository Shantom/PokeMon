#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pokemon.h"
#include "pmstrength.h"
#include "pmagility.h"
#include "pmshield.h"
#include "pmdefense.h"
#include "logindialog.h"
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    serverAddress("127.0.0.1")

{
    ui->setupUi(this);

    socket=new QUdpSocket(this);
    socket->bind(selfPort);

    qsrand(QTime::currentTime().msec());
    A=new PMAgility(Epic);
//    A->gainExp(200);
    QString info_A=A->getInfomation();


    ui->labelPM_1->setText(info_A);
    ui->comboBox_Rarity_1->setCurrentIndex(0);
    ui->comboBox_Type_1->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    /*log out*/
    QByteArray datagram;//datagram to send
    QDataStream outStream(&datagram,QIODevice::ReadWrite);
    datagramType type=EXIT;
    qDebug()<<userName;
    outStream<<type<<userName;
    socket->writeDatagram(datagram,serverAddress,serverPort);

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

