#include <QApplication>

#include <QDebug>
#include <QTime>
#include <QGraphicsView>

#include <QSplashScreen>

#include "sqlratascene.h"
#include "rataview.h"
#include "ratalaite.h"

#include "ratapalvelin.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen(QPixmap(":/r/pic/tausta.png"));
    splash->show();
    splash->showMessage("Ladataan rataa", Qt::AlignBottom | Qt::AlignCenter, Qt::white);

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
    splash->finish(&view);
    delete splash;
    return a.exec();
}
