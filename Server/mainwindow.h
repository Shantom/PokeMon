#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <pokemon.h>
#include <QUdpSocket>
#include "database.h"

enum datagramType{
    LOGIN,SIGNUP,EXIT
};

QDataStream & operator>> (QDataStream &in, datagramType & type );
QDataStream & operator<< (QDataStream &out, const datagramType type );

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QUdpSocket *server;
    Database * database;

private:
    void initUsersTable();

    void initMonsTable();

    PokeMon *getRandomPM();

private slots:

    void onLogin(QHostAddress senderAddr, quint16 port, QString name);

    void onLogout(QString name);

    void onSignup(QString name);

    void on_readyread();

    void on_pushButton_LvUp_1_clicked();

    void on_pushButton_LvMax_1_clicked();

    void on_pushButton_Attack_1_clicked();

    void on_pushButton_Create_1_clicked();


    void on_tableWidget_users_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
    Ui::MainWindow *ui;
    PokeMon *A;
    quint16 serverPort=13400;
    QList<quint16> clientPorts;

};

#endif // MAINWINDOW_H
