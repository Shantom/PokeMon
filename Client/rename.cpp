#include "rename.h"
#include "ui_rename.h"

Rename::Rename(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Rename)
{
    ui->setupUi(this);
    QIcon icon(QPixmap(":/new/prefix1/image/egg.png"));
    setWindowIcon(icon);
}

QString Rename::value()
{
    return name;
}

Rename::~Rename()
{
    delete ui;
}

void Rename::accept()
{
    name=ui->lineEdit_name->text();
    QDialog::accept();
}

void Rename::on_pushButton_OK_clicked()
{
    this->accept();
}
