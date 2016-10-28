#include "mainwindow.h"
#include "logindialog.h"
#include "users.h"
#include <QApplication>
#include "pokemon.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginDialog login;
    MainWindow *w;
    if(login.exec()==QDialog::Accepted)
    {
        w=new MainWindow(login.getName(),login.getPort());
        Users users;
        w->setWindowTitle("Client");
        w->setAttribute(Qt::WA_DeleteOnClose);
        users.setWindowTitle("Users");
        QWidget::connect(w,SIGNAL(openUsers()),&users,SLOT(on_OpenUsers()));
        w->show();
        return a.exec();
    }
    else
        return 0;
}
