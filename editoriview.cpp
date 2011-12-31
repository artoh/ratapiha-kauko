/**************************************************************************
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 18.6.2011
**************************************************************************/

#include "editoriview.h"
#include "editorikisko.h"
#include "editoriikkuna.h"
#include "editoritekstikentta.h"

#include <QMouseEvent>
#include <QPointF>
#include <QScrollBar>
#include <cmath>
#include <QSqlQuery>

EditoriView::EditoriView(EditoriScene *skene, EditoriIkkuna *ikkuna) :
    QGraphicsView(skene), skene_(skene), ikkuna_(ikkuna), tila_(0), piirtoViiva_(0),
    valittuKisko_(0)
{
    setMouseTracking(true);

    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn);
}

void EditoriView::valitseTila(int tila)
{
    tila_ = tila;
    switch(tila)
    {
    case Osoitin :
        setCursor( Qt::ArrowCursor);
        break;
    case Piirto:
        setCursor( QCursor(QPixmap(":/r/pic/kyna.png"),1,30));
        poistaValinta();
        break;
    case Pyyhi:
        setCursor( QCursor(QPixmap(":/r/pic/pyyhekumi.png"),3,28));
        poistaValinta();
        break;
    case Teksti:
        setCursor( Qt::IBeamCursor);
        break;

    }
    emit editorinTilaVaihtunut(tila_);
}

void EditoriView::poistaValinta()
{
    if( valittuKisko())
        valittuKisko()->valitse(false);
    valittuKisko_ = 0;
}

void EditoriView::mousePressEvent(QMouseEvent *event)
{
    QPointF sijainti = mapToScene( event->pos());
    // Kokeillaan löytyykö tuolta kiskoa

    EditoriKisko* kisko = 0;
    qreal viivaetaisyys = 99.9; // Valinnan etäisyys raideviivasta

    QList<QGraphicsItem*> lista = items( event->pos() );

    // Koska kiskot voivat olla osittain päällekkäin, etsitään se,
    // joka lähimpänä keskiviivaa
    foreach( QGraphicsItem* item, lista)
    {
        EditoriKisko* koekisko = qgraphicsitem_cast<EditoriKisko*>(item);
        if( koekisko)
        {
            QPointF itemkoord = koekisko->mapFromScene( sijainti);
            if( qAbs(itemkoord.y()) < viivaetaisyys)
            {
                kisko = koekisko;
                viivaetaisyys = qAbs(itemkoord.y());
                   // Jee, lähin kisko on löytynyt !!!
            }
         }
    }


    if( event->button() == Qt::LeftButton)
    {
        switch( tila())
        {
        case Osoitin:
            if( kisko)
            {
                if( valittuKisko())
                    valittuKisko()->valitse(false);
                valittuKisko_ = kisko;
                valittuKisko()->valitse(true);
                emit kiskoValittu( valittuKisko()); // Päivitetään muokattavat esille

            }
            break;


        case Piirto:
        {

            // Aloitetaan piirtäminen

            // Kohdistetaan tasalukuun.. tähän voitaisiin tarvita magneettia
            QPointF kohdistettu = kohdista(sijainti);

            // Laitetaan piirtoviiva paikalleen
            piirtoViiva_ = new QGraphicsLineItem( QLineF(kohdistettu,kohdistettu));
            skene_->addItem(piirtoViiva_);
        }
            break;

        case Pyyhi:
            if( kisko )
            {
                // Poistetaan kyseinen. Pitää ottaa ensin naapurit turvaan...
                QList<QGraphicsItem*> naapurit = kisko->collidingItems();

                if( kisko->kiskoId() )
                {
                    // Poistetaan myös tietokannasta
                    QSqlQuery kysely;
                    kysely.exec( QString("DELETE FROM kisko WHERE kisko=%1").arg(kisko->kiskoId()) );
                }

                // Poistetaan valittu kisko
                scene()->removeItem(kisko);
                delete kisko;

                // Naapureille vähän mietittävää
                foreach( QGraphicsItem* naapuri, naapurit)
                {
                    if( EditoriKisko* naapuriKisko = qgraphicsitem_cast<EditoriKisko*>(naapuri))
                        naapuriKisko->tarkistaPaanTyypit();
                }
            }
            break;

        case Teksti:
            // Luo uuden tekstikentän
            EditoriTekstiKentta* tkentta = new EditoriTekstiKentta(skene_, sijainti);
            valitseTila(Osoitin);
            break;
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
               new EditoriKisko(skene_, piirtoViiva_->line(),0,ikkuna_->nykyLiikennePaikka());
            }
            else
                //rata_->addItem( new MittaNauha(piirtoviiva_->line()) );
            {;}
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

