#include <QWheelEvent>
#include <cmath>

#include "kaukoview.h"
#include "kaukokisko.h"

KaukoView::KaukoView(KaukoScene *scene)
    : QGraphicsView(scene), skene_(scene), tila_(VIERITA)
{
    setDragMode( ScrollHandDrag);
    scale(2.5, 2.5);
}

KaukoView::~KaukoView()
{

}

void KaukoView::valitseTila(int tila)
{
    tila_ = tila;
    setDragMode( NoDrag );
    emit tilaVaihtunut(tila);

    switch (tila) {
    case VIERITA:
        setCursor( Qt::ArrowCursor);
        setDragMode( ScrollHandDrag);
        break;
    case JUNAKULKUTIE_ALKAA:
        setCursor( QCursor( QPixmap(":/kauko/pic/junakulkutienkursori.png"),9,0));
        break;
    case JUNAKULKUTIE_PAATTYY :
        setCursor( QCursor( QPixmap(":/kauko/pic/junakulkutieloppuunkursori.png"),11,0));
        break;
    default:
        break;
    }
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

void KaukoView::mousePressEvent(QMouseEvent *event)
{
    QString klikattuTunnus = raiteenPaaPisteessa( event->pos());
    switch (tila_) {
    case JUNAKULKUTIE_ALKAA:
        if( !klikattuTunnus.isEmpty())
        {
            alkaaTunnuksesta_ = klikattuTunnus;
            valitseTila( JUNAKULKUTIE_PAATTYY);
        }
        break;
    case JUNAKULKUTIE_PAATTYY :
        if( !klikattuTunnus.isEmpty())
        {
            skene_->kasky( QString("JK %1 %2")
                           .arg( alkaaTunnuksesta_.left( alkaaTunnuksesta_.length() -1 ) )
                           .arg( klikattuTunnus.left( klikattuTunnus.length() - 1 )));
            valitseTila( JUNAKULKUTIE_ALKAA);
        }
    default:
        break;
    }
}

QString KaukoView::raiteenPaaPisteessa(const QPoint &piste)
{
    QString tunnus;
    qreal etaisyys = 99.0;

    QList<QGraphicsItem*> lista = items(piste);

    foreach (QGraphicsItem *item, lista)
    {
        KaukoKisko *kisko = qgraphicsitem_cast<KaukoKisko*>(item);
        if( kisko )
        {
            QPointF kiskoPos = kisko->mapFromScene( mapToScene(piste) );
            qreal tamaEtaisyys = qAbs( kiskoPos.y());
            if( tamaEtaisyys < etaisyys)
            {
                etaisyys = tamaEtaisyys;
                // Tämä on tähän asti läheisin kisko
                // Liitetään raiteen tunnustekstiin kirjain kuvaamaan, kumpaa päätä tarkoitetaan
                if( kisko->etelaPaassa() ==KaukoKisko::PAASSA && kisko->pohjoisPaassa() == KaukoKisko::PAASSA)
                {
                    if( kiskoPos.x() < kisko->pituus() / 2)
                        tunnus = kisko->raidetunnus() + "p";
                    else
                        tunnus = kisko->raidetunnus() + "e";
                }
                else if( kisko->etelaPaassa() == KaukoKisko::PAASSA)
                {
                    tunnus = kisko->raidetunnus() + "e";
                }
                else if( kisko->pohjoisPaassa() == KaukoKisko::PAASSA)
                {
                    tunnus = kisko->raidetunnus() + "p";
                }
                else
                {
                    tunnus = kisko->raidetunnus() + "x";
                }
            }
        }

    }
    return tunnus;
}
