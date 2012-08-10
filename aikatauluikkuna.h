/**************************************************************************
**  aikatauluikkuna.h
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

#ifndef AIKATAULUIKKUNA_H
#define AIKATAULUIKKUNA_H

#include "graafinenaikatauluscene.h"
#include "ratapihaikkuna.h"



#include <QMainWindow>
#include <QComboBox>
#include <QToolBar>
#include <QAction>
#include <QSlider>

class AikatauluMuokkaaja;

class AikatauluView;

class AikatauluIkkuna : public QMainWindow
{
    Q_OBJECT
public:
    explicit AikatauluIkkuna(RatapihaIkkuna *parent = 0);
    
signals:
    
public slots:
    void taulunVaihto(int valintaind);

    void vieSvg();
    void tulosta();

    void asetaSuunta();

    void vaihdaAikavali();
    void reittiDialogi();

protected:

    void luoAktiot();
    void luoTyokalurivi();
    void luoDockit();


    GraafinenAikatauluScene* skene_;
    AikatauluView* view_;

    QComboBox* taulunValintaCombo_;
    AikatauluMuokkaaja* aikatauluMuokkaaja_;

    QAction* vieSvgAktio_;
    QAction* tulostaAktio_;
    QAction* reittiAktio_;
    QAction* pohjoiseenAktio_;
    QAction* etelaanAktio_;

    QSlider* alkaaSlider_;
    QSlider* paattyySlider_;
};

#endif // AIKATAULUIKKUNA_H
