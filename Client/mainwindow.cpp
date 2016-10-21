#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pokemon.h"
#include "pmstrength.h"
#include "pmagility.h"
#include "pmshield.h"
#include "pmdefense.h"
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qsrand(QTime::currentTime().msec());
    A=new PMAgility(Epic);
//    A->gainExp(200);
    QString info_A=A->getInfomation();

    qsrand(QTime::currentTime().msec()*2);
    B=new PMStrength(Epic);
//    B->gainExp(200);
    QString info_B=B->getInfomation();

    ui->labelPM_1->setText(info_A);
    ui->labelPM_2->setText(info_B);
    ui->comboBox_Rarity_1->setCurrentIndex(0);
    ui->comboBox_Rarity_2->setCurrentIndex(0);
    ui->comboBox_Type_1->setCurrentIndex(0);
    ui->comboBox_Type_2->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::on_pushButton_Attack_2_clicked()
{
    int movement=B->move();
    QString moveInfo;
    if(movement==ordAttack)
        moveInfo+=tr("PokeMon B gives a hit on A.");
    else
        moveInfo+= tr("PokeMon B uses his limitbreak ")+
                LimitBreak_toString[movement]+".";
    ui->label_moveInfo_2->setText(moveInfo);
}

void MainWindow::on_pushButton_LvUp_2_clicked()
{
    B->gainExp(expToLvUp[B->level]-expToLvUp[B->level-1]);

    if(B->level==15)
        ui->pushButton_LvUp_2->setEnabled(false);

    QString info_B=B->getInfomation();
    ui->labelPM_2->setText(info_B);

}

void MainWindow::on_pushButton_LvMax_1_clicked()
{
    A->gainExp(expToLvUp[14]);
    ui->pushButton_LvMax_1->setEnabled(false);
    ui->pushButton_LvUp_1->setEnabled(false);
    QString info_A=A->getInfomation();
    ui->labelPM_1->setText(info_A);

}

void MainWindow::on_pushButton_LvMax_2_clicked()
{
    B->gainExp(expToLvUp[14]);
    ui->pushButton_LvMax_2->setEnabled(false);
    ui->pushButton_LvUp_2->setEnabled(false);
    QString info_B=B->getInfomation();
    ui->labelPM_2->setText(info_B);

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

void MainWindow::on_pushButton_Create_2_clicked()
{
    delete B;
    PMType type=(PMType)ui->comboBox_Type_2->currentIndex();
    PMRarity rarity=(PMRarity)ui->comboBox_Rarity_2->currentIndex();
    switch (type) {
    case Strength:
        B=new PMStrength(rarity);
        break;
    case Defense:
        B=new PMDefense(rarity);
        break;
    case Shield:
        B=new PMShield(rarity);
        break;
    case Agility:
        B=new PMAgility(rarity);
        break;
    default:
        break;
    }
    QString info_B=B->getInfomation();
    ui->labelPM_2->setText(info_B);

    ui->pushButton_LvMax_2->setEnabled(true);
    ui->pushButton_LvUp_2->setEnabled(true);

}
