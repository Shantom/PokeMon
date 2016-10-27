#ifndef PMDEFENSE_H
#define PMDEFENSE_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "pokemon.h"

class PMDefense : public PokeMon
{
public:
    PMDefense(){}
    PMDefense(PMRarity rarity);
    int move();
    void levelUp();
};

#endif // PMDEFENSE_H
