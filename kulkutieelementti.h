/**************************************************************************
**  kulkutieelementti.h
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

#ifndef KULKUTIEELEMENTTI_H
#define KULKUTIEELEMENTTI_H

#include "rataraide.h"
#include "ratakisko.h"
#include "naapuruus.h"

class KulkutienMuodostaja;

#include <QList>

class KulkutieElementti
{
public:
    KulkutieElementti(KulkutienMuodostaja* muodostaja, KulkutieElementti* vanhempi, Naapuruus* naapuri, RaiteenPaa* opastin);
    ~KulkutieElementti();

    qreal pituus() const { return pituus_; }

    void lukitseKulkutielle( KulkutienMuodostaja* kulkutie);
    void laitaVarit(KulkutienMuodostaja* kulkutie);

protected:
    Naapuruus* naapuruus_;
    qreal pituus_;
    RaiteenPaa* opastin_;


    QList<KulkutieElementti*> lapset_;
    KulkutieElementti* isi_;

    bool tarkistaKulkutieEhdot(KulkutienMuodostaja* muodostaja);
    void maalissaOllaan(KulkutienMuodostaja* muodostaja);

};

#endif // KULKUTIEELEMENTTI_H
