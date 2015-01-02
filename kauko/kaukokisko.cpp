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

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QFont>

#include "kaukokisko.h"
#include "kaukoraide.h"

#include "ratapiha.h"

using namespace Ratapiha;

KaukoKisko::KaukoKisko(KaukoRaide *raide, const QString &kiskotieto, qreal pituus)
    : raide_(raide), pituus_(pituus)


{
    naytaRaidenumero_ = kiskotieto.contains("Nr");
    laituriVasemmalla_ = kiskotieto.contains("Lv");
    laituriOikealla_ = kiskotieto.contains("Lo");
    silta_ = kiskotieto.contains("Silta");

    if(silta_)
        setZValue(100); // Sillan pitää olla vähän ylempänä

    if( kiskotieto.contains("E*"))
        etelaPaa_ = PAASSA;
    else if( kiskotieto.contains("E-"))
        etelaPaa_ = VASEN;
    else if( kiskotieto.contains("E+"))
        etelaPaa_ = OIKEA;
    else if( kiskotieto.contains("E="))
        etelaPaa_ = KANTA;

    if( kiskotieto.contains("P*"))
        pohjoisPaa_ = PAASSA;
    else if( kiskotieto.contains("P-"))
        pohjoisPaa_ = VASEN;
    else if( kiskotieto.contains("P+"))
        pohjoisPaa_ = OIKEA;
    else if( kiskotieto.contains("P="))
        pohjoisPaa_ = KANTA;

}

void KaukoKisko::paint(QPainter *painter, const QStyleOptionGraphicsItem * /* option */, QWidget * /* widget */)
{
    // Piirretään tyypin mukaan
    if( raide()->tyyppi() == RAIDE_RAIDE)
        piirraRaide(painter);
    else if( raide()->tyyppi() == RAIDE_VAIHDE || raide()->tyyppi() == RAIDE_RISTEYSVAIHDE)
        piirraVaihde(painter);
    else if( raide()->tyyppi() == RAIDE_RAIDERISTEYS)
        piirraRaideRisteys(painter);


}

QRectF KaukoKisko::boundingRect() const
{
    return QRectF( -5.0, -15.0, pituus()+10.0, 30.0);
}

void KaukoKisko::valkky()
{
    valkky__ = !valkky__;
}

QColor KaukoKisko::raidevari()
{
    if( raide()->vapaana() == RAIDE_VARATTU)
        return Qt::red;
    else if( raide()->kulkutieTyyppi() == JUNAKULKUTIE)
        return Qt::green;
    else if( raide()->vapaana() == RAIDE_VAPAA)
        return Qt::white;
    else if( raide()->vapaana() == RAIDE_VIKATILA)
    {
        if( valkkyyko())
            return Qt::gray;
        else
            return Qt::red;
    }

    return Qt::magenta;
}

QColor KaukoKisko::opastevari(Opaste opaste)
{
    switch ( opaste)
    {
    case OPASTE_SEIS : return Qt::red;
    case OPASTE_AJA: case OPASTE_AJASN: return Qt::green;
    case OPASTE_AJAVAROVASTI: return Qt::yellow;
    default:
        return Qt::black;
    }
}

