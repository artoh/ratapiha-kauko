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
    : opaste_(Ratapiha::OPASTE_PIMEA),
      opastinlaji_(Ratapiha::OPASTIN_PUUTTU)
{
}

void KaukoRaiteenpaa::paivita(const QString tieto)
{
    // Opastimen laji

    if( tieto.contains('p'))
        opastinlaji_ = Ratapiha::OPASTIN_PAAOPASTIN;
    else if( tieto.contains('r'))
        opastinlaji_ = Ratapiha::OPASTIN_RAIDEOPASTIN;
    else if( tieto.contains('y'))
        opastinlaji_ = Ratapiha::OPASTIN_YHDISTELMAOPASTIN;


    // Opaste
    if( tieto.contains('0'))
            opaste_ = Ratapiha::OPASTE_SEIS;
    else if( tieto.contains('1'))
            opaste_ = Ratapiha::OPASTE_AJASN;
    else if( tieto.contains('2'))
            opaste_ = Ratapiha::OPASTE_AJA;
    else if( tieto.contains('3'))
            opaste_ = Ratapiha::OPASTE_AJAVAROVASTI;
    else if( tieto.contains('4'))
            opaste_= Ratapiha::OPASTE_EIOPASTETTA;

}
