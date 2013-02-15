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

    tbar->addWidget(asemaValintaCombo_);

}
