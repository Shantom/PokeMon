#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

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
