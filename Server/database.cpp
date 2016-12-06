#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
Database::Database()
{
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
    {
        db = QSqlDatabase::addDatabase("QMYSQL"); // 使用mysql数据库驱动
        db.setHostName("localhost");
//        db.setHostName("121.42.209.206");
        db.setDatabaseName("PokeMonServer"); // 数据库名称
        db.setUserName("root"); //  用户名
        db.setPassword("sushe322"); // 密码
    }
    bool ok = db.open(); // 尝试连接数据库
    if(!ok)
        exit(0);
}

bool Database::login(QString username, QString password)
{
    QSqlQuery query;
    query.exec(QString("SELECT * FROM users WHERE username = '%1' "
                       "AND password = '%2'").arg(username,password));
    if(query.next())
        return true;
    else
        return false;
}

bool Database::signup(QString username, QString password)
{
    QSqlQuery query;
    query.exec(QString("SELECT * FROM users WHERE username = '%1'").arg(username));
    if(query.next())
        return false;
    else
    {
        query.prepare( "INSERT INTO users (username, password) values(?,?)");
        query.addBindValue(QVariant(username));
        query.addBindValue(QVariant(password));
        query.exec();
        return true;
    }

}

QStringList Database::allUsers()
{
    QSqlQuery query;
    QList<QString> users;
    query.exec(QString("SELECT username FROM users "));
    while(query.next())
    {
        users.append(query.value(0).toString());
    }
    return users;

}

QList<PokeMon *> Database::pmsOfUser(QString username)
{
    QSqlQuery query;
    QList<PokeMon *> monsters;
    PokeMon *tmpPM;
    query.exec(QString("SELECT level, type, rarity, attack, defence, maxHealth,"
                       " speed, exp, limitbreak, name, id FROM monsters where owner='%1'").arg(username));
    while(query.next())
    {
        PMType type=(PMType)query.value(1).toInt();
        switch (type) {
        case Strength:
            tmpPM=new PMStrength();
            break;
        case Defense:
            tmpPM=new PMDefense();
            break;
        case Shield:
            tmpPM=new PMShield();
            break;
        case Agility:
            tmpPM=new PMAgility();
            break;
        default:
            break;
        }
        tmpPM->type=type;
        tmpPM->rarity=(PMRarity)(int)query.value(2).toInt();
        tmpPM->attack=query.value(3).toDouble();
        tmpPM->defence=query.value(4).toDouble();
        tmpPM->maxHealth=query.value(5).toDouble();
        tmpPM->speed=query.value(6).toDouble();
        tmpPM->exp=query.value(7).toInt();
        tmpPM->limitBreak=(LimitBreak)(int)query.value(8).toInt();
        tmpPM->name=query.value(9).toString();
        tmpPM->level=query.value(0).toInt();
        tmpPM->id=query.value(10).toInt();

        monsters.append(tmpPM);
    }
    return monsters;
}

void Database::addPokeMon(QString owner, PokeMon *pm)
{
    QSqlQuery query;
    query.prepare("INSERT INTO monsters (owner, level, type, rarity, attack, defence, maxHealth,"
                  " speed, exp, limitbreak, name) values (?,?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(QVariant(owner));
    query.addBindValue(QVariant(pm->level));
    query.addBindValue(QVariant((int)pm->type));
    query.addBindValue(QVariant((int)pm->rarity));
    query.addBindValue(QVariant(pm->attack));
    query.addBindValue(QVariant(pm->defence));
    query.addBindValue(QVariant(pm->maxHealth));
    query.addBindValue(QVariant(pm->speed));
    query.addBindValue(QVariant(pm->exp));
    query.addBindValue(QVariant((int)pm->limitBreak));
    query.addBindValue(QVariant(pm->name));
    query.exec();
    qDebug()<<query.lastError().text();
}

void Database::deletePokeMon(int id)
{
    QSqlQuery query;
    query.exec(QString("DELETE from monsters where id=%1").arg(id));

}

PokeMon *Database::pmAt(int id)
{
    QSqlQuery query;
    PokeMon *tmpPM;
    query.exec(QString("SELECT level, type, rarity, attack, defence, maxHealth,"
                       " speed, exp, limitbreak, name, id FROM monsters where id='%1'").arg(id));
    if(query.next())
    {
        PMType type=(PMType)query.value(1).toInt();
        switch (type) {
        case Strength:
            tmpPM=new PMStrength();
            break;
        case Defense:
            tmpPM=new PMDefense();
            break;
        case Shield:
            tmpPM=new PMShield();
            break;
        case Agility:
            tmpPM=new PMAgility();
            break;
        default:
            break;
        }
        tmpPM->type=type;
        tmpPM->rarity=(PMRarity)(int)query.value(2).toInt();
        tmpPM->attack=query.value(3).toDouble();
        tmpPM->defence=query.value(4).toDouble();
        tmpPM->maxHealth=query.value(5).toDouble();
        tmpPM->speed=query.value(6).toDouble();
        tmpPM->exp=query.value(7).toInt();
        tmpPM->limitBreak=(LimitBreak)(int)query.value(8).toInt();
        tmpPM->name=query.value(9).toString();
        tmpPM->level=query.value(0).toInt();
        tmpPM->id=query.value(10).toInt();

    }
    return tmpPM;

}

void Database::updatePM(int id, PokeMon *pm)
{
    QSqlQuery query;

    query.exec(QString("UPDATE monsters SET level=?, attack=?, defence=?,"
                       " maxHealth=?, speed=?, exp=?, name=? where id='%1'").arg(id));
    query.addBindValue(QVariant(pm->level));
    query.addBindValue(QVariant(pm->attack));
    query.addBindValue(QVariant(pm->defence));
    query.addBindValue(QVariant(pm->maxHealth));
    query.addBindValue(QVariant(pm->speed));
    query.addBindValue(QVariant(pm->exp));
    query.addBindValue(QVariant(pm->name));
    query.exec();
    qDebug()<<query.lastError().text();

}
