#include "logindialog.h"
#include "ui_logindialog.h"
#include "QMessageBox"
#include <QCryptographicHash>
#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    serverAddress("127.0.0.1")
{
    ui->setupUi(this);
    ui->lineEdit_username->setText("Shantom");
    ui->lineEdit_password->setText("5589");
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_repeat->setEchoMode(QLineEdit::Password);
    ui->lineEdit_repeat->hide();
    ui->label_repeat->hide();
    socket = new QUdpSocket(this);
    while(!socket->bind(selfPort))
    {
        selfPort++;
    }
}

LoginDialog::~LoginDialog()
{
    delete ui;

}

QString LoginDialog::getName()
{
    return ui->lineEdit_username->text().trimmed();
}

quint16 LoginDialog::getPort()
{
    return selfPort;
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

void LoginDialog::on_pushButton_login_clicked()
{
    QByteArray datagram;//datagram to send
    QDataStream outStream(&datagram,QIODevice::ReadWrite);
    QString username=ui->lineEdit_username->text().trimmed();
    QString password=ui->lineEdit_password->text().trimmed();
    password = (QString)QCryptographicHash::hash(password.toLatin1(),
                                                 QCryptographicHash::Md5).toHex().toUpper();
    datagramType type;
    if(ui->lineEdit_repeat->isHidden()) //login
        type=LOGIN;
    else //signup
        type=SIGNUP;

    outStream<<type<<selfPort<<username<<password;

    socket->writeDatagram(datagram,serverAddress,serverPort);

    if(socket->waitForReadyRead(500))
    {
        QByteArray receiveData;
        receiveData.resize(socket->pendingDatagramSize());
        socket->readDatagram(receiveData.data(),receiveData.size());

        QDataStream inStream(receiveData);
        QString result;
        inStream>>result;
        if(result=="Yes")
        {
            if(ui->lineEdit_repeat->isHidden()) //login
            {
                QMessageBox::warning(this,tr("成功"),tr("登录成功!"),QMessageBox::Yes);
                socket->close();
                delete socket;
                accept();
            }
            else //signup
            {
                QMessageBox::warning(this,tr("成功"),tr("注册成功!\n请重新登录"),QMessageBox::Yes);
                on_pushButton_signup_clicked();
                ui->lineEdit_password->clear();
                ui->label_password->setFocus();
            }
        }
        else//server rejects
        {
            if(ui->lineEdit_repeat->isHidden()) //login
            {
                QMessageBox::warning(this,tr("警告"),tr("用户名或密码错误!"),QMessageBox::Yes);
            }
            else //signup
            {
                QMessageBox::warning(this,tr("警告"),tr("用户名已存在!"),QMessageBox::Yes);
            }
            ui->lineEdit_username->setFocus();
            ui->lineEdit_username->selectAll();

        }
    }
    else{
        QMessageBox::warning(this,tr("警告"),tr("连接超时!"),QMessageBox::Yes);
    }


}

void LoginDialog::on_pushButton_signup_clicked()
{
    if(ui->lineEdit_repeat->isHidden())
    {
        ui->lineEdit_repeat->show();
        ui->label_repeat->show();
        ui->pushButton_login->setText("确认注册");
        ui->pushButton_login->setEnabled(false);
        ui->pushButton_signup->setText("返回");
    }
    else
    {
        ui->lineEdit_repeat->hide();
        ui->label_repeat->hide();
        ui->lineEdit_repeat->clear();
        ui->pushButton_login->setEnabled(true);
        ui->label_warning->setText("");
        ui->pushButton_login->setText("登录");
        ui->pushButton_signup->setText("注册");
    }
}

void LoginDialog::on_lineEdit_repeat_textEdited(const QString &arg1)
{
    if(!ui->lineEdit_repeat->isHidden())
    {
        QString password=ui->lineEdit_password->text();
    //    QString repeat=ui->lineEdit_repeat->text();
        if(password!=arg1)
        {
            ui->label_warning->setText("两次密码不一致");
            ui->label_warning->setStyleSheet("color:red;");
            ui->pushButton_login->setEnabled(false);
        }
        else
        {
            ui->label_warning->setText("");
            ui->pushButton_login->setEnabled(true);
        }
    }
}

void LoginDialog::on_lineEdit_password_textEdited(const QString &arg1)
{
    if(!ui->lineEdit_repeat->isHidden())
    {
    //    QString password=ui->lineEdit_password->text();
        QString repeat=ui->lineEdit_repeat->text();
        if(repeat!=arg1)
        {
            ui->label_warning->setText("两次密码不一致");
            ui->label_warning->setStyleSheet("color:red;");
            ui->pushButton_login->setEnabled(false);
        }
        else
        {
            ui->label_warning->setText("");
            ui->pushButton_login->setEnabled(true);
        }
    }
}
