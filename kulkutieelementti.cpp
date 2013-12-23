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
#include "kulkutienraide.h"

#include "rataikkuna.h"
#include "ratascene.h"

#include <QDebug>

KulkutieElementti::KulkutieElementti(KulkutienMuodostaja *muodostaja, KulkutieElementti *vanhempi, Naapuruus *naapuri, RaiteenPaa* opastin, RataRaide* lahtoraide)
    :  naapuruus_(naapuri), opastin_(opastin), lahtoraide_(lahtoraide),
     isi_(vanhempi), taso_(1)
{

    // Taso
    if( vanhempi)
        taso_ = vanhempi->taso() + 1;

    if( taso() > 14 )
        return; //Liian pitkä kulkutie...


    // Se suunta, johon tutkittavaa (naapurin) raidetta pitkin kuljetaan
    if( naapuri->naapurinSuunta() == RaiteenPaa::Etelaan)
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
    int omapituus = naapuri->naapuriRaide()->pituus();
    // 1xxx: Sallitaan yksi pitkä raide
    if(omapituus > 8000 )
        omapituus = 8000;

    if( vanhempi)
        pituus_ = vanhempi->pituus() + omapituus;
    else
        pituus_ = omapituus;

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

    // Junakulkutie ei saa mennä suojastuksen läpi
    if( naapurinPaa()->opastin()==RaiteenPaa::SuojastusOpastin && muodostaja->kulkutienTyyppi() == RaideTieto::Junakulkutie)
    {
        return;
    }

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
        if( uusinaapuri->naapuriRaide() && uusinaapuri->omaSuunta() != naapuruus_->naapurinSuunta()
                && ( naapuruus_->naapuriRaide()->onkoVaihde() ||
                     naapuruus_->naapurinVaihde() == uusinaapuri->omaVaihde() /* RR-suoja */)   )
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

    // Muodostaa suojastuksen, jos tarpeen
    // Lisää elementit suojastukselle
    if( !suojastus_.isEmpty())
    {
        KulkuTie* linjasuojastus = new KulkuTie(RaideTieto::Linjasuojastus);

        QString opastin;

        if( naapuruus_->naapurinSuunta() == RaiteenPaa::Pohjoiseen)
           opastin = QString( QChar('E') + naapuruus_->naapuriRaide()->raidetunnusLiikennepaikalla());
        else
            opastin = QString( QChar('P') + naapuruus_->naapuriRaide()->raidetunnusLiikennepaikalla());

        foreach( QString suojastusraide, suojastus_)
        {
            linjasuojastus->lisaaElementti( suojastusraide, opastin );

            RaiteenPaa* tamanPaa = RataIkkuna::rataSkene()->haeRaiteenPaa(suojastusraide);
            if( tamanPaa->opastin() == RaiteenPaa::SuojastusOpastin )
            {
                opastin = suojastusraide;
            }
            // Tällä raiteella on suojastusopastin, joka jatkaa suojastusta!!!
        }
        linjasuojastus->vahvistaKulkutie();

    }

}

