/**************************************************************************
**  aikataulunselausikkuna.cpp
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
**  AikataulunSelausIkkuna  3.8.2012
**************************************************************************/

#include "aikataulunselausikkuna.h"

#include <QSqlQuery>
#include <QToolBar>

AikataulunSelausIkkuna::AikataulunSelausIkkuna(QWidget *parent) :
    QMainWindow(parent)
{
    selaaja_ = new AikatauluSelaaja(this);
    setCentralWidget(selaaja_);

    luoTyokalurivi();

    setAttribute( Qt::WA_DeleteOnClose);
    haeAsemaAikataulu(0);


}


void AikataulunSelausIkkuna::haeAsemaAikataulu(int indeksi)
{
    selaaja_->haeAsemaAikataulu( asemaValintaCombo_->itemData(indeksi).toString() );
    setWindowTitle( tr("Aikataulu: %1").arg( asemaValintaCombo_->currentText() ));
}

void AikataulunSelausIkkuna::suodatus()
{
    selaaja_->asetaFiltteri(saapuvatSuodinAktio_->isChecked(), lahtevatSuodinAktio_->isChecked(),
                            etelaanSuodinAktio_->isChecked(), pohjoiseenSuodinAktio_->isChecked(),
                            lahijunaSuodinAktio_->isChecked(), kaukojunaSuodinAktio_->isChecked(), muujunaSuodinAktio_->isChecked());
}


void AikataulunSelausIkkuna::luoTyokalurivi()
{
    QToolBar* tbar = addToolBar(tr("Aikataulu"));

    asemaValintaCombo_ = new QComboBox;
    QSqlQuery asemakysely("select liikennepaikka, nimi from liikennepaikka where henkiloliikenne !=\"\" order by nimi");
    while( asemakysely.next())
    {
        asemaValintaCombo_->addItem( asemakysely.value(1).toString(),
                                      asemakysely.value(0).toString());
    }
    connect( asemaValintaCombo_, SIGNAL(currentIndexChanged(int)),
             this, SLOT(haeAsemaAikataulu(int)));


    refreshAktio_ = new QAction(QIcon(":/r/pic/refresh.png"),tr("Lataa uudelleen"),this);
    connect(refreshAktio_, SIGNAL(triggered()), selaaja_, SLOT(paivita()));

    saapuvatSuodinAktio_ = new QAction(tr("Saapuvat"),this);
    saapuvatSuodinAktio_->setCheckable(true);
    saapuvatSuodinAktio_->setChecked(true);

    lahtevatSuodinAktio_ = new QAction( tr("Lähtevät"), this );
    lahtevatSuodinAktio_->setCheckable(true);
    lahtevatSuodinAktio_->setChecked(true);

    etelaanSuodinAktio_ = new QAction( tr("Etelään"), this);
    etelaanSuodinAktio_->setCheckable(true);
    etelaanSuodinAktio_->setChecked(true);

    pohjoiseenSuodinAktio_ = new QAction( tr("Pohjoiseen"), this);
    pohjoiseenSuodinAktio_->setCheckable(true);
    pohjoiseenSuodinAktio_->setChecked(true);

    lahijunaSuodinAktio_ = new QAction("H", this);
    lahijunaSuodinAktio_->setCheckable(true);
    lahijunaSuodinAktio_->setChecked(true);

    kaukojunaSuodinAktio_ = new QAction("PS", this);
    kaukojunaSuodinAktio_->setCheckable(true);
    kaukojunaSuodinAktio_->setChecked(true);

    muujunaSuodinAktio_ = new QAction("TMX", this);
    muujunaSuodinAktio_->setCheckable(true);
    muujunaSuodinAktio_->setChecked(true);

    connect( saapuvatSuodinAktio_, SIGNAL(triggered()), this, SLOT(suodatus()));
    connect( lahtevatSuodinAktio_, SIGNAL(triggered()), this, SLOT(suodatus()));
    connect( etelaanSuodinAktio_, SIGNAL(triggered()), this, SLOT(suodatus()));
    connect( pohjoiseenSuodinAktio_, SIGNAL(triggered()), this, SLOT(suodatus()));
    connect( lahijunaSuodinAktio_, SIGNAL(triggered()), this, SLOT(suodatus()));
    connect( kaukojunaSuodinAktio_, SIGNAL(triggered()), this, SLOT(suodatus()));
    connect( muujunaSuodinAktio_, SIGNAL(triggered()), this, SLOT(suodatus()));


    tbar->addWidget(asemaValintaCombo_);
    tbar->addSeparator();
    tbar->addAction(refreshAktio_);
    tbar->addSeparator();
    tbar->addAction(saapuvatSuodinAktio_);
    tbar->addAction(lahtevatSuodinAktio_);
    tbar->addSeparator();
    tbar->addAction(etelaanSuodinAktio_);
    tbar->addAction(pohjoiseenSuodinAktio_);
    tbar->addSeparator();
    tbar->addAction(lahijunaSuodinAktio_);
    tbar->addAction(kaukojunaSuodinAktio_);
    tbar->addAction(muujunaSuodinAktio_);

}
