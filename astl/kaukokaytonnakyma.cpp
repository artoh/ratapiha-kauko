#include "kaukokaytonnakyma.h"

KaukokaytonNakyma::KaukokaytonNakyma(const QString &nimi)
    : nakymaNimi_(nimi)
{

}

KaukokaytonNakyma::KaukokaytonNakyma(int nakymaId, const QString &nimi, const QString &tekstit)
    : nakymaId_(nakymaId), nakymaNimi_(nimi), nakymaTeksti_(tekstit)
{

}

KaukokaytonNakyma::~KaukokaytonNakyma()
{

}

void KaukokaytonNakyma::lisaaRaide(RaideTieto *raide)
{
    raiteet_.append(raide);
}

