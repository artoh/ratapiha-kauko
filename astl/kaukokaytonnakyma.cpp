#include "kaukokaytonnakyma.h"

KaukokaytonNakyma::KaukokaytonNakyma(const QString &nimi)
    : nakymaNimi_(nimi)
{

}

KaukokaytonNakyma::~KaukokaytonNakyma()
{

}

void KaukokaytonNakyma::lisaaTeksti(int x, int y, const QString &teksti)
{
    nakymaTeksti_.append(QString("T %1 %2 %3").arg(x).arg(y).arg(teksti));
}

void KaukokaytonNakyma::lisaaKisko(RaideTieto *raide, int etelaX, int etelaY, int pohjoinenX, int pohjoinenY, int kiskotietokoodi)
{
    QString teksti = QString("K %1 %2 %3 %4 %5 %6").arg(raide->raideTunnusTeksti()).arg(etelaX)
            .arg(etelaY).arg(pohjoinenX).arg(pohjoinenY).arg(kiskokoodi);

    raiteet_.append(raide);
}



