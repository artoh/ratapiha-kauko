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


#include "akseli.h"

#include "kiskonpaa.h"
#include "ratakiskotieto.h"

Akseli::Akseli() :
    edessa_(0), takana_(0)
{
}

void Akseli::sijoita(Kiskonpaa *edessa, qreal matkaEteen, Kiskonpaa *takana, qreal matkaTaakse)
{
    edessa_=edessa;
    matkaEteen_ = matkaEteen;
    takana_ = takana;
    matkaTaakse_ = matkaTaakse;
    
    edessa_->kiskotieto()->akseliKiskolle(this);
    takana_->kiskotieto()->akseliKiskolle(this);

    laskeKulkuViiva();
    laskeSijainti();
}

bool Akseli::liiku(qreal matka)
{
    if(liikeKiskolla(matka))
    {
        laskeSijainti();
        return true;
    }
    else
        return false;
}

void Akseli::kytkeToinenAkseli(Akseli *toinen)
{
    toinenAkseli_= toinen;
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
            takana_ = edessa_->ajaUlos();
            edessa_->kiskotieto()->akseliKiskolta(this);
            if( takana_ )
            {
                takana_->ajaSisaan();
                takana_->kiskotieto()->akseliKiskolle(this);
                edessa_ = takana_->toinenPaa();
                
                laskeKulkuViiva();
                
                matkaTaakse_ = 0;
                matkaEteen_ = kulkuViiva_.length();
                
                return liikeKiskolla(0.0-matkaTaakse_);  // Mennään lisää eteenpäin
            }
            else
                return false;
        }
        else if( matkaTaakse_ < 0.00)
        {
            // Mennään taaksepäin
            edessa_ = takana_->ajaUlos();
            takana_->kiskotieto()->akseliKiskolta(this);
            if( edessa_)
            {
                edessa_->ajaSisaan();
                edessa_->kiskotieto()->akseliKiskolle(this);
                takana_ = edessa_->toinenPaa();
                
                laskeKulkuViiva();
                
                matkaTaakse_ = 0;
                matkaEteen_ = kulkuViiva_.length();
                
                return liikeKiskolla(0.0-matkaTaakse_);  // Mennään lisää taaksepäin
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
}
