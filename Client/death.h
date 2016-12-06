#ifndef DEATH_H
#define DEATH_H

#include <QDialog>
#include <pokemon.h>

namespace Ui {
class Death;
}

class Death : public QDialog
{
    Q_OBJECT

public:
    explicit Death(QList<PokeMon*> mons,QWidget *parent = 0);
    ~Death();
    int value();

private slots:
    void on_pushButton_OK_clicked();

private:
    Ui::Death *ui;
    QList<PokeMon*> monsters;
    int id;
signals:
    void killPM(int id);
};



#endif // DEATH_H
