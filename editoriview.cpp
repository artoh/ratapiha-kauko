/**************************************************************************
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 18.6.2011
**************************************************************************/

#include "editoriview.h"
#include "editorikisko.h"

#include <QMouseEvent>
#include <QPointF>
#include <QScrollBar>
#include <cmath>

EditoriView::EditoriView(EditoriScene *skene) :
    QGraphicsView(skene), skene_(skene), tila_(Piirto), piirtoViiva_(0)
{
    setMouseTracking(true);

    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn);
}


void EditoriView::mousePressEvent(QMouseEvent *event)
{
    QPointF sijainti = mapToScene( event->pos());

    if( event->button() == Qt::LeftButton)
    {
        if( tila()==Piirto )
        {
            // Aloitetaan piirtäminen

            // Kohdistetaan tasalukuun.. tähän voitaisiin tarvita magneettia
            QPointF kohdistettu = kohdista(sijainti);

            // Laitetaan piirtoviiva paikalleen
            piirtoViiva_ = new QGraphicsLineItem( QLineF(kohdistettu,kohdistettu));
            skene_->addItem(piirtoViiva_);
        }


    }
}


void EditoriView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF sijainti = mapToScene( event->pos());


    if( tila() == Piirto && piirtoViiva_)
    {
        // Piirto on käynnissä. Korjataan piirtoviivan loppupää
            piirtoViiva_->setLine( QLineF(piirtoViiva_->line().p1(), kohdista(sijainti)  )   );
       // Skene kasvatetaan, niin että mahtuu
 /*
        if( rata_->mahdutaRadalle( piirtoviiva_->line().p2() ) && ruudukko_)
        {
            ruudukko_->update(scene()->sceneRect());
            ruudukko_->naytaPiirtoViivat(true);
        }
  */
        ensureVisible( QRectF( piirtoViiva_->line().x2(), piirtoViiva_->line().y2(), 10.0, 10.0) );
        emit naytettavaRaiteenPituus( piirtoViiva_->line().length() );
    }
    else
        // Vuodetaan käsittelemätön ulos
        QGraphicsView::mouseMoveEvent(event);
}


void EditoriView::mouseReleaseEvent(QMouseEvent *event)
{
    if( piirtoViiva_ )
    {
        // Piirtoviiva on tullut valmiiksi
        // Viivan vähimmäispituus 5, estää napsautukset
        if( piirtoViiva_->line().length() > 5 )
        {
            if( tila_ == Piirto)
            {
               EditoriKisko* kisko = new EditoriKisko(skene_, piirtoViiva_->line());
               skene_->addItem(kisko);
            }
            else
                ; //rata_->addItem( new MittaNauha(piirtoviiva_->line()) );

            ;
        }

        // Poistetaan tilapäinen piirtoviiva
        scene()->removeItem(piirtoViiva_);
        delete piirtoViiva_;
        piirtoViiva_ = 0L;
        emit naytettavaRaiteenPituus(0.0);
    }
    else
        // Vuodetaan hiiritoiminto ulos
        QGraphicsView::mouseReleaseEvent(event);
}

void EditoriView::wheelEvent(QWheelEvent *event)
{
    bool skaalausTila = true;

    if( event->orientation() == Qt::Horizontal)
    {
        if( skaalausTila )
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
        if( skaalausTila)
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

qreal EditoriView::pyorista(qreal koordinaatti)
{
    // Pyöristää lähimpään viiteen
    // Käytetään kohdistamaan hiiren kursoria 5 x 5 -ruudukkoon
    int itseisarvo = (int) std::abs(koordinaatti);
    int jaama = itseisarvo % 5;

    if( koordinaatti > 0.0 )
    {
        if( jaama < 3)
            return (qreal) ( itseisarvo - jaama );    // pyöristetään alas
        else
            return (qreal)  ( itseisarvo  - jaama + 5);    // pyöristetään ylös
    }
    else
    {
        if( jaama < 3)
            return (qreal) ( 0 - itseisarvo + jaama);   // Pyöristetään kohti nollaa
        else
            return (qreal) ( 0 - itseisarvo + jaama - 5);  // Pyöristetään pois nollasta
    }
}

QPointF EditoriView::kohdista(const QPointF& pos)
{
    return QPointF( pyorista( pos.x() ), pyorista( pos.y())  );
}

