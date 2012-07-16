/**************************************************************************
**  kulkutieelementti.cpp
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
**  KulkutieElementti  12.7.2012
**************************************************************************/

#include "kulkutieelementti.h"
#include "kulkutienmuodostaja.h"
#include "kulkutie.h"

#include <QDebug>

KulkutieElementti::KulkutieElementti(KulkutienMuodostaja *muodostaja, KulkutieElementti *vanhempi, Naapuruus *naapuri, RaiteenPaa* opastin, RataRaide* lahtoraide)
    :  naapuruus_(naapuri), opastin_(opastin), lahtoraide_(lahtoraide), isi_(vanhempi), taso_(1)
{

    // Taso
    if( vanhempi)
        taso_ = vanhempi->taso() + 1;

    qDebug() << naapuri->naapuriRaide()->raidetunnus();


    // Se suunta, johon tutkittavaa (naapurin) raidetta pitkin kuljetaan
    if( naapuri->naapurinSuunta() == Naapuruus::Etela)
        naapurinPaa_ = naapuri->naapuriRaide()->pohjoinen();
    else
        naapurinPaa_ = naapuri->naapuriRaide()->etelainen();

    // 1A) Onko hitaampi??? Valitsee aina nopeamman kiskon, vaikka pidempi!
    if( naapuri->pieninNopeus() < muodostaja->pieninNopeus())
        return;  // Liian hidas!

    // Otetaan pienin nopeus muistiin
    if( vanhempi && vanhempi->pieninNopeus() < naapuri->pieninNopeus() )
        pieninNopeus_ = vanhempi->pieninNopeus();
    else
        pieninNopeus_ = naapuri->pieninNopeus();

    // 1) Lisätään pituus
    if( vanhempi)
        pituus_ = vanhempi->pituus() + naapuri->naapuriRaide()->pituus();
    else
        pituus_ = naapuri->naapuriRaide()->pituus();

    if( pituus_ > muodostaja->lyhinPituus())
        return;     // Ei tarvitse jatkaa

    // 2) Tarkistetaan ehdot
    if( !tarkistaKulkutieEhdot(muodostaja) )
        return;

    // 3) Jos ollaan maaliraiteella, tarkastellaan
    if( naapuruus_->naapuriRaide() == muodostaja->minne())
    {
        maalissaOllaan(muodostaja);
        return;
    }
    // 3b) Lisäehdot keskellä oleville


    // 4) Jatketaan seuraaviin

    // Jos tässä on ollut opastin, niin se jatkossa eteenpäin
    RaiteenPaa* uusiopastin = opastin;
    RataRaide* uusiLahtoraide = lahtoraide;
    if( naapurinPaa()->opastin() != RaiteenPaa::EiOpastinta )
    {
        uusiopastin = naapurinPaa();
        uusiLahtoraide = naapuri->naapuriRaide();
    }


    QList<Naapuruus*> naapurit = naapuruus_->naapuriRaide()->naapurit();
    foreach( Naapuruus* uusinaapuri, naapurit)
    {
        if( uusinaapuri->naapuriRaide() && uusinaapuri->omaSuunta() != naapuruus_->naapurinSuunta() )
        {
            // Mennään toiseen suuntaan eli eteenpäin
            lapset_.append(new KulkutieElementti(muodostaja, this, uusinaapuri, uusiopastin, uusiLahtoraide));
        }
    }


}


KulkutieElementti::~KulkutieElementti()
{
    foreach(KulkutieElementti* lapsi, lapset_)
        delete lapsi;
}

void KulkutieElementti::lukitseKulkutielle(KulkuTie* kulkutie)
{
    // Lukitsee ensin tämän elementin kulkutielle
    // Jos vaihteita, kääntää ne
    naapuruus_->lukitseVaihteet();

    // Lisää tämän olennon kulkutielle ;)
    naapuruus_->naapuriRaide()->lukitseKulkutielle( kulkutie->lisaaElementti(this) );


    // sitten isin
    if( isi_)
        isi_->lukitseKulkutielle(kulkutie);
}

void KulkutieElementti::laitaVarit(KulkutienMuodostaja *kulkutie)
{

    if( kulkutie->kulkutienTyyppi() == RataRaide::Vaihtokulkutie)
    {
        if( opastin_->opaste() != RaiteenPaa::AjaVarovasti)
            opastin_->asetaOpaste( RaiteenPaa::AjaVarovasti);
    }
    else if( kulkutie->kulkutienTyyppi() == RataRaide::Junakulkutie)
    {
        // Aja vai AjaSn?
        int nopeus = naapuruus_->pieninNopeus();

        if( nopeus < 50 || nopeus < naapuruus_->naapuriRaide()->suurinNopeus()  )
            // AJA SN EHDOT: nopeus alle 50 km/h tai poikkeava (hitaampi) raide
        {
            if( opastin_->opasteKasite() != RaiteenPaa::AjaSn)
                opastin_->asetaOpaste( RaiteenPaa::AjaSn);
        }
        else if( opastin_->opasteKasite() == RaiteenPaa::Seis)
            opastin_->asetaOpaste( RaiteenPaa::Aja);
    }

    // Päivitetään ja viedään tietokantaankin
    naapuruus_->naapuriRaide()->paivita();

    if( isi_)
            isi_->laitaVarit(kulkutie);
}





bool KulkutieElementti::tarkistaKulkutieEhdot(KulkutienMuodostaja *muodostaja)
{
    // Raide ei saa olla toisen kulkutien käytössä
    if( naapuruus_->naapuriRaide()->kulkutieTyyppi() != RataRaide::EiKulkutieta)
        return false;

    // Junakulkutie ei saa olla varattu!
    if( muodostaja->kulkutienTyyppi() == RataRaide::Junakulkutie)
        if( naapuruus_->naapuriRaide()->akseleita())
            return false;

    if( naapuruus_->sallittuKulkutie() == Kisko::VainVaihto && muodostaja->kulkutienTyyppi() == RaideTieto::Junakulkutie)
        return false; // Tätä kautta ei voi muodostaa junakulkutietä

    // Jos kulkutie on määritelty toissijaiseksi, lisätään pituus tuplana!!!
    if( naapuruus_->sallittuKulkutie() == Kisko::Toissijainen && muodostaja->kulkutienTyyppi() == RaideTieto::Junakulkutie)
        pituus_ += naapuruus_->naapuriRaide()->pituus();

    return true;
}

void KulkutieElementti::maalissaOllaan(KulkutienMuodostaja *muodostaja)
{
    // Tarkastellaan maaliraiteen erityisehdot
    // Pitää olla päättävä opastin!

    if( naapurinPaa()->opastin() == RaiteenPaa::EiOpastinta && naapurinPaa()->paanTyyppi() != RaiteenPaa::RaidePuskuri )
        return;

    // Jos ne täyttyvät, ilmoitetaan lyhyimmäksi

    muodostaja->reittiLoytynyt(this);
}
