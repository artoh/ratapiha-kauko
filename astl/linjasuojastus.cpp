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
    tila_(Ratapiha::SUOJASTUS_EIMAARITELTY)
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

    // Suojastus on mahdollinen, jos liikenteen suunta on oikea tai määrittelemätön, eikä suojastuksen alueella ole kulkuteitä
    if( !vastaSuunta_)
        return false;
    if( vastaSuunta_->tila()==Ratapiha::SUOJASTUS_VALMIS || vastaSuunta_->tila()==Ratapiha::SUOJASTUS_VARATTU)
        return false;

    for(int i=1; i < linja_.count(); i++)
    {
        if( linja_[i]->raide()->kulkutie())
            return false;       // Linjaa ei hyväksytä, jos kulkutie aktiivinen
    }

    return true;
}

bool LinjaSuojastus::suojasta(SuoranRaiteenPaa *paa)
{
    if( !voikoSuojastaa(paa))
        return false;
    if( tila_ == Ratapiha::SUOJASTUS_EIMAARITELTY)
        tila_ = Ratapiha::SUOJASTUS_VALMIS;
    return true;
}
