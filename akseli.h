/**************************************************************************
**  akseli.h
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
**  Akseli  17.7.2012
**************************************************************************/

#ifndef AKSELI_H
#define AKSELI_H

#include "raiteenpaa.h"

#include <QGraphicsItem>

class RataKisko;
class Vaunu;

class Akseli : public QGraphicsItem
{
public:
    enum { Type = UserType + 500 };

    Akseli(Vaunu* vaunu);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget* );
    int type() const { return Type; }

    void sijoitaKiskolle( RataKisko* kiskolle, qreal sijainti, RaiteenPaa::Suunta suunta);
    void sijoitaKiskolle( RataKisko* kiskolle, qreal sijainti, QChar suuntakirjain);
    void asetaToinenAkseli(Akseli* toinenAkseli);

    RataKisko* kiskolla() { return kiskolla_; }
    qreal sijaintiKiskolla() { return sijaintiKiskolla_; }
    RaiteenPaa::Suunta suuntaKiskolla() { return suuntaKiskolla_; }
    QChar suuntaKirjain();

    bool onkoKytketty() const { return  kytkettyAkseli_!=0;  }
    void liiku( qreal matka);

    void moottoriLiike( qreal matka);
    void kytkinLiike( qreal matka);
    void vaunuLiike( qreal matka);

    void irrota();
    void irrotaLiiatVaunut(int vaunuja);    /** Irrottaa TAAKSEPÄIN jos enempi vaunuja */

    qreal junanPituusKysely(qreal tahanasti);
    bool ajopoytaKysely();  /** Selvittää, onko junassa aktiivista ajopöytää */

    Akseli* kytkettyAkseli() { return kytkettyAkseli_; }
    Vaunu* vaunu() { return vaunu_; }

private:

    void laskeSijainti();
    void vaihdaSuunta();

    Vaunu* vaunu_;

    /** Saman vaunun toinen akseli */
    Akseli* toinenAkseli_;

    /** Kytkimellä kiinni liitetty akseli*/
    Akseli* kytkettyAkseli_;

    /** Kisko, jolla ollaan */
    RataKisko* kiskolla_;

    /** sijainti kiskolla, väli 0 - pituus */
    qreal sijaintiKiskolla_;

    /** Positiivinen kulkusuunta */
    RaiteenPaa::Suunta suuntaKiskolla_;

};

#endif // AKSELI_H
