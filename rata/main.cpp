#include <QApplication>

#include <QDebug>
#include <QTime>
#include <QGraphicsView>

#include <QSplashScreen>

#include "sqlratascene.h"
#include "ratalaite.h"

#include "rataikkuna.h"
#include "ratapalvelin.h"
#include "vaunu.h"
#include "moottori.h"


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen(QPixmap(":/r/pic/tausta.png"));
    splash->show();
    splash->showMessage("Ladataan rataa", Qt::AlignBottom | Qt::AlignCenter, Qt::white);

    QTime t;
     t.start();

    SqlRataScene *skene = new SqlRataScene(0);
    Ratalaite::asetaSkene(skene);
    Vaunu::alustaRenderoija();

    a.processEvents();

    skene->lataaRata();
    RataPalvelin palvelin(skene);
    if( !palvelin.listen(QHostAddress::Any, 5432))
        qDebug() << "Virhe palvelun käynnistyksessä " << palvelin.errorString();

    RataIkkuna *ikkuna = new RataIkkuna(skene);
    ikkuna->show();

    skene->asetaNopeus(1);

    qDebug("Time elapsed: %d ms", t.elapsed());
    splash->finish(ikkuna);

//    skene->asetaAjoPoyta(2400,Veturi::AJOPOYTA_EDESSA);
//    skene->veturi(2400)->moottori()->asetaTavoiteNopeusKmh(80);

    delete splash;
    return a.exec();
}
