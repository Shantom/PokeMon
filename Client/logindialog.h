#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QUdpSocket>

namespace Ui {
class LoginDialog;
}

enum datagramType{
    LOGIN,SIGNUP,EXIT,GETSELFMONS,GETUSERS,GETCERTAINMONS
};

QDataStream & operator>> (QDataStream &in, datagramType & type );
QDataStream & operator<< (QDataStream &out, const datagramType type );

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    QUdpSocket *socket;
    QString getName();
    quint16 getPort();

private slots:


    void on_pushButton_login_clicked();

    void on_pushButton_signup_clicked();

    void on_lineEdit_repeat_textEdited(const QString &arg1);

    void on_lineEdit_password_textEdited(const QString &arg1);

private:
    Ui::LoginDialog *ui;
    const QHostAddress serverAddress;
    const quint16 serverPort=13400;
    quint16 selfPort=60200;
    QString username;


};

#endif // LOGINDIALOG_H
