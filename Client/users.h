#ifndef USERS_H
#define USERS_H

#include <QWidget>
#include <QUdpSocket>
#include <QTableWidgetItem>

namespace Ui {
class Users;
}

class Users : public QWidget
{
    Q_OBJECT

public:
    explicit Users(QWidget *parent = 0);
    ~Users();
    void initUsersTable();
    void initMonsTable();
    void on_getUsers(QDataStream & inStream);
    void refreshTableUsers();

private:
    Ui::Users *ui;
    QUdpSocket * socket;
    const QHostAddress serverAddress;
    const quint16 serverPort=13400;
    quint16 selfPort=60100;
    QList<QTableWidgetItem *> users;

public slots:
    void on_OpenUsers();
    void on_readyRead();
private slots:
    void on_pushButton_refresh_clicked();
};

#endif // USERS_H
