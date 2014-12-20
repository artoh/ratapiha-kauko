/**************************************************************************
**
**  Ratapiha
**
**  Copyright (c) 2012-2015 Arto Hyvättinen
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
**
**************************************************************************/


#include "rataview.h"
#include "ratakisko.h"
#include "vaunukataloogi.h"

#include <QWheelEvent>
#include <QScrollBar>
#include <cmath>

#include <QMimeData>

RataView::RataView(RataScene *skene)
    : QGraphicsView(skene), rullaSkaalaa_(true), skene_(skene)
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

RataKisko *RataView::kiskoKohdalla(const QPoint &sijainti)
{
    RataKisko* klikattuKisko = 0;
    qreal etaisyys = 99;
    QList<QGraphicsItem*> lista = items( sijainti);
    foreach( QGraphicsItem* item, lista)
    {
        RataKisko* kisko = qgraphicsitem_cast<RataKisko*>(item);
        if( kisko )
        {
            // Lasketaan etäisyyttä kiskolle
            QPointF kiskoPos = kisko->mapFromScene( mapToScene(sijainti) );
            qreal tamaEtaisyys = qAbs(kiskoPos.y());
            if( kiskoPos.x() < 0)
                tamaEtaisyys += 0 - kiskoPos.x() * 2;
            else if( kiskoPos.x() > kisko->pituus())
                tamaEtaisyys += (kiskoPos.x() - kisko->pituus()) * 2;

            if( tamaEtaisyys < etaisyys)
            {
                etaisyys = tamaEtaisyys;
                klikattuKisko = kisko;
            }

        } // if

    } // foreach
    return klikattuKisko;
}

void RataView::dragMoveEvent(QDragMoveEvent *event)
{
    VaunuKataloogi *lahde = qobject_cast<VaunuKataloogi*>(event->source());
    if( lahde )
    {
        RataKisko* kisko = kiskoKohdalla(event->pos());
        if( kisko  )
        {
            // Hyväksytään jos ollaan kiskon kohdalla, ja kiskolle mahtuu
            if( Vaunu::vaununSijaintiKiskolla(kisko, true) || Vaunu::vaununSijaintiKiskolla(kisko, false) )
            {

                event->accept();
                return;
            }
        }
    }
    event->ignore(QRect());
}

void RataView::dropEvent(QDropEvent *event)
{
    RataKisko* kisko = kiskoKohdalla( event->pos());
    QString tyyppi = event->mimeData()->text();

    if( kisko && !tyyppi.isEmpty())
    {
        // Koordinaatit kiskon mukaan
        QPointF koord = kisko->mapFromScene( mapToScene( event->pos()) );
        bool pohjoiseen = true;
        if( koord.x() < kisko->pituus() / 2)
            pohjoiseen = false;

        // Mahtuuko kiskolle ???
        if( Vaunu::vaununSijaintiKiskolla(kisko, pohjoiseen) == 0)
        {
            // Ei mahtunut !!!
            // Kokeillaan, mahtuuko toiseen päähän (pitkä juna)
            pohjoiseen = !pohjoiseen;
            if( Vaunu::vaununSijaintiKiskolla(kisko, pohjoiseen) == 0)
                return;     // Ei mahdu sinnekään ...
        }

        Vaunu* uusivaunu = 0;
        if( tyyppi.startsWith('S') || tyyppi.startsWith('D'))
        {
            // Veturi - Toistaiseksi ei käsitellä moottorivaunuja
            uusivaunu = new Veturi( skene_, tyyppi );
        }
        else
        {
            uusivaunu = new Vaunu( skene_, tyyppi);
        }

        uusivaunu->sijoitaKiskolle(kisko, pohjoiseen );  // Sijoitetaan kiskolle
    }

}
