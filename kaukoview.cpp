/**************************************************************************
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 6.1.2012
**************************************************************************/

#include "kaukoview.h"
#include "kaukokisko.h"

#include "rataikkuna.h"
#include "ratascene.h"

#include "ratapihaikkuna.h"

#include <QMouseEvent>
#include <QScrollBar>
#include <cmath>
#include <QInputDialog>
#include <QMessageBox>

KaukoView::KaukoView(KaukoScene* skene) :
    QGraphicsView(skene)
{
}

void KaukoView::valitseTila(int tila)
{
    tila_ = tila;
    setDragMode( NoDrag);

    emit tilaVaihtunut(tila);

    switch( tila )
    {
    case Vierita:
        setCursor( Qt::ArrowCursor);
        setDragMode( ScrollHandDrag );
        break;
    case JunaKulkutieAlkaa:
        setCursor( QCursor( QPixmap(":/r/pic/junakulkutienkursori.png"),9,0));
        break;
    case JunaKulkutiePaattyy :
        setCursor( QCursor( QPixmap(":/r/pic/junakulkutieloppuunkursori.png"),11,0));
        break;
    case VaihtoKulkutieAlkaa :
        setCursor( QCursor( QPixmap(":/r/pic/vaihtokulkutienkursori.png"),9,0));
        break;
    case VaihtoKulkutiePaattyy :
        setCursor( QCursor( QPixmap(":/r/pic/vaihtokulkutieloppuunkursori.png"),11,0));
        break;
    case PeruKulkutie:
        setCursor( QCursor( QPixmap(":/r/pic/kulkutienpurkukursori.png"),9,0));
        break;
    case VaihteenKaanto:
        setCursor( QCursor( QPixmap(":/r/pic/vaihdekursori.png"),9,0));
        break;
    case SeisKasky:
        setCursor( QCursor( QPixmap(":/r/pic/seiskursori.png"),9,0));
        break;
    case AjaKasky:
        setCursor( QCursor( QPixmap(":/r/pic/ajakursori.png"),9,0));
        break;
    case JunaNumeronSyotto :
        setCursor( QCursor( QPixmap(":r/pic/junanumerokursori.png"),9,0));
        break;
    case AukiajetunVaihteenKaanto:
        setCursor( QCursor( QPixmap(":/r/pic/vaihdekursori-aukiajettu.png"),9,0));
        break;
    case AutomaatioOn :
        setCursor( QCursor( QPixmap(":/r/pic/automaatiokursori.png"),9,0));
        break;
    case AutomaatioEi :
        setCursor( QCursor( QPixmap(":/r/pic/automaatiokursori-pois.png"),9,0));
        break;
    case AlHp :
        setCursor( QCursor( QPixmap(":/r/pic/alhpkursori.png"),9,0));
        break;
    case VarattuKulkutieAlkaa:
         setCursor( QCursor( QPixmap(":/r/pic/varattukulkutiekursori.png"),9,0));
         break;
    case VarattuKulkutiePaattyy :
        setCursor( QCursor( QPixmap(":/r/pic/varattukulkutieloppukursori.png"),11,0));
        break;
    case AutomaatioMuokkaus :
        setCursor( QCursor( QPixmap(":/r/pic/automaatiomuokkaus-kursori.png"),11,0));
        break;
    case AutomaatioHerate : case AutomaatioMaali :
        setCursor( QCursor( QPixmap(":/r/pic/automaatiomuokkaus-kasi.png"),11,0));
        break;


    }

}

void KaukoView::valitseAutomaatioHerate()
{
    valitseTila( AutomaatioHerate);
}

