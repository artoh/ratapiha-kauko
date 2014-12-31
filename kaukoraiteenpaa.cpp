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


#include "kaukoraiteenpaa.h"

using namespace Ratapiha;

KaukoRaiteenpaa::KaukoRaiteenpaa()
    : opaste_(Ratapiha::OPASTE_PIMEA)
{
}

void KaukoRaiteenpaa::paivita(const QString tieto)
{
    if( tieto=="0")
        opaste_ = OPASTE_SEIS;
    else if( tieto == "1")
        opaste_ = OPASTE_AJASN;
    else if( tieto == "2")
        opaste_ = OPASTE_AJA;
}
