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


#include <QStringList>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QFont>

#include "ajopoyta.h"
#include "ui_ajopoyta.h"

#include "ratapiha.h"

AjoPoyta::AjoPoyta(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AjoPoyta),
    valkky_(false)
{
    ui->setupUi(this);
    soketti_.connectToHost("localhost",5432);

    connect( ui->veturiValintaEdit, SIGNAL(returnPressed()), this, SLOT(valitseVeturi()));
    connect( &soketti_, SIGNAL(readyRead()), this, SLOT(paivita()));
    connect( ui->poyta1Nappi, SIGNAL(clicked(bool)), this, SLOT(ajoPoytaYksi(bool)));
    connect( ui->poyta2Nappi, SIGNAL(clicked(bool)), this, SLOT(ajoPoytaKaksi(bool)));
    connect( ui->nopeusSlider, SIGNAL(sliderMoved(int)), this, SLOT(muutaNopeus(int)));
    connect( ui->SeisOhitusNappi, SIGNAL(clicked(bool)), this, SLOT(seisOhitus(bool)));

    QTimer *kyselin = new QTimer(this);
    connect( kyselin, SIGNAL(timeout()), this, SLOT(pyydaTiedot()));
    kyselin->start(200);

    setWindowTitle("Ajopöytä");
}

AjoPoyta::~AjoPoyta()
{
    delete ui;
}

