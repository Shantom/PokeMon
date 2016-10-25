#include "mainwindow.h"
#include <QApplication>
#include "pokemon.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Server");


    w.show();

    return a.exec();
}
