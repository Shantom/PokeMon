#include "battle.h"
#include "ui_battle.h"

Battle::Battle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Battle)
{
    ui->setupUi(this);
    setWindowTitle("Battle!");
    setWindowFlags(Qt::Dialog);
    setWindowModality(Qt::WindowModal);
    hide();
    ui->tableView_chellenges->hide();
    ui->label_type->hide();
    ui->textBrowser_info->hide();
    ui->pushButton_back->hide();
    ui->pushButton_Battle->hide();
    ui->pushButton_skip->hide();
}

Battle::~Battle()
{
    delete ui;
}

void Battle::on_pushButton_back_clicked()
{
    ui->pushButton_back->hide();
    ui->tableView_chellenges->hide();
    ui->pushButton_Battle->hide();
    ui->label_type->hide();
    ui->pushButton_UpTour->show();
    ui->pushButton_DuRace->show();
}

void Battle::on_pushButton_UpTour_clicked()
{
    ui->pushButton_back->show();
    ui->tableView_chellenges->show();
    ui->pushButton_Battle->show();
    ui->label_type->setText("Upgrade Tournament");
    this->battleType=1;
    ui->label_type->show();
    ui->pushButton_UpTour->hide();
    ui->pushButton_DuRace->hide();

}

void Battle::on_pushButton_DuRace_clicked()
{
    ui->pushButton_back->show();
    ui->tableView_chellenges->show();
    ui->pushButton_Battle->show();
    ui->label_type->setText("Duel Race");
    this->battleType=2;
    ui->label_type->show();
    ui->pushButton_UpTour->hide();
    ui->pushButton_DuRace->hide();
}

void Battle::on_pushButton_Battle_clicked()
{
    ui->tableView_chellenges->hide();
    ui->pushButton_Battle->hide();
    ui->pushButton_back->hide();
    ui->pushButton_skip->show();
    ui->textBrowser_info->show();
}
