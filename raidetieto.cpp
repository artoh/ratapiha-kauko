#include "raidetieto.h"

RaideTieto::RaideTieto() :
    pohjoinen_(), etelainen_(),
    sahkoistetty_(true), valvottu_(true),
    akseleita_(0),
    kulkutietyyppi_(EiKulkutieta)
{
}


void RaideTieto::paivita(int akseleita, const QString &junanumero, const QString &tila, const QString& etelatila, const QString &pohjoistila, const QString& kulkutietila)
{
    akseleita_ = akseleita;
    junanumero_ = junanumero;

    if( tila.contains("SäEi"))
        sahkoistetty_ = false;
    if( tila.contains("ValvEi"))
        valvottu_ = false;

    if( kulkutietila.startsWith('J'))
        kulkutietyyppi_ = Junakulkutie;
    else if(kulkutietila.startsWith('U'))
        kulkutietyyppi_ = Vaihtokulkutie;
    else
        kulkutietyyppi_ = EiKulkutieta;

    etelainen_.paivitaTila(etelatila);
    pohjoinen_.paivitaTila(pohjoistila);
}

bool RaideTieto::onkoVaihde()
{
    return ( etelainen()->paanTyyppi() == RaiteenPaa::Vaihde ||
             pohjoinen()->paanTyyppi() == RaiteenPaa::Vaihde);
}
