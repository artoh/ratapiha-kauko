/**************************************************************************
**  laajennettukisko.h
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
**  LaajennettuKisko  10.7.2012
**************************************************************************/

#ifndef LAAJENNETTUKISKO_H
#define LAAJENNETTUKISKO_H

#include "kisko.h"

/**  Kiskon id, liikennepaikka etc. tietoja joita rata ei tartte, muut kyllä
 */
class LaajennettuKisko : public Kisko
{
public:
    LaajennettuKisko(const QLineF viiva, int kiskoid=0, const QString liikennepaikka=QString(), int raide=0,
                     const QString& kiskodata = QString());

    QString liikennePaikka() const {return liikennepaikka_; }
    int kiskoId() const { return kiskoid_; }
    int raide() const { return raide_; }

    QList<LaajennettuKisko*> haeNaapurit(QPointF sijainnista);

    bool naytaRaideNumero() const { return naytaRaideNumero_; }
    bool naytaJunaNumero() const { return naytaJunaNumero_; }

    void tarkistaPaanTyypit();


protected:
    int kiskoid_;
    QString liikennepaikka_;
    int raide_;

    bool naytaJunaNumero_;
    bool naytaRaideNumero_;

private:
    PaanTyyppi tarkistaTyyppiPaalle( QPointF piste );

};

#endif // LAAJENNETTUKISKO_H
