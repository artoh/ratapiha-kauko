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
#include "aikatauluselaaja.h"
#include "aikataulumuokkaaja.h"
#include "reittidialogi.h"

#include <QSqlQuery>
#include <QDockWidget>
#include <QFileDialog>
#include <QSvgGenerator>
#include <QPainter>
#include <QIcon>
#include <QPrintDialog>
#include <QPrinter>

AikatauluIkkuna::AikatauluIkkuna(RatapihaIkkuna *parent) :
    QMainWindow(parent)
{
    skene_ = new GraafinenAikatauluScene;
    view_ = new AikatauluView( skene_ );

    luoAktiot();
    luoTyokalurivi();
    luoDockit();
    taulunVaihto(0);

    setCentralWidget(view_);

    setAttribute( Qt::WA_DeleteOnClose);

    connect(view_, SIGNAL(asemaValittu(QString)), aikatauluMuokkaaja_->selaaja(), SLOT(haeAsemaAikataulu(QString)));
    connect(view_, SIGNAL(junaValittu(QString)), aikatauluMuokkaaja_->selaaja(), SLOT(haeJunaAikataulu(QString)));
    connect( aikatauluMuokkaaja_->selaaja(), SIGNAL(naytetaanJuna(QString)), skene_, SLOT(valitseJuna(QString)));
    connect( aikatauluMuokkaaja_->selaaja(), SIGNAL(naytetaanAsema(QString)), skene_, SLOT(valitseJuna()));
    connect( aikatauluMuokkaaja_, SIGNAL(junaPaivitetty(QString)), skene_, SLOT(paivitaJuna(QString)));
}

void AikatauluIkkuna::taulunVaihto(int valintaind)
{
    skene_->lataaTaulu( taulunValintaCombo_->itemData( valintaind).toInt() );
    setWindowTitle( tr("Aikataulu %1").arg( skene_->tauluNimi()));
}

void AikatauluIkkuna::vieSvg()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Vie aikataulugrafiikka"),
                                                    QString(),
                               tr("SVG-kuva (*.svg)"));
    if( !fileName.isEmpty())
    {
        QSvgGenerator svg;
        svg.setSize( QSize( skene_->maxX(), skene_->maxY() ) );
        svg.setFileName(fileName);
        QPainter painter(&svg);
        painter.rotate(180);
        skene_->render(&painter);
    }

}

void AikatauluIkkuna::tulosta()
{

    RatapihaIkkuna::getInstance()->printteri()->setOrientation(QPrinter::Landscape);
    QPrintDialog dialog( RatapihaIkkuna::getInstance()->printteri()  );
    if( dialog.exec())
    {
        QPainter painter( RatapihaIkkuna::getInstance()->printteri() );
        skene_->render(&painter);
    }
}

void AikatauluIkkuna::asetaSuunta()
{
    if( pohjoiseenAktio_->isChecked() )
        skene_->asetaSuuntaEhto(RaiteenPaa::Pohjoiseen);
    else if( etelaanAktio_->isChecked())
        skene_->asetaSuuntaEhto(RaiteenPaa::Etelaan);
    else
        skene_->asetaSuuntaEhto(RaiteenPaa::Virhe);
}

void AikatauluIkkuna::vaihdaAikavali()
{
    skene_->asetaAikavali( alkaaSlider_->value(), paattyySlider_->value() );
    alkaaSlider_->setRange(0, paattyySlider_->value()-1);
    paattyySlider_->setRange( alkaaSlider_->value()+1, 24);
}

void AikatauluIkkuna::luoAktiot()
{
    vieSvgAktio_ = new QAction(QIcon(":/r/pic/viesvg.png"),tr("Vie svg-kuvana"), this);
    connect( vieSvgAktio_, SIGNAL(triggered()), this, SLOT(vieSvg()));

    tulostaAktio_ = new QAction( QIcon(":/r/pic/tulosta.png"), tr("Tulosta grafiikka"), this);
    connect( tulostaAktio_, SIGNAL(triggered()), this, SLOT(tulosta()));

    reittiAktio_ = new QAction( QIcon(":/r/pic/reitit.png"), tr("Reittien suunnittelu"),this);
    connect( reittiAktio_, SIGNAL(triggered()), this, SLOT(reittiDialogi()));

    pohjoiseenAktio_ = new QAction(tr("P"), this);
    pohjoiseenAktio_->setCheckable(true);
    connect( pohjoiseenAktio_, SIGNAL(changed()), this, SLOT(asetaSuunta()));

    etelaanAktio_ = new QAction( tr("E"), this);
    etelaanAktio_->setCheckable(true);
    connect( etelaanAktio_, SIGNAL(changed()), this, SLOT(asetaSuunta()));


}

void AikatauluIkkuna::reittiDialogi()
{
    ReittiDialogi* dialogi = new ReittiDialogi(this);
    dialogi->show();
    connect( dialogi, SIGNAL(muutettu()), skene_, SLOT(paivitaKaikki()) );
    connect( dialogi, SIGNAL(reittiListaaMuutettu()), aikatauluMuokkaaja_, SLOT(lataaReitit()));
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

    tbar->addAction(tulostaAktio_);
    tbar->addAction(vieSvgAktio_);
    tbar->addAction(reittiAktio_);

    tbar->addSeparator();
    tbar->addAction(etelaanAktio_);
    tbar->addAction(pohjoiseenAktio_);
    tbar->addSeparator();

    alkaaSlider_ = new QSlider(Qt::Horizontal, this);
    paattyySlider_ = new QSlider (Qt::Horizontal, this);
    alkaaSlider_->setRange(0,23);
    alkaaSlider_->setValue(0);
    paattyySlider_->setRange(1,24);
    paattyySlider_->setValue(24);
    connect( alkaaSlider_, SIGNAL(sliderMoved(int)), this, SLOT(vaihdaAikavali()));
    connect( paattyySlider_, SIGNAL(sliderMoved(int)), this, SLOT(vaihdaAikavali()));
    tbar->addWidget(alkaaSlider_);
    tbar->addWidget(paattyySlider_);



}

void AikatauluIkkuna::luoDockit()
{
    aikatauluMuokkaaja_ = new AikatauluMuokkaaja(this);
    QDockWidget* selaajaDock = new QDockWidget( tr("Aikataulu"));
    selaajaDock->setWidget(aikatauluMuokkaaja_);
    addDockWidget(Qt::RightDockWidgetArea, selaajaDock);

}
