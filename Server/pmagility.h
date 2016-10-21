#ifndef PMAGILITY_H
#define PMAGILITY_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "pokemon.h"

class PMAgility : public PokeMon
{
public:
    PMAgility(PMRarity rarity);
    int move();
    void levelUp();
};

#endif // PMAGILITY_H
