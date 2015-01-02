#include <QWheelEvent>
#include <cmath>

#include "kaukoview.h"

KaukoView::KaukoView(KaukoScene *scene)
    : QGraphicsView(scene)
{
    setDragMode( ScrollHandDrag);
    scale(2.5, 2.5);
}

KaukoView::~KaukoView()
{

}

void KaukoView::wheelEvent(QWheelEvent *event)
{
    if( event->orientation() == Qt::Vertical )
     {
        double numDegrees = -event->delta() / 8.0 ;
        double numSteps = numDegrees / 15.0;
        double factor = std::pow(1.125, numSteps);
        scale( factor, factor);
     }
    event->accept();
}
