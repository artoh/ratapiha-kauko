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


#include "linjasuojastus.h"
#include "opastin.h"
#include "raidetieto.h"

LinjaSuojastus::LinjaSuojastus() :
    vastaSuunta_(0),
    tila_(Ratapiha::SUOJASTUS_EIMAARITELTY),
    suojastusValmis_(false)
{
}

LinjaSuojastus *LinjaSuojastus::muodostaLinjaSuojastus(SuoranRaiteenPaa *ekaPaa)
{
    // Jos mahdollista, muodostaa linjasuojastuksen sanotusta päästä alkaen
    // Ensimmäisessä elementissä pitää olla suojastus eteen ja muu
    // pääopastin taakse
    if( !ekaPaa->opastin() || (ekaPaa->opastin()->tyyppitieto() & Ratapiha::OPASTIN_SUOJASTUS) == 0)
        return 0;

    SuoranRaiteenPaa *vastapaa = dynamic_cast<SuoranRaiteenPaa*>(ekaPaa->aktiivinenVastapaa());
    if( !vastapaa)
        return 0;

    if( !vastapaa->opastin() || (vastapaa->opastin()->tyyppitieto() & Ratapiha::OPASTIN_SUOJASTUS))
        return 0;

    // Nyt jatketaan eteenpäin
    QList<SuoranRaiteenPaa*> lista;
    lista.append(ekaPaa);

    while( true )
    {
        if( !lista.last()->seuraavaRaiteenpaa() )
            return 0;
        SuoranRaiteenPaa *seuraava = dynamic_cast<SuoranRaiteenPaa*>(lista.last()->seuraavaRaiteenpaa());
        if( !seuraava || !seuraava->opastin())
            return 0;
        lista.append(seuraava);
        if( (seuraava->opastin()->tyyppitieto() & Ratapiha::OPASTIN_SUOJASTUS) == 0 )
            break;  // Lista on valmis
    }

    LinjaSuojastus *uusi = new LinjaSuojastus;
    uusi->linja_ = lista;

    if( vastapaa->suojastus() != 0)
    {
        uusi->vastaSuunta_ = vastapaa->suojastus();
        vastapaa->suojastus()->vastaSuunta_ = uusi;
    }

    foreach (SuoranRaiteenPaa *paa, lista)
    {
        paa->asetaSuojastus(uusi);
    }
    return uusi;
}

QChar LinjaSuojastus::tilaKirjain() const
{

    if( tila()==Ratapiha::SUOJASTUS_VALMIS)
        return 'i';
    else if( tila()==Ratapiha::SUOJASTUS_VARATTU)
        return 'I';

    else if( vastaSuunta_)
    {
        if( vastaSuunta_->tila()==Ratapiha::SUOJASTUS_VALMIS)
            return 'o';
        else if( vastaSuunta_->tila()==Ratapiha::SUOJASTUS_VARATTU)
            return 'O';
    }
    return QChar('s');
}

QChar LinjaSuojastus::tilaKirjain(SuoranRaiteenPaa *paa) const
{
    if( paa == linja_.first())
        return tilaKirjain();
    else if( tila()==Ratapiha::SUOJASTUS_VALMIS || tila()==Ratapiha::SUOJASTUS_VARATTU)
        return 'S';
    else
        return QChar(' ');
}

bool LinjaSuojastus::voikoSuojastaa(SuoranRaiteenPaa *paa)
{
    if( paa != linja_.first())
        return false;

    // Suojastus on mahdollinen, jos liikenteen suunta on oikea tai määrittelemätön, eikä suojastuksen alueella ole kulkuteitä eikä raiteita varattu
    if( !vastaSuunta_)
        return false;
    if( vastaSuunta_->tila()==Ratapiha::SUOJASTUS_VALMIS || vastaSuunta_->tila()==Ratapiha::SUOJASTUS_VARATTU)
        return false;

    for(int i=1; i < linja_.count(); i++)
    {
        if( linja_[i]->raide()->kulkutie())
            return false;       // Linjaa ei hyväksytä, jos kulkutie aktiivinen
        if( tila_ == Ratapiha::SUOJASTUS_EIMAARITELTY && linja_[i]->raide()->vapaanaOlo() != Ratapiha::RAIDE_VAPAA)
            return false;       // Uutta suojastusta ei hyväksytä, jos linjalla on akseleita
    }

    return true;
}

