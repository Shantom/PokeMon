#ifndef BATTLE_H
#define BATTLE_H

#include <QWidget>
#include <QDebug>
#include <QUdpSocket>
#include <pokemon.h>
#include <pmagility.h>
#include <pmdefense.h>
#include <pmshield.h>
#include <pmstrength.h>
#include <fighting.h>
#include <death.h>
#include <pokemon.h>
#include <logindialog.h>
#include <mainwindow.h>
#include <QMessageBox>
#include <rename.h>
namespace Ui {
class Battle;
}

class Battle : public QWidget
{
    Q_OBJECT

public:
    explicit Battle(QString name,quint16 port,QList<PokeMon *> allPM,QWidget *parent = 0);
    ~Battle();

    void sendResult(int id, bool result,int level, PMRarity rarity);
    void getTrophy(PokeMon *opponent);
    void refreshAllPM(QList<PokeMon *> mons){allPM=mons;}
    void setID(int id){selectedID=id;}
private slots:
    void on_pushButton_back_clicked();

    void on_pushButton_UpTour_clicked();

    void on_pushButton_DuRace_clicked();

    void on_pushButton_Battle_clicked();

    void on_pushButton_skip_clicked();

    void on_pushButton_close_clicked();

public slots:
    void recvPM(PokeMon *src);
    void on_killPM(int id);
private:

    QList<PokeMon *> selectedPM;
    Ui::Battle *ui;
    QUdpSocket * socket;
    PokeMon *self;
    const QHostAddress serverAddress;
    const quint16 serverPort=13400;
    int battleType=0;
    bool isSkipped;
    quint16 battlePort;
    QString name;
    QList<PokeMon *> allPM;
    Death * death;
    int selectedID;
signals:
    void initMonsTable();
};

#endif // BATTLE_H
