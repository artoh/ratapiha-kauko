#include "raidetieto.h"

RaideTieto::RaideTieto() :
    pohjoinen_(), etelainen_(),
    sahkoistetty_(true), valvottu_(true),
    akseleita_(0),
    kulkutietyyppi_(EiKulkutieta),
    kulkutietila_(Virhetila),
    suunta_ (RaiteenPaa::Virhe)
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
    else if(kulkutietila.startsWith('S'))
        kulkutietyyppi_ = Linjasuojastus;
    else
        kulkutietyyppi_ = EiKulkutieta;

    etelainen_.paivitaTila(etelatila);
    pohjoinen_.paivitaTila(pohjoistila);

    if( kulkutieTyyppi() != EiKulkutieta)
    {
        if( kulkutietila[1]=='E')
            suunta_ = RaiteenPaa::Etelaan;
        else if( kulkutietila[1]=='P')
            suunta_ = RaiteenPaa::Pohjoiseen;

        if( kulkutietila[2]==QChar('-'))
            kulkutietila_ = Valmis;
        else if( kulkutietila[3]==QChar('+'))
            kulkutietila_ = Varattu;
        else
            kulkutietila_ = Virhetila;

    }
}

bool RaideTieto::onkoVaihde()
{
    return ( etelainen()->paanTyyppi() == RaiteenPaa::Vaihde ||
             pohjoinen()->paanTyyppi() == RaiteenPaa::Vaihde);
}
