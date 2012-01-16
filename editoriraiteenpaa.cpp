#include "editoriraiteenpaa.h"

EditoriRaiteenPaa::EditoriRaiteenPaa(const QString &tilatieto)
    : paanTyyppi_(Suora), paaOpastin_(false), raideOpastin_(false),
      suojastusOpastin_(false)
{
    if( tilatieto.contains("RP") )
        paanTyyppi_ = RaidePuskuri;
    else if( tilatieto.contains("V"))
        paanTyyppi_ = Vaihde;
    else if( tilatieto.contains("RR"))
        paanTyyppi_ = RaideRisteys;

    if( tilatieto.contains("Po"))
        paaOpastin_ = true;
    if( tilatieto.contains("Ro"))
        raideOpastin_ = true;
    if( tilatieto.contains("So"))
        suojastusOpastin_ = true;

}


QString EditoriRaiteenPaa::tilaTieto() const
{
    QString tila;
    if( paanTyyppi() == Vaihde)
        tila.append("V- ");
    else if(paanTyyppi() == RaideRisteys)
        tila.append("RR- ");
    else if(paanTyyppi() == RaidePuskuri)
        tila.append("RP ");

    if( paaOpastin())
        tila.append("Po ");
    if( raideOpastin())
        tila.append("Ro ");
    if( suojastusOpastin())
        tila.append("So ");

    return tila;

}

void EditoriRaiteenPaa::asetaPaanTyyppi(PaanTyyppi tyyppi)
{
    paanTyyppi_ = tyyppi;
}

void EditoriRaiteenPaa::asetaPaaOpastin(bool onko)
{
    paaOpastin_ = onko;
}

void EditoriRaiteenPaa::asetaRaideOpastin(bool onko)
{
    raideOpastin_ = onko;
}

void EditoriRaiteenPaa::asetaSuojastusOpastin(bool onko)
{
    suojastusOpastin_ = onko;
}
