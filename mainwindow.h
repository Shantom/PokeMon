#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <pokemon.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    PokeMon *A;
    PokeMon *B;

private slots:
    void on_pushButton_LvUp_1_clicked();

    void on_pushButton_Attack_2_clicked();

    void on_pushButton_LvUp_2_clicked();

    void on_pushButton_LvMax_1_clicked();

    void on_pushButton_LvMax_2_clicked();

    void on_pushButton_Attack_1_clicked();

    void on_pushButton_Create_1_clicked();

    void on_pushButton_Create_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
