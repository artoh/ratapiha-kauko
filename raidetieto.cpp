#include "raidetieto.h"

RaideTieto::RaideTieto() :
    pohjoinen_(), etelainen_(),
    sahkoistetty_(true), valvottu_(true),
    akseleita_(0), junanumero_(0)
{
}


void RaideTieto::paivita(int akseleita, int junanumero, const QString &tila, const QString& etelatila, const QString &pohjoistila)
{
    akseleita_ = akseleita;
    junanumero_ = junanumero;

    if( tila.contains("SäEi"))
        sahkoistetty_ = false;
    if( tila.contains("ValvEi"))
        valvottu_ = false;

    etelainen_.paivitaTila(etelatila);
    pohjoinen_.paivitaTila(pohjoistila);
}


