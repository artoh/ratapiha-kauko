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

#include "rataikkuna.h"


KulkutienRaide::KulkutienRaide(RataRaide *raide, RaiteenPaa::Suunta suunta, QString lahtoOpastin, int moneskoraide, KulkuTie *kulkutie)
    : raide_(raide), suunta_(suunta), lahtoOpastinTunnus_(lahtoOpastin),
      moneskoRaide_(moneskoraide), kulkutie_(kulkutie)
{
}


KulkutienRaide::KulkutienRaide(KulkutieElementti* elementti, KulkuTie *kulkutie)
    : kulkutie_(kulkutie)
{
    raide_ = elementti->raide();

    QChar suuntakirjain;
    if( elementti->lahtoOpastin() == elementti->lahtoRaide()->etelainen())
        suuntakirjain = 'E';
    else
        suuntakirjain= 'P';

    lahtoOpastinTunnus_ = QString("%1%2").arg(suuntakirjain).arg( elementti->lahtoRaide()->raidetunnusLiikennepaikalla());


    if( elementti->onkoPohjoiseen())
        suunta_ = RaiteenPaa::Pohjoiseen;
    else
        suunta_ = RaiteenPaa::Etelaan;

    moneskoRaide_ = elementti->taso();

    kulkutie_ = kulkutie;
}

bool KulkutienRaide::operator <(const KulkutienRaide &toinen) const
{
    return moneskoRaide_<toinen.moneskoRaide();
}

RaiteenPaa *KulkutienRaide::lahtoOpastin()
{
    return RataIkkuna::rataSkene()->haeRaiteenPaa(lahtoOpastinTunnus());
}

RataRaide *KulkutienRaide::lahtoRaide()
{
    return RataIkkuna::rataSkene()->haeRaide(lahtoOpastinTunnus().mid(1));
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

    return QString("%1%2%3 %4 %5").arg(kulkutiekirjain).arg(suuntakirjain).arg(moneskoRaide()).arg(kulkutie()->maaliRaideTunnusSuunnalla() ).arg(lahtoOpastinTunnus());
}

void KulkutienRaide::puraKulkutielta()
{
    // Opastin punaiselle
    if( lahtoOpastin()->opasteKasite() != RaiteenPaa::Seis)
    {
        lahtoOpastin()->asetaOpaste(RaiteenPaa::Seis);
        lahtoRaide()->paivita();
    }

    // Ensimmäisenä poistaa raiteelta
    raide()->kulkutiePurettu();
    // Sitten listalta
    kulkutie()->poistaElementti(this);
    // Huom! Nyt on tuhottu!!!!!!!!
}


