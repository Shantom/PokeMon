#include "fighting.h"

const QList<QString> MovetoString={"火焰漩涡(FireSpin)", "猛撞(TakeDown)",
                                    "水波动(WaterPulse)", "毒刺(PoisonJab)",
                                   "吸血(LeechLife)","芳香治疗(Aromatherapy)",
                                   "破空斩(AirCutter)","疯狂乱抓(FurySwipes)",
                                          "普通攻击(OrdAttack)"};


Fighting::Fighting(QObject *parent) : QObject(parent)
{

}

Fighting::Fighting(PokeMon *a, PokeMon *b)
{
    switch(a->type)
    {
    case Strength:
        A=new PMStrength(*a);
        break;
    case Defense:
        A=new PMDefense(*a);
        break;
    case Shield:
        A=new PMShield(*a);
        break;
    case Agility:
        A=new PMAgility(*a);
        break;
    }
    switch (b->type) {
    case Strength:
        B=new PMStrength(*b);
        break;
    case Defense:
        B=new PMDefense(*b);
        break;
    case Shield:
        B=new PMShield(*b);
        break;
    case Agility:
        B=new PMAgility(*b);
        break;
    }

    healthPoint_A=A->maxHealth;
    healthPoint_B=B->maxHealth;
    stateA={0,0,0,0};
    stateB={0,0,0,0};

}

QList<movement> Fighting::procedure()
{
    QList<movement> procedures;
    int interval_A=SPEEDARG/A->speed;
    int interval_B=SPEEDARG/B->speed;
    int rest_A=interval_A;
    int rest_B=interval_B;
    quint32 time=0;
    while(true)
    {
        if(rest_A<rest_B){ // A 攻击
            time+=rest_A;

            qsrand(time);
            int dice=qrand()%100;
            bool critical=false;
            bool miss=false;
            if(dice>90)
                critical=true;
            else if(dice<10)
                miss=true;

            rest_B-=rest_A;
            rest_A=interval_A;

            LimitBreak move = (LimitBreak)A->move();
            int damage=(A->attack-B->defence)>10?(A->attack-B->defence):10;
            int recovery=0;
            switch (move) {
            case OrdAttack:
                break;
            case FireSpin:
                damage*=1.5;
                if(!miss)
                  stateA[Burning]=3;
                break;
            case TakeDown:
                if(!miss)
                    recovery-=damage*.8;
                damage*=2.5;
                break;
            case WaterPulse:
                damage*=1.5;
                if(dice>80)
                    stateB[Confused]=3;
                break;
            case PoisonJab:
                damage*=1.5;
                if(!miss)
                    stateB[Toxic]=3;
                break;
            case LeechLife:
                damage*=1.2;
                recovery+=damage*.7;
                break;
            case Aromatherapy:
                damage=0;
                recovery+=A->maxHealth*.5;
                stateA={0,0,0,0};
            case AirCutter:
                if(dice>40)
                    critical=true;
                damage*=1.5;
                break;
            case FurySwipes:
                dice%=10;
                if(dice==9)
                    damage*=2.5;
                else if(dice==8||dice==7)
                    damage*=2.0;
                else if(dice==6||dice==5||dice==4||dice==3)
                    damage*=1.5;
                else if(dice==2||dice==1||dice==0)
                    damage*=1.0;
                break;

            }

            if(critical)
                damage*=1.5;
            if(miss)
                damage=0;

            int extraDamage=0;
            if(stateA[Burning])
            {
                extraDamage=A->maxHealth/10;
                damage*=.7;
            }
            if(stateA[Toxic])
                extraDamage=A->maxHealth/8;
            if(stateA[Confused])
            {
                dice=qrand()%10;
                if(dice<5)
                {
                    extraDamage=damage;
                    damage=0;
                }

            }

            exceptionFade('A');

            healthPoint_B-=damage;
            healthPoint_A+=(recovery-extraDamage);


            if(healthPoint_A>A->maxHealth)
                healthPoint_A=A->maxHealth;
            else if(healthPoint_A<0)
                healthPoint_A=0;
            if(healthPoint_B<0)
                healthPoint_B=0;
            movement tmp={time, 'A', critical, miss, damage,
                          extraDamage, recovery, move, stateA, stateB,
                          healthPoint_A, healthPoint_B};
            procedures.append(tmp);
        }
        else //rest_B<rest_A
        {
            time+=rest_B;

            qsrand(time);
            int dice=qrand()%100;
            bool critical=false;
            bool miss=false;
            if(dice>90)
                critical=true;
            else if(dice<10)
                miss=true;

            rest_A-=rest_B;
            rest_B=interval_B;

            LimitBreak move = (LimitBreak)B->move();
            int damage=(B->attack-A->defence)>10?(B->attack-A->defence):10;
            int recovery=0;
            switch (move) {
            case OrdAttack:
                break;
            case FireSpin:
                damage*=1.5;
                if(!miss)
                  stateA[Burning]=3;
                break;
            case TakeDown:
                if(!miss)
                    recovery-=damage*.8;
                damage*=2.5;
                break;
            case WaterPulse:
                damage*=1.5;
                if(dice>80)
                    stateA[Confused]=3;
                break;
            case PoisonJab:
                damage*=1.5;
                if(!miss)
                    stateA[Toxic]=3;
                break;
            case LeechLife:
                damage*=1.2;
                recovery+=damage*.7;
                break;
            case Aromatherapy:
                damage=0;
                recovery+=B->maxHealth*.5;
                stateB={0,0,0,0};
            case AirCutter:
                if(dice>40)
                    critical=true;
                damage*=1.5;
                break;
            case FurySwipes:
                dice%=10;
                if(dice==9)
                    damage*=2.5;
                else if(dice==8||dice==7)
                    damage*=2.0;
                else if(dice==6||dice==5||dice==4||dice==3)
                    damage*=1.5;
                else if(dice==2||dice==1||dice==0)
                    damage*=1.0;
                break;

            }

            if(critical)
                damage*=1.5;
            if(miss)
                damage=0;

            int extraDamage=0;
            if(stateB[Burning])
            {
                extraDamage=B->maxHealth/10;
                damage*=.7;
            }
            if(stateB[Toxic])
                extraDamage=B->maxHealth/8;
            if(stateB[Confused])
            {
                dice=qrand()%10;
                if(dice<5)
                {
                    extraDamage=damage;
                    damage=0;
                }

            }

            exceptionFade('B');

            healthPoint_A-=damage;
            healthPoint_B+=(recovery-extraDamage);


            if(healthPoint_B>=B->maxHealth)
                healthPoint_B=B->maxHealth;
            else if(healthPoint_B<0)
                healthPoint_B=0;
            if(healthPoint_A<0)
                healthPoint_A=0;
            movement tmp={time, 'B', critical, miss, damage,
                          extraDamage, recovery, move, stateA, stateB,
                          healthPoint_A, healthPoint_B};
            procedures.append(tmp);

        }
        if(healthPoint_B==0)
        {
            result=true;
            break;
        }
        else if(healthPoint_A==0)
        {
            result=false;
            break;
        }
    }
    return procedures;
}

