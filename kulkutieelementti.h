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

/** Kulkutietä MUODOSTETTAESSA käytettävä elementti */
class KulkutieElementti
{
public:
    KulkutieElementti(KulkutienMuodostaja* muodostaja, KulkutieElementti* vanhempi, Naapuruus* naapuri, RaiteenPaa* opastin, RataRaide* lahtoraide);
    ~KulkutieElementti();

    qreal pituus() const { return pituus_; }
    int pieninNopeus() const { return pieninNopeus_; }

    RataRaide* raide() { return naapuruus_->naapuriRaide(); }
    RaiteenPaa* lahtoOpastin()  { return opastin_; }
    RataRaide* lahtoRaide() { return lahtoraide_; }
    bool onkoPohjoiseen() { return naapuruus_->naapurinSuunta()==Naapuruus::Etela; }

    void lukitseKulkutielle( KulkuTie* kulkutie);
    void laitaVarit(KulkutienMuodostaja* kulkutie);

    int taso() const { return taso_; }  //* Monesko elementti alusta lukien

    KulkutieElementti* vanhempi() { return isi_; }

protected:
    Naapuruus* naapuruus_;
    qreal pituus_;
    RaiteenPaa* opastin_;
    RataRaide* lahtoraide_;
    int pieninNopeus_;

    RaiteenPaa* naapurinPaa_;
    RaiteenPaa* naapurinPaa() { return naapurinPaa_; }


    QList<KulkutieElementti*> lapset_;
    KulkutieElementti* isi_;

    int taso_;

    bool tarkistaKulkutieEhdot(KulkutienMuodostaja* muodostaja);
    void maalissaOllaan(KulkutienMuodostaja* muodostaja);

};

#endif // KULKUTIEELEMENTTI_H
