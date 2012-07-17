/**************************************************************************
**  rataview.h
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
**  RataView  11.7.2012
**************************************************************************/

#ifndef RATAVIEW_H
#define RATAVIEW_H

#include "ratascene.h"

#include <QGraphicsView>

class RataKisko;

class RataView : public QGraphicsView
{
    Q_OBJECT
public:
    enum RataViewTila { Vierita, SijoitaAkseli } ;

    RataView(RataScene* skene);
    
signals:
    
public slots:

protected:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

    RataKisko* kiskoKohdalla(const QPoint& sijainti);


    /** Käytetäänkö hiiren rullaa skaalaamiseen vai vierittäimiseen */
    bool rullaSkaalaa_;
    RataViewTila tila_;
    
};

#endif // RATAVIEW_H