QStringList Fighting::generateInfo(QList<movement> procedures)
{   QStringList Infos;

    QString init=tr("决斗开始:\n你的HP:%1,对方的HP:%2").arg(1).arg(2);
    for(auto proc:procedures)
    {
        QString tmp;
        QString act=proc.active=='A'?"你":"对方";
        QString pas=proc.active=='A'?"对方":"你";
        tmp+=act;
        tmp+="使出了"+MovetoString[proc.method]+".";
        if(proc.critical)
            tmp+="会心一击!";
        else if(proc.miss)
            tmp+="但是被"+pas+"灵敏地躲开了!";
        if(proc.damage)
            tmp+=tr("造成%1点伤害.").arg(proc.damage);
        if(proc.recovery>0)
            tmp+=tr("回复%1点生命").arg(proc.recovery);
        else if(proc.recovery<0)
            tmp+=tr("反嗜%1点生命").arg(proc.recovery);
        if(proc.extraDamage)
            tmp+=tr("异常状态损失%1点生命").arg(proc.extraDamage);
        if(proc.active=='A')
        {
            if(proc.stateB[Burning]==3)
                tmp+="对方着火了.";
            else if(proc.stateB[Toxic]==3)
                tmp+="对方中毒了.";
            else if(proc.stateB[Confused]==3)
                tmp+="对方混乱了.";
        }
        else
        {
            if(proc.stateA[Burning]==3)
                tmp+="你着火了.";
            else if(proc.stateA[Toxic]==3)
                tmp+="你中毒了.";
            else if(proc.stateA[Confused]==3)
                tmp+="你混乱了.";
        }
        tmp+=tr("你的HP:%1,对方的HP:%2.").arg(proc.HP_A).arg(proc.HP_B);
        if(proc.HP_A==0&&proc.HP_B==0)
            tmp+="平局.";
        else if(proc.HP_A==0)
            tmp+="你挂了.";
        else if(proc.HP_B==0)
            tmp+="你赢了.";
        tmp+="\n";
        Infos.append(tmp);
    }
    return Infos;
}

void Fighting::exceptionFade(char index)
{
    if(index=='A')
        for(auto s:stateA)
             if(s)
                 s--;
    else if(index=='B')
        for(auto s:stateB)
            if(s)
                s--;
}
