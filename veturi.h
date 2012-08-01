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

#include "vaunu.h"
#include "jkvopaste.h"
#include "reittitieto.h"

#include <QTimer>
#include <QObject>

class JunaNuppi;
class RataScene;

class Veturi : public QObject, public Vaunu
{
    Q_OBJECT
public:
    enum { Type = UserType + 510 } ;

    enum VeturiTyyppi { EiTyyppia = 0, Sr1, Sr2, Sm2, Sm3, Sm4, Dv12, Dr16, Dm12, Eio };
    enum JkvTila { EiJkv, JunaJkv, VaihtoJkv } ;
    enum VeturiAutomaatio { AutoEi, AutoOn, AutoAktiivinen } ;

    Veturi(const QString& tyyppi, int vaununumero, RataScene* skene);
    Veturi(const QString &tyyppi, int vaunuNumero, RataKisko* etu_kisko, qreal etu_etaisyys, QChar etu_suunta,
          RataKisko* taka_kisko, qreal taka_etaisyys, QChar taka_suunta, RataScene* skene);

    int type() const { return Type; }

    void tietojaKannasta( int matkamittari, int tavoitenopeus, int ajopoyta, const QString& junanumero, JkvTila jkvtila, VeturiAutomaatio automaatiotila );
    
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int nopeusMs() const { return metriaSekunnissa_; }
    int nopeus() const { return metriaSekunnissa_ * 3.6; }  /** Nopeus km/h */

    int ajopoyta() const { return ajopoyta_; }
    int jkvNopeus() const { return jkvNopeus_; }

    qreal kiihtyvyys() const;
    qreal hidastuvuus() const;
    int enimmaisNopeus() const;

    virtual AjoPoydat ajopoydat() const ;

    int tavoiteNopeus() const { return tavoiteNopeus_; }

    void kirjoitaLokiin(const QString& ilmoitustyyppi, RataRaide* raide = 0,
                        const QString& lisatieto=QString());

    void siirtyyRaiteelle(RataRaide *raiteelle);
    QPixmap jkvKuva();
    qreal junaPituus() const { return junaPituus_; }
    qreal matkaMittari() const { return matkaMittari_; }

    int nopeusRajoitus() const { return nopeusRajoitus_; }
    int myohassa() const { return myohassa_; }

    QString junaNumero() const { return junaNumero_; }
    void tarkistaRaiteenJunanumero();
    JkvTila jkvTila() const { return jkvTila_; }
    VeturiAutomaatio veturiAutomaationTila() const { return veturiAutomaatio_; }

    virtual void paivita();

signals:
    void nopeusIlmoitus(int nopeus);
    void automaatioIlmoitus(int ajopoyta, int tavoitenopeus, const QPixmap& jkvkuva);
    
public slots:
    void asetaAjoPoyta(int poyta);
    void asetaTavoiteNopeus(int tavoiteNopeus);

    void aja();
    void nayttoonKoskettu(QPoint pos);
    void kaynnista();   /** Kutsutaan timerilla, kun asetettu ajopöytä */

protected:
    bool haeReitti( Akseli *akseli);
    void merkitseTyyppi(const QString& tyyppi);
    void paivitaJkvTiedot();    // Päivittää jkv-tietojen luettelon
    Akseli* aktiivinenAkseli();
    void asetaReitti(const QString& reitti);
    bool tarkistaRaiteenNumeroAkselilta(Akseli* akseli);
    void tyhjennaReitti();

    QTimer timer_;

    int tavoiteNopeus_;
    qreal metriaSekunnissa_;

    int ajopoyta_;

    VeturiTyyppi veturiTyyppi_;
    RataRaide* edellinenLokiRaide_;

    QVector<JkvOpaste> jkvTiedot_;
    qreal jkvNopeus_;
    JkvTila jkvTila_;

    qreal junaPituus_;
    qreal matkaMittari_;

    QList<QPair<qreal,int > > nopeusRajoitukset_;
    int nopeusRajoitus_;
    JunaNuppi* junaNuppi_;

    QString junaNumero_;
    VeturiAutomaatio veturiAutomaatio_;

    QMap<QString,ReittiTieto> reitti_;
    QString maaraAsema_;
    QString reittitunnus_;

    QDateTime pysahtyi_;
    RataKisko* pysahtyiKiskolle_;

    qreal myohassa_;    /** Montako sekuntia ollaan myöhässä aikataulusta */


};

#endif // VETURI_H
