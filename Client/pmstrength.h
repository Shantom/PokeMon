#ifndef PMSTRENGTH_H
#define PMSTRENGTH_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <pokemon.h>

class PMStrength : public PokeMon
{
public:
    PMStrength(){}
    PMStrength(PMRarity rarity);
    PMStrength(PokeMon& src):PokeMon(src){}
    int move();
    void levelUp();
};

#endif // PMSTRENGTH_H
