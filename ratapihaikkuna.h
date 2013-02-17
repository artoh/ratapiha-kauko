/**************************************************************************
**  ratapihaikkuna.h
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
**  RatapihaIkkuna  23.7.2012
**************************************************************************/

#ifndef RATAPIHAIKKUNA_H
#define RATAPIHAIKKUNA_H

#include "palvelin.h"

#include <QMainWindow>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QPrinter>
#include <QDateTime>

class RataScene;


namespace Ui {
class RatapihaIkkuna;
}

/**  Ohjelman pääikkuna

    Singleton: näitä voi olla vain yksi
    Yhteyksien muodostaminen ja toimintaikkunoiden avaaminen

  */
class RatapihaIkkuna : public QMainWindow
{
    Q_OBJECT
    
public:
    enum YhteysTila { EiYhteytta = 0, LukuYhteys, KaukoYhteys, PaikallinenPalvelin };

    ~RatapihaIkkuna();
    
    /** Instanssi saadaan vain tämän kautta !!! */
    static RatapihaIkkuna* getInstance();
    static RatapihaIkkuna* createInstance();

    RataScene* skene() { return ratascene_; }

    YhteysTila tila() const { return yhteystila_; }

    /* Toteuttaa yksittäisen asetinlaitekäskyn */
    bool aslKasky(const QString& kasky);

    /** Asiakkaita lisää tai vähemmän */
    void asiakasMuutos(int muutos);

public slots:
    void yhdistaPalvelimeen();
    void yhdistettyPalvelimeen();   // tiedetään, että yhteys pelaa...
    void verkkovirhe();             // että ei pelaa..
    void palvelinSulkiYhteyden();
    void katkaiseYhteys();
    void vastausPalvelimelta();

    void kaynnistaPalvelin();
    void pysaytaPalvelin();

    void ohjausIkkuna();
    void rataIkkuna();

    void muokkaaNakymaa();
    void muokkaaRataa();
    void muokkaaLiikennepaikat();

    void graafinenAikataulu();
    void selaaAikataulua();
    void muokkaaReitteja();
    void aikatauluMonitori();

    /** Onko yhteyttä asetinlaitteelle? */
    bool onkoYhteydessa();

    void aikaMuuttunut(const QDateTime& aika);

    QDateTime simulaatioAika() { return simulaatioAika_; }

signals:
    /* Asetinlaitteelle annettuun käskyyn tullut vastaus */
    void aslVastaus(const QString& kasky);
    void yhdistetty(bool onkoyhteydessa);

    void kello(const QDateTime& aika);


private:
    bool yhdistaTietokantaan();

    void nappienHimmennykset();
    void lukuYhteysMuodostettu();
    void muokkaaNakymaa(int nakyma);

    /** Muodostin on private, jotta singleton! */
    RatapihaIkkuna(QWidget *parent = 0);

    Ui::RatapihaIkkuna *ui;

    QTcpSocket tcpsokka_;
    QSqlDatabase tietokanta_;
    Palvelin* palvelin_; // Tcp-palvelin

    YhteysTila yhteystila_;
    RataScene* ratascene_;

    int asiakkaita_;
    QDateTime simulaatioAika_;

    static RatapihaIkkuna* instance__;
};

#endif // RATAPIHAIKKUNA_H
