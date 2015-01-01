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

