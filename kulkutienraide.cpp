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


#include <QDebug>

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
    else if( kulkutie()->kulkutienTyyppi() == RataRaide::Linjasuojastus)
        kulkutiekirjain = QChar('S');

    QChar tilateksti;
    switch( kulkutie()->tila())
    {
    case RaideTieto::Valmis:
        tilateksti = QChar('-');
        break;
    case RaideTieto::Varattu:
        tilateksti = QChar('+');
        break;
    default:
        tilateksti = QChar('!');
    }


    return QString("%1%2%3%4 %5 %6").arg(kulkutiekirjain).arg(suuntakirjain).arg(tilateksti).arg(moneskoRaide()).arg(kulkutie()->maaliRaideTunnusSuunnalla() ).arg(lahtoOpastinTunnus());
}

void KulkutienRaide::puraKulkutielta()
{
    RataRaide* raidePtr = raide();
    // Opastin punaiselle
    if( lahtoOpastin() && lahtoOpastin()->opasteKasite() != RaiteenPaa::Seis)
    {
        lahtoOpastin()->asetaOpaste(RaiteenPaa::Seis);
        lahtoRaide()->paivita();
    }

    // Ensimmäisenä poistaa raiteelta
    raidePtr->kulkutiePurettu();
    // Sitten listalta
    kulkutie()->poistaElementti(this);
    // Huom! Nyt on tuhottu!!!!!!!!

    raidePtr->naapuritTarkistakaaKulkutiet();
    // Kulkutien purkautuminen voi purkaa suojastuksen..
}

void KulkutienRaide::tarkistaNaapurinPyynnosta(KulkuTie *pyytaja)
{
    if( pyytaja != kulkutie())
        kulkutie()->tarkista();

}

void KulkutienRaide::raideVarautuu(RaiteenPaa::Suunta suunta)
{
    // Ekana opastin punaiselle!!
    if( lahtoOpastin()->opaste() != RaiteenPaa::Seis)
    {
        lahtoOpastin()->asetaOpaste(RaiteenPaa::Seis);
        lahtoRaide()->paivita();
    }

    // Tarkastetaan, ollaanko tultu oikeasta suunnasta!!
    if( suunta != suunta_)
    {
        kulkutie()->vikatilaan();
        qDebug() << "Vikatila: Suunta " << raide()->raidetunnusLiikennepaikalla() << " " << kulkutieto();
    }
    else
        kulkutie()->raideVarautuu(this);
}

void KulkutienRaide::raideVapautuu(RaiteenPaa::Suunta suunta)
{
    if( kulkutie()->kulkutienTyyppi() == RataRaide::Linjasuojastus)
    {
        kulkutie()->tarkista();
    }
    else if( kulkutie()->tila() == RataRaide::Varattu)
    {
        if( (suunta == suunta_ && kulkutie()->ekaRaide() == this) || raide()==kulkutie()->maaliRaide() )
        {
            // Jos vapautuva raide on kulkutien ensimmäinen, niin kaikki on hyvin ja voi purkautua
            // Myös maaliraide purkautuu aina, mihin suuntaan tahansa
            // (pitäisi itse asiassa purkautua aikaviiveellä vaikkei olisi tyhjä)
            puraKulkutielta();
        }
        else
        {
            // Muuten ollaan vikatilassa!!!
            kulkutie()->vikatilaan();
            qDebug() << "Vikatila: Purkautuminen " << raide()->raidetunnusLiikennepaikalla() << " " << kulkutieto();

        }
    }

}


