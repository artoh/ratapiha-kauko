/**************************************************************************
**  kulkutienraide.cpp
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
**  KulkutienRaide  13.7.2012
**************************************************************************/

#include "kulkutienraide.h"

#include "kulkutie.h"
#include "kulkutieelementti.h"


KulkutienRaide::KulkutienRaide(RataRaide *raide, RaiteenPaa *lahtoOpastin, RataRaide *lahtoRaide, bool pohjoiseenko, int moneskoraide, KulkuTie *kulkutie)
    : raide_(raide), lahtoOpastin_(lahtoOpastin), lahtoRaide_(lahtoRaide), pohjoiseenko_(pohjoiseenko), moneskoRaide_(moneskoraide), kulkutie_(kulkutie)
{
}


KulkutienRaide::KulkutienRaide(KulkutieElementti* elementti, KulkuTie *kulkutie)
    : kulkutie_(kulkutie)
{
    raide_ = elementti->raide();
    lahtoOpastin_ = elementti->lahtoOpastin();
    lahtoRaide_ = elementti->lahtoraide();
    pohjoiseenko_ = elementti->onkoPohjoiseen();
    moneskoRaide_ = elementti->taso();
}

bool KulkutienRaide::operator <(const KulkutienRaide &toinen) const
{
    return moneskoRaide_<toinen.moneskoRaide();
}

QString KulkutienRaide::lahtoOpastinTunnus()
{
    QChar suuntakirjain;
    if( lahtoOpastin() == lahtoRaide()->etelainen())
        suuntakirjain = 'E';
    else
        suuntakirjain= 'P';
    return QString("%1%2%3").arg(suuntakirjain).arg(lahtoRaide()->liikennepaikka()).arg(lahtoRaide()->raidetunnus(),3,10,QChar('0'));
}


QString KulkutienRaide::kulkutieto()
{
    if( kulkutie()->kulkutienTyyppi() == RataRaide::EiKulkutieta)
        return QString();

    QChar suuntakirjain;
    QChar kulkutiekirjain;

    if( onkoPohjoiseen())
        suuntakirjain = QChar('P');
    else
        suuntakirjain = QChar('E');

    if( kulkutie()->kulkutienTyyppi() == RataRaide::Junakulkutie)
        kulkutiekirjain = QChar('J');
    else if( kulkutie()->kulkutienTyyppi() == RataRaide::Vaihtokulkutie)
        kulkutiekirjain = QChar('U');

    return QString("%1%2%3 %4 %5").arg(kulkutiekirjain).arg(suuntakirjain).arg(moneskoRaide()).arg(kulkutie()->maaliRaideTunnus()).arg(lahtoOpastinTunnus());
}


