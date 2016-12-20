#include "pmstrength.h"

PMStrength::PMStrength(PMRarity rarity):PokeMon(rarity)
{
    this->type=Strength;
    attack*=StrengthRate;
    qsrand(QTime::currentTime().msec()*type);
    limitBreak=(LimitBreak)(2*type+((qrand()%100)/50));
}

int PMStrength::move()
{
//    qsrand(QTime::currentTime().msec()+speed);
    int flag=qrand()%100;
    if(flag>20){
        return ordAttack;
    }
    else {
        return limitBreak;
    }
}
void PMStrength::levelUp()
{
    while(exp>=expToLvUp[level])
    {
        level++;
        attack+=AttackGain*RarityGainRate[rarity]*(StrengthRate);
        defence+=DefenceGain*RarityGainRate[rarity];
        maxHealth+=HealthGain*RarityGainRate[rarity];
        speed+=SpeedGain*RarityGainRate[rarity];
    }
}
