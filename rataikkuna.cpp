/**************************************************************************
**  rataikkuna.cpp
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
**  RataIkkuna  10.7.2012
**************************************************************************/

#include "rataikkuna.h"
#include "vaunukataloogi.h"
#include "ajopoyta.h"

#include <QDockWidget>

RataIkkuna::RataIkkuna(QWidget *parent) :
    QMainWindow(parent)
{
    // Luodaan ensin skene, ellei sellaista ole
    rataSkene();

    view_ = new RataView(scene__);
    view_->scale(0.5, 0.5);
    setCentralWidget(view_);

    teeDockit();


}


RataScene* RataIkkuna::rataSkene()
{
    if( !scene__)
        scene__ = new RataScene;
    return scene__;
}

bool RataIkkuna::onkoSkenea()
{
    return( scene__ != 0);
}


void RataIkkuna::teeDockit()
{
    Ajopoyta* ajopoyta = new Ajopoyta();
    QDockWidget* ajopoytaDock = new QDockWidget( tr("Ajopöytä"),this);
    ajopoytaDock->setWidget( ajopoyta);
    addDockWidget( Qt::RightDockWidgetArea, ajopoytaDock);

    VaunuKataloogi* luettelo = new VaunuKataloogi();
    QDockWidget *vaunuLuetteloDock = new QDockWidget( tr("Lisää vaunu"), this);
    vaunuLuetteloDock->setWidget( luettelo);
    addDockWidget( Qt::RightDockWidgetArea, vaunuLuetteloDock);

}

RataScene* RataIkkuna::scene__ = 0;
