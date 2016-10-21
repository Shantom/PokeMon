#ifndef PMSTRENGTH_H
#define PMSTRENGTH_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <pokemon.h>

class PMStrength : public PokeMon
{
public:
    PMStrength(PMRarity rarity);
    int move();
    void levelUp();
};

#endif // PMSTRENGTH_H
