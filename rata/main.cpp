#include <QApplication>

#include <QDebug>
#include <QTime>
#include <QGraphicsView>

#include "sqlratascene.h"
#include "rataview.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QTime t;
     t.start();

qDebug() << QCoreApplication::libraryPaths();

    SqlRataScene *skene = new SqlRataScene(0);
    skene->lataaRata();
    RataView view(skene);
    view.show();
    view.ensureVisible(0.0,0.0,10.0,10.0);

    qDebug("Time elapsed: %d ms", t.elapsed());
    return a.exec();
}
