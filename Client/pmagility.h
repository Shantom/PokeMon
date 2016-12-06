#ifndef PMAGILITY_H
#define PMAGILITY_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "pokemon.h"

class PMAgility : public PokeMon
{
public:
    PMAgility(){}
    PMAgility(PMRarity rarity);
    PMAgility(PokeMon& src):PokeMon(src){}
    int move();
    void levelUp();
};

#endif // PMAGILITY_H
