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

#include <QDebug>

KulkutieElementti::KulkutieElementti(KulkutienMuodostaja *muodostaja, KulkutieElementti *vanhempi, Naapuruus *naapuri, RaiteenPaa* opastin)
    :  naapuruus_(naapuri), opastin_(opastin), isi_(vanhempi)
{

    qDebug() << naapuri->naapuriRaide()->raidetunnus();

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
    // 4) Jatketaan seuraaviin

    // Jos tässä on ollut opastin, niin se jatkossa eteenpäin
    RaiteenPaa* uusiopastin = opastin;
    if( naapuruus_->omaSuunta() == Naapuruus::Pohjoinen &&
            naapuruus_->naapuriRaide()->pohjoinen()->opastin() != RaiteenPaa::EiOpastinta)
        uusiopastin = naapuruus_->naapuriRaide()->pohjoinen();
    else if( naapuruus_->omaSuunta() == Naapuruus::Etela &&
            naapuruus_->naapuriRaide()->etelainen()->opastin() != RaiteenPaa::EiOpastinta)
        uusiopastin = naapuruus_->naapuriRaide()->etelainen();


    QList<Naapuruus*> naapurit = naapuruus_->naapuriRaide()->naapurit();
    foreach( Naapuruus* uusinaapuri, naapurit)
    {
        if( uusinaapuri->naapuriRaide() && uusinaapuri->omaSuunta() != naapuruus_->naapurinSuunta() )
        {
            // Mennään toiseen suuntaan eli eteenpäin
            lapset_.append(new KulkutieElementti(muodostaja, this, uusinaapuri, uusiopastin));
        }
    }


}


KulkutieElementti::~KulkutieElementti()
{
    foreach(KulkutieElementti* lapsi, lapset_)
        delete lapsi;
}

void KulkutieElementti::lukitseKulkutielle(KulkutienMuodostaja *kulkutie)
{
    // Lukitsee ensin tämän elementin kulkutielle
    // Jos vaihteita, kääntää ne
    if( naapuruus_->omaVaihde() == RaiteenPaa::Vasen)
    {
        if( naapuruus_->omaSuunta()==Naapuruus::Etela)
            naapuruus_->omaRaide()->etelainen()->kaannaVaihde(RaiteenPaa::Vasen);
        else
            naapuruus_->omaRaide()->pohjoinen()->kaannaVaihde(RaiteenPaa::Vasen);
    }
    else if( naapuruus_->omaVaihde() == RaiteenPaa::Oikea)
    {
        if( naapuruus_->omaSuunta()==Naapuruus::Etela)
            naapuruus_->omaRaide()->etelainen()->kaannaVaihde(RaiteenPaa::Vasen);
        else
            naapuruus_->omaRaide()->pohjoinen()->kaannaVaihde(RaiteenPaa::Vasen);
    }


    if( naapuruus_->naapurinVaihde() == RaiteenPaa::Vasen)
    {
        if( naapuruus_->naapurinSuunta()==Naapuruus::Etela)
            naapuruus_->naapuriRaide()->etelainen()->kaannaVaihde(RaiteenPaa::Vasen);
        else
            naapuruus_->naapuriRaide()->pohjoinen()->kaannaVaihde(RaiteenPaa::Vasen);
    }
    else if( naapuruus_->naapurinVaihde() == RaiteenPaa::Oikea)
    {
        if( naapuruus_->naapurinSuunta()==Naapuruus::Etela)
            naapuruus_->naapuriRaide()->etelainen()->kaannaVaihde(RaiteenPaa::Vasen);
        else
            naapuruus_->naapuriRaide()->pohjoinen()->kaannaVaihde(RaiteenPaa::Vasen);
    }


    naapuruus_->naapuriRaide()->lukitseKulkutielle(opastin_, RaideTieto::Vaihtokulkutie);


    // sitten isin
    if( isi_)
        isi_->lukitseKulkutielle(kulkutie);
}

void KulkutieElementti::laitaVarit(KulkutienMuodostaja *kulkutie)
{
    if( opastin_->opaste() == RaiteenPaa::Seis)
    {
        if( kulkutie->kulkutienTyyppi() == KulkutienMuodostaja::Junakulkutie)
            opastin_->asetaOpaste( RaiteenPaa::Aja);
        else if( kulkutie->kulkutienTyyppi() == KulkutienMuodostaja::Vaihtokulkutie)
            opastin_->asetaOpaste( RaiteenPaa::AjaVarovasti);
    }
    naapuruus_->naapuriRaide()->paivita();

    if( isi_)
            isi_->laitaVarit(kulkutie);
}





bool KulkutieElementti::tarkistaKulkutieEhdot(KulkutienMuodostaja *muodostaja)
{
    // Raide ei saa olla toisen kulkutien käytössä
    // jne..

    // Junakulkutie ei saa olla varattu!
    if( muodostaja->kulkutienTyyppi() == KulkutienMuodostaja::Junakulkutie)
        if( naapuruus_->naapuriRaide()->akseleita())
            return false;

    return true;
}

void KulkutieElementti::maalissaOllaan(KulkutienMuodostaja *muodostaja)
{
    // Tarkastellaan maaliraiteen erityisehdot

    // Jos ne täyttyvät, ilmoitetaan lyhyimmäksi

    muodostaja->reittiLoytynyt(this);
}
