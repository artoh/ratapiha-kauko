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


#include "kiskonpaa.h"
#include "kiskoliitos.h"

Kiskonpaa::Kiskonpaa(KiskoLiitos *kiskoliitos, int  kiskonpaikka )
    : kiskoliitos_(kiskoliitos), opastin_(0)
{
    // Tyyppisuojan takia kiskonpaikka pitää käsitellä switchillä
    switch (kiskonpaikka) {
    case E:
        kiskonpaikka_ = E;
        break;
    case A:
        kiskonpaikka_ = A;
        break;
    case B:
        kiskonpaikka_ = B;
        break;
    case P:
        kiskonpaikka_ = P;
        break;
    case C:
       kiskonpaikka_ = C;
       break;
    case D:
        kiskonpaikka_ = D;
        break;
    default:
        kiskonpaikka_ = VIRHE;
        break;
    }
}

Kiskonpaa *Kiskonpaa::vastakkainenPaa()
{
    return kiskoliitos_->seuraava(this);
}

Kiskonpaa *Kiskonpaa::ajaUlos()
{
    return kiskoliitos_->siirrySeuraavalle(this);
}

void Kiskonpaa::ajaSisaan()
{
    ;
}

int Kiskonpaa::x()
{
    return kiskoliitos_->x();
}

int Kiskonpaa::y()
{
    return kiskoliitos_->y();
}

bool Kiskonpaa::onkoAktiivinen()
{
    return kiskoliitos_->onkoAktiivinenPaa(this);
}

bool Kiskonpaa::onkoPuskuri()
{
    return ( kiskoliitos()->tyyppi() == KiskoLiitos::RAIDEPUSKURI);
}

Kiskonpaa::RaiteenSulku Kiskonpaa::raiteenSulku()
{
    return SP_EI;
}

void Kiskonpaa::kytkeToinenPaa(Kiskonpaa *toinenpaa)
{
    kiskonToisenPaanLiitos_ = toinenpaa;
}

void Kiskonpaa::kytkeOpastin(RataOpastin *opastin)
{
    opastin_ = opastin;
}




