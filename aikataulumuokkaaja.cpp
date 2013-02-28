/**************************************************************************
Ratapiha - railway construction and simulation toy

(c) Arto Hyvättinen 2012
4.8.2012

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


**************************************************************************/

#include "aikataulumuokkaaja.h"

#include <QSqlQuery>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

AikatauluMuokkaaja::AikatauluMuokkaaja(QWidget *parent) :
    QWidget(parent)
{
    tunnusEdit_ = new QLineEdit(this);
    tunnusEdit_->setPlaceholderText(tr("Junanumero"));

    aikaEdit_ = new QTimeEdit(this);
    aikaEdit_->setDisplayFormat("hh.mm");

    selaaja_ = new AikatauluSelaaja(this);
    selaaja()->zoomOut(4);

    reittiCombo_ = new QComboBox(this);
    reittiCombo_->setMaxVisibleItems(25);
    lataaReitit();

    vaunumaaraSpin_ = new QSpinBox(this);
    vaunumaaraSpin_->setRange(0,50);
    vaunumaaraSpin_->setSuffix(tr(" vaunua"));

    QVBoxLayout* leiskaVasen = new QVBoxLayout();
    leiskaVasen->addWidget(tunnusEdit_);
    leiskaVasen->addWidget(aikaEdit_);
    leiskaVasen->addWidget(reittiCombo_);
    leiskaVasen->addWidget(vaunumaaraSpin_);

    lisaaNappi_ = new QPushButton(QIcon(":/r/pic/lisaa.png"),QString());
    poistaNappi_ = new QPushButton(QIcon(":/r/pic/poista.png"), QString());
    valmisNappi_ = new QPushButton( QIcon(":/r/pic/hyvaksy.png"), QString());
    peruNappi_ = new QPushButton( QIcon(":/r/pic/peru.png"), QString());

    QVBoxLayout* leiskaOikea = new QVBoxLayout();
    leiskaOikea->addWidget(lisaaNappi_);
    leiskaOikea->addWidget(poistaNappi_);
    leiskaOikea->addWidget(valmisNappi_);
    leiskaOikea->addWidget(peruNappi_);

    QHBoxLayout* ylaleiska = new QHBoxLayout;
    ylaleiska->addLayout(leiskaVasen);
    ylaleiska->addLayout(leiskaOikea);

    QVBoxLayout* paaleiska = new QVBoxLayout();
    paaleiska->addLayout(ylaleiska);
    paaleiska->addWidget(selaaja_);
    setLayout(paaleiska);

    connect( selaaja_, SIGNAL(naytetaanJuna(QString)), this, SLOT(valittuJuna(QString)));
    connect( selaaja_, SIGNAL(naytetaanAsema(QString)), this, SLOT(eiValittuaJunaa()));
    connect( aikaEdit_, SIGNAL(timeChanged(QTime)) , this, SLOT(katsoKelpaako()));
    connect( tunnusEdit_, SIGNAL(textChanged(QString)), this, SLOT(katsoKelpaako()));
    connect( reittiCombo_, SIGNAL(currentIndexChanged(QString)), this, SLOT(katsoKelpaako()));
    connect( vaunumaaraSpin_, SIGNAL(valueChanged(int)), this, SLOT(katsoKelpaako()));
    connect( this, SIGNAL(junaPaivitetty(QString)), selaaja_, SLOT(haeJunaAikataulu(QString)));


    connect( lisaaNappi_, SIGNAL(clicked()), this, SLOT(uusiJuna()));
    connect( peruNappi_, SIGNAL(clicked()), this, SLOT(peruMuokkaus()));
    connect( valmisNappi_, SIGNAL(clicked()), this, SLOT(muokkausValmis()));
    connect( poistaNappi_, SIGNAL(clicked()), this, SLOT(poistaJuna()));


    valmisNappi_->setEnabled(false);
    poistaNappi_->setEnabled(false);
}

void AikatauluMuokkaaja::valittuJuna(const QString &tunnus)
{
    // Junan lähtöaika tulee editoitavaksi
    QSqlQuery lahtoaikakysely( QString("select lahtee, reitti, vaunuja from juna where junanro=\"%1\"").arg(tunnus));
    if( lahtoaikakysely.next())
    {
        tunnusEdit_->setText( tunnus );

        aikaEdit_->setTime( lahtoaikakysely.value(0).toTime() );
        aikaEdit_->setStyleSheet( "color: black;"); // Kelpo eli musta junatunnus
        junaTunnus_ = tunnus;

        reittiCombo_->setCurrentIndex( reittiCombo_->findText( lahtoaikakysely.value(1).toString()));
        vaunumaaraSpin_->setValue( lahtoaikakysely.value(2).toInt());

        poistaNappi_->setEnabled(true);
        valmisNappi_->setEnabled(false);
        peruNappi_->setEnabled(false);

    }
}

