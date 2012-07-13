/**************************************************************************
**  naapuruus.cpp
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
**  Naapuruus  12.7.2012
**************************************************************************/

#include "naapuruus.h"

#include <QGraphicsScene>

Naapuruus::Naapuruus(RataKisko *omaKisko, Ilmansuunta omasuunta)
    : omaSuunta_(omasuunta), naapurinSuunta_(Virhe), naapurinVaihde_(RaiteenPaa::EiVaihdetta),
      pieninNopeus_(0),
      sallittuKulkutie_(Kisko::Ensisijainen)
{
    // Hakee tässä päässä olevan naapurin ja laittaa sen tietoja ylös

    // Oma vaihde
    // Kertoo, missä suunnassa oman vaihteen pitää olla, että tähän
    // naapuriin päästään

    omaraide_ = omaKisko->raide();

    if( omasuunta == Etela)
    {
        if( omaKisko->pohjoisTyyppi()==Kisko::VaihdeVasen )
            omaVaihde_ = RaiteenPaa::Vasen;
        else if( omaKisko->pohjoisTyyppi() == Kisko::VaihdeOikea)
            omaVaihde_ = RaiteenPaa::Oikea;
        else
            omaVaihde_ = RaiteenPaa::EiVaihdetta;
    }
    else if( omasuunta == Pohjoinen)
    {
        if( omaKisko->etelaTyyppi()==Kisko::VaihdeVasen )
            omaVaihde_ = RaiteenPaa::Vasen;
        else if( omaKisko->etelaTyyppi() == Kisko::VaihdeOikea)
            omaVaihde_ = RaiteenPaa::Oikea;
        else
            omaVaihde_ = RaiteenPaa::EiVaihdetta;
    }

    // Haetaan naapuri
    QPointF piste;
    if( omasuunta == Etela)
        piste = omaKisko->viiva().p1();
    else
        piste = omaKisko->viiva().p2();

    RataKisko* naapuriKisko = 0;

    QList<QGraphicsItem*> lista = omaKisko->scene()->items(piste);
    QList<RataKisko*> kiskolista;
    foreach( QGraphicsItem* item, lista )
    {
        // Käytetään dynamic_cast jotta hakisi myös lapsiluokat
        RataKisko* kisko = qgraphicsitem_cast<RataKisko*>(item);
        if( kisko && kisko != omaKisko &&
                (kisko->etelainen()==piste || kisko->pohjoinen() == piste) )
        {
            naapuriKisko = kisko;
            break;
        }
    }

    // Nyt jos ei olekaan löydetty naapuria, niin sitten ei tästä mitään tule
    // eli ei ole naapuria, nollatieto
    if( !naapuriKisko)
    {
        naapuriraide_ = 0;
        return;
    }

    naapuriraide_ = naapuriKisko->raide();
    if( naapuriKisko->etelainen() == piste)
        naapurinSuunta_ = Etela;
    else if( naapuriKisko->pohjoinen() == piste)
        naapurinSuunta_ = Pohjoinen;

    // Sitten vielä pitäisi päätellä, miten naapurin mahdollisen vaihteen pitää olla
    if( naapurinSuunta() == Etela)
    {
        if( naapuriKisko->pohjoisTyyppi() == Kisko::VaihdeVasen)
            naapurinVaihde_ = RaiteenPaa::Vasen;
        else if( naapuriKisko->pohjoisTyyppi() == Kisko::VaihdeOikea )
            naapurinVaihde_ = RaiteenPaa::Oikea;
    }
    else if( naapurinSuunta() == Pohjoinen)
    {
        if( naapuriKisko->etelaTyyppi() == Kisko::VaihdeVasen)
            naapurinVaihde_ = RaiteenPaa::Vasen;
        else if( naapuriKisko->etelaTyyppi() == Kisko::VaihdeOikea )
            naapurinVaihde_ = RaiteenPaa::Oikea;
    }

    // Sitten pienempi nopeus
    pieninNopeus_ = omaKisko->sn();
    if( naapuriKisko->sn() < pieninNopeus())
        pieninNopeus_ = naapuriKisko->sn();


    // Mitä kulkuteitä voidaan tätä kautta muodostaa??
    if( omaKisko->kulkutietyypit()==Kisko::VainVaihto || naapuriKisko->kulkutietyypit() == Kisko::VainVaihto )
        sallittuKulkutie_ = Kisko::VainVaihto;
    else if( omaKisko->kulkutietyypit() == Kisko::Toissijainen || naapuriKisko->kulkutietyypit() == Kisko::Toissijainen)
        sallittuKulkutie_ = Kisko::Toissijainen;


}

void Naapuruus::lukitseVaihteet()
{
    // Ensin oma vaihde
    if( omaVaihde() == RaiteenPaa::Vasen)
    {
        if( omaSuunta() == Etela)
            omaRaide()->etelainen()->lukitseVaihde(RaiteenPaa::Vasen);
        else if(omaSuunta() == Pohjoinen)
            omaRaide()->pohjoinen()->lukitseVaihde(RaiteenPaa::Vasen);
    }
    else if( omaVaihde() == RaiteenPaa::Oikea)
    {
        if( omaSuunta() == Etela)
            omaRaide()->etelainen()->lukitseVaihde(RaiteenPaa::Oikea);
        else if(omaSuunta() == Pohjoinen)
            omaRaide()->pohjoinen()->lukitseVaihde(RaiteenPaa::Oikea);
    }

    // Sitten naapurin vaihde
    if( naapurinVaihde() == RaiteenPaa::Vasen)
    {
        if( naapurinSuunta() == Etela)
            naapuriRaide()->etelainen()->lukitseVaihde(RaiteenPaa::Vasen);
        else if(omaSuunta() == Pohjoinen)
            naapuriRaide()->pohjoinen()->lukitseVaihde(RaiteenPaa::Vasen);
    }
    else if( naapurinVaihde() == RaiteenPaa::Oikea)
    {
        if( naapurinSuunta() == Etela)
            naapuriRaide()->etelainen()->lukitseVaihde(RaiteenPaa::Oikea);
        else if(omaSuunta() == Pohjoinen)
            naapuriRaide()->pohjoinen()->lukitseVaihde(RaiteenPaa::Oikea);
    }

}
