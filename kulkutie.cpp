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
#include <QDebug>

KulkuTie::KulkuTie(RaideTieto::Kulkutietyyppi kulkutientyyppi) :
    kulkutienTyyppi_( kulkutientyyppi ), tila_(RaideTieto::Valmis)
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

KulkutienRaide *KulkuTie::lisaaElementti(RataRaide *raide, RaiteenPaa::Suunta suunta, const QString& lahtoOpastin, int moneskoraide )
{

    KulkutienRaide* ktraide = new KulkutienRaide(raide, suunta, lahtoOpastin, moneskoraide, this);
    lisaaListaan(ktraide);

    return ktraide;
}

KulkutienRaide *KulkuTie::lisaaElementti(const QString &raidesuunnalla, const QString &lahtoopastin)
{
    RataRaide* raide = RataIkkuna::rataSkene()->haeRaide( raidesuunnalla.mid(1));
    RaiteenPaa::Suunta suunta = RaiteenPaa::Etelaan;
    if( raidesuunnalla.startsWith('P'))
        suunta = RaiteenPaa::Pohjoiseen;
    int moneskoraide = elementit_.count() + 1;

    KulkutienRaide* ktraide = new KulkutienRaide(raide, suunta, lahtoopastin, moneskoraide, this);
    elementit_.append(ktraide);
    return ktraide;
}

void KulkuTie::puraKulkutie()
{
    if( kulkutienTyyppi_ == RaideTieto::EiKulkutieta)
        return;

    kulkutienTyyppi_ = RaideTieto::EiKulkutieta;
    RataIkkuna::rataSkene()->poistaKulkutieListalta( maaliRaideTunnus() );

    foreach( KulkutienRaide* ktraide, elementit_)
    {
        ktraide->puraKulkutielta();
    }
    elementit_.clear(); // Poistetaan pointterit

}

void KulkuTie::poistaElementti(KulkutienRaide *elementti)
{
    QString maaliraidetunnus = maaliRaideTunnus();
    elementit_.removeOne(elementti);
    delete elementti;
    if( elementit_.empty())
        RataIkkuna::rataSkene()->poistaKulkutieListalta(maaliraidetunnus);
}

void KulkuTie::vahvistaKulkutie()
{
    foreach( KulkutienRaide* elementti, elementit_)
    {
        elementti->raide()->lukitseKulkutielle(elementti);
        elementti->raide()->paivita();
    }
    RataIkkuna::rataSkene()->kulkutieValmis(maaliRaideTunnus(), this);
    tarkista();
}

void KulkuTie::tarkista()
{
    if( tila() == RaideTieto::Virhetila )
        return;     // Jos vikatilassa, niin vikatilassa kunnes puretaan !

    if( kulkutienTyyppi() == RataRaide::Linjasuojastus )
    {
        QSet<QString> punaisetOpastimet;
        QSet<QString> vihreatOpastimet;
        // Suojastuksen tarkistaminen
        // Tarkistaa kaikki suojastuksen raiteet
        foreach( KulkutienRaide* ktraide, elementit_)
        {
            if( ktraide->raide()->akseleita())
            {
                // On varattu raide, opastimen pitää olla punainen
                punaisetOpastimet.insert( ktraide->lahtoOpastinTunnus() );
            }
        }

        // Jos edelliselle raiteelle junakulkutie, niin varaudutaan junan tuloon
        bool junaTulossa = lahtoRaide()->kulkutieTyyppi() == RataRaide::Junakulkutie;

        // Jos junaa ei ole tulossa, eikä yhtään raidetta varattu, joutaa kulkutien purkaa
        if( punaisetOpastimet.empty() && !junaTulossa)
            puraKulkutie();
        else
        {
            // Asetetaan opastimet alusta loppuun niin, että varatusta eteenpäin
            // ei-punaiset ovat vihreitä
            bool vihreaAalto = junaTulossa;

            foreach( KulkutienRaide* ktraide, elementit_)
            {
                if( ktraide->raide()->akseleita())
                    vihreaAalto = true; // Tästä eteenpäin tarttee vihreän aallon

                if( vihreaAalto && !punaisetOpastimet.contains( ktraide->lahtoOpastinTunnus() ) )
                    vihreatOpastimet.insert( ktraide->lahtoOpastinTunnus());
                else
                    punaisetOpastimet.insert( ktraide->lahtoOpastinTunnus());
            }
            // Opastimien asettaminen
            foreach( QString opastintunnus, vihreatOpastimet)
                RataIkkuna::rataSkene()->haeRaiteenPaa(opastintunnus)->asetaOpaste(RaiteenPaa::Aja);
            foreach( QString opastintunnus, punaisetOpastimet)
                RataIkkuna::rataSkene()->haeRaiteenPaa(opastintunnus)->asetaOpaste(RaiteenPaa::Seis);
        }
    }

    if( varattujaRaiteita() )
        tila_ = RataRaide::Varattu;
    else
        tila_ = RataRaide::Valmis;

    paivitaKaikki();

}

void KulkuTie::raideVarautuu(KulkutienRaide* elementti)
{
    if( tila_ == RataRaide::Valmis)
        tila_ = RataRaide::Varattu;

    if( kulkutienTyyppi() != RataRaide::Linjasuojastus)
    {
        // Kaikkien ennen tätä pitää olla varattuja!
        foreach( KulkutienRaide* ktraide, elementit_)
        {
            if( !ktraide->raide()->akseleita() && ktraide->raide()->pituus() > 200 )
            {
                // Ei ole varattu oikein
                vikatilaan();
                qDebug() << "Vikatila: Väärä varautuminen " << elementti->kulkutieto();
                return;
            }
            if( ktraide == elementti)
                break;  // Meni aivan oikein!!
        }
    }
    else
        tarkista();
}

int KulkuTie::varattujaRaiteita()
{
    int varattuja = 0;
    foreach( KulkutienRaide* elementti, elementit_)
        if( elementti->raide()->akseleita())
            varattuja++;
    return varattuja;
}


RataRaide *KulkuTie::lahtoRaide()
{
    if( elementit_.empty())
        return 0;

    return elementit_.first()->lahtoRaide();

}

QString KulkuTie::lahtoRaideTunnus()
{
    if( elementit_.empty())
        return QString();

    return elementit_.first()->lahtoOpastinTunnus().mid(1);
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

void KulkuTie::paivitaKaikki()
{
    if( elementit_.count())
        lahtoRaide()->paivita();

    foreach( KulkutienRaide* ktraide, elementit_)
    {
        ktraide->raide()->paivita();
    }
}

KulkutienRaide *KulkuTie::ekaRaide()
{
    if( elementit_.empty())
        return 0;
    return elementit_.first();
}

void KulkuTie::vikatilaan()
{
    tila_ = RataRaide::Virhetila;

    // Vikatilassa, kaikki opastimet punaiselle!
    foreach( KulkutienRaide* ktraide, elementit_)
    {
        if(ktraide->lahtoOpastin()->opasteKasite() != RaiteenPaa::Seis )
            ktraide->lahtoOpastin()->asetaOpaste(RaiteenPaa::Seis);
    }

    paivitaKaikki();
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
