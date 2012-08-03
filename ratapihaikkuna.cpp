/**************************************************************************
**  ratapihaikkuna.cpp
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

#include "ratapihaikkuna.h"
#include "ui_ratapihaikkuna.h"

#include "ratascene.h"
#include "rataikkuna.h"
#include "kaukoikkuna.h"
#include "editoriikkuna.h"

// ** TILAPÄINEN **
#include "graafinenaikatauluscene.h"
#include <QGraphicsView>

#include <QMessageBox>
#include <QPixmap>
#include <QSqlError>
#include <QApplication>

#include <QSettings>

RatapihaIkkuna::RatapihaIkkuna(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RatapihaIkkuna),
    palvelin_(0),
    yhteystila_(EiYhteytta),
    ratascene_(0),
    asiakkaita_(0)
{
    ui->setupUi(this);

    tietokanta_ = QSqlDatabase::addDatabase("QMYSQL");

    setAttribute( Qt::WA_DeleteOnClose);

    connect( ui->yhdistaNappi, SIGNAL(clicked()), this, SLOT(yhdistaPalvelimeen()));
    connect( ui->katkaiseNappi, SIGNAL(clicked()), this, SLOT(katkaiseYhteys()));

    connect( ui->kaynnistaPalvelinNappi, SIGNAL(clicked()), this, SLOT(kaynnistaPalvelin()));
    connect( ui->rataButton, SIGNAL(clicked()), this, SLOT(rataIkkuna()));
    connect( ui->ohjausKaukoButton, SIGNAL(clicked()), this, SLOT(ohjausIkkuna()));
    connect( ui->ohjausPaikButton, SIGNAL(clicked()), this, SLOT(ohjausIkkuna()));
    connect( ui->pysaytaPalvelinNappi, SIGNAL(clicked()), this, SLOT(pysaytaPalvelin()));

    connect( ui->muokkaaNakymiaNappi, SIGNAL(clicked()), this, SLOT(muokkaaNakymaa()));
    connect( ui->muokkaaRataaNappi, SIGNAL(clicked()), this, SLOT(muokkaaRataa()));

    connect( ui->graafinenaikatauluNappi, SIGNAL(clicked()), this, SLOT(graafinenAikataulu()));

    connect( &tcpsokka_, SIGNAL(connected()), this, SLOT(yhdistettyPalvelimeen()));
    connect( &tcpsokka_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(verkkovirhe()));
    connect( &tcpsokka_, SIGNAL(disconnected()), this, SLOT(palvelinSulkiYhteyden()));
    connect( &tcpsokka_, SIGNAL(readyRead()), this, SLOT(vastausPalvelimelta()));

    QSettings settings;
    restoreGeometry( settings.value("geometry").toByteArray());
    ui->palvelinEdit->setText( settings.value("palvelin").toString());
    ui->porttiSpin->setValue( settings.value("portti",6432).toInt());
    ui->kayttajatunnusLine->setText( settings.value("kayttaja").toString());
    ui->salasanaLine->setText(settings.value("salasana").toString());
    ui->nopeusSlider->setValue( settings.value("nopeutus",10).toInt());
    ui->nopeusArvoLabel->setNum(  ui->nopeusSlider->value());
}

RatapihaIkkuna::~RatapihaIkkuna()
{
    qApp->closeAllWindows();    // Suljetaan KAIKKI ohjelman ikkunat

    QSettings settings;
    settings.setValue("geometry",saveGeometry());
    settings.setValue("palvelin", ui->palvelinEdit->text());
    settings.setValue("portti", ui->porttiSpin->value());
    settings.setValue("kayttaja", ui->kayttajatunnusLine->text());
    settings.setValue("salasana", ui->salasanaLine->text());
    settings.setValue("nopeutus",ui->nopeusSlider->value());

    delete ui;
    instance__ = 0; // Estetään käyttö luokkamuuttujan kautta

    if( ratascene_ )    // Tuhotaan skene
        delete ratascene_;
    if( palvelin_)
    {
        palvelin_->close();
        delete palvelin_;
    }

}


RatapihaIkkuna* RatapihaIkkuna::instance__ = 0;

RatapihaIkkuna* RatapihaIkkuna::getInstance()
{
    return instance__;
}

RatapihaIkkuna *RatapihaIkkuna::createInstance()
{
    instance__ = new RatapihaIkkuna();
    return instance__;
}

bool RatapihaIkkuna::aslKasky(const QString &kasky)
{
    if( tila() == PaikallinenPalvelin )
    {
        // Käsky ohjataan paikalliselle palvelimelle...
        QString vastaus = skene()->ASLKasky(kasky);
        // Ja vastaus komentonäkymiin
        emit aslVastaus( QString("[%1] %2").arg(kasky).arg(vastaus) );
        return true;
    }
    else if( tila() == KaukoYhteys)
    {
        // Käsky ohjataan verkon yli
        // ja vastaus tulee ajallaan
        QString viesti = kasky + "\n";
        tcpsokka_.write( viesti.toAscii() );
        return true;
    }

    // Ellei löydy yhteyttä, tulee virhe!
    emit aslVastaus( tr("[%1] JÄRJESTELMÄVIRHE - EI YHTEYTTÄ ").arg(kasky));
    return false;
}

void RatapihaIkkuna::asiakasMuutos(int muutos)
{
    asiakkaita_ += muutos;
    ui->tilaLabel->setText( tr("Palvelimella %1 asiakasta").arg(asiakkaita_));
}

void RatapihaIkkuna::yhdistaPalvelimeen()
{
    if( yhdistaTietokantaan())
    {
        tcpsokka_.connectToHost( ui->palvelinEdit->text(), ui->porttiSpin->value() );
        ui->varivalo->setPixmap( QPixmap(":/r/pic/ktas_keltainen.png") );
        ui->tilaLabel->setText( tr("Yhdistetään palvelimeen..."));
        yhteystila_ = LukuYhteys;
        nappienHimmennykset();
    }

}

void RatapihaIkkuna::yhdistettyPalvelimeen()
{
    ui->varivalo->setPixmap( QPixmap(":/r/pic/ktas_vihrea.png") );
    ui->tilaLabel->setText( tr("Yhdistetty palvelimeen."));
    yhteystila_ = KaukoYhteys;
    emit yhdistetty(true);
    nappienHimmennykset();
}

void RatapihaIkkuna::verkkovirhe()
{
    ui->tilaLabel->setText(tr("Virhe verkkoyhteydessä %1").arg( tcpsokka_.errorString() ));

}

void RatapihaIkkuna::palvelinSulkiYhteyden()
{
    ui->varivalo->setPixmap( QPixmap(":/r/pic/ktas_punainen.png") );
    ui->tilaLabel->setText( tr("Palvelin sulki yhteyden"));
    tcpsokka_.close();
    yhteystila_ = EiYhteytta;
    emit yhdistetty(false);
    nappienHimmennykset();
}

void RatapihaIkkuna::katkaiseYhteys()
{
    ui->varivalo->setPixmap( QPixmap(":/r/pic/ktas_punainen.png") );
    ui->tilaLabel->setText( tr("Yhteys palvelimeen katkaistu"));
    tcpsokka_.close();
    yhteystila_ = EiYhteytta;
    emit yhdistetty(false);
    nappienHimmennykset();
}

void RatapihaIkkuna::vastausPalvelimelta()
{
    // Ottaa vastauksen palvelimelta ja välittää eteenpäin
    // Ehkä joskus ohjauskomentojakin täältä tulisi

    while( tcpsokka_.canReadLine())
    {
        QString vastaus = tcpsokka_.readLine();
        if( vastaus.startsWith('@'))
        {
            // Ohjauskomento
            if( vastaus.startsWith("@aika"))
                // Ilmoitus kellonajasta
                aikaMuuttunut( QDateTime::fromString(vastaus.mid(6),Qt::ISODate));
        }
        else
            // On vastaus asetinlaitekomentoon
            emit aslVastaus( vastaus );
    }
}


void RatapihaIkkuna::kaynnistaPalvelin()
{
    if( yhdistaTietokantaan())
    {
        // Luodaan skene
        ratascene_ = new RataScene(this);
        connect( ratascene_, SIGNAL(kello(QDateTime)), this, SLOT(aikaMuuttunut(QDateTime)));

        ratascene_->asetaNopeutus( ui->nopeusSlider->value() );

        connect( ui->nopeusSlider, SIGNAL(valueChanged(int)), ratascene_, SLOT(asetaNopeutus(int)) );

        yhteystila_ = PaikallinenPalvelin;
        nappienHimmennykset();

        if( !palvelin_)
            palvelin_ = new Palvelin(this);

        if( palvelin_->listen(QHostAddress::Any, ui->porttiSpin->value()))
        {
            // Palvelimen käynnistäminen onnistui
            ui->varivalo->setPixmap( QPixmap(":/r/pic/ktas_vihrea.png") );
            ui->tilaLabel->setText( tr("Palvelin käynnistetty"));

        }
        else
        {
            // Palvelimen käynnistäminen ei onnistunut, mutta kuitenkin
            // palvellaan paikallisesti
            ui->varivalo->setPixmap( QPixmap(":/r/pic/ktas_oranssi.png") );
            ui->tilaLabel->setText( tr("Paikallinen käyttö, verkkopalvelin ei käytössä"));
            palvelin_->close();
            delete palvelin_;
            palvelin_ = 0;

        }

        emit yhdistetty(true);
    }

}

void RatapihaIkkuna::pysaytaPalvelin()
{
    yhteystila_ = EiYhteytta;
    emit yhdistetty(false);
    if( palvelin_)
    {
        palvelin_->close();
        delete palvelin_;
        palvelin_ = 0;
    }
    // Ensin suljetaan paikalliset rataikkunat
    // (kaukoikkunat eivät haittaa, vaikka eivät sitten saakaan yhteyttä)
    foreach( QWidget *win, qApp->topLevelWidgets())
    {
        if( RataIkkuna* ikkuna = qobject_cast<RataIkkuna*>(win))
            ikkuna->close();
    }
    // Sitten poistetaan skene
    delete ratascene_;
    ratascene_ = 0;

    // Ja muutetaan tiedot
    nappienHimmennykset();

    ui->varivalo->setPixmap( QPixmap(":/r/pic/ktas_punainen.png") );
    ui->tilaLabel->setText( tr("Paikallinen palvelin pysäytetty"));

}


void RatapihaIkkuna::ohjausIkkuna()
{
    KaukoIkkuna* ikkuna = new KaukoIkkuna(this);
    ikkuna->show();
    connect( this, SIGNAL(kello(QDateTime)), ikkuna, SLOT(paivitaKello(QDateTime)));
}

void RatapihaIkkuna::rataIkkuna()
{
    RataIkkuna* ikkuna = new RataIkkuna(this);
    ikkuna->show();
}

void RatapihaIkkuna::muokkaaNakymaa()
{
    // Muokataan ensimmäistä näkymää
    muokkaaNakymaa(-1);
}

void RatapihaIkkuna::muokkaaRataa()
{
    // Näkymä 0 on rata
    muokkaaNakymaa(0);
}

void RatapihaIkkuna::graafinenAikataulu()
{
    QGraphicsView* view = new QGraphicsView( new GraafinenAikatauluScene, 0);
    view->show();
}

bool RatapihaIkkuna::onkoYhteydessa()
{
    return( tila()==KaukoYhteys || tila()==PaikallinenPalvelin );
}

void RatapihaIkkuna::aikaMuuttunut(const QDateTime &aika)
{
    ui->viestiLabel->setText( aika.toString("dd.MM.yyyy    HH:mm") );
    emit kello(aika);
}


void RatapihaIkkuna::nappienHimmennykset()
{

    ui->yhdistaNappi->setEnabled( tila() == EiYhteytta || tila() == LukuYhteys);
    ui->katkaiseNappi->setEnabled( tila() == KaukoYhteys || tila() == LukuYhteys);
    ui->ohjausKaukoButton->setEnabled( tila() == KaukoYhteys || tila() == LukuYhteys );

    ui->kaynnistaPalvelinNappi->setEnabled( tila() == EiYhteytta || tila() == LukuYhteys);
    ui->pysaytaPalvelinNappi->setEnabled( tila() == PaikallinenPalvelin);

    ui->nopeusSlider->setEnabled( tila() == PaikallinenPalvelin);
    ui->ohjausPaikButton->setEnabled( tila() == PaikallinenPalvelin || tila() == LukuYhteys );
    ui->rataButton->setEnabled( tila() == PaikallinenPalvelin );

    ui->muokkaaRataaNappi->setEnabled( tila()==EiYhteytta || tila()==LukuYhteys);

    if( tila()==EiYhteytta || tila()==LukuYhteys)
        ui->viestiLabel->setText("Ratapiha");

}

void RatapihaIkkuna::lukuYhteysMuodostettu()
{
    ui->varivalo->setPixmap( QPixmap(":/r/pic/ktas_keltainen.png") );
    ui->tilaLabel->setText( tr("Yhdistetty ainoastaan tietokantaan"));
    yhteystila_ = LukuYhteys;
    nappienHimmennykset();
}

void RatapihaIkkuna::muokkaaNakymaa(int nakyma)
{
    if( !tietokanta_.isOpen())
        yhdistaTietokantaan();

    EditoriIkkuna* ikkuna = new EditoriIkkuna(nakyma, this);
    ikkuna->show();

    if( tila() == EiYhteytta)
        lukuYhteysMuodostettu();
}


bool RatapihaIkkuna::yhdistaTietokantaan()
{
    ui->tilaLabel->setText(tr("Yhdistetään tietokantaan..."));

    if( tietokanta_.isOpen())
        tietokanta_.close();

    tietokanta_.setHostName( ui->palvelinEdit->text() );
    tietokanta_.setDatabaseName("ratapiha");
    tietokanta_.setUserName( ui->kayttajatunnusLine->text());
    tietokanta_.setPassword( ui->salasanaLine->text());

    if( !tietokanta_.open())
    {
        QMessageBox::critical(0, tr("Ratapiha: Tietokantavirhe"),tietokanta_.lastError().text());
        return false;
    }
    return true;
}

