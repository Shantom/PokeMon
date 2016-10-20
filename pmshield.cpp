#include "pmshield.h"

PMShield::PMShield(PMRarity rarity):PokeMon(rarity)
{
    this->type=Shield;
    maxHealth*=ShieldRate;
    qsrand(QTime::currentTime().msec()*type);
    limitBreak=(LimitBreak)(2*type+((qrand()%100)/50));

}

int PMShield::move()
{
    qsrand(QTime::currentTime().msec()+speed);
    int flag=qrand()%100;
    if(flag>15){
        return ordAttack;
    }
    else {
        return limitBreak;
    }

}

void PMShield::levelUp()
{
    while(exp>=expToLvUp[level])
    {
        level++;
        attack+=AttackGain*RarityGainRate[rarity];
        defence+=DefenceGain*RarityGainRate[rarity];
        maxHealth+=HealthGain*RarityGainRate[rarity]*(ShieldRate);
        speed+=SpeedGain*RarityGainRate[rarity];
    }
}
