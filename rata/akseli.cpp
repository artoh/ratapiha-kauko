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

#include "akseli.h"

#include "kiskonpaa.h"
#include "ratakiskotieto.h"
#include "vaunu.h"

#include "ratalaite.h"
#include "ratascene.h"

Akseli::Akseli(Vaunu *vaunu) :
    vaunu_(vaunu), toinenAkseli_(0), kytkettyAkseli_(0) ,edessa_(0), takana_(0), moottori_(0)
{
}

void Akseli::sijoita(Kiskonpaa *edessa, qreal matkaEteen, Kiskonpaa *takana, qreal matkaTaakse)
{
    edessa_=edessa;
    matkaEteen_ = matkaEteen;
    takana_ = takana;
    matkaTaakse_ = matkaTaakse;

    // Selvittää, onko kytkettävää akselia
    foreach (Akseli* akseli, edessa_->kiskotieto()->akselitKiskolla())
    {
        if( edessa==akseli->takana() && matkaEteen == akseli->matkaTaakse())
        {
                // Kiskolla on jo akseli samassa paikassa, eli nämä akselit
                // on syytä kytkeä yhteen
                kytkeVaunu(akseli);
        }
    }

    edessa_->kiskotieto()->akseliKiskolle(this);
    takana_->kiskotieto()->akseliKiskolle(this);

    // Lähetetään akselinlaskentalaitteelle sanoma akselin laskemiseksi raiteelle
    unsigned int sanoma = ( edessa->kiskotieto()->raideId() << 4 ) | 0xf410000f;
    Ratalaite::skene()->sanoma(sanoma);


    laskeKulkuViiva();
    laskeSijainti();


}

bool Akseli::liiku(qreal matka)
{
    // Lisätään liikuttu matka matkamittariin
    matkaMittari_ += qAbs(matka);

    if(liikeKiskolla(matka))
    {
        laskeSijainti();
        vaunu_->akseliSiirtynyt();  // Jotta vaunun sijainti lasketaan
        return true;
    }
    else
        return false;
}

void Akseli::kytkinLiike(qreal matka)
{
    // Liike tulee kytkimestä, leviää vaunun toiseen akseliin
    liiku(matka);
    toinenAkseli_->vaunuLiike(0.0 - matka);
}

void Akseli::vaunuLiike(qreal matka)
{
    // Liike tulee vaunun toisesta akselista, leviää kytkimeen
    liiku(matka);
    if( kytkettyAkseli_)
        kytkettyAkseli_->kytkinLiike(0.0-matka);
}

void Akseli::kytkeToinenAkseli(Akseli *toinen)
{
    toinenAkseli_= toinen;
}

void Akseli::kytkeVaunu(Akseli *kytkinakseli)
{
    kytkettyAkseli_ = kytkinakseli;
    kytkinakseli->kytkettyAkseli_ = this;
}

void Akseli::moottoriLiike(qreal matka)
{
    liiku(matka);
    if( kytkettyAkseli_)
        kytkettyAkseli_->kytkinLiike(0.0-matka);
    toinenAkseli_->vaunuLiike(0.0 - matka);
}

void Akseli::kytkeMoottori(Moottori *moottori)
{
    moottori_ = moottori;
}

bool Akseli::liikeKiskolla(qreal matka)
{
    if( edessa_ && takana_)
    {
        matkaTaakse_ = matkaTaakse_ + matka;
        matkaEteen_ = matkaEteen_ - matka;
        
        // Mennäänkö toiselle kiskolle ???
        if( matkaEteen_ < 0.00)
        {
            // Joo - mennään eteenpäin
            edessa_->kiskotieto()->akseliKiskolta(this);
            takana_ = edessa_->ajaUlos();
            if( takana_ )
            {
                qreal jaljella = 0.0 - matkaEteen_;

                takana_->ajaSisaan();
                takana_->kiskotieto()->akseliKiskolle(this);

                edessa_ = takana_->toinenPaa();               
                laskeKulkuViiva();
                
                matkaTaakse_ = 0;
                matkaEteen_ = kulkuViiva_.length();
                
                return liikeKiskolla( jaljella );  // Mennään lisää eteenpäin
            }
            else
                return false;
        }
        else if( matkaTaakse_ < 0.00)
        {
            // Mennään taaksepäin
            takana_->kiskotieto()->akseliKiskolta(this);
            edessa_ = takana_->ajaUlos();
            if( edessa_)
            {
                qreal jaljella = matkaTaakse_;

                edessa_->ajaSisaan();
                edessa_->kiskotieto()->akseliKiskolle(this);
                takana_ = edessa_->toinenPaa();
                
                laskeKulkuViiva();
                
                matkaEteen_ = 0;
                matkaTaakse_ = kulkuViiva_.length();
                
                return liikeKiskolla(jaljella);  // Mennään lisää taaksepäin
            }
            else
                return false;
        }
        
    }
    return true;
}

void Akseli::laskeSijainti()
{
    sijainti_ = kulkuViiva_.pointAt(matkaTaakse_ / (matkaEteen_ + matkaTaakse_));
}

void Akseli::laskeKulkuViiva()
{
    if( edessa_ && takana_)
        kulkuViiva_.setLine(takana_->x(), takana_->y(),edessa_->x(), edessa_->y());
    else
        qDebug() << "!" << edessa_ << takana_;
}
