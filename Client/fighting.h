#ifndef FIGHTING_H
#define FIGHTING_H

#include <QObject>
#include <QDebug>
#include <pokemon.h>
#include <pmagility.h>
#include <pmdefense.h>
#include <pmshield.h>
#include <pmstrength.h>

enum StateFlag{
    Burning,
    Toxic,
    Confused
};

struct movement{
    quint32 time;
    char active;
    bool critical;
    bool miss;
    int damage;
    int extraDamage;
    int recovery;
    LimitBreak method;
    QList<quint8> stateA;
    QList<quint8> stateB;
    qint16 HP_A;
    qint16 HP_B;
};

class Fighting : public QObject
{
    Q_OBJECT
public:
    explicit Fighting(QObject *parent = 0);

    Fighting(PokeMon *a, PokeMon *b);

    QList<movement> procedure();

    static QStringList generateInfo(QList<movement> procedures);

    bool isVictory(){return result;}

private:
    void exceptionFade(char index);


    const int SPEEDARG = 4e6;

    bool result;

    qint16 healthPoint_A;
    qint16 healthPoint_B;
    QList<quint8> stateA;
    QList<quint8> stateB;

    PokeMon *A;
    PokeMon *B;

signals:

public slots:
};

#endif // FIGHTING_H
