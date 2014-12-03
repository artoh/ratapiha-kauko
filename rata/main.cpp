#include <QApplication>

#include <QDebug>
#include <QTime>
#include <QGraphicsView>

#include "ratascene.h"
#include "laitekeskus.h"
#include "rataview.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QTime t;
     t.start();

qDebug() << QCoreApplication::libraryPaths();

    RataScene skene;
    skene.lataaRata();
    RataView view(&skene);
    view.show();
    view.ensureVisible(0.0,0.0,10.0,10.0);

    qDebug("Time elapsed: %d ms", t.elapsed());
    qDebug() << Laitekeskus::laitetta() << " laitetta ";
    return a.exec();
}
