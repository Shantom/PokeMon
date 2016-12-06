#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <pokemon.h>
#include <QUdpSocket>
#include "battle.h"

namespace Ui {
class MainWindow;
}
QDataStream & operator>> (QDataStream &in, PokeMon * &pm );
QDataStream & operator<< (QDataStream &out, const PokeMon* &pm );

class Battle;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString name,quint16 port,QWidget *parent = 0);
    ~MainWindow();
    void setName(QString name){userName=name;}
    void setPort(quint16 port){selfPort=port;}

private:

private slots:
    void initMonsTable();

    void on_readyRead();

    void on_getSelfMons(QDataStream &inStream);

    void refreshMonsTable();

    void on_pushButton_LvUp_1_clicked();

    void on_pushButton_LvMax_1_clicked();

    void on_pushButton_Attack_1_clicked();

    void on_pushButton_Create_1_clicked();

    void on_tableWidget_monsters_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_pushButton_allUsers_clicked();

    void on_pushButton_Battle_clicked();

private:
    Ui::MainWindow *ui;
    PokeMon *A;
    QUdpSocket * socket;
    const QHostAddress serverAddress;
    const quint16 serverPort=13400;
    QString userName;
    quint16 selfPort;

    QList<PokeMon *> allPM;

    Battle *battle;

signals:
    void openUsers();
    void sendPM(PokeMon *self);

};

#endif // MAINWINDOW_H
