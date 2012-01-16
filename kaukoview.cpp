/**************************************************************************
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 6.1.2012
**************************************************************************/

#include "kaukoview.h"


#include <QMouseEvent>
#include <QScrollBar>
#include <cmath>

KaukoView::KaukoView(KaukoScene* skene) :
    QGraphicsView(skene)
{
}



void KaukoView::wheelEvent(QWheelEvent *event)
{

    if( event->orientation() == Qt::Horizontal)
    {
        // Sivusuuntaan scrollaus
        horizontalScrollBar()->setValue( horizontalScrollBar()->value() + event->delta() / -120 * horizontalScrollBar()->singleStep()  );

    }
    else
    {
        // Pystysuuntaan skaalaus
        double numDegrees = -event->delta() / 8.0 ;
        double numSteps = numDegrees / 15.0;
        double factor = std::pow(1.125, numSteps);
        scale( factor, factor);
    }
    event->accept();

}
