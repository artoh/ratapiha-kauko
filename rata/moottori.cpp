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

#include <QDebug>

#include "moottori.h"

#include "akseli.h"
#include "veturi.h"

#include "jkvlaite.h"

Moottori::Moottori(Veturi *veturi, Akseli *akseli)
    : veturi_(veturi), akseli_(akseli), nopeusMs_(0.0), tavoiteNopeusMs_(0.0)
{
    akseli->kytkeMoottori(this);
    jkv_ = new JKVLaite(akseli, veturi->hidastuvuus() - 0.1);
}

Moottori::~Moottori()
{
    // Poistetaan tämä moottori akselilta
    if( akseli() && akseli()->moottori() == this )
        akseli()->kytkeMoottori(0);

    delete jkv_;
}

void Moottori::aja(qreal nopeutusKerroin)
{
    // TODO : jkv-tiedot
    jkv_->paivitaJkv();

qDebug() << " JKV " << jkv_->jkvNopeusKmh() << " km/h " << jkv_->jkvMatka() << " m ";

    qreal tavoite = tavoiteNopeusMs();

    // Ajetaan jkv-nopeuden mukaan, jos rajoittaa
    if( tavoite > jkv_->jkvNopeusMs())
        tavoite = jkv_->jkvNopeusMs();

    if( tavoite > nopeusMs())
    {
        if( tavoite - nopeusMs() > veturi()->kiihtyvyys() * nopeutusKerroin )
            nopeusMs_ = nopeusMs() + veturi()->kiihtyvyys() * nopeutusKerroin;
        else
            nopeusMs_ = tavoite;
    }
    else if( tavoite < nopeusMs())
    {
        if( nopeusMs() - tavoite > veturi()->hidastuvuus() * nopeutusKerroin )
            nopeusMs_ = nopeusMs() - veturi()->hidastuvuus() * nopeutusKerroin;
        else
            nopeusMs_ = tavoite;
    }

    // Sitten liikutaan nopeuden verran
    akseli()->moottoriLiike( nopeusMs() * nopeutusKerroin );

}

void Moottori::asetaTavoiteNopeus(qreal nopeusMs)
{
    tavoiteNopeusMs_ = nopeusMs;
}

void Moottori::asetaTavoiteNopeusKmh(int nopeusKmh)
{
    asetaTavoiteNopeus((qreal) nopeusKmh / 3.6);
}