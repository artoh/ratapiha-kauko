/**************************************************************************
**  aikatauluikkuna.cpp
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
**  AikatauluIkkuna  3.8.2012
**************************************************************************/

#include "aikatauluikkuna.h"
#include "aikatauluview.h"

#include <QSqlQuery>

AikatauluIkkuna::AikatauluIkkuna(RatapihaIkkuna *parent) :
    QMainWindow(parent)
{
    skene_ = new GraafinenAikatauluScene;
    view_ = new AikatauluView( skene_ );

    luoTyokalurivi();
    taulunVaihto(0);

    setCentralWidget(view_);

    setAttribute( Qt::WA_DeleteOnClose);
}

void AikatauluIkkuna::taulunVaihto(int valintaind)
{
    skene_->lataaTaulu( taulunValintaCombo_->itemData( valintaind).toInt() );
    setWindowTitle( tr("Aikataulu %1").arg( skene_->tauluNimi()));
}

void AikatauluIkkuna::luoTyokalurivi()
{
    QToolBar* tbar = addToolBar(tr("Aikataulu"));

    taulunValintaCombo_ = new QComboBox;
    QSqlQuery taulukysely("select taulu, taulunimi from taulu");
    while( taulukysely.next())
    {
        taulunValintaCombo_->addItem( taulukysely.value(1).toString(),
                                      taulukysely.value(0).toInt());
    }
    connect( taulunValintaCombo_, SIGNAL(currentIndexChanged(int)),
             this, SLOT(taulunVaihto(int)));

    tbar->addWidget(taulunValintaCombo_);

}