bool LinjaSuojastus::suojasta(SuoranRaiteenPaa *paa)
{
    if( !voikoSuojastaa(paa))
        return false;
    if( tila_ == Ratapiha::SUOJASTUS_EIMAARITELTY)
        tila_ = Ratapiha::SUOJASTUS_VALMIS;
    laitaVarit();
    suojastusValmis_ = true;
    return true;
}

void LinjaSuojastus::laitaVarit()
{
    for(int i=0; i < linja_.count()-1;i++)
    {
        if( linja_[i+1]->raide()->vapaanaOlo() == Ratapiha::RAIDE_VAPAA)
            linja_[i]->opastin()->asetaOpaste(Ratapiha::OPASTE_AJA);
    }
}

void LinjaSuojastus::raideVarautuu(RaideTieto *raide)
{
    if( tila_ == Ratapiha::SUOJASTUS_EIMAARITELTY)
        return;     // Suojastus ehkä toiseen suuntaan


    int indeksi = -1;
    for( int i=0; i < linja_.count(); i++)
    {
        if( linja_[i]->raide() == raide )
        {
            indeksi = i;
            break;
        }
    }

    if( tila_ == Ratapiha::SUOJASTUS_VALMIS )
        tila_ = Ratapiha::SUOJASTUS_VARATTU;

    if( indeksi < 0)
        return;     // Raidetta ei löydy
    else if( indeksi == 0 )
    {
        suojastusValmis_ = false;
    }
    else if( indeksi > 0)
        linja_.at(indeksi-1)->opastin()->asetaOpaste(Ratapiha::OPASTE_SEIS);

    // Ensin pitäisi tarkistaa, että mennään oikeaan suuntaan
}

void LinjaSuojastus::raideVapautuu(RaideTieto *raide)
{
    if( tila_ == Ratapiha::SUOJASTUS_EIMAARITELTY)
        return;     // Suojastus ehkä toiseen suuntaan

    int indeksi = -1;   // Tämän raiteen indeksi
    int varattuja = 0;  // Montako raidetta on varattuina
    int ekavarattu = 9999;  // Ensimmäisen varatun raiteen indeksi

    for( int i=0; i < linja_.count(); i++)
    {
        if( linja_[i]->raide() == raide )
        {
            indeksi = i;
        }
        if( linja_[i]->raide()->vapaanaOlo() == Ratapiha::RAIDE_VARATTU)
        {
            varattuja++;
            if( i < ekavarattu)
                ekavarattu = i;
        }
    }

    // Jos koko linja jää tyhjäksi, puretaan suojastus
    if( !varattuja )
    {
        if( suojastusValmis_)
            tila_ = Ratapiha::SUOJASTUS_VALMIS;
        else
            tila_ = Ratapiha::SUOJASTUS_EIMAARITELTY;
    }

    // Jos linjalle on tulossa juna TAI linjalla on jo juna tämän takana, annetaan aja-opaste
    if( indeksi > 0 && (ekavarattu < indeksi || suojastusValmis_))
        linja_.at(indeksi-1)->opastin()->asetaOpaste(Ratapiha::OPASTE_AJA);
}

void LinjaSuojastus::heratePurkautuu()
{
    if( tila_ == Ratapiha::SUOJASTUS_VALMIS)
    {
        tila_ = Ratapiha::SUOJASTUS_EIMAARITELTY;
        for(int i=0; i<linja_.count()-1; i++)
            linja_[i]->opastin()->asetaOpaste(Ratapiha::OPASTE_SEIS);
    }
}
