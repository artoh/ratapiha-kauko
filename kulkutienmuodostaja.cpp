/**************************************************************************
**  kulkutienmuodostaja.cpp
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
**  KulkutienMuodostaja  12.7.2012
**************************************************************************/

#include "kulkutienmuodostaja.h"
#include "kulkutie.h"
#include "ratascene.h"
#include "rataikkuna.h"

#include <QLineF>

KulkutienMuodostaja::KulkutienMuodostaja(RataRaide::Kulkutietyyppi tyyppi, RataRaide *mista, RataRaide *minne, RaiteenPaa::Suunta lahtosuunta)
    : tyyppi_(tyyppi), mista_(mista), minne_(minne),
      pieninNopeus_(0),  lyhinReitti_(0), lahtoSuunta_(lahtosuunta)
{
}

void KulkutienMuodostaja::reittiLoytynyt(KulkutieElementti *reitinViimeinenElementti)
{
    lyhinReitti_ = reitinViimeinenElementti;
    lyhinPituus_ = reitinViimeinenElementti->pituus();
    pieninNopeus_ = reitinViimeinenElementti->pieninNopeus();
}

bool KulkutienMuodostaja::muodostaKulkutie()
{

   lyhinPituus_ = 12000; // Testikoodia, jatkossa lasketaan etäisyyksistä


    // Tehdään elementtien muodostaminen
    QList<Naapuruus*> naapurit = mista_->naapurit();
    QList<KulkutieElementti*> elementit;

    // Raiteelta, jonne on junakulkutie varatulle raiteelle, ei saa muodostaa kulkutietä
    if( mista_->kulkutieTyyppi() == RataRaide::Varattukulkutie )
        return false;


    foreach( Naapuruus* naapuri, naapurit)
        if( naapuri->naapuriRaide())
        {
            RaiteenPaa* lahtoPaa;

            // Jos on määritelty lähtösuunta, seurataan vain sitä
            if( lahtoSuunta_ == RaiteenPaa::Etelaan && naapuri->omaSuunta() == RaiteenPaa::Pohjoiseen )
                continue;
            else if( lahtoSuunta_ == RaiteenPaa::Pohjoiseen && naapuri->omaSuunta() == RaiteenPaa::Etelaan)
                continue;

            if( naapuri->omaSuunta() == RaiteenPaa::Pohjoiseen)
                lahtoPaa = mista_->pohjoinen();
            else
                lahtoPaa = mista_->etelainen();

            // Pitää olla sopiva aloittava opastin!
            if( ( ( kulkutienTyyppi() == RataRaide::Junakulkutie || kulkutienTyyppi() == RataRaide::Varattukulkutie ) && ( lahtoPaa->opastin() == RaiteenPaa::PaaOpastin ||
                    lahtoPaa->opastin() == RaiteenPaa::RaideOpastin) ) ||
                    ( kulkutienTyyppi() == RataRaide::Vaihtokulkutie && lahtoPaa->opastin() != RaiteenPaa::EiOpastinta) )
                elementit.append(new KulkutieElementti(this, 0, naapuri, lahtoPaa, mista_));
        }


    // Nyt pitäisi olla elementit lukittavaksi
    if( lyhinReitti_)
    {
        KulkuTie* kulkutie = new KulkuTie(tyyppi_);

        lyhinReitti_->lukitseKulkutielle(kulkutie);
        lyhinReitti_->laitaVarit(this);
        mista_->paivita();
        minne_->naapuritTarkistakaaKulkutiet(kulkutie);

        RataIkkuna::rataSkene()->kulkutieValmis(minne_->raidetunnusLiikennepaikalla(), kulkutie);
        RataScene::turvaLoki(3001, QString("%1 %2-%3").arg(kulkutie->kulkutienTyyppi())
                             .arg(kulkutie->lahtoRaideTunnus()).arg(kulkutie->maaliRaideTunnus()));
    }


    return( lyhinReitti_ != 0);
}

