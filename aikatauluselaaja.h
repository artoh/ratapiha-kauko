/**************************************************************************
**  aikatauluselaaja.h
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
**  AikatauluSelaaja  4.8.2012
**************************************************************************/

#ifndef AIKATAULUSELAAJA_H
#define AIKATAULUSELAAJA_H

#include <QTextBrowser>
#include <QUrl>

class AikatauluSelaaja : public QTextBrowser
{
    Q_OBJECT
public:
    explicit AikatauluSelaaja(QWidget *parent = 0);

    enum Selattava { Tyhja, Juna, Asema } ;

    QString tunnus() const { return selattavanTunnus_; }
    Selattava tyyppi() const { return selattavanTyyppi_; }

signals:
    void naytetaanAsema(const QString& liikennepaikka);
    void naytetaanJuna( const QString& junanumero);
    
public slots:
    void klikattu(const QUrl& linkki);

    void haeJunaAikataulu(const QString& juna);
    void haeAsemaAikataulu(const QString& liikennepaikka);

    void asetaFiltteri(bool saapuvat, bool lahtevat, bool etelaan, bool pohjoiseen, bool lahi, bool kauko, bool muut);

    void paivita();

protected:
    void naytaLoki(const QString& junanumero);

    QString selattavanTunnus_;
    Selattava selattavanTyyppi_;

    bool lahtevat_;
    bool saapuvat_;
    bool etelaan_;
    bool pohjoiseen_;
    bool lahijunat_;
    bool kaukojunat_;
    bool muutjunat_;
};

#endif // AIKATAULUSELAAJA_H
