#include <QApplication>

#include <QDebug>
#include <QTime>
#include <QGraphicsView>

#include "sqlratascene.h"
#include "rataview.h"
#include "ratalaite.h"

#include "ratapalvelin.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QTime t;
     t.start();

qDebug() << QCoreApplication::libraryPaths();

    SqlRataScene *skene = new SqlRataScene(0);
    Ratalaite::asetaSkene(skene);

    skene->lataaRata();
    RataView view(skene);

    RataPalvelin palvelin(skene);
    if( !palvelin.listen(QHostAddress::Any, 5432))
        qDebug() << "Virhe palvelun käynnistyksessä " << palvelin.errorString();


    view.show();
    view.ensureVisible(0.0,0.0,10.0,10.0);

    skene->asetaNopeus(5);

    qDebug("Time elapsed: %d ms", t.elapsed());
    return a.exec();
}
