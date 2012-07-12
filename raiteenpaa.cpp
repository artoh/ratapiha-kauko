/**************************************************************************
** (c) Arto Hyvättinen 2012
** Gnu Public Licence 2 - No Warranty !!!
** 19.2.2011
**
** Ratapiha - kauko
**************************************************************************/

#include "raiteenpaa.h"

#include <QStringList>

RaiteenPaa::RaiteenPaa() :
    paanTyyppi_(Suora), vaihdeTila_(EiVaihdetta),
    opastinTyyppi_(EiOpastinta), opaste_(Seis), raiteenSulku_(SpPuuttuu)
{

}

QString RaiteenPaa::tilaTieto() const
{
    QString tila;

    if( paanTyyppi() == Vaihde && vaihde()==Vasen)
        tila.append("V- ");
    else if(paanTyyppi() == Vaihde && vaihde()==Oikea)
        tila.append("V+");

    if( paanTyyppi() == RaideRisteys && vaihde() == Vasen )
        tila.append("RR- ");
    else if(paanTyyppi()==RaideRisteys && vaihde() == Oikea)
        tila.append("RR+ ");

    if( opastin() == PaaOpastin)
        tila.append("Po ");
    else if(opastin() == SuojastusOpastin )
        tila.append("So ");
    else if(opastin() == RaideOpastin)
        tila.append("Ro ");


    switch( opaste() )
    {
    case Seis:
        tila.append("Seis "); break;
    case Aja:
        tila.append("Aja "); break;
    case AjaSn:
        tila.append("AjaSn "); break;
    case AjaVarovasti:
        tila.append("AjaVarovasti "); break;
    case EiOpastetta:
        tila.append("EiOpastetta "); break;
    default:
        break;
    }



    if( raiteenSulku() == SpSallii)
        tila.append("Sp- ");
    else if(raiteenSulku() == SpSulkee)
        tila.append("Sp+ ");

    if( paanTyyppi() == RaidePuskuri)
        tila.append("RP ");


    return tila;
}

bool RaiteenPaa::kaannaVaihde()
{
    if( paanTyyppi()==Vaihde || paanTyyppi()==RaideRisteys)
    {
        // Tässä pitäisi tarkistaa lukitus...
        if( vaihde()==Vasen)
            vaihdeTila_=Oikea;
        else if(vaihde()==Oikea)
            vaihdeTila_=Vasen;
        return true;
    }
    return false;   // Eipä onnistunut kiäntää tai viäntää
}


void RaiteenPaa::paivitysTehtava(const QString &lause)
{
    if( lause == "Po")
    {
        opastinTyyppi_ = PaaOpastin;
    }
    else if( lause == "Ro" && opastin() != PaaOpastin)
    {
        opastinTyyppi_ = RaideOpastin;
    }
    else if( lause == "So")
    {
        opastinTyyppi_ = SuojastusOpastin;
    }
    else if( lause == "Seis")
    {
        opaste_ = Seis;
    }
    else if( lause == "AjaVarovasti")
    {
        opaste_ = AjaVarovasti;
    }
    else if( lause == "AjaSn")
    {
        opaste_ = AjaSn;
    }
    else if( lause == "Aja")
    {
        opaste_ = Aja;
    }
    else if( lause == "EiOpastetta")
    {
        opaste_ = EiOpastetta;
    }
    else if( lause.startsWith("Sp"))
    {
        if( lause.startsWith("Sp-"))
            raiteenSulku_ = SpSallii;
        else
            raiteenSulku_ = SpSulkee;
    }
    else if( lause.startsWith("RP"))
        paanTyyppi_ = RaidePuskuri;
    else if( lause.startsWith("RR"))
    {
        paanTyyppi_ = RaideRisteys;
        if( lause.startsWith("RR-"))
            vaihdeTila_ = Vasen;
        else if(lause.startsWith("RR+"))
            vaihdeTila_ = Oikea;
    }
    else if( lause.startsWith("V"))
    {
        paanTyyppi_ = Vaihde;
        if(lause.startsWith("V-"))
            vaihdeTila_ = Vasen;
        else if( lause.startsWith("V+"))
            vaihdeTila_ = Oikea;
    }
}


void RaiteenPaa::paivitaTila(const QString &tila)
{
    QStringList listana = tila.split(" ");
    foreach( QString kohde, listana)
    {
        paivitysTehtava(kohde);
    }
}