void KaukoView::valitseAutomaatioMaali()
{
    valitseTila( AutomaatioMaali) ;
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

void KaukoView::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::XButton1)
    {
        if( tila() != JunaKulkutieAlkaa)
            valitseTila( JunaKulkutieAlkaa);
        else
            valitseTila( VaihtoKulkutieAlkaa);
        return;
    }
    else if( event->button() == Qt::MiddleButton)
    {
        valitseTila( Vierita);
        return;
    }
    else if( event->button() == Qt::XButton2)
    {
        valitseTila( PeruKulkutie);
        return;
    }


    QString klikattuTunnus = raiteenPaaTunnusPisteessa( event->pos());

    switch( tila() )
    {
    case JunaKulkutieAlkaa :
            if( !klikattuTunnus.isEmpty() )
            {
                alkaaRaiteesta_ = klikattuTunnus;
                valitseTila( JunaKulkutiePaattyy);
            }
        break;

    case JunaKulkutiePaattyy:
        if( !klikattuTunnus.isEmpty())
        {
            komento( QString("JK %1 %2").arg(alkaaRaiteesta_.mid(1)).arg(klikattuTunnus.mid(1))   ) ;
            valitseTila( JunaKulkutieAlkaa );
        }
        break;

    case VaihtoKulkutieAlkaa :
            if( !klikattuTunnus.isEmpty() )
            {
                alkaaRaiteesta_ = klikattuTunnus;
                valitseTila( VaihtoKulkutiePaattyy);
            }
        break;

    case VaihtoKulkutiePaattyy:
        if( !klikattuTunnus.isEmpty())
        {
            komento( QString("UK %1 %2").arg(alkaaRaiteesta_.mid(1)).arg(klikattuTunnus.mid(1))   ) ;
            valitseTila( VaihtoKulkutieAlkaa );
        }
        break;

    case VarattuKulkutieAlkaa :
            if( !klikattuTunnus.isEmpty() )
            {
                alkaaRaiteesta_ = klikattuTunnus;
                valitseTila( VarattuKulkutiePaattyy);
            }
        break;

    case VarattuKulkutiePaattyy:
        if( !klikattuTunnus.isEmpty())
        {
            if( QMessageBox::question(this, tr("Kriittinen komento"),
                                      tr("Vahvista junakulkutien muodostamispyyntö raiteelta %1 varatulle raiteeelle %2")
                                      .arg(alkaaRaiteesta_.mid(1)).arg(klikattuTunnus.mid(1)),
                                      QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok )
            {
                komento( QString("VT %1 %2").arg(alkaaRaiteesta_.mid(1)).arg(klikattuTunnus.mid(1))   ) ;
                valitseTila( Vierita );
            }
        }
        break;

    case PeruKulkutie:
        if( !klikattuTunnus.isEmpty())
            komento( QString("KPER %1").arg(klikattuTunnus.mid(1)) );
        break;

    case VaihteenKaanto :
        if( !klikattuTunnus.isEmpty())
        {
            if( klikattuTunnus.startsWith('X'))
                komento( QString("V %1").arg(klikattuTunnus.mid(1)) );
            else
                komento( QString("V %1").arg(klikattuTunnus) );
        }
        break;

    case AukiajetunVaihteenKaanto:
        if( !klikattuTunnus.isEmpty())
        {
            if( QMessageBox::question(this, tr("Kriittinen komento"),
                                      tr("Vahvista aukiajetun vaihteen %1 kääntäminen")
                                      .arg(klikattuTunnus.mid(1)),
                                      QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok )
            {
                komento( QString("VAP %1").arg(klikattuTunnus.mid(1)) );
            }
        }
        break;


    case SeisKasky:
        if( !klikattuTunnus.isEmpty())
            komento( QString("SEIS %1").arg(klikattuTunnus) );
        break;

    case AjaKasky:
        if( !klikattuTunnus.isEmpty())
            komento( QString("AJA %1").arg(klikattuTunnus) );
        break;

    case JunaNumeronSyotto :
        if( !klikattuTunnus.isEmpty())
        {
            QString tunnus = QInputDialog::getText(0,"Ratapiha", tr("Syötä junanumero raiteelle %1 ").arg( klikattuTunnus.mid(1) ));
            if( !tunnus.isNull())
                komento( QString("JN %1 %2 ").arg(klikattuTunnus.mid(1)).arg(tunnus));
        }
    case AutomaatioOn:
        if( !klikattuTunnus.isEmpty())
            komento( QString("AUON %1").arg(klikattuTunnus) );
        break;
    case AutomaatioEi:
        if( !klikattuTunnus.isEmpty())
            komento( QString("AUEI %1").arg(klikattuTunnus) );
        break;
    case AlHp:
        if( !klikattuTunnus.isEmpty())
        {
            if( QMessageBox::question(this, tr("Kriittinen komento"),
                                      tr("Vahvista akselinlaskennan hätävarainen purku raiteelle %1")
                                      .arg(klikattuTunnus.mid(1)),
                                      QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok )
            {
                komento( QString("ALHP %1").arg(klikattuTunnus.mid(1)) );
            }
        }
        break;
    case AutomaatioMuokkaus :
        emit automaatioMuokkausOpastin(klikattuTunnus);
        break;
    case AutomaatioHerate :
        emit automaatioHerate(klikattuTunnus);
        valitseTila(AutomaatioMuokkaus);
        break;
    case AutomaatioMaali :
        emit automaatioMaali(klikattuTunnus);
        valitseTila(AutomaatioMuokkaus);
        break;


    default:
        // Vuodetaan oletuskäsittelijälle
        QGraphicsView::mousePressEvent(event);
    }

}

QString KaukoView::raiteenPaaTunnusPisteessa(QPoint piste)
{
    // Hakee pisteessä parhaiten olevan kiskon pään tunnuksen
    QList<QGraphicsItem*> lista = items(piste);
    QString tunnus;
    qreal etaisyys = 99;

    foreach( QGraphicsItem* item, lista)
    {
        KaukoKisko* kisko = qgraphicsitem_cast<KaukoKisko*>(item);
        if( kisko )
        {
            // Lasketaan etäisyydellä että saadaan paras kisko
            QPointF kiskoPos = kisko->mapFromScene( mapToScene(piste) ) ;
            qreal tamaEtaisyys = qAbs(kiskoPos.y());
            if( kiskoPos.x() < 0)
                tamaEtaisyys += 0 - kiskoPos.x() * 2;
            else if( kiskoPos.x() > kisko->pituus())
                tamaEtaisyys += (kiskoPos.x() - kisko->pituus()) * 2;

            if( tamaEtaisyys < etaisyys)
            {
                etaisyys = tamaEtaisyys;

                // Jos on "molemmissa päissä" niin kumpi lienee lähempänä
                if( (kisko->etelaTyyppi() == Kisko::Paa || kisko->etelaTyyppi()==Kisko::LiikennePaikanPaa)
                        && ( kisko->pohjoisTyyppi()==Kisko::Paa || kisko->pohjoisTyyppi()==Kisko::LiikennePaikanPaa))
                {
                    if( kiskoPos.x() < kisko->pituus() / 2 )
                        tunnus = QChar('E') + kisko->raideTunnariLiikennepaikalla();
                   else
                        tunnus = QChar('P') + kisko->raideTunnariLiikennepaikalla();
                }
                // Jos toisessa päässä, sen pään tunnus
                else if( kisko->etelaTyyppi() == Kisko::Paa || kisko->etelaTyyppi() == Kisko::LiikennePaikanPaa)
                    tunnus = QChar('E') + kisko->raideTunnariLiikennepaikalla();
                else if( kisko->pohjoisTyyppi() == Kisko::Paa || kisko->pohjoisTyyppi() == Kisko::LiikennePaikanPaa)
                    tunnus = QChar('P') + kisko->raideTunnariLiikennepaikalla();
                else
                    tunnus = QChar('X') + kisko->raideTunnariLiikennepaikalla();

            }
        }

    }
    return tunnus;
}

void KaukoView::komento(const QString &komento)
{
    RatapihaIkkuna::getInstance()->aslKasky(komento);
}