void KaukoKisko::piirraRaide(QPainter *painter)
{

    if( silta_)
    {
        painter->setBrush( Qt::gray);
        painter->setPen(Qt::NoPen);
        painter->drawRect(QRectF(0.0, -3.0, pituus(),  6.0));
        painter->setPen( QPen(QBrush(Qt::black),0.7, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        painter->drawLine( QLineF(4.0, -3.0, pituus()-4.0, -3.0));
        painter->drawLine( QLineF(4.0, 3.0, pituus()-4.0, 3.0));
    }

    // Piirtää suoran raiteen
    qreal alku = 0.0;
    qreal loppu = pituus();
    if( etelaPaassa()==PAASSA)
    {
        alku = 0.8;

        // Sitten mahdolliset opastimet
        if( raide()->etela()->opastinlaji() == Ratapiha::OPASTIN_YHDISTELMAOPASTIN)
        {
            painter->setPen(QPen(QBrush(Qt::black), 0.5));
            painter->drawRect(alku, -5.0, alku+10.0, 10.0);

            Ratapiha::Opaste opaste = raide()->etela()->opaste();
            if( opaste == Ratapiha::OPASTE_SEIS || opaste == Ratapiha::OPASTE_AJA || opaste == Ratapiha::OPASTE_AJASN)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush( opastevari(opaste));
            }
            else
            {
                painter->setPen(QPen(QBrush(opastevari(opaste)), 1.0));
                painter->setBrush(Qt::NoBrush);
            }
            QPolygonF kuvio;
            kuvio << QPointF(alku + 8.5, 4.0) << QPointF(alku + 0.5, 0.0) << QPointF(alku + 8.5, -4.0);
            painter->drawPolygon( kuvio);
            alku += 10.25;
        }
        else if( raide()->etela()->opastinlaji()==Ratapiha::OPASTIN_PAAOPASTIN)
        {
            Ratapiha::Opaste opaste = raide()->etela()->opaste();
            QPolygonF kuvio;
            kuvio << QPointF(alku + 8.0, 4.0) << QPointF(alku + 0, 0.0) << QPointF(alku + 8.0, -4.0);
            painter->setPen(Qt::NoPen);
            painter->setBrush( opastevari(opaste));
            painter->drawPolygon( kuvio);
            alku += 8.25;
        }
        else if( raide()->etela()->opastinlaji()==Ratapiha::OPASTIN_RAIDEOPASTIN)
        {
            Ratapiha::Opaste opaste = raide()->etela()->opaste();
            QPolygonF kuvio;
            kuvio << QPointF(alku + 8.0, 4.0) << QPointF(alku + 0, 0.0) << QPointF(alku + 8.0, -4.0);
            painter->setPen(opastevari(opaste));
            painter->setBrush( Qt::NoBrush);
            painter->drawPolyline( kuvio);
            alku += 8.25;
        }

        // Suojastusnuolia
        if( raide()->etela()->suojastusTila() != SUOJASTUS_EISUOJASTUSTA)
        {
            QColor sisaannuoli(Qt::white);
            QColor ulosnuoli(Qt::white);

            if( raide()->etela()->suojastusSuunta() == SUOJASTUS_SISAAN)
            {
                ulosnuoli = Qt::gray;
                if( raide()->etela()->suojastusTila() == SUOJASTUS_VALMIS)
                    sisaannuoli = Qt::green;
                else if( raide()->etela()->suojastusTila() == SUOJASTUS_VARATTU)
                    sisaannuoli = Qt::yellow;
            }
            else if( raide()->etela()->suojastusSuunta() == SUOJASTUS_ULOS)
            {
                sisaannuoli = Qt::gray;
                if( raide()->etela()->suojastusTila() == SUOJASTUS_VALMIS)
                    ulosnuoli = Qt::yellow;
                else if( raide()->etela()->suojastusTila() == SUOJASTUS_VARATTU)
                    ulosnuoli = Qt::red;
            }

            painter->setPen(Qt::NoPen);
            painter->setBrush(QBrush( sisaannuoli ));
            QPolygonF kuvio;
            kuvio << QPointF( alku+2, -7.0) << QPointF(alku + 18.0, -3.0) << QPointF( alku + 18.0, -11.0);
            painter->drawPolygon(kuvio);

            kuvio.clear();
            painter->setBrush( QBrush( ulosnuoli));
            kuvio << QPointF( alku+2, 3.0) << QPointF( alku + 18.0, -7.0) << QPointF(alku + 2.0, -11.0);
            painter->drawPolygon(kuvio);

        }


    }

    if( pohjoisPaassa() == PAASSA )
    {
        loppu -= 0.8;
        // Sitten mahdolliset opastimet
        if( raide()->pohjoinen()->opastinlaji() == Ratapiha::OPASTIN_YHDISTELMAOPASTIN)
        {
            painter->setPen(QPen(QBrush(Qt::black), 0.5));
            painter->setBrush(Qt::NoBrush);
            painter->drawRect(loppu-10.25, -5.0, 10.0, 10.0);

            Ratapiha::Opaste opaste = raide()->pohjoinen()->opaste();
            if( opaste == Ratapiha::OPASTE_SEIS || opaste == Ratapiha::OPASTE_AJA || opaste == Ratapiha::OPASTE_AJASN)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush( opastevari(opaste));
            }
            else
            {
                painter->setPen(QPen(QBrush(opastevari(opaste)), 1.0));
                painter->setBrush(Qt::NoBrush);
            }
            QPolygonF kuvio;
            kuvio << QPointF(loppu - 8.75, 4.0) << QPointF(loppu - 0.75, 0.0) << QPointF(loppu - 8.75, -4.0);
            painter->drawPolygon( kuvio);
            loppu -= 10.25;
        }
        else if( raide()->pohjoinen()->opastinlaji()==Ratapiha::OPASTIN_PAAOPASTIN)
        {
            Ratapiha::Opaste opaste = raide()->pohjoinen()->opaste();
            QPolygonF kuvio;
            kuvio << QPointF(loppu - 8.0, 4.0) << QPointF(loppu - 0, 0.0) << QPointF(loppu - 8.0, -4.0);
            painter->setPen(Qt::NoPen);
            painter->setBrush( opastevari(opaste));
            painter->drawPolygon( kuvio);
            loppu -= 8.25;
        }
        else if( raide()->pohjoinen()->opastinlaji()==Ratapiha::OPASTIN_RAIDEOPASTIN)
        {
            Ratapiha::Opaste opaste = raide()->pohjoinen()->opaste();
            QPolygonF kuvio;
            kuvio << QPointF(loppu - 8.0, 4.0) << QPointF(loppu - 0, 0.0) << QPointF(loppu - 8.0, -4.0);
            painter->setPen(opastevari(opaste));
            painter->setBrush( Qt::NoBrush);
            painter->drawPolyline( kuvio);
            loppu -= 8.25;
        }
        // Suojastusnuolia
        if( raide()->pohjoinen()->suojastusTila() != SUOJASTUS_EISUOJASTUSTA)
        {
            QColor sisaannuoli(Qt::white);
            QColor ulosnuoli(Qt::white);

            if( raide()->pohjoinen()->suojastusSuunta() == SUOJASTUS_SISAAN)
            {
                ulosnuoli = Qt::gray;
                if( raide()->pohjoinen()->suojastusTila() == SUOJASTUS_VALMIS)
                    sisaannuoli = Qt::green;
                else if( raide()->pohjoinen()->suojastusTila() == SUOJASTUS_VARATTU)
                    sisaannuoli = Qt::yellow;
            }
            else if( raide()->etela()->suojastusSuunta() == SUOJASTUS_ULOS)
            {
                sisaannuoli = Qt::gray;
                if( raide()->pohjoinen()->suojastusTila() == SUOJASTUS_VALMIS)
                    ulosnuoli = Qt::yellow;
                else if( raide()->pohjoinen()->suojastusTila() == SUOJASTUS_VARATTU)
                    ulosnuoli = Qt::red;
            }

            painter->setPen(Qt::NoPen);
            painter->setBrush(QBrush( sisaannuoli ));
            QPolygonF kuvio;
            kuvio << QPointF( loppu - 2, -7.0) << QPointF(loppu - 18.0, -3.0) << QPointF( loppu - 18.0, -11.0);
            painter->drawPolygon(kuvio);

            kuvio.clear();
            painter->setBrush( QBrush( ulosnuoli));
            kuvio << QPointF( loppu - 2, 3.0) << QPointF( loppu - 18.0, -7.0) << QPointF(loppu - 2.0, -11.0);
            painter->drawPolygon(kuvio);

        }

    }

    painter->setPen( QPen(QBrush(raidevari()),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->drawLine(alku, 0.0, loppu , 0.0);


    if( naytaRaidenumero_ )
    {
        painter->setFont( QFont("Helvetica",4,QFont::Bold));
        painter->setPen( QPen(Qt::black));
        painter->drawText(QRectF(-10.0, -9.0, pituus()+20, 5.0), raide_->raidenumeroteksti() , QTextOption(Qt::AlignCenter));

        // Lukitusneliö. Vilkkuva neliö tarkoittaa, että on lukittumassa ja kiinteä, että on lukittu
        // Myöhemmin: punainen vilkkuva vikatilaa, punainen hätävaraista purkamista
        painter->setBrush( Qt::gray);
        painter->setPen( Qt::NoPen);

        if( raide()->elementinLukitus() == ELEMENTTI_LUKITTU )
            painter->setBrush( QBrush(Qt::green));
        else if( raide()->elementinLukitus() == ELEMENTTI_LUKITAAN && valkkyyko() )
            painter->setBrush( QBrush(Qt::green));

        painter->drawRect( pituus() / 2.0 - 0.5, 2.0, 2.5, 2.5 );


    }
    if( laituriVasemmalla_)
    {
        painter->setPen( QPen(QBrush(Qt::black), 1.0, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin) );
        QPolygonF laituriViiva;
        laituriViiva << QPointF(10.0, -7.0) << QPointF(10.0, -4.0) << QPointF( pituus()-10.0, -4.0) << QPointF( pituus()-10.0, -7.0);
        painter->drawPolyline(laituriViiva);
    }
    if( laituriOikealla_)
    {
        painter->setPen( QPen(QBrush(Qt::black), 1.0, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin) );
        QPolygonF laituriViiva;
        laituriViiva << QPointF(10.0, 7.0) << QPointF(10.0, 4.0) << QPointF( pituus()-10.0, 4.0) << QPointF( pituus()-10.0, 7.0);
        painter->drawPolyline(laituriViiva);
    }


}

void KaukoKisko::piirraVaihde(QPainter *painter)
{

    qreal alku = 0.0;
    qreal loppu = pituus();


    PaanPiirtoOhje ohje = VARILLA;

    // Kumpaa vaihdetta tämä kisko kuvaa
    KaukoraideVaihde *vaihde = raide()->vaihdeAB();
    if( raide()->tyyppi() == RAIDE_RISTEYSVAIHDE && pohjoisPaassa() == PAASSA )
        vaihde = raide()->vaihdeCD();


    // Etsitään "piirto-ohje", joka kuvaa, miten tämä haara piirretään
    if( etelaPaassa() == PAASSA )
    {
        // Vaihde on kiskon pohjoispäässä
        ohje = paanPiirtoOhje(pohjoisPaassa(), vaihde );
        alku = 0.8;
    }
    else
    {
        ohje = paanPiirtoOhje(etelaPaassa(), vaihde );
        loppu = pituus()-0.8;
    }

    // Väriksi raiteen varauksesta riippuva väri
    QColor vari(raidevari());

    // Jos tämä sakara on pimeä (passiivinen) tai välkkyy (kääntymässä)
    if( ohje == PIMEA_VALKOINEN ||
            ( valkkyyko() && ohje == VALKKYEN_VARI   ) )
    {
        // Vaihteenpuoleista viivaa ei piirretä
        if( etelaPaassa() == PAASSA )
        {
            if( pituus() > 15)
                loppu -= 10.0;
            else
                loppu -= pituus() / 2 ;  // Jotta lyhyeenkin jäisi vähän piirrettävää
        }
        else
        {
            if( pituus() > 15)
                alku += 10.0;
            else
                alku += pituus() / 2 ;
        }

        if( ohje == PIMEA_VALKOINEN)
            vari = Qt::white;
    }
    else if( ohje == VALKKYEN_KOKONAAN && valkkyyko())
        vari = Qt::gray;    // Koko raide välkkyy



    painter->setPen( QPen(QBrush( vari ),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->drawLine(alku, 0.0, loppu , 0.0);

    if( naytaRaidenumero_ )
    {
        painter->setFont( QFont("Helvetica",4,QFont::Bold));
        painter->setPen( QPen(Qt::blue));
        painter->drawText(QRectF(-10.0, -9.0, pituus()+20, 5.0), raide_->raidenumeroteksti() , QTextOption(Qt::AlignCenter));
    }


    QColor lukitusvari(Qt::gray);
    if( vaihde->lukitus() == ELEMENTTI_LUKITTU || (vaihde->lukitus()==ELEMENTTI_LUKITAAN && !valkkyyko() ) )
        lukitusvari = Qt::green;

    // Vaihteen lukitusneliö. Vilkkuva neliö tarkoittaa, että on lukittumassa ja kiinteä, että on lukittu
    // Risteysvaihteelle piirretään molempien puoliskojen alle, aktiiviselle puoliskolle
    if(( ( (raide()->tyyppi() == RAIDE_VAIHDE) && ( etelaPaassa() == KANTA || pohjoisPaassa() == KANTA) )) ||
         ( (raide()->tyyppi() == RAIDE_RISTEYSVAIHDE) &&
               (( ( (etelaPaassa() == VASEN) || (pohjoisPaassa() == VASEN) )  && vaihde->asento()==ASENTO_VASEMMALLE) ||
               ( ( (etelaPaassa() == OIKEA) || (pohjoisPaassa() == OIKEA) ) && vaihde->asento() == ASENTO_OIKEALLE) ) ))
    {
        painter->setBrush( QBrush( lukitusvari ));
        painter->setPen( Qt::NoPen);

        painter->drawRect( pituus() / 2.0 - 0.5, 2.0, 2.5, 2.5 );
    }

}

void KaukoKisko::piirraRaideRisteys(QPainter *painter)
{

    bool aktiivihaara = false;

    if( 1==1 )
    {
        if( raide()->vaihdeAB()->asento() == ASENTO_VASEMMALLE &&
                (etelaPaassa()==VASEN || pohjoisPaassa() == VASEN ))
            // Ollaan aktiivihaarassa
            aktiivihaara = true;
        else if( raide()->vaihdeAB()->asento() == ASENTO_OIKEALLE &&
                 (etelaPaassa()==OIKEA  || pohjoisPaassa()==OIKEA))
                 aktiivihaara = true;
    }

    if( etelaPaassa() == PAASSA)
    {
        if( aktiivihaara)
            painter->setPen( QPen(QBrush(raidevari()),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        else
            painter->setPen( QPen(QBrush(Qt::white),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

        painter->drawLine(0.0, 0.0, pituus()-4.0 , 0.0);

        if( aktiivihaara || raide()->vaihdeAB()->asento() == ASENTO_EITIEDOSSA)
        {
            painter->setPen( QPen(QBrush(raidevari()),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
            painter->drawLine(pituus()-3.5, 0.0, pituus(), 0.0);
        }

    }
    else
    {
        // Pohjoinen pääty, eli alkaa risteystiedolla
        if( aktiivihaara || raide()->vaihdeAB()->asento() == ASENTO_EITIEDOSSA)
        {
            painter->setPen( QPen(QBrush(raidevari()),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
            painter->drawLine(-1.0, 0.0, 3.5, 0.0);
        }
        if( aktiivihaara)
            painter->setPen( QPen(QBrush(raidevari()),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        else
            painter->setPen( QPen(QBrush(Qt::white),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

        painter->drawLine(4.0, 0.0, pituus() , 0.0);

    }
    if( naytaRaidenumero_ )
    {
        painter->setFont( QFont("Helvetica",4,QFont::Bold));
        painter->setPen( QPen(Qt::blue));
        painter->drawText(QRectF(-10.0, -9.0, pituus()+20, 5.0), raide_->raidenumeroteksti() , QTextOption(Qt::AlignCenter));

    }

}

KaukoKisko::PaanPiirtoOhje KaukoKisko::paanPiirtoOhje(KaukoKisko::PaanTila paantila, KaukoraideVaihde *vaihde)
{
    // Tutkii, ollaanko kyseisellä pään tilalla aktiivisessa haarassa
    // Valvottu vaihde : käännetty tätä kohti
    if( vaihde->tila() == KaukoraideVaihde::VALVOTTU)
    {
        if(( paantila == VASEN  && vaihde->asento()==ASENTO_VASEMMALLE) ||
             (paantila == OIKEA && vaihde->asento() == ASENTO_OIKEALLE) ||
             paantila == KANTA )
            return VARILLA;
        else
            return PIMEA_VALKOINEN; // Passiivisen kannan toinen pää valkoisella
    }
    else if( vaihde->tila() == KaukoraideVaihde::KAANTYY)
    {
        if(( paantila == VASEN  && vaihde->asento()==ASENTO_VASEMMALLE) ||
             (paantila == OIKEA && vaihde->asento() == ASENTO_OIKEALLE) ||
             paantila == KANTA )
            return VALKKYEN_VARI;
        else
            return PIMEA_VALKOINEN; // Passiivisen kannan toinen pää valkoisella
    }
    else if( vaihde->tila() == KaukoraideVaihde::AUKIAJETTU)
    {
        return VALKKYEN_KOKONAAN;
    }

    // Erilaisissa vikatiloissa koko vaihde näytetään raidetilan mukaisella värillä

    return VARILLA;
}

bool KaukoKisko::valkky__ = false;
