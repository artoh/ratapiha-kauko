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

#include "ratascene.h"
#include "rataikkuna.h"

#include <QMutableListIterator>

KulkuTie::KulkuTie(RaideTieto::Kulkutietyyppi kulkutientyyppi) :
    kulkutienTyyppi_( kulkutientyyppi )
{
}

KulkuTie::~KulkuTie()
{
    foreach( KulkutienRaide* ktraide, elementit_)
        delete ktraide;
}

KulkutienRaide* KulkuTie::lisaaElementti(KulkutieElementti *elementti)
{
    KulkutienRaide* ktraide = new KulkutienRaide(elementti, this);
    lisaaListaan(ktraide);
    return ktraide;
}

KulkutienRaide *KulkuTie::lisaaElementti(RataRaide *raide, RaiteenPaa::Suunta suunta, QString lahtoOpastin, int moneskoraide)
{

    KulkutienRaide* ktraide = new KulkutienRaide(raide, suunta, lahtoOpastin, moneskoraide, this);
    lisaaListaan(ktraide);

    return ktraide;
}

void KulkuTie::puraKulkutie()
{
    foreach( KulkutienRaide* ktraide, elementit_)
    {
        ktraide->puraKulkutielta();
    }
    elementit_.clear(); // Poistetaan pointterit
}

void KulkuTie::poistaElementti(KulkutienRaide *elementti)
{
    elementit_.removeOne(elementti);
    delete elementti;
}



QString KulkuTie::maaliRaideTunnus()
{
    if( elementit_.empty())
        return QString();

    return elementit_.last()->raide()->raidetunnusLiikennepaikalla();
}

QString KulkuTie::maaliRaideTunnusSuunnalla()
{
    if( elementit_.empty())
        return QString();

    QChar suuntakirjain;
    if( elementit_.last()->onkoEtelaan())
        suuntakirjain = 'E';
    else
        suuntakirjain= 'P';

    return QString("%1%2").arg(suuntakirjain).arg(elementit_.last()->raide()->raidetunnusLiikennepaikalla());
}


void KulkuTie::lisaaListaan(KulkutienRaide *elementti)
{
    QMutableListIterator<KulkutienRaide*> i(elementit_);

    while( i.hasNext() )
    {
        if(i.next()->moneskoRaide() > elementti->moneskoRaide() )
        {
            // Lisätään paikalleen ennen isompaa
            i.previous();
            i.insert(elementti);
            return;
        }
    }
    // Ollaan lopussa, lisätään loppuun
    i.insert(elementti);
}

RataRaide *KulkuTie::maaliRaide()
{
    if( elementit_.empty())
        return 0;
    return elementit_.last()->raide();
}