void KulkutieElementti::laitaVarit(KulkutienMuodostaja *kulkutie, RaiteenPaa::Opaste lapsenOpaste)
{

    RaiteenPaa::Opaste opaste = RaiteenPaa::Tyhja;

    if( kulkutie->kulkutienTyyppi() == RataRaide::Vaihtokulkutie)
    {
        opaste = RaiteenPaa::AjaVarovasti;
    }
    else if( kulkutie->kulkutienTyyppi() == RataRaide::Varattukulkutie)
    {
        opaste = RaiteenPaa::AjaVaratulle;
    }
    else if( kulkutie->kulkutienTyyppi() == RataRaide::Junakulkutie)
    {
        // Aja vai AjaSn?
        // AjaSn tulee, jos mennään poikkeavalle raiteelle
        opaste = RaiteenPaa::Aja;

        if( naapuruus_->naapurinVaihde() != RaiteenPaa::EiVaihdetta ||
            naapuruus_->omaVaihde() != RaiteenPaa::EiVaihdetta )
        {
            // Tutkitaan vaihteen vaihtoehdot, onko hitain tai yhtä suuri

            QList<Naapuruus*> naapurit = naapuruus_->omaRaide()->naapurit();
            foreach(Naapuruus* toinen, naapurit)
            {
                if( toinen != naapuruus_ &&
                   toinen->omaSuunta() == naapuruus_->omaSuunta())
                {
                    // Nyt löydettiin vaihdenaapuri! Tässä pitäisi siis olla
                    // pienempi nopeus, jotta saadaan vihreä

                    if( toinen->pieninNopeus() >= naapuruus_->pieninNopeus() )
                        opaste = RaiteenPaa::AjaSn;
                }
            }

            QList<Naapuruus*> toisennaapurit = naapuruus_->naapuriRaide()->naapurit();
            foreach(Naapuruus* viekku, toisennaapurit)
            {
                if( viekku->naapuriRaide() != naapuruus_->omaRaide() &&
                   viekku->omaSuunta() == naapuruus_->naapurinSuunta())
                {
                    // Nyt löydettiin vaihdenaapuri! Tässä pitäisi siis olla
                    // pienempi nopeus, jotta saadaan vihreä

                    if( viekku->pieninNopeus() >= naapuruus_->pieninNopeus() )
                        opaste = RaiteenPaa::AjaSn;
                }
            }


        }

    }


    // Jos on myös lapsella opaste, määritellään rajoittavampi
    if( opaste == RaiteenPaa::Aja && lapsenOpaste != RaiteenPaa::Aja
            && lapsenOpaste != RaiteenPaa::Tyhja)
        opaste = lapsenOpaste;

    if( isi_)
    {
        if( isi_->opastin_ == opastin_)
        {
            isi_->laitaVarit( kulkutie, opaste);
        }
        else
        {
            opastin_->asetaOpaste( opaste);
            isi_->laitaVarit(kulkutie);
        }
    }
    else
        opastin_->asetaOpaste(opaste);

    // Päivitetään ja viedään tietokantaankin
    naapuruus_->naapuriRaide()->paivita();

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

    // Varattu junakulkutie saa olla varattu vain maaliraiteella - jonka pitää olla varattu
    if( muodostaja->kulkutienTyyppi() == RataRaide::Varattukulkutie)
    {
        if( naapuruus_->naapuriRaide()->akseleita() && naapuruus_->naapuriRaide() != muodostaja->minne() )
            return false;   // Välissä oleva raide on varattu
        else if( !naapuruus_->naapuriRaide()->akseleita() && naapuruus_->naapuriRaide() == muodostaja->minne() )
            return false;   // Maaliraide ei ole varattu

    }

    if( naapuruus_->sallittuKulkutie() == Kisko::VainVaihto && ( muodostaja->kulkutienTyyppi() == RaideTieto::Junakulkutie || muodostaja->kulkutienTyyppi() == RataRaide::Varattukulkutie ))
        return false; // Tätä kautta ei voi muodostaa junakulkutietä

    // Jos kulkutie on määritelty toissijaiseksi, lisätään pituus tuplana!!!
    if( naapuruus_->sallittuKulkutie() == Kisko::Toissijainen && ( muodostaja->kulkutienTyyppi() == RaideTieto::Junakulkutie || muodostaja->kulkutienTyyppi() == RataRaide::Varattukulkutie ))
        pituus_ += naapuruus_->naapuriRaide()->pituus();

    // Aukiajetun vaihteen kautta EI sallita kulkuteitä!
    if( naapuruus_->naapuriRaide()->etelainen()->vaihde() == RaiteenPaa::Aukiajettu ||
            naapuruus_->naapuriRaide()->pohjoinen()->vaihde() == RaiteenPaa::Aukiajettu)
        return false;


    return true;
}

