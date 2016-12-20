#include "pmdefense.h"

PMDefense::PMDefense(PMRarity rarity):PokeMon(rarity)
{
    this->type=Defense;
    defence*=DefenseRate;
    qsrand(QTime::currentTime().msec()*type);
    limitBreak=(LimitBreak)(2*type+((qrand()%100)/50));

}

int PMDefense::move()
{
    int flag=qrand()%100;
    if(flag>20){
        return ordAttack;
    }
    else {
        return limitBreak;
    }

}

void PMDefense::levelUp()
{
    while(exp>=expToLvUp[level])
    {
        level++;
        attack+=AttackGain*RarityGainRate[rarity];
        defence+=DefenceGain*RarityGainRate[rarity]*(DefenseRate);
        maxHealth+=HealthGain*RarityGainRate[rarity];
        speed+=SpeedGain*RarityGainRate[rarity];
    }
}
