#include "junanuppi.h"
#include "veturi.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>


JunaNuppi::JunaNuppi(Veturi *veturi)
    : QGraphicsItem(veturi), veturi_(veturi)
{
//    setFlag( QGraphicsItem::ItemIgnoresTransformations, true);
    setPos( veturi->pituus() / 2.0, 0.0);
    setZValue(100);
}

void JunaNuppi::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{

    if( QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform()) > 0.75 )
        return;

    if( veturi_->ajopoyta() )
    {
        // Säde joka huomioi transformin
        qreal kerroin = 1 / QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform()  );
        if( QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform()) < 0.02)
            kerroin = 0.5 / QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform());

        int sade = (int) (kerroin * 10);


        QRadialGradient veturigrad( 0, 0, sade, 0, 0);

        if( veturi_->myohassa() > 10 * 60 ||
            (veturi_->myohassa() > 5 * 60 &&    veturi_->junaNumero().startsWith('P') ) ||
                (veturi_->myohassa() > 3 * 60 &&
                 (  veturi_->junaNumero().startsWith('S') || veturi_->junaNumero().startsWith('H') )))
        {
            // Punainen nuppi myöhässä olevalle junalle
            // T,V 10 min / P 5 min / H,S 3 min
            //
            veturigrad.setColorAt(1.0,QColor(117,0,0) );
            veturigrad.setColorAt(0.8,QColor(167,0,0));
            veturigrad.setColorAt(0.0,  QColor(255,0,0));
        }
        else if( veturi_->myohassa() > 30 )
        {
            // Puna-keltainen aikataulustaan puoli minuuttia jääneelle junalle

            veturigrad.setColorAt(0.0,QColor(245,255,43) );
            veturigrad.setColorAt(0.8,QColor(255,140,10));
            veturigrad.setColorAt(1.0,  QColor(255,24,0));

        }
        else if( veturi_->nopeus() < 36 )
        {
            // Keltainen nappi hitaalle/pysähtyneelle, ajoissa kulkevalle veturille
            veturigrad.setColorAt(0.0,QColor(250,255,0) );
            veturigrad.setColorAt(0.8,QColor(250,255,150));
            veturigrad.setColorAt(1.0,  QColor(170,172,111));
        }
        else
        {
            // Vihreä nappi ajoissa kulkevalle veturille
            veturigrad.setColorAt(0.0,QColor(155,255,54) );
            veturigrad.setColorAt(0.8,QColor(12,255,40));
            veturigrad.setColorAt(1.0,  QColor(6,191,132));
        }



        painter->setBrush( QBrush(veturigrad));
        painter->setPen( QPen(Qt::black, 0.5 ));
        painter->drawEllipse(QPointF(0.0, 0.0),(qreal)sade, (qreal) sade);
        if( !veturi_->junaNumero().isEmpty()  &&  QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform()) > 0.01)
        {
            QString tunnus;
            // Henkilöjunilla merkitään alkuun TOINEN kirjain, joka siis merkitsee
            // reittitunnusta
            if( veturi_->junaNumero().startsWith('H'))
                tunnus = veturi_->junaNumero().mid(1);
            else
                tunnus = veturi_->junaNumero();

            painter->setFont( QFont("Helvetica", (int) (kerroin * 10), QFont::Bold));
            painter->drawText( QRect(-sade, -sade, sade*2, (int) (kerroin * 14)), tunnus.left(1), QTextOption(Qt::AlignCenter));
            painter->setFont( QFont("Helvetica", (int)(4 * kerroin)));
            painter->drawText( QRect( -sade, -sade+(int)(11 * kerroin), sade*2, sade*2-(int)(kerroin*12)), tunnus.mid(1), QTextOption(Qt::AlignCenter));
        }
    }

}


QRectF JunaNuppi::boundingRect() const
{
    return QRect( -50, -50, 100, 100);
}

