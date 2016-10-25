#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>
#include "pokemon.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString username;
    quint16 port;
    MainWindow w;
    w.setWindowTitle("Client");
    LoginDialog login;
    if(login.exec()==QDialog::Accepted)
    {
        w.setName(login.getName());
        w.setPort(login.getPort());

        w.show();
        return a.exec();

    }
    else
        return 0;
}
