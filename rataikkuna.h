/**************************************************************************
**  rataikkuna.h
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

#ifndef RATAIKKUNA_H
#define RATAIKKUNA_H


#include "ratascene.h"


#include <QMainWindow>
#include <QGraphicsView>

class RataIkkuna : public QMainWindow
{
    Q_OBJECT
public:
    explicit RataIkkuna(QWidget *parent = 0);
    
signals:
    
public slots:


private:
    QGraphicsView* view_;
    static RataScene* scene_;   // Vain yksi scene!!!
    
};

#endif // RATAIKKUNA_H
