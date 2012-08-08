/**************************************************************************
**  kulkutie.h
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
**  Kulkutie  13.7.2012
**************************************************************************/

#ifndef KULKUTIE_H
#define KULKUTIE_H


#include "raidetieto.h"

#include <QList>
#include <QObject>

class KulkutienRaide;
class KulkutieElementti;
class RataRaide;

/** Tieto aktiivisesta kulkutiestä */
class KulkuTie : public QObject
{
    Q_OBJECT

public:

    KulkuTie(RaideTieto::Kulkutietyyppi kulkutientyyppi);
    ~KulkuTie();

    RaideTieto::Kulkutietyyppi kulkutienTyyppi() const { return kulkutienTyyppi_; }

    KulkutienRaide* lisaaElementti(KulkutieElementti* elementti);
    KulkutienRaide* lisaaElementti(RataRaide* raide, RaiteenPaa::Suunta suunta, const QString& lahtoOpastin, int moneskoraide, bool onkokaytetty);
    KulkutienRaide* lisaaElementti(const QString& raidesuunnalla, const QString& lahtoopastin);


    void poistaElementti(KulkutienRaide* elementti);    // Elementti itse kutsuu tätä purkautuessaan!
    void vahvistaKulkutie();    // Vahvistaa (suojastus)kulkutien
    void tarkista(); // Tätä kutsutaan joiden varautumisten yms jälkeen

    void raideVarautuu(KulkutienRaide* elementti);  // Elementti kutsuu varautuessaan
    void raideVapautuu(KulkutienRaide* elementti);  // Elementti kutsuu kun on varautunut

    int varattujaRaiteita();

    RataRaide* lahtoRaide();
    QString lahtoRaideTunnus();
    RataRaide* maaliRaide();
    QString maaliRaideTunnus();
    QString maaliRaideTunnusSuunnalla();

    void paivitaKaikki();
    KulkutienRaide* ekaRaide();



    RaideTieto::KulkutieTila tila() const { return tila_; }
    void vikatilaan();  // Kutsutaan, kun jossain elementissa havaitaan vika!

public slots:

   void puraKulkutie();    // Purkaa kulkutien
   void puraKulkutieViiveella(int sekuntia);

protected:
    RaideTieto::Kulkutietyyppi kulkutienTyyppi_;
    RaideTieto::KulkutieTila tila_;
    QList<KulkutienRaide*> elementit_;

    void lisaaListaan(KulkutienRaide* elementti); // Lisää listaan järjestyksessä
};

#endif // KULKUTIE_H
