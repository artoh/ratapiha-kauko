
#include <QApplication>
#include <QLabel>
#include <QTime>
#include <QDebug>

#include "sqlasetinlaite.h"
#include "asetinlaitepaneeli.h"

#include "ratayhteys.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);  

    QTime t;
    t.start();

    SqlAsetinlaite asetinlaite;
    Asetinlaite::rekisteroiInstanssi(&asetinlaite);
    asetinlaite.lataaRata();
 
    RataYhteys yhteys;


    AsetinlaitePaneeli paneeli;

    QObject::connect( &yhteys, SIGNAL(sanomaSaapunut(unsigned int)),
                      &asetinlaite, SLOT(sanomaAsetinlaitteelta(unsigned int)));
    QObject::connect( &yhteys, SIGNAL(yhdistettyRataan(bool)),
                      &paneeli, SLOT(yhdistettyRataan(bool)));
    QObject::connect( &asetinlaite, SIGNAL(simulaatioAikaMuutos(int)),
                      &paneeli, SLOT(ajanPaivitys(int)));
    QObject::connect( &asetinlaite, SIGNAL(sanomaAsetinlaitteelle(uint)),
                      &yhteys, SLOT(lahetaSanoma(uint)));
    QObject::connect( &yhteys, SIGNAL(yhdistettyRataan(bool)),
                      &asetinlaite, SLOT(yhdistettyRataan(bool)));
    QObject::connect( &asetinlaite, SIGNAL(kulkutiemaaraMuutos(int)),
                      &paneeli, SLOT(kulkutiemaaraPaivitys(int)));
    QObject::connect( &asetinlaite, SIGNAL(asiakasMaaraMuutos(int)),
                      &paneeli, SLOT(asiakasmaaraPaivitys(int)));

    paneeli.show();

    asetinlaite.kaynnistaPalvelin(6543);

    qDebug("Time elapsed: %d ms", t.elapsed());

    return a.exec();  
}
