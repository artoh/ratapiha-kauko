#include "raidetieto.h"

RaideTieto::RaideTieto() :
    pohjoinen_(), etelainen_(),
    sahkoistetty_(true), valvottu_(true),
    akseleita_(0), junanumero_(0),
    kulkutie_(EiKulkutieta)
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
    if( tila.contains("UK"))
        kulkutie_ = Vaihtokulkutie;

    etelainen_.paivitaTila(etelatila);
    pohjoinen_.paivitaTila(pohjoistila);
}

bool RaideTieto::onkoVaihde()
{
    return ( etelainen()->paanTyyppi() == RaiteenPaa::Vaihde ||
             pohjoinen()->paanTyyppi() == RaiteenPaa::Vaihde);
}
