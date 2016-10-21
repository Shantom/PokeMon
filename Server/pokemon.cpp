#include "pokemon.h"
#include <QList>
using namespace std;
const char* PMType_toString[]=
{
    "Strength", "Defense", "Shield", "Agility"
};
const char* PMRarity_toString[]=
{
    "Common", "Rare", "Epic", "Legendary"
};
QList<int> expToLvUp={0,5,10,17,30,
                       42,60,80,110,150,
                       200,260,320,390,470,
                      0x3f3f3f3f};
QList<QString> LimitBreak_toString={"FireSpin", "TakeDown",
                                    "WaterPulse", "PoisonJab",
                                   "LeechLife","Aromatherapy",
                                   "AirCutter","FurySwipes"};

double RarityGainRate[4]={0.8,1.0,1.2,1.4};

PokeMon::PokeMon()
{

}

PokeMon::PokeMon(PMRarity rarity)
{
    level=1;
    exp=0;

    qsrand(QTime::currentTime().msec());
    this->rarity=rarity;

    attack=qrand()%5+25;
    defence=qrand()%3+12;
    maxHealth=qrand()%10+50;
    speed=qrand()%100+900;


    /*growth rate of different rarity*/
    switch (rarity) {
    case Common:
        attack*=CommonRate;
        defence*=CommonRate;
        maxHealth*=CommonRate;
        speed*=CommonRate;
        break;
    case Rare:
        attack*=RareRate;
        defence*=RareRate;
        maxHealth*=RareRate;
        speed*=RareRate;
        break;
    case Epic:
        attack*=EpicRate;
        defence*=EpicRate;
        maxHealth*=EpicRate;
        speed*=EpicRate;
        break;
    case Legendary:
        attack*=LegendaryRate;
        defence*=LegendaryRate;
        maxHealth*=LegendaryRate;
        speed*=LegendaryRate;
        break;
    default:
        break;
    }
}

PokeMon::~PokeMon()
{

}

void PokeMon::gainExp(int newExp)
{
    exp+=newExp;
    levelUp();
}

void PokeMon::levelUp()
{
}

void PokeMon::rename(QString newName)
{
    name=newName;
}

QString PokeMon::getInfomation()
{
    QString info;
    info+=tr("Type: %1").arg(PMType_toString[type]);
    info+=tr("\nRarity: %1").arg(PMRarity_toString[rarity]);
    info+=tr("\nLevel: %1").arg(level);
    info+=tr("\nAttack: %1").arg((int)attack);
    info+=tr("\nDefence: %1").arg((int)defence);
    info+=tr("\nMaxHealth: %1").arg((int)maxHealth);
    info+=tr("\nSpeed: %1").arg((int)speed);
    info+=tr("\nLimitBreak: %1").arg(LimitBreak_toString[limitBreak]);
    return info;
}
