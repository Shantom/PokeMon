#ifndef BATTLE_H
#define BATTLE_H

#include <QWidget>

namespace Ui {
class Battle;
}

class Battle : public QWidget
{
    Q_OBJECT

public:
    explicit Battle(QWidget *parent = 0);
    ~Battle();

private slots:
    void on_pushButton_back_clicked();

    void on_pushButton_UpTour_clicked();

    void on_pushButton_DuRace_clicked();

    void on_pushButton_Battle_clicked();

private:
    Ui::Battle *ui;
    int battleType=0;
};

#endif // BATTLE_H
