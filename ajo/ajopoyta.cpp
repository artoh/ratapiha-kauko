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

AjoPoyta::AjoPoyta(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AjoPoyta)
{
    ui->setupUi(this);
    soketti_.connectToHost("localhost",5432);

    connect( ui->veturiValintaEdit, SIGNAL(returnPressed()), this, SLOT(valitseVeturi()));
    connect( &soketti_, SIGNAL(readyRead()), this, SLOT(paivita()));
    connect( ui->poyta1Nappi, SIGNAL(clicked(bool)), this, SLOT(ajoPoytaYksi(bool)));
    connect( ui->nopeusSlider, SIGNAL(sliderMoved(int)), this, SLOT(muutaNopeus(int)));

    QTimer *kyselin = new QTimer(this);
    connect( kyselin, SIGNAL(timeout()), this, SLOT(pyydaTiedot()));
    kyselin->start(200);
}

AjoPoyta::~AjoPoyta()
{
    delete ui;
}

void AjoPoyta::paivita()
{
    while( soketti_.canReadLine())
    {
        bool poyta1 = false;
        bool poyta2 = false;
        int jkvNopeus = 0;
        int jkvMatka = 0;
        int nopeus = 0;
        QString tyyppi;

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

            // PITÄISI VIELÄ TEHDÄ JKV-TIETOJEN NÄYTTÖ NÄYTTÖRUUDULLE
            // Nyt laittaa vain tekstiä, myöhemmin hieno näyttö, kuinkas muutenkaan
            QPixmap kuva(200,400);
            QPainter painter(&kuva);
            painter.setBrush( QBrush(Qt::black));
            painter.drawRect(0,0,200,400);

            painter.setPen( QPen(QBrush(Qt::yellow), 2));

            painter.setFont( QFont("Helvetica",18));
            painter.drawText( QPoint(10,40), QString("%1 km/h").arg(jkvNopeus));
            painter.drawText( QPointF(10,60), QString("%1 m").arg(jkvMatka ));
            ui->naytto->setPixmap(kuva);

        }
        ui->poyta1Nappi->setChecked(poyta1);
        ui->poyta2Nappi->setChecked(poyta2);
    }
}

void AjoPoyta::ajoPoytaYksi(bool onko)
{
    if( onko )
        soketti_.write("A1\n");
}

void AjoPoyta::valitseVeturi()
{
    soketti_.write( QString("V%1\n").arg(ui->veturiValintaEdit->text()).toLatin1() );
}

void AjoPoyta::muutaNopeus(int nopeus)
{
    soketti_.write( QString("T%1\n").arg(nopeus).toLatin1());
}

void AjoPoyta::pyydaTiedot()
{
    soketti_.write("\n");
}

