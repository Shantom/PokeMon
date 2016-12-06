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
const QList<int> expToLvUp={0,100,300,600,1000,
                       1500,2100,2800,3600,4500,
                       5500,6831,8559,10756,13500,
                      0x3f3f3f3f};
const QList<QString> LimitBreak_toString={"FireSpin", "TakeDown",
                                    "WaterPulse", "PoisonJab",
                                   "LeechLife","Aromatherapy",
                                   "AirCutter","FurySwipes","OrdAttack"};

const double RarityGainRate[4]={0.8,1.0,1.2,1.4};

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

PokeMon::PokeMon(PokeMon &src)
{
    type=src.type;
    rarity=src.rarity;
    limitBreak=src.limitBreak;
    name=src.name;
    level=src.level;
    exp=src.exp;
    attack=src.attack;
    defence=src.defence;
    maxHealth=src.maxHealth;
    speed=src.speed;

    seed=qrand();
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
    info+=tr("Name: %1").arg(name);
    info+=tr("\nType: %1").arg(PMType_toString[type]);
    info+=tr("\nRarity: %1").arg(PMRarity_toString[rarity]);
    info+=tr("\nLevel: %1").arg(level);
    info+=tr("\nAttack: %1").arg((int)attack);
    info+=tr("\nDefence: %1").arg((int)defence);
    info+=tr("\nMaxHealth: %1").arg((int)maxHealth);
    info+=tr("\nSpeed: %1").arg((int)speed);
    info+=tr("\nLimitBreak: %1").arg(LimitBreak_toString[limitBreak]);
    return info;
}
