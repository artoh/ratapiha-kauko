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

#include "asetinlaite.h"
#include "suoranraiteenpaa.h"


Asetinlaite::Asetinlaite(QObject *parent) :
    QObject(parent), simulaatioAika_(0)
{

}

void Asetinlaite::sanomaAsetinlaitteelta(unsigned int sanoma)
{
    // Ensimmäisenä t8estataan, onko kyse ajasta
    if( !(sanoma & 0x80000000))
    {
        // Aikasanoma
        simulaatioAika_ = sanoma & 0x7fffffff;
        emit simulaatioAikaMuutos(simulaatioAika());
    }
}

void Asetinlaite::lahetaSanoma(int laite, int komento)
{
    unsigned int sanoma = 0xf0000000 | ( komento << 20) | laite ;
    emit sanomaAsetinlaitteelle(sanoma);
}

void Asetinlaite::lahetaSanoma(int raide, int laite, int komento)
{
    unsigned int sanoma = 0xf0000000 | ( komento << 20) | ( raide << 4 )| laite ;
    emit sanomaAsetinlaitteelle(sanoma);
}

void Asetinlaite::yhdistettyRataan(bool onko)
{
    // Tässä pitäisi alustaa hakemalla tilat asetinlaitteelta,
    // mutta eikös tässä vain tehdäkin niin, että
    // laitetaan opastimille värejä ???
    if(onko)
    {

        QHashIterator<int,RaideTieto*> iter(raiteet_);
        while(iter.hasNext())
        {
            iter.next();
            RaiteenPaa* paa = iter.value()->raiteenPaa(RaiteenPaa::P);
            if( paa )
            {
                Opastin* opastin = paa->opastin();
                if( opastin )
                    opastin->asetaOpaste(Opastin::AJA);
            }
        }
    }
}

void Asetinlaite::rekisteroiInstanssi(Asetinlaite *instanssi)
{
    instanssi__ = instanssi;
}

Asetinlaite* Asetinlaite::instanssi__ = 0;