void AikatauluMuokkaaja::lataaReitit(const QString &lahtoasema)
{
    QString valittu = reittiCombo_->currentText();
    reittiCombo_->clear();

    QString kysymys;

    if( lahtoasema.isEmpty())
        kysymys = "select distinct(reitti) from aikataulu order by reitti";
    else
        kysymys =   QString("select distinct reitti from aikataulu natural join liikennepaikka where nimi like \"%1%\" and tapahtuma=\"L\"").arg(lahtoasema);


    QSqlQuery reitit(kysymys);


    while( reitit.next())
        reittiCombo_->addItem( reitit.value(0).toString());

    if( !valittu.isEmpty())
        reittiCombo_->setCurrentIndex( reittiCombo_->findText(valittu));
}




void AikatauluMuokkaaja::eiValittuaJunaa()
{
    junaTunnus_ = QString();
    tunnusEdit_->setStyleSheet("color: red;");

    tunnusEdit_->clear();

    valmisNappi_->setEnabled(false);
    poistaNappi_->setEnabled(false);
}

void AikatauluMuokkaaja::muokkausValmis()
{
    QString vaunutext;
    if( vaunumaaraSpin_->value())
        vaunutext = QString::number( vaunumaaraSpin_->value());
    else
        vaunutext = QString("NULL");

    if( junaTunnus_.isEmpty())
    {
        QSqlQuery lisays;

        if( lisays.exec( QString("insert into juna(junanro, reitti, lahtee, vaunuja) values(\"%1\",\"%2\",\"%3\",%4) " )
                          .arg(tunnusEdit_->text()).arg( reittiCombo_->currentText() ).arg( aikaEdit_->time().toString() ).arg(vaunutext)))
        {
            junaTunnus_ = tunnusEdit_->text();
            emit junaPaivitetty( junaTunnus_);
            poistaNappi_->setEnabled(true);
            valmisNappi_->setEnabled(false);
            peruNappi_->setEnabled(false);
        }
    }
    else
    {
        // Update !!!
        QSqlQuery paivitys;
        if( paivitys.exec( QString("update juna set junanro=\"%1\", reitti=\"%2\", lahtee=\"%3\", vaunuja=%5 where junanro=\"%4\" ")
                         .arg(tunnusEdit_->text()).arg( reittiCombo_->currentText() ).arg( aikaEdit_->time().toString() )
                         .arg(junaTunnus_ ).arg( vaunutext )  ))
        {
            emit junaPaivitetty(junaTunnus_);
            if( junaTunnus_ != tunnusEdit_->text())
            {
                junaTunnus_ = tunnusEdit_->text();
                emit junaPaivitetty(junaTunnus_);
            }
            valmisNappi_->setEnabled(false);
            peruNappi_->setEnabled(false);
        }
    }

}

void AikatauluMuokkaaja::katsoKelpaako()
{
    // Muokkaus kelpaa, jos uusi junan tunnus on kelpo, ts. vähintään 3 merkkiä pitkä ja uudella junalla ei ole käytössä
    if( junaTunnus_.isEmpty() && junaTunnus_ != tunnusEdit_->text() )
    {
        QSqlQuery onkojo( QString("select junanro from juna where junanro=\"%1\"").arg( tunnusEdit_->text() ));
        if( onkojo.next() || tunnusEdit_->text().length() < 3)
        {
            // Tunnus ON jo käytössä
            tunnusEdit_->setStyleSheet("color: red;");
            valmisNappi_->setEnabled(false);
            peruNappi_->setEnabled(true);
            return;
        }
    }
    tunnusEdit_->setStyleSheet( "color: black; ");
    valmisNappi_->setEnabled(true);
    peruNappi_->setEnabled(true);
}

void AikatauluMuokkaaja::peruMuokkaus()
{
    if( junaTunnus_.isEmpty())
        eiValittuaJunaa();    // Tyhjennetään kentät
    else
        valittuJuna( junaTunnus_);  // Aloitetaan muokkaaminen alusta
}

void AikatauluMuokkaaja::poistaJuna()
{
    QSqlQuery poisto(QString("delete from juna where junanro=\"%1\"").arg(junaTunnus_));
    emit junaPaivitetty(junaTunnus_);
    eiValittuaJunaa();
}

void AikatauluMuokkaaja::uusiJuna()
{
    junaTunnus_.clear();
    tunnusEdit_->setStyleSheet("color: red;");
    valmisNappi_->setEnabled(false);
    poistaNappi_->setEnabled(false);
}

