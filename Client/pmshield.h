#ifndef PMSHIELD_H
#define PMSHIELD_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "pokemon.h"

class PMShield : public PokeMon
{
public:
    PMShield(){}
    PMShield(PMRarity rarity);
    int move();
    void levelUp();
};

#endif // PMSHIELD_H
