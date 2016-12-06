#ifndef POKEMON_H
#define POKEMON_H

#include <QMainWindow>
#include <QObject>
#include <QDebug>
#include <QWidget>
#include <vector>
#include <QTime>
#include <QList>

//using namespace std;
const double CommonRate=0.8;
const double RareRate=1.0;
const double EpicRate=1.2;
const double LegendaryRate=1.4;

const double StrengthRate=1.3;
const double ShieldRate=1.8;
const double DefenseRate=1.3;
const double AgilityRate=1.3;

const double AttackGain=5;
const double DefenceGain=3.5;
const double HealthGain=15;
const double SpeedGain=60;

const int ordAttack=100;
extern const QList<int> expToLvUp;
extern const double RarityGainRate[4];
extern const char* PMType_toString[];
extern const char* PMRarity_toString[];
extern const QList<QString> LimitBreak_toString;

enum PMType
{
    Strength, Defense, Shield, Agility
};

enum PMRarity
{
    Common, Rare, Epic, Legendary
};

enum LimitBreak
{
    FireSpin,//火焰漩涡120%，灼烧
    TakeDown,//猛撞180.，反噬
    WaterPulse,//水波动，混乱20
    PoisonJab,//毒刺，毒
    LeechLife,//吸血
    Aromatherapy,//芳香治疗,回血,去除异常
    AirCutter,//破空斩,暴击
    FurySwipes,//疯狂乱抓,连续攻击2～5次,
    //1⁄6的几率攻击5或4次，1⁄3的几率攻击3或2次，数学期望是3.168次
    OrdAttack//普通攻击
};

class PokeMon : public QObject
{
    Q_OBJECT
public:
//    explicit PokeMon(QObject *parent = 0);
    PokeMon();
    PokeMon(PMRarity rarity);
    PokeMon(PokeMon &src);
    ~PokeMon();
    void gainExp(int newExp);
    virtual void levelUp();
    virtual int move() = 0;
    void rename(QString newName);
    virtual QString getInfomation();

//private:
    PMType type;
    PMRarity rarity;
    LimitBreak limitBreak;
    QString name;
    int level;
    int exp;
    double attack;
    double defence;
    double maxHealth;
    double speed;
    int id;

    unsigned seed;
signals:

public slots:
};

#endif // POKEMON_H
