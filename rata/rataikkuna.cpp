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

#include <QStatusBar>
#include <QToolBar>
#include <QSpinBox>
#include <QComboBox>

#include <QLineEdit>
#include <QCompleter>

#include "rataikkuna.h"

#include "rataview.h"


RataIkkuna::RataIkkuna(RataScene *skene) :
    QMainWindow(0), skene_(skene)
{

    view_ = new RataView(skene_);
    view_->ensureVisible(0.0,0.0,10.0,10.0);

    setCentralWidget(view_);

    luoTilarivi();
    luoTyokalurivi();
    setWindowTitle("Ratapiha 5 - Ratanäkymä");

}

void RataIkkuna::kellonPaivitys(int simulaatioAika)
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

    aikaLabel_->setText( QString("%1.%2.%3  klo %4.%5.%6").arg(pvm+1).arg(kk+1).arg(vuosi)
                         .arg(tunnit,2,10,QChar('0')).arg(minuutit,2,10,QChar('0')).arg(sekunnit,2,10,QChar('0'))  );

}

void RataIkkuna::naytaLiikennepaikka(const QString &liikennepaikka)
{
    QPoint sijainti = skene()->liikennepaikanKoordinaatit(liikennepaikka);
    if( !sijainti.isNull())
    {
        view_->ensureVisible(sijainti.x(), sijainti.y(),100.0,100.0);
    }
}

void RataIkkuna::luoTilarivi()
{
    aikaLabel_ = new QLabel("Tervetuloa!");
    statusBar()->addPermanentWidget(aikaLabel_);
    connect( skene(), SIGNAL(ajanMuutos(int)), this, SLOT(kellonPaivitys(int)));
}

void RataIkkuna::luoTyokalurivi()
{
    QToolBar *trivi = addToolBar("Rata");

    QSpinBox *nopeusSpin = new QSpinBox();
    nopeusSpin->setRange(0,20);
    nopeusSpin->setSuffix("x");

    connect(nopeusSpin, SIGNAL(valueChanged(int)), skene(), SLOT(asetaNopeus(int)));
    connect( skene(), SIGNAL(nopeutuksenMuutos(int)), nopeusSpin, SLOT(setValue(int)));

    trivi->addWidget(nopeusSpin);

    // Liikennepaikan etsiminen
    QLineEdit *liikennepaikkaEdit = new QLineEdit;
    QCompleter *taydentaja = new QCompleter(skene()->liikepaikkojenNimet() ,this);
    taydentaja->setCaseSensitivity( Qt::CaseInsensitive);
    taydentaja->setCompletionMode( QCompleter::UnfilteredPopupCompletion);
    liikennepaikkaEdit->setCompleter(taydentaja);
    connect( liikennepaikkaEdit, SIGNAL(textChanged(QString) ), this, SLOT(naytaLiikennepaikka(QString)));

    trivi->addWidget(liikennepaikkaEdit);
}