void AjoPoyta::paivita()
{

    bool poyta1 = false;
    bool poyta2 = false;
    int jkvNopeus = 0;
    int jkvMatka = 0;
    int nopeus = 0;
    int veturiId = 0;
    int maxNopeus = 120;
    QString tyyppi;
    Ratapiha::Opaste opaste = Ratapiha::OPASTE_PUUTTUU;
    bool ohitaSeis = false;
    valkky_ = !valkky_;

    while( soketti_.canReadLine())
    {
        QString rivi = soketti_.readLine().simplified();
        QStringList lista = rivi.split(' ');
        foreach (QString sana, lista)
        {
            if( sana == "A1")
            {
                poyta1 = true;
            }
            else if( sana == "A2")
            {
                poyta2 = true;
            }
            else if( sana.startsWith("N") )
            {
                nopeus = sana.mid(1).toInt();
                ui->nopeusLabel->setText(QString("%1 km/h").arg(nopeus));
            }
            else if( sana.startsWith("T"))
            {
                int tavoitenopeus = sana.mid(1).toInt();
                ui->nopeusSlider->setValue(tavoitenopeus);
            }
            else if( sana.startsWith('J'))
            {
                jkvNopeus = sana.mid(1).toInt();
            }
            else if( sana.startsWith('M'))
            {
                jkvMatka = sana.mid(1).toInt();
            }
            else if( sana.startsWith('O'))
            {
                if( sana == "OSEIS" )
                    opaste = Ratapiha::OPASTE_SEIS;
                else if(sana == "OAJA")
                    opaste = Ratapiha::OPASTE_AJA;
                else if( sana == "OSN")
                    opaste = Ratapiha::OPASTE_AJASN;
            }
            else if( sana == "SEISOHITUS")
                ohitaSeis = true;
            else if( sana.startsWith('m'))
                maxNopeus = sana.mid(1).toInt();
            else if( sana.startsWith('t'))
                tyyppi = sana.mid(1);
            else if( sana.startsWith('V'))
                veturiId = sana.mid(1).toInt();

        }

    }



    // PITÄISI VIELÄ TEHDÄ JKV-TIETOJEN NÄYTTÖ NÄYTTÖRUUDULLE
    // Nyt laittaa vain tekstiä, myöhemmin hieno näyttö, kuinkas muutenkaan
    QPixmap kuva(200,400);
    QPainter painter(&kuva);

    painter.setBrush( QBrush(Qt::black));
    painter.drawRect(0,0,200,400);

    QTextOption textOption;
    textOption.setAlignment(Qt::AlignCenter);

    if( poyta1 || poyta2)
    {
       qreal kmhAste = ( 270.0 / (maxNopeus + 20) );


        for(int n=0; n<maxNopeus+21; n=n+5)
        {
            painter.save();
            painter.translate(100,100);
            painter.rotate(45 + n * kmhAste);

            if( n % 20 == 0)
                painter.setPen(QPen(Qt::red));
            else
                painter.setPen(QPen(Qt::white));

            painter.drawLine(0, 70, 0, 80);

            if( n % 20 == 0)
            {
                painter.translate(0,90);
                painter.rotate(-180);
                painter.setFont( QFont("Helvetica",10));
                painter.drawText(QRectF( -10, -15,20,30), QString::number(n), textOption);
            }

            painter.restore();
        }

        painter.save();
        painter.translate(100,100);
        if( jkvNopeus > maxNopeus)
            jkvNopeus = maxNopeus;
        painter.rotate(45 + (qreal) jkvNopeus * kmhAste);

        painter.setPen(QPen(QBrush(Qt::green),2.0));
        painter.drawLine(QLine(0,20,0,90));
        painter.rotate(0-jkvNopeus * kmhAste);

        painter.rotate( nopeus * kmhAste);

        painter.setPen(Qt::NoPen);
        painter.setBrush( QBrush(Qt::white));
        QPolygon viisari;
        viisari << QPoint(-5,0) << QPoint(5,0) << QPoint(0,75);
        painter.drawPolygon(viisari);

        painter.restore();

        if( jkvNopeus + 1 < nopeus)
            painter.setPen(QPen(Qt::red));
        else if( jkvNopeus > nopeus + 1 )
            painter.setPen(QPen(Qt::green));
        else
            painter.setPen(QPen(Qt::white));

        painter.setFont( QFont("Helvetica",18));
        painter.drawEllipse(QPointF(100,100),25,25);
        painter.drawText( QRect(80,90,40,20), QString::number(jkvNopeus), textOption);


        if( jkvMatka < 600 )
            painter.setPen( QPen(Qt::red));
        else if( jkvMatka < 1200)
            painter.setPen( QPen(Qt::yellow));
        else if( jkvMatka < 4900)
            painter.setPen( QPen(Qt::green));

        if( jkvMatka < 4900)
        {
            painter.setFont( QFont("Helvetica",16));
            painter.drawText( QRectF(50,160,100,20), QString("%1 m").arg(jkvMatka), textOption);
        }

        if( opaste == Ratapiha::OPASTE_SEIS && (!ohitaSeis || valkky_))  // Välkkyy, jos ohitetaan SEIS-opastetta
            painter.drawPixmap(80,190, QPixmap(":/ajo/jkvkuvat/poSeis.png"));
        else if( opaste == Ratapiha::OPASTE_AJASN)
            painter.drawPixmap(80,190, QPixmap(":/ajo/jkvkuvat/poAjaSn.png"));
        else if( opaste == Ratapiha::OPASTE_AJA)
            painter.drawPixmap(80,190, QPixmap(":/ajo/jkvkuvat/poAja.png"));


    }
    ui->naytto->setPixmap(kuva);


    ui->poyta1Nappi->setChecked(poyta1);
    ui->poyta2Nappi->setChecked(poyta2);
    ui->SeisOhitusNappi->setChecked(ohitaSeis);

    if( maxNopeus)
        ui->nopeusSlider->setMaximum(maxNopeus + 20);

    if( veturiId)
        setWindowTitle(tr("%1 # %2").arg(tyyppi).arg(veturiId));
    else
        setWindowTitle("Ajopöytä");
}

void AjoPoyta::ajoPoytaYksi(bool onko)
{
    if( onko )
        soketti_.write("A1\n");
    else
        soketti_.write("A0\n");
}

void AjoPoyta::ajoPoytaKaksi(bool onko)
{
    if( onko)
        soketti_.write("A2\n");
    else
        soketti_.write("A0\n");
}

void AjoPoyta::valitseVeturi()
{
    soketti_.write( QString("V%1\n").arg(ui->veturiValintaEdit->text()).toLatin1() );
}

void AjoPoyta::muutaNopeus(int nopeus)
{
    soketti_.write( QString("T%1\n").arg(nopeus).toLatin1());
}

void AjoPoyta::seisOhitus(bool onko)
{
    if( onko )
        soketti_.write("JKVSEISOHITUS\n");
    else
        soketti_.write("JKVON");
}

void AjoPoyta::pyydaTiedot()
{
    soketti_.write("\n");
}

