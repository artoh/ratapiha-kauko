/**************************************************************************
**
**  Ratapiha
**
**  Copyright (c) 2012-2015 Arto Hyvättinen
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
**
**************************************************************************/

#include <QStringList>

#include "kaukoraide.h"

using namespace Ratapiha;

KaukoRaide::KaukoRaide(const QString &datarivi)
    : tyyppi_(RAIDE_TUNTEMATONTYYPPI),
      vapaana_(RAIDE_TIEDONSIIRTOVIRHE),
      kulkutieTyyppi_(EIKULKUTIETA),
      elementinLukitus_(ELEMENTTI_VAPAA)
{
    QStringList listana = datarivi.split(" ");

    QString tunnus = listana.at(1);
    for(int i=0; i< tunnus.length(); i++)
    {
        if( tunnus[i].isDigit())
        {
            numeroTekstina_ = tunnus.mid(i);
            break;
        }
    }

    QChar lajikirjain = listana.at(2).at(0);
    if( lajikirjain == '-')
        tyyppi_ = RAIDE_RAIDE;
    else if( lajikirjain == 'v')
    {
        tyyppi_ = RAIDE_VAIHDE;
        numeroTekstina_ = QString("V").append(raidenumeroteksti());
    }
    else if( lajikirjain == 'x')
    {
        tyyppi_ = RAIDE_RISTEYSVAIHDE;
        numeroTekstina_ = QString("V").append(raidenumeroteksti());
    }
    else if( lajikirjain == 'r')
    {
        tyyppi_ = RAIDE_RAIDERISTEYS;
        numeroTekstina_ = QString("RR").append(raidenumeroteksti());
    }

    paivita(listana);

}

void KaukoRaide::paivita(const QStringList& dataLista)
{
    // Päivitetään ensin vapaa/varattu/kulkutietiedot

    QString ekatieto = dataLista.at(2);

    if( ekatieto.contains('O'))
        vapaana_ = RAIDE_VAPAA;
    else if( ekatieto.contains('X'))
        vapaana_ = RAIDE_VARATTU;
    else if( ekatieto.contains('!'))
        vapaana_ = RAIDE_VIKATILA;

    if( ekatieto.contains('J'))
        kulkutieTyyppi_ = JUNAKULKUTIE;
    else if( ekatieto.contains('V'))
        kulkutieTyyppi_ = VAIHTOKULKUTIE;

    if( ekatieto.contains('L'))
        elementinLukitus_ = ELEMENTTI_LUKITTU;
    else if( ekatieto.contains('l'))
        elementinLukitus_ = ELEMENTTI_LUKITAAN;


    for(int i=3; i < dataLista.count(); i++)
    {
        QString tieto = dataLista.at(i);

        if( tieto.startsWith("E"))
            etelaPaa_.paivita(tieto.mid(1));
        else if( tieto.startsWith("P"))
            pohjoisPaa_.paivita(tieto.mid(1));
        else if( tieto.startsWith('V') || tieto.startsWith('A') ||
                 tieto.startsWith("R"))
            vaihdeAB_.paivita(tieto.mid(1));
        else if( tieto.startsWith('C'))
            vaihdeCD_.paivita(tieto.mid(1));

    }

}
