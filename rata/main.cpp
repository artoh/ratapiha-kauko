#include <QApplication>

#include <QDebug>
#include <QGraphicsView>

#include "ratascene.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

qDebug() << QCoreApplication::libraryPaths();

    RataScene skene;
    skene.lataaRata();
    QGraphicsView view(&skene);
    view.show();
    view.ensureVisible(0.0,0.0,10.0,10.0);


    return a.exec();
}
