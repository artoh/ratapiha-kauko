/**************************************************************************
** (c) Arto Hyvättinen 2012
** Gnu Public Licence 2 - No Warranty !!!
** 19.2.2011
**
** Ratapiha - kauko
**************************************************************************/

#include "raiteenpaa.h"
#include "rataraide.h"

#include <QStringList>

RaiteenPaa::RaiteenPaa(const QString &tila) :
    paanTyyppi_(Suora), vaihdeTila_(EiVaihdetta),
    opastinTyyppi_(EiOpastinta), opaste_(Seis),
    opastinSeis_(false),
    raiteenSulku_(SpPuuttuu)
{
    paivitaTila(tila);
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

    if( opastinSeis())
        tila.append("SEIS ");


    return tila;
}

bool RaiteenPaa::kaannaVaihde()
{
    if( vaihde()==Vasen)
        return kaannaVaihde(Oikea);
    else if(vaihde()==Oikea)
        return kaannaVaihde(Vasen);
    return false;
}

bool RaiteenPaa::kaannaVaihde(RaiteenPaa::VaihdeKasite tilaan)
{
    if( paanTyyppi() == Vaihde || paanTyyppi() == RaideRisteys )
    {
        // Tässä pitäisi tarkistaa lukitukset sun muut!
        vaihdeTila_ = tilaan;
        return true;
    }
    return false;
}

bool RaiteenPaa::lukitseVaihde(RaiteenPaa::VaihdeKasite tilaan)
{
    // Myöhemmin tässä myös haetaan sivusuojia sun muita!!!
    return kaannaVaihde(tilaan);
}




void RaiteenPaa::asetaPaanTyyppi(PaanTyyppi tyyppi)
{
    paanTyyppi_ = tyyppi;
}

void RaiteenPaa::asetaRaiteenSulku(bool onko)
{
    if( onko && raiteenSulku() == SpPuuttuu)
        raiteenSulku_ = SpSallii;
    else if(!onko)
        raiteenSulku_ = SpPuuttuu;
}

void RaiteenPaa::asetaOpastin(RaiteenPaa::OpastinTyyppi opastintyyppi)
{
    opastinTyyppi_ = opastintyyppi;
}

void RaiteenPaa::asetaOpaste(RaiteenPaa::Opaste opaste)
{
    opaste_ = opaste;

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
    else if( lause == "SEIS")
    {
        opastinSeis_ = true;    // Liikenteenohjaus asettanut SEIS-käsitteen!
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
    opastinSeis_ = false;   // Oletuksena opastimella ei SEIS-asetusta
    foreach( QString kohde, listana)
    {
        paivitysTehtava(kohde);
    }
}

RaiteenPaa::Opaste RaiteenPaa::opaste() const
{
    // Jos asetettu liikenteenohjauksessa SEIS, niin sitten SEIS
    if( opastinSeis())
        return Seis;
    return opaste_;
}

void RaiteenPaa::seis()
{
    opastinSeis_ = true;
}

void RaiteenPaa::aja()
{
    opastinSeis_ = false;
}
