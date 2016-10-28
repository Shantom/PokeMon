#ifndef USERS_H
#define USERS_H

#include <QWidget>
#include <QUdpSocket>
#include <QTableWidgetItem>
#include <pokemon.h>

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
    void on_getCertainMons(QDataStream & inStream);
    void refreshTableUsers();
    void refreshTableMons();

private:
    Ui::Users *ui;
    QUdpSocket * socket;
    const QHostAddress serverAddress;
    const quint16 serverPort=13400;
    quint16 selfPort=60100;
    QList<QTableWidgetItem *> users;
    QList<PokeMon *> allPM;

public slots:
    void on_OpenUsers();
    void on_readyRead();
private slots:
    void on_pushButton_refresh_clicked();
    void on_pushButton_showPM_clicked();
    void on_tableWidget_users_doubleClicked(const QModelIndex &index);
};

#endif // USERS_H
