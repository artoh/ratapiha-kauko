/**************************************************************************
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 18.6.2011
**************************************************************************/

#include "editoriview.h"
#include "editorikisko.h"
#include "editoriikkuna.h"
#include "mittanauha.h"

#include <QMouseEvent>
#include <QPointF>
#include <QScrollBar>
#include <cmath>
#include <QSqlQuery>
#include <QInputDialog>
#include <QRegExp>

EditoriView::EditoriView(EditoriScene *skene, EditoriIkkuna *ikkuna) :
    QGraphicsView(skene), skene_(skene), ikkuna_(ikkuna), tila_(0), piirtoViiva_(0),
    valittuKisko_(0), rullaSkaalaa_(true)
{
    setMouseTracking(true);

    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn);
}

void EditoriView::valitseTila(int tila)
{
    tila_ = tila;
    setDragMode( NoDrag );
    switch(tila)
    {
    case Osoitin :
        setCursor( Qt::ArrowCursor);
        break;
    case Piirto:
        setCursor( QCursor(QPixmap(":/r/pic/kyna.png"),1,30));
        break;
    case Pyyhi:
        setCursor( QCursor(QPixmap(":/r/pic/pyyhekumi.png"),3,28));
        poistaValinta();
        break;
    case Teksti:
        setCursor( Qt::IBeamCursor);
        poistaValinta();
        break;
    case Viivain:
        setCursor( QCursor(QPixmap(":/r/pic/viivain.png"),8,21));
        poistaValinta();
        break;
    case Vierita:
        setDragMode(ScrollHandDrag);
        break;

    }
    emit editorinTilaVaihtunut(tila_);
}

void EditoriView::poistaValinta()
{
    if( valittuKisko())
        valittuKisko()->valitse(false);
    ikkuna_->kiskoValittu(0);
    valittuKisko_ = 0;
    emit metriTeksti(QString());
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
        case Viivain:
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
            // Myöhemmin tänne tulee parempi editoridialogi, jossa fontin valintaa tms.
            QList<QGraphicsItem*> items = skene_->items(sijainti);
            QGraphicsSimpleTextItem* tItem = 0;
            foreach( QGraphicsItem* item, items)
            {
                if( qgraphicsitem_cast<QGraphicsSimpleTextItem*>(item) )
                    tItem = qgraphicsitem_cast<QGraphicsSimpleTextItem*>(item);
            }


            if( tItem && tItem->data(1).toInt() > 0 )  // Siellä on jo tekstiä, eli muokataan
            {
                bool ok;
                QString teksti = QInputDialog::getText(this, tr("Editori"),tr("Muokattava teksti"),QLineEdit::Normal, tItem->text(), &ok );
                if( !ok)
                {
                    ;
                }
                else if( teksti.isEmpty())
                {
                    skene_->removeItem(tItem);
                    QSqlQuery kysely;
                    kysely.exec( QString("DELETE FROM teksti WHERE tekstiid=%1")
                                 .arg(tItem->data(1).toInt()));
                    delete tItem;
                }
                else
                {
                    tItem->setText(teksti);
                    QSqlQuery kysely;
                    kysely.exec( QString("UPDATE teksti SET teksti=\"%1\" WHERE tekstiid=%2")
                                 .arg(teksti).arg(tItem->data(1).toInt() ));

                }
            }
            else
            {
                // Uusi teksti
                QString teksti = QInputDialog::getText(this, tr("Editori"),tr("Lisättävä teksti"));
                if( !teksti.isEmpty())
                {
                    QGraphicsSimpleTextItem* tItem = skene_->addSimpleText(teksti, QFont("Helvetica",8));
                    tItem->setPos(sijainti);

                    QSqlQuery kysely;
                    kysely.exec( QString("INSERT INTO teksti (nakyma,sijainti_x,sijainti_y,teksti) VALUES (%1,%2,%3,\"%4\");")
                                 .arg(skene_->nakyma()).arg( qRound(sijainti.x())).arg( qRound(sijainti.y())).arg(teksti.remove(QRegExp("[\"\']"))) );
                    tItem->setData(1, kysely.lastInsertId().toInt()); // Tästä tunnistaa itemin...
                }
            }

            break;
        }


    }
    else if( event->button() == Qt::MiddleButton)
    {
        // Keskimmäinen nappi vaihtaa hiiren rullan tilaa skaalauksen ja vierityksen väliltä.
        rullaSkaalaa_ = !rullaSkaalaa_;
    }
    else if( event->button() == Qt::XButton1)
    {
        // Sivussa oleva erikoisnappi
        // vaihtaa piirtotilan ja pyyhkimistilan välillä
        if( tila() == Piirto)
            valitseTila(Pyyhi);
        else
            valitseTila(Piirto);
    }
    else if( event->button() == Qt::XButton2)
    {
        // Toisella nappulalla laitetaan valinta tai vieritystila
        if( tila() == Osoitin )
            valitseTila(Vierita);
        else
            valitseTila(Osoitin);
    }

    // Ellei hiiren painallus ole johtanut toimiin, vuodetaan se
    // oletuskäsittelijälle
    QGraphicsView::mousePressEvent(event);
}


void EditoriView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF sijainti = mapToScene( event->pos());


    if( (tila() == Piirto  || tila()==Viivain) && piirtoViiva_)
    {
        // Piirto on käynnissä. Korjataan piirtoviivan loppupää
            piirtoViiva_->setLine( QLineF(piirtoViiva_->line().p1(), kohdista(sijainti)  )   );

        ensureVisible( QRectF( piirtoViiva_->line().x2(), piirtoViiva_->line().y2(), 10.0, 10.0) );
        emit metriTeksti( QString("%1 m").arg( qRound( piirtoViiva_->line().length() )) );
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
                EditoriKisko* uusi;
                if( skene_->nakyma())
                    // Näkymä
                    uusi = new EditoriKisko(skene_, piirtoViiva_->line(),0,ikkuna_->nykyLiikennePaikka());
                else
                    // Ratakisko - tällä on sn
                    uusi = new EditoriKisko(skene_, piirtoViiva_->line(),0,ikkuna_->nykyLiikennePaikka(),0,QString(),ikkuna_->nykyNopeusRajoitus());

                // Valitaan uusi kisko, jotta voidaan heti muokata
                if( valittuKisko())
                    valittuKisko()->valitse(false);
                valittuKisko_ = uusi;
                valittuKisko()->valitse(true);
                emit kiskoValittu( valittuKisko()); // Päivitetään muokattavat esille
            }
            else if(tila() == Viivain)
                skene_->addItem( new MittaNauha(piirtoViiva_->line()));

        }

        // Poistetaan tilapäinen piirtoviiva
        scene()->removeItem(piirtoViiva_);
        delete piirtoViiva_;
        piirtoViiva_ = 0L;
    }
    else
        // Vuodetaan hiiritoiminto ulos
        QGraphicsView::mouseReleaseEvent(event);
}

void EditoriView::wheelEvent(QWheelEvent *event)
{

    if( event->orientation() == Qt::Horizontal)
    {
        if( rullaSkaalaa_ )
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

