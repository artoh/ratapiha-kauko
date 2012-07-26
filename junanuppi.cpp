#include "junanuppi.h"
#include "jkvlaite.h"
#include "raideelementti.h"
#include "rata.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>


JunaNuppi::JunaNuppi(Veturi *veturi)
    : QGraphicsItem(veturi), veturi_(veturi)
{
    setFlag( QGraphicsItem::ItemIgnoresTransformations, true);
    setPos( veturi->pituus() / 2.0, 0.0);
}

void JunaNuppi::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    if( veturi_->ajopoyta())
    {
        QRadialGradient veturigrad( 0, 0, SADE, 0, 0);

        if( veturi_->nopeus() < 1 && veturi_->jkvlaite() && veturi_->jkvlaite()->jumissa())
        {
            // Punainen jumittuneelle junalle - seisonut 15 s. yli pysähdysajan
            veturigrad.setColorAt(1.0,QColor(117,0,0) );
            veturigrad.setColorAt(0.8,QColor(167,0,0));
            veturigrad.setColorAt(0.0,  QColor(255,0,0));
        }
        else if( veturi_->nopeus() < 1 )
        {
            // Puna-keltainen pysähtyneelle junalle

            veturigrad.setColorAt(0.0,QColor(245,255,43) );
            veturigrad.setColorAt(0.8,QColor(255,140,10));
            veturigrad.setColorAt(1.0,  QColor(255,24,0));

        }
        else if( veturi_->nopeus() < 30)
        {
            // Keltainen nappi hitaalle veturille
            veturigrad.setColorAt(0.0,QColor(250,255,0) );
            veturigrad.setColorAt(0.8,QColor(250,255,150));
            veturigrad.setColorAt(1.0,  QColor(170,172,111));
        }
        else
        {
            // Vihreä nappi nopealle veturille
            veturigrad.setColorAt(0.0,QColor(155,255,54) );
            veturigrad.setColorAt(0.8,QColor(12,255,40));
            veturigrad.setColorAt(1.0,  QColor(6,191,132));
        }



        painter->setBrush( QBrush(veturigrad));
        painter->setPen( QPen(Qt::black, 0.5 ));
        painter->drawEllipse(QPointF(0.0, 0.0),(qreal)SADE, (qreal) SADE);
        if( veturi_->jkvlaite() && !veturi_->jkvlaite()->junaNumero().isEmpty())
        {
            painter->setFont( QFont("Helvetica", 10, QFont::Bold));
            painter->drawText( QRect(-SADE, -SADE, SADE*2, 14), veturi_->jkvlaite()->junaNumero().left(1), QTextOption(Qt::AlignCenter));
            painter->setFont( QFont("Helvetica", 4));
            painter->drawText( QRect( -SADE, -SADE+11, SADE*2, SADE*2-12), veturi_->jkvlaite()->junaNumero().mid(1), QTextOption(Qt::AlignCenter));

        }
    }

}

QRectF JunaNuppi::boundingRect() const
{
    return QRect( -SADE-2, -SADE-2, SADE*2+4, SADE*2+4);
}

const int JunaNuppi::SADE;