void KulkutieElementti::maalissaOllaan(KulkutienMuodostaja *muodostaja)
{
    // Tarkastellaan maaliraiteen erityisehdot
    // Pitää olla päättävä opastin!

    if( naapurinPaa()->opastin() == RaiteenPaa::EiOpastinta && naapurinPaa()->paanTyyppi() != RaiteenPaa::RaidePuskuri )
        return;

    // Varatun kulkutien päättävän opasteen pitää olla Seis
    if( (naapurinPaa()->opaste() != RaiteenPaa::Seis && naapurinPaa()->paanTyyppi() != RaiteenPaa::RaidePuskuri) && muodostaja->kulkutienTyyppi() == RaideTieto::Varattukulkutie   )
        return;

    // Junakulkutien pitää herättää suojastus
    if( muodostaja->kulkutienTyyppi() == RaideTieto::Junakulkutie && naapurinPaa()->opastin() == RaiteenPaa::SuojastusOpastin)
    {
        // Nyt pitää tarkistaa, onko seuraavalla raiteelle sopiva suojastus

        QList<Naapuruus*> naapurit = naapuruus_->naapuriRaide()->naapurit();
        foreach( Naapuruus* uusinaapuri, naapurit)
        {
            if( uusinaapuri->naapuriRaide() && uusinaapuri->omaSuunta() != naapuruus_->naapurinSuunta() )
            {
                // Tässä oikea naapuri!
                KulkutienRaide* ktraide = uusinaapuri->naapuriRaide()->kulkutienRaide();
                if( ktraide && ktraide->kulkutie()->kulkutienTyyppi() == RataRaide::Linjasuojastus
                        && ktraide->suunta() != naapuruus_->naapurinSuunta())
                {
                        muodostaja->reittiLoytynyt(this);
                        return;
                }
            }
        }

        // Suojastus ei vielä valmis
        if( !voikoMuodostaaSuojastuksen(naapuruus_))
            return; // Ei onnaa suojastusvaatimuksen takia!!!
    }

    // Jos ne täyttyvät, ilmoitetaan lyhyimmäksi

    muodostaja->reittiLoytynyt(this);
}

bool KulkutieElementti::voikoMuodostaaSuojastuksen(Naapuruus *naapuri)
{
    // Suojastuksen voi muodostaa, jos suojaväli seuraavaan pääopastimeen asti on selvä ja vaihteeton   

    QList<Naapuruus*> naapurit = naapuri->naapuriRaide()->naapurit();
    foreach( Naapuruus* uusinaapuri, naapurit)
    {

        if( uusinaapuri->naapuriRaide() && uusinaapuri->omaSuunta() != naapuri->naapurinSuunta() )
        {
            // Tarkastetaan kelpaako raide
            if( uusinaapuri->naapuriRaide()->akseleita() ||
                    uusinaapuri->naapuriRaide()->kulkutieTyyppi() != RaideTieto::EiKulkutieta ||
                    uusinaapuri->omaVaihde()!=RaiteenPaa::EiVaihdetta ||
                    uusinaapuri->naapurinVaihde()!=RaiteenPaa::EiVaihdetta )
                return false;   // Tämä suojastus ei kerta kaikkiaan kelpaa!!!!

            RaiteenPaa* naapurinpaa;

            // Se suunta, johon tutkittavaa (naapurin) raidetta pitkin kuljetaan
            if( uusinaapuri->naapurinSuunta() == RaiteenPaa::Etelaan)
                naapurinpaa = uusinaapuri->naapuriRaide()->pohjoinen();
            else
                naapurinpaa = uusinaapuri->naapuriRaide()->etelainen();

            if( naapurinpaa->opastin() == RaiteenPaa::RaideOpastin)
                return false;       // Raideopastin ei kelpaa suojastuksen päättäväksi opastimeksi, vaan pitää olla oikea, korkea pääopastin!

            if( uusinaapuri->naapurinSuunta() == RaiteenPaa::Etelaan)
                suojastus_.append( QChar('P') + uusinaapuri->naapuriRaide()->raidetunnusLiikennepaikalla());
            else
                suojastus_.append( QChar('E') + uusinaapuri->naapuriRaide()->raidetunnusLiikennepaikalla());

            if( naapurinpaa->opastin() == RaiteenPaa::PaaOpastin )
                return true;        // Hienoa, suojastus päättyi pääopastimelle, eli tämä suojastus kelpaa vallan mainiosti!!!


            // Mennään eteenpäin rekursiolla
            return voikoMuodostaaSuojastuksen(uusinaapuri);

        }
    }
    // Tänne ei pitäisi tulla!!!
    return false;

}
