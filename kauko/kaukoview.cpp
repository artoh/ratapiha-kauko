#include <QWheelEvent>
#include <cmath>
#include <QMessageBox>

#include "kaukoview.h"
#include "kaukokisko.h"

KaukoView::KaukoView(KaukoScene *scene)
    : QGraphicsView(scene), skene_(scene), tila_(VIERITA)
{
    scale(2.5, 2.5);
    valitseTila( VIERITA);
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
        setDragMode( ScrollHandDrag );
        break;
    case JUNAKULKUTIE_ALKAA:
        setCursor( QCursor( QPixmap(":/kauko/pic/junakulkutienkursori.png"),9,0));
        break;
    case JUNAKULKUTIE_PAATTYY :
        setCursor( QCursor( QPixmap(":/kauko/pic/junakulkutieloppuunkursori.png"),11,0));
        break;
    case KULKUTIENPERUMINEN :
        setCursor( QCursor( QPixmap(":/kauko/pic/kulkutienpurkukursori.png"),9,0));
        break;
    case HATAKULKUTIENPURKU :
        setCursor( QCursor( QPixmap(":/kauko/pic/hatakulkutienpurkukursori.png"),9,0));
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
    // Tunnus, jossa mukana pää: Hki001e, Hki001p, Hki001x
    QString klikattuTunnus = raiteenPaaPisteessa( event->pos());

    QString raidetunnus;    // Valitun raiteen tunnus Hki001
    if( klikattuTunnus.length() > 3)
        raidetunnus = klikattuTunnus.left( klikattuTunnus.length()-1);


    switch (tila_) {
    case JUNAKULKUTIE_ALKAA:
        if( !klikattuTunnus.isEmpty())
        {
            alkaaTunnuksesta_ = klikattuTunnus;
            valitseTila( JUNAKULKUTIE_PAATTYY);
        }
        break;
    case JUNAKULKUTIE_PAATTYY :
        if( !raidetunnus.isEmpty())
        {
            skene_->kasky( QString("JK %1 %2")
                           .arg( alkaaTunnuksesta_.left( alkaaTunnuksesta_.length() -1 ) )
                           .arg( raidetunnus ) );
            valitseTila( JUNAKULKUTIE_ALKAA);
        }
        break;
    case KULKUTIENPERUMINEN:
        if( !raidetunnus.isEmpty())
        {
            skene_->kasky( QString("KPER %1").arg( raidetunnus));
        }
        break;
    case HATAKULKUTIENPURKU :
        if( !klikattuTunnus.isEmpty())
        {
            if( QMessageBox::question(this, tr("Kriittinen komento"),
                                      tr("Vahvista kulkutien hätävarainen purku raiteella %1")
                                      .arg(raidetunnus),
                                      QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok )
            {
                skene_->kasky( QString("HP %1").arg( raidetunnus ));
            }
            valitseTila(VIERITA);
        }
        break;
    default:
        // Oletuksena vuodetaan oletuskäsittelijälle (näin toimii vieritys)
        QGraphicsView::mousePressEvent(event);
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
