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


#include "asetinlaitepaneeli.h"
#include "ui_asetinlaitepaneeli.h"

#include "asetinlaite.h"
#include "raidetieto.h"

AsetinlaitePaneeli::AsetinlaitePaneeli(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AsetinlaitePaneeli)
{
    ui->setupUi(this);
    setWindowTitle("Asetinlaite");

    connect( ui->kysymysEdit, SIGNAL(returnPressed()), this, SLOT(haeInfo()));
    connect( ui->kaskyEdit, SIGNAL(returnPressed()), this, SLOT(aslKomento()));
}

AsetinlaitePaneeli::~AsetinlaitePaneeli()
{
    delete ui;
}

void AsetinlaitePaneeli::ajanPaivitys(int simulaatioAika)
{
    int paiva = simulaatioAika / 86400;
    int paivanJalkeen = simulaatioAika % 86400;
    int tunnit = paivanJalkeen / 3600;
    int tunninJalkeen = paivanJalkeen % 3600;
    int minuutit = tunninJalkeen / 60;
    int sekunnit = tunninJalkeen % 60;

    int pvm = paiva % 30;
    int kuukausia = pvm / 30;
    int kk = kuukausia % 12;
    int vuosi = 2000 + kuukausia / 12;

    ui->kelloLabel->setText( QString("%1.%2.%3  klo %4.%5.%6").arg(pvm+1).arg(kk+1).arg(vuosi)
                         .arg(tunnit,2,10,QChar('0')).arg(minuutit,2,10,QChar('0')).arg(sekunnit,2,10,QChar('0'))  );
}

void AsetinlaitePaneeli::yhdistettyRataan(bool onko)
{
    if( onko )
        ui->rataLabel->setText("<font color=green>RATA</font>");
    else
        ui->rataLabel->setText("<font color=red>RATA</font>");
}

void AsetinlaitePaneeli::kulkutiemaaraPaivitys(int kulkuteita)
{
    ui->kulkutiemaaraLabel->setText( QString::number(kulkuteita));
}

void AsetinlaitePaneeli::haeInfo()
{
    QString raidetunnus = ui->kysymysEdit->text();
    RaideTieto* raide = Asetinlaite::instanssi()->raideTunnustekstilla(raidetunnus);
    if( raide )
    {
        QString teksti = raide->raideTila();
        ui->raideInfoLabel->setText( teksti);
    }

}

void AsetinlaitePaneeli::aslKomento()
{
    QString komento = ui->kaskyEdit->text();
    QString vastaus = Asetinlaite::instanssi()->aslKomento(komento);
    ui->aslVastausLabel->setText(vastaus);
}
