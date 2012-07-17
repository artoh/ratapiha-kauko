/**************************************************************************
**  rataview.cpp
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

#include "rataview.h"
#include "ratakisko.h"

#include <QWheelEvent>
#include <QScrollBar>
#include <QMouseEvent>

#include "vaunu.h"
#include "rataraide.h"

#include <cmath>
#include <QDebug>

RataView::RataView(RataScene *skene) :
    QGraphicsView(skene), rullaSkaalaa_(true), tila_(Vierita)
{
    setDragMode(ScrollHandDrag);
}


void RataView::wheelEvent(QWheelEvent *event)
{

    if( event->orientation() == Qt::Horizontal)
    {
        if( rullaSkaalaa_ )
        {
            double asteet = event->delta() / 8.0;
            rotate(asteet);
        }
        else
        {
            horizontalScrollBar()->setValue( horizontalScrollBar()->value() + event->delta() / -120 * horizontalScrollBar()->singleStep()  );
        }
    }
    else
    {
        if( rullaSkaalaa_ )
        {
            double numDegrees = -event->delta() / 8.0 ;
            double numSteps = numDegrees / 15.0;
            double factor = std::pow(1.125, numSteps);
            scale( factor, factor);
        }
        else
             verticalScrollBar()->setValue( verticalScrollBar()->value() + event->delta() / -120 * verticalScrollBar()->singleStep()  );
    }
    event->accept();

}

void RataView::mousePressEvent(QMouseEvent *event)
{

    if( event->button() == Qt::MidButton )
    {
        if( tila_ == Vierita)
        {
            setDragMode(NoDrag);
            tila_ = SijoitaAkseli;
        }
        else
        {
            tila_ = Vierita;
            setDragMode( ScrollHandDrag );
        }
    }
    else if( tila_ == SijoitaAkseli)
    {
        RataKisko* klikattuKisko = 0;
        qreal etaisyys = 99;
        QList<QGraphicsItem*> lista = items(event->pos());
        foreach( QGraphicsItem* item, lista)
        {
            RataKisko* kisko = qgraphicsitem_cast<RataKisko*>(item);
            if( kisko )
            {
                // Lasketaan etäisyyttä kiskolle
                QPointF kiskoPos = kisko->mapFromScene( mapToScene(event->pos()) ) ;
                qreal tamaEtaisyys = qAbs(kiskoPos.y());
                if( kiskoPos.x() < 0)
                    tamaEtaisyys += 0 - kiskoPos.x() * 2;
                else if( kiskoPos.x() > kisko->pituus())
                    tamaEtaisyys += (kiskoPos.x() - kisko->pituus()) * 2;

                qDebug()  << kisko->raide()->raidetunnusLiikennepaikalla() << "..." << tamaEtaisyys;

                if( tamaEtaisyys < etaisyys)
                {
                    etaisyys = tamaEtaisyys;
                    klikattuKisko = kisko;
                }

            } // if

        } // foreach

        // Nyt pitäisi olla olemassa valittu kisko, jolle laitetaan akseli ;)

        if( klikattuKisko )
        {
            Vaunu* vaunu = new Vaunu("Dr16",0);
            vaunu->sijoitaKiskolle(klikattuKisko);
            qDebug() << "Vaunu sijoitettu kiskolle " << klikattuKisko->raide()->raidetunnusLiikennepaikalla() << " at " << vaunu->pos();
        }


    }
    else
        QGraphicsView::mousePressEvent(event);

}
