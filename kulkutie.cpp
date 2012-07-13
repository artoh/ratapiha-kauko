/**************************************************************************
**  kulkutie.cpp
**  Copyright (c) 2012 Arto Hyvättinen 
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
**  Kulkutie  13.7.2012
**************************************************************************/

#include "kulkutie.h"
#include "kulkutienraide.h"

#include "kulkutieelementti.h"

#include <QMutableListIterator>

KulkuTie::KulkuTie() :
    kulkutienTyyppi_( RataRaide::EiKulkutieta)
{
}

KulkutienRaide* KulkuTie::lisaaElementti(KulkutieElementti *elementti, RaideTieto::Kulkutietyyppi tyyppi)
{
    if( kulkutienTyyppi() == RataRaide::EiKulkutieta)
        kulkutienTyyppi_ = tyyppi;

    KulkutienRaide* ktraide = new KulkutienRaide(elementti, this);
    lisaaListaan(ktraide);
    return ktraide;
}

KulkutienRaide* KulkuTie::lisaaElementti(const QString &kulkutieto)
{
    // Eipä vielä osaa lisätä kannasta...
    return 0;
}

QString KulkuTie::maaliRaideTunnus()
{
    if( elementit_.empty())
        return QString();

    QChar suuntakirjain;
    if( elementit_.last()->onkoEtelaan())
        suuntakirjain = 'E';
    else
        suuntakirjain= 'P';

    return QString("%1%2%3").arg(suuntakirjain).arg(elementit_.last()->raide()->liikennepaikka()).arg(elementit_.last()->raide()->raidetunnus(),3,10,QChar('0'));
}


void KulkuTie::lisaaListaan(KulkutienRaide *elementti)
{
    QMutableListIterator<KulkutienRaide*> i(elementit_);

    while( i.hasNext() && i.next()->moneskoRaide() < elementti->moneskoRaide() )
    {;}

    if( i.hasNext())    // Ei olla lopussa
        i.previous();

    i.insert(elementti);
}

RataRaide *KulkuTie::maaliRaide()
{
    if( elementit_.empty())
        return 0;
    return elementit_.last()->raide();
}
