#include "pmagility.h"

PMAgility::PMAgility(PMRarity rarity):PokeMon(rarity)
{
    this->type=Agility;
    speed*=AgilityRate;
    qsrand(QTime::currentTime().msec()*type);
    limitBreak=(LimitBreak)(2*type+((qrand()%100)/50));

}

int PMAgility::move()
{
    qsrand(seed++);
    int flag=qrand()%100;
//    qDebug()<<flag;
    if(flag>45){
        return OrdAttack;
    }
    else {
        return limitBreak;
    }

}
void PMAgility::levelUp()
{
    while(exp>=expToLvUp[level])
    {
        level++;
        attack+=AttackGain*RarityGainRate[rarity];
        defence+=DefenceGain*RarityGainRate[rarity];
        maxHealth+=HealthGain*RarityGainRate[rarity];
        speed+=SpeedGain*RarityGainRate[rarity]*(AgilityRate);
    }
}
