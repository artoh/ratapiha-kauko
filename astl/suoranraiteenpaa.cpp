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


#include "suoranraiteenpaa.h"

SuoranRaiteenPaa::SuoranRaiteenPaa(RaideTieto *raide)
    : RaiteenPaa(raide), opastin_(0), suojastus_(0)
{

}

void SuoranRaiteenPaa::lisaaOpastin(int opastintunnus, int tyyppitieto)
{
    opastin_ = new Opastin(this, opastintunnus, tyyppitieto);
}

void SuoranRaiteenPaa::muodostaSuojastus()
{
    if( !suojastus_)
        LinjaSuojastus::muodostaLinjaSuojastus(this);
}

void SuoranRaiteenPaa::asetaSuojastus(LinjaSuojastus *suojastus)
{
    suojastus_ = suojastus;
}
