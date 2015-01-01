/**************************************************************************
**
**  Ratapiha
**
**  Copyright (c) 2012-2015 Arto Hyvättinen
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  See <http://www.gnu.org/licenses/>
**
**
**************************************************************************/

#include <QDebug>

#include "raidetieto.h"

#include "raide.h"
#include "vaihde.h"
#include "risteysvaihde.h"
#include "raideristeys.h"
#include "kulkutie.h"

RaideTieto::RaideTieto() :
    kulkutie_(0)
{
}

void RaideTieto::asetaTiedot(int raideId, const QString &liikennepaikka, int raideTunnus, int raidepituus, bool sahkoistetty, bool valvottu)
{
    raideId_ = raideId;
    liikennepaikka_ = liikennepaikka;
    raideTunnus_ = raideTunnus;
    raidepituus_ = raidepituus;
    sahkoistetty_ = sahkoistetty;
    valvottu_ = valvottu;
}

bool RaideTieto::voikoLukitaKulkutielle(Ratapiha::KulkutieTyyppi /* tyyppi */)
{
    return( onkoLukittuKulkutielle() == Ratapiha::ELEMENTTI_VAPAA && !onkoAjonestoa() && !kulkutie());
}

void RaideTieto::asetinLaiteSanoma(int laite, int sanoma)
{
    qDebug() <<  raideTunnusTeksti() << " laite" << laite << " sanoma " << sanoma;
    if( laite == 0xf)
    {
        // Vapaanaolon valvontaa koskeva sanoma
        if( sanoma == 0x81)
            vapaanaOlo_ = VARATTU;
        else if( sanoma == 0x82)
            vapaanaOlo_ = VAPAA;
        else
            vapaanaOlo_ = VIRHE;
        // Sitten tehdään vapaanaoloon liittyvät herätteet

    }
    else
    {
        // Delegoidaan alaluokalle
        laiteSanoma(laite, sanoma);
    }
}

void RaideTieto::laiteSanoma(int /*laite*/, int /*sanoma*/)
{
    // Tyhjä runko
}

RaideTieto *RaideTieto::luoRaide(RaideTyyppi tyyppi)
{
    switch (tyyppi) {
    case RAIDE:
        return new Raide();
    case VAIHDE:
        return new Vaihde();
    case RISTEYSVAIHDE:
        return new RisteysVaihde();
    case RAIDERISTEYS:
        return new RaideRisteys();
    default:
        return 0;
    }

}

QString RaideTieto::raideTunnusTeksti() const
{
    return QString("%1%2").arg(liikennepaikka() ).arg( raidetunnus() ,3,10,QChar('0'));
}

QString RaideTieto::raideInfo() const
{
    QString info = raideTunnusTeksti() + " " + QString::number(raideId()) + "\n";
    if( vapaanaOlo() == VARATTU)
        info += "VARATTU ";
    else if( vapaanaOlo() == VIRHE)
        info += "VALVONTAVIRHE";
    return info;
}

QString RaideTieto::raideTila()
{
    QString tila = raideTunnusTeksti() + " ";

    switch (raideTyyppi())
    {
        case RAIDE: tila.append('-'); break;
        case VAIHDE: tila.append('v'); break;
        case RISTEYSVAIHDE: tila.append('x'); break;
        case RAIDERISTEYS: tila.append('r'); break;
        default: break;
    }

    switch (vapaanaOlo())
    {
        case VARATTU: tila.append('X'); break;
        case VAPAA: tila.append('O'); break;
        default: tila.append('!'); break;
    }

    if( kulkutie())
    {
        if( kulkutie()->tyyppi() == Ratapiha::JUNAKULKUTIE )
            tila.append("J");

        if( onkoLukittuKulkutielle() == Ratapiha::ELEMENTTI_LUKITTU)
            tila.append("L");
        else if( onkoLukittuKulkutielle() == Ratapiha::ELEMENTTI_LUKITAAN)
            tila.append('l');
    }

    return tila;
}

Ratapiha::ElementinLukitus RaideTieto::onkoLukittuKulkutielle()
{
    if( kulkutie())
        return Ratapiha::ELEMENTTI_LUKITTU;
    else
        return Ratapiha::ELEMENTTI_VAPAA;
}

void RaideTieto::lukitseKulkutielle(Kulkutie *kulkutie, RaiteenPaa * /*mista*/, RaiteenPaa * /*minne*/)
{
    // Perustilanteessa, eli ellei ole mitään käännettäviä elementtejä,
    // sijoitetaan vain osaksi kulkutietä
    kulkutie_ = kulkutie;
}
