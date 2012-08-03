/**************************************************************************
**  aikatauluview.cpp
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
**  AikatauluView  3.8.2012
**************************************************************************/

#include "aikatauluview.h"
#include "graafinenaikatauluscene.h"

#include <QWheelEvent>
#include <QScrollBar>

#include <cmath>

AikatauluView::AikatauluView(GraafinenAikatauluScene *skene) :
    QGraphicsView(skene),
    rullaSkaalaa_(true)
{
}


void AikatauluView::wheelEvent(QWheelEvent *event)
{

    if( event->orientation() == Qt::Horizontal)
    {
        horizontalScrollBar()->setValue( horizontalScrollBar()->value() + event->delta() / -120 * horizontalScrollBar()->singleStep()  );
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

void AikatauluView::mousePressEvent(QMouseEvent *event)
{

    if( event->button() == Qt::LeftButton)
    {

        // Mitäköhän junaa oltaisiin klikattu? Tai asemaa?
        QRect valintaApu( event->pos().x()-2, event->pos().y()-2, 4, 4);

        QList<QGraphicsItem*> lista = items( valintaApu);
        foreach(QGraphicsItem* item, lista)
        {
            if( item->data( GraafinenAikatauluScene::JUNANRODATAKENTTA).isValid())
            {
                emit junaValittu( item->data(GraafinenAikatauluScene::JUNANRODATAKENTTA).toString());
                return;
            }
            else if( item->data( GraafinenAikatauluScene::ASEMANTUNNUSKENTTA).isValid())
            {
                emit asemaValittu( item->data(GraafinenAikatauluScene::ASEMANTUNNUSKENTTA).toString());
                return;
            }

        }

    }
    else if( event->button() == Qt::MiddleButton)
    {
        rullaSkaalaa_ = !rullaSkaalaa_;
        return;
    }
    QGraphicsView::mousePressEvent(event);
}




