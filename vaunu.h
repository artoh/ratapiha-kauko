/**************************************************************************
**  vaunu.h
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
**  Vaunu  17.7.2012
**************************************************************************/

#ifndef VAUNU_H
#define VAUNU_H

#include "akseli.h"
#include "ratakisko.h"

#include <QGraphicsItem>
#include <QSvgRenderer>

class RataScene;

class Vaunu : public QGraphicsItem
{
public:
    enum { Type = UserType + 505 } ;
    enum AjoPoydat { EiAjopoytaa = 0, AjopoytaYksi, AjopoytaKaksi, MolemmatAjopoydat };

    Vaunu(const QString& tyyppi, int vaunuNumero, RataScene* skene);
    Vaunu(const QString &tyyppi, int vaunuNumero, RataKisko* etu_kisko, qreal etu_etaisyys, QChar etu_suunta,
          RataKisko* taka_kisko, qreal taka_etaisyys, QChar taka_suunta, RataScene* skene);

    bool sijoitaKiskolle(RataKisko* kiskolle);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int type() const { return Type; }
    QString vaununTyyppi() const { return vaununTyyppi_; }

    qreal pituus() const { return vaununPituus_; }

    virtual void paivita();
    void laskeSijainti();

    void poista();  // Vaunun poistaminen asettaa nollasijainnin

    int vaunuNumero() const { return vaununNumero_; }
    virtual AjoPoydat ajopoydat() const { return EiAjopoytaa; }
    virtual void siirtyyRaiteelle(RataRaide* raiteelle);  // Veturilokia varten!
    virtual void akseliKytketty() {;} // Junan pysäyttämistä varten...

    virtual void tormays(int nopeudella);

    Akseli* etuakseli() const { return etuAkseli_; }
    Akseli* takaakseli() const { return takaAkseli_; }

protected:
    void luoVaunu(RataScene* skene); // Yhteiset luontitoimet


    Akseli* etuAkseli_;
    Akseli* takaAkseli_;

    qreal vaununPituus_;
    QString vaununTyyppi_;

    int vaununNumero_;

    static QSvgRenderer* renderoija();
    static QSvgRenderer* vaununpiirtaja__;
};

#endif // VAUNU_H
