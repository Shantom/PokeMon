#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <pokemon.h>
#include <QUdpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QUdpSocket * socket;
    void setName(QString name){userName=name;}
    void setPort(quint16 port){selfPort=port;}

private:
    PokeMon *A;
    PokeMon *B;

private slots:
    void on_pushButton_LvUp_1_clicked();

    void on_pushButton_LvMax_1_clicked();

    void on_pushButton_Attack_1_clicked();

    void on_pushButton_Create_1_clicked();

private:
    Ui::MainWindow *ui;
    const QHostAddress serverAddress;
    const int serverPort=13400;
    QString userName;
    quint16 selfPort;

};

#endif // MAINWINDOW_H
