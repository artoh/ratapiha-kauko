/**************************************************************************
**  veturi.h
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
**  Veturi  22.7.2012
**************************************************************************/

#ifndef VETURI_H
#define VETURI_H

#include <QObject>
#include "vaunu.h"
#include "QTimer"

class RataScene;

class Veturi : public QObject, public Vaunu
{
    Q_OBJECT
public:
    enum { Type = UserType + 510 } ;

    enum VeturiTyyppi { EiTyyppia = 0, Sr1, Sm2, Sm3, Sm4, Dv12, Dr16, Dm12 };


    Veturi(const QString& tyyppi, int vaununumero, RataScene* skene);
    Veturi(const QString &tyyppi, int vaunuNumero, RataKisko* etu_kisko, qreal etu_etaisyys, QChar etu_suunta,
          RataKisko* taka_kisko, qreal taka_etaisyys, QChar taka_suunta, RataScene* skene);
    
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int nopeusMs() const { return metriaSekunnissa_; }
    int nopeus() const { return metriaSekunnissa_ * 3.6; }  /** Nopeus km/h */

    int ajopoyta() const { return ajopoyta_; }

    qreal kiihtyvyys() const { return 1.0; }
    qreal hidastuvuus() const { return 1.0; }
    int enimmaisNopeus() const { return 120; }

    virtual AjoPoydat ajopoydat() const { return MolemmatAjopoydat; }

    int tavoiteNopeus() const { return tavoiteNopeus_; }

    void kirjoitaLokiin(const QString& ilmoitustyyppi, RataRaide* raide = 0,
                        const QString& lisatieto=QString());

    void siirtyyRaiteelle(RataRaide *raiteelle);
signals:
    void nopeusIlmoitus(int nopeus);
    
public slots:
    void asetaAjoPoyta(int poyta);
    void asetaTavoiteNopeus(int tavoiteNopeus);

    void aja();

protected:
    void merkitseTyyppi(const QString& tyyppi);

    QTimer timer_;

    int tavoiteNopeus_;
    qreal metriaSekunnissa_;

    int ajopoyta_;

    VeturiTyyppi veturiTyyppi_;
    
};

#endif // VETURI_H
