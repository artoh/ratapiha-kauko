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

RataIkkuna::RataIkkuna(QWidget *parent) :
    QMainWindow(parent)
{
    // Luodaan ensin skene, ellei sellaista ole
    if( !scene_)
        scene_ = new RataScene;

    view_ = new QGraphicsView(scene_);
    view_->scale(2.0, 2.0);
    setCentralWidget(view_);

}


RataScene* RataIkkuna::scene_ = 0;
