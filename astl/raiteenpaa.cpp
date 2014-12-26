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


#include "raiteenpaa.h"
#include "raidetieto.h"

RaiteenPaa::RaiteenPaa(RaideTieto *raide)
    : raide_(raide),
      liitettyPaa_(0), hidasKulkutie_(false), kulkutieLajit_(JUNAKULKUTIE)
{
}

RaiteenPaa *RaiteenPaa::aktiivinenVastapaa()
{
    return raide()->aktiivinenVastapaa(this);
}

RaiteenPaa *RaiteenPaa::seuraavaRaiteenpaa()
{
    if( liitettyPaa() )
        return liitettyPaa()->aktiivinenVastapaa();
    else
        return 0;
}

RaiteenPaa *RaiteenPaa::edellinenRaiteenpaa()
{
    RaiteenPaa *paa = aktiivinenVastapaa();
    if( paa )
        return paa->liitettyPaa();
    else
        return 0;
}

void RaiteenPaa::liitaPaa(RaiteenPaa *paa)
{
    liitettyPaa_ = paa;
}

void RaiteenPaa::asetaHidas()
{
    hidasKulkutie_ = true;
}

void RaiteenPaa::asetaKulkutieLajit(RaiteenPaa::KulkutieLajit lajit)
{
    kulkutieLajit_ = lajit;
}
