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


#ifndef KAUKOPALVELIN_H
#define KAUKOPALVELIN_H


#include <QObject>
#include <QList>

#include <QTcpServer>

class Asetinlaite;
class KaukokaytonNakyma;

/**
 * @brief Palvelin kaukokäyttöyhteyksiä varten
 */
class KaukoPalvelin : public QObject
{
    Q_OBJECT
public:
    KaukoPalvelin(Asetinlaite *asetinlaite);

    bool kaynnistaPalvelin(int portti);

    void lataaSql();

    QString nakymaLista() const { return nakymaLista_; }
    KaukokaytonNakyma *nakyma(int nakymaid);

    Asetinlaite *asetinlaite() { return asetinlaite_; }

signals:
    void asiakasMaaraMuutos(int asiakkaita);

public slots:
    void yhteysSuljettu();

private slots:
    void uusiYhteys();

private:
    Asetinlaite* asetinlaite_;
    QList<KaukokaytonNakyma*> nakymat_;
    QString nakymaLista_;
    int asiakkaat_;

    QTcpServer *palvelin_;

};

#endif // KAUKOPALVELIN_H
