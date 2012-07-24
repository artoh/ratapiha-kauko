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
    raiteenSulku_(SpPuuttuu),
    automaatio_(EiAutomaatiota)
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
    else if(paanTyyppi() == Vaihde && vaihde() == Aukiajettu)
        tila.append("V! ");

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

    if( automaatioTila() == AutomaatioKaytossa)
        tila.append("Auto ");
    else if( automaatioTila() == AutomaatioAktiivinen )
        tila.append("AutoAktiivinen ");
    else if( automaatioTila() == AutomaatioViive)
        tila.append("AutomaatioViive");
    else if( automaatioTila() == Lapikulku)
        tila.append("Läpikulku");
    else if( automaatioTila() == LapikulkuAktiivinen )
        tila.append("LäpikulkuAktiivinen");
    else if( automaatioTila() == LapikulkuViive )
        tila.append("LäpikulkuViive");

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

void RaiteenPaa::aukiaja()
{
    vaihdeTila_ = Aukiajettu;
}

bool RaiteenPaa::aukiajonKaanto()
{
    if( vaihde() == Aukiajettu)
    {
        vaihdeTila_ = Vasen;
        return true;
    }
    return false;
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

void RaiteenPaa::asetaAutomaationTila(RaiteenPaa::Automaatio automaatio)
{
    automaatio_ = automaatio;
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
        if(lause == "V-")
            vaihdeTila_ = Vasen;
        else if( lause == "V+")
            vaihdeTila_ = Oikea;
        else if( lause == "V!")
            vaihdeTila_ = Aukiajettu;
    }
    else if( lause.startsWith("Auto"))
    {
        if( lause == "AutoViive")
            automaatio_ = AutomaatioViive;
        else if( lause == "AutoAktiivinen")
            automaatio_ = AutomaatioAktiivinen;
        else
            automaatio_ = AutomaatioKaytossa;
    }
    else if( lause.startsWith("Läpikulku"))
    {
        if( lause == "LäpikulkuViive")
            automaatio_ = LapikulkuViive;
        else if( lause == "LäpikulkuAktiivinen")
            automaatio_ = LapikulkuAktiivinen;
        else
            automaatio_ = Lapikulku;
    }

}


void RaiteenPaa::paivitaTila(const QString &tila)
{
    QStringList listana = tila.split(" ");
    opastinSeis_ = false;   // Oletuksena opastimella ei SEIS-asetusta
    opaste_ = RaiteenPaa::Seis; // Oletuksena opastin näyttää SEIS
    automaatio_ = EiAutomaatiota;   // Oletuksena ei kulkutieautomaatiota
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

QChar RaiteenPaa::suuntakirjain(RaiteenPaa::Suunta suunta)
{
    if(suunta == Etelaan)
        return QChar('E');
    else if( suunta == Pohjoiseen )
        return QChar('P');
    return QChar();
}
