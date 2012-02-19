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
    paanTyyppi_(Suora), vaihdeTila_(EiVaihdetta), paaOpastin_(Puuttuu),
    raideOpastin_(Puuttuu), suojastusOpastin_(Puuttuu), raiteenSulku_(SpPuuttuu)
{

}


void RaiteenPaa::paivitysTehtava(const QString &lause)
{
    if( lause.startsWith("Po"))
    {
        // Pääopastin
        if( lause.startsWith("PoSn"))
            paaOpastin_ = AjaVarovasti;
        else if( lause.startsWith("PoAja"))
            paaOpastin_ = Aja;
        else
            paaOpastin_ = Seis;
    }
    else if( lause.startsWith("Ro"))
    {
        if( lause.startsWith("RoAja"))
            raideOpastin_ = AjaVarovasti;
        else
            raideOpastin_ = Seis;
    }
    else if( lause.startsWith("So"))
    {
        if(lause.startsWith("SoVarovasti"))
            suojastusOpastin_ = AjaVarovasti;
        else if(lause.startsWith("SoAja"))
            suojastusOpastin_ = Aja;
        else
            suojastusOpastin_ = Seis;
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
