/**************************************************************************
**  ratakisko.cpp
**  Copyright (c) 2012 Arto Hyvättinen 
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  See <http://www.gnu.org/licenses/>
**
**  RataKisko  10.7.2012
**************************************************************************/

#include "ratakisko.h"
#include "raiteenpaa.h"
#include "rataraide.h"

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>

#include <QDebug>

RataKisko::RataKisko(RataRaide *raide, const QLineF &viiva, int kiskoid, const QString &kiskodata, int sn)
    : Kisko(viiva, kiskoid, kiskodata), raide_(raide), sn_(sn), opastinEtelaan_(0), opastinPohjoiseen_(0)
{


    // Opastimien laittamiset
        RaiteenPaa::Opaste esiopastinkasiteEtela = RaiteenPaa::OpastinPuuttuu;
        if( kiskodata.contains("EoE"))
            esiopastinkasiteEtela = RaiteenPaa::Tyhja;

        if( etelaTyyppi() == Paa || etelaTyyppi() == LiikennePaikanPaa )
        {
            if( raide->etelainen()->opastin() != RaiteenPaa::EiOpastinta )
                opastinEtelaan_ = new Opastin(this, raide->etelainen(), esiopastinkasiteEtela,
                                              QString("E%1").arg(raide->raidetunnus(),3,10,QChar('0')));
        }
        else if( esiopastinkasiteEtela != RaiteenPaa::OpastinPuuttuu)
            opastinEtelaan_ = new Opastin(this, 0, esiopastinkasiteEtela);

        if( opastinEtelaan_)
        {
            opastinEtelaan_->setParentItem(this);
            opastinEtelaan_->setPos(0.0, -5.0);
            opastinEtelaan_->rotate(-90.0);
        }

        // Sama pohjoiseen
        RaiteenPaa::Opaste esiopastinkasitePohjoinen = RaiteenPaa::OpastinPuuttuu;
        if( kiskodata.contains("EoP"))
            esiopastinkasitePohjoinen= RaiteenPaa::Tyhja;

        if( pohjoisTyyppi() == Paa || pohjoisTyyppi() == LiikennePaikanPaa )
        {
            if( raide->pohjoinen()->opastin() != RaiteenPaa::EiOpastinta )
                opastinPohjoiseen_ = new Opastin(this, raide->pohjoinen(), esiopastinkasitePohjoinen,
                                                 QString("P%1").arg(raide->raidetunnus(),3,10,QChar('0')));
        }
        else if( esiopastinkasitePohjoinen != RaiteenPaa::OpastinPuuttuu)
            opastinPohjoiseen_ = new Opastin(this, 0, esiopastinkasiteEtela);

        if( opastinPohjoiseen_)
        {
            opastinPohjoiseen_->setParentItem(this);
            opastinPohjoiseen_->setPos( pituus(), 5.0);
            opastinPohjoiseen_->rotate(90.0);
        }


    // Jos silta, niin ylennetään
    if(silta())
        setZValue(10.0);

}

QRectF RataKisko::boundingRect() const
{
    return QRectF(-15.0, -15.0, pituus()+30.0, 30.0);
}

void RataKisko::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    // Piirretään yksinkertaistettu näkymä
    if( QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform()) < 0.25 )
    {
        // Piirretään asema-alueet
        if( laituri() != LaituriEi )
        {
            painter->setPen( Qt::NoPen);
            painter->setBrush( QBrush(Qt::blue));
            painter->drawRect( QRectF(0.0, -4.0 / QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform()) , pituus(), 8.0 / QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform() )));

        }

        painter->setPen( QPen( QBrush( Qt::darkGray ), 2.0 / QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform()), Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        painter->drawLine( 0.0, 0.0, pituus(), 0.0 );
        return;

    }

    // Silta
    if( silta())
    {
        painter->setBrush( Qt::gray);
        painter->setPen(Qt::NoPen);
        painter->drawRect(QRectF(0.0, -3.0, pituus(),  6.0));
        painter->setPen( QPen(QBrush(Qt::black),0.7, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        painter->drawLine( QLineF(4.0, -3.0, pituus()-4.0, -3.0));
        painter->drawLine( QLineF(4.0, 3.0, pituus()-4.0, 3.0));
    }



    // Laituri
    if( laituri() == LaituriVasemmalla  || laituri() == LaituriMolemmat)
    {
        painter->setPen( Qt::NoPen);
        painter->setBrush( QBrush( Qt::gray));
        painter->drawRect(QRectF(10.0, -18.0, pituus()-20.0, 15.0));
    }
    if( laituri() == LaituriOikealla || laituri() == LaituriMolemmat )
    {
        painter->setPen( Qt::NoPen);
        painter->setBrush( QBrush( Qt::gray));
        painter->drawRect(QRectF(10.0, 3.0, pituus()-20.0, 15.0));
    }


    // Raideristeykseen lisäviivat
    if( raide()->etelainen()->paanTyyppi() == RaiteenPaa::RaideRisteys)
    {
        painter->setPen(Qt::black);
        painter->drawLine(4.0, 2.5, pituus()-4.0, 2.5);
        painter->drawLine(4.0, -2.5, pituus()-4.0, -2.5);
    }

    // Sitten myös vaihteiden mukaista piirtelyä
    qreal alku = 0.0;
    qreal loppu = pituus();

    if( etelaTyyppi() == VaihdeVasen  &&  raide()->etelainen()->paanTyyppi()!=RaiteenPaa::RaideRisteys && raide()->pohjoinen()->vaihde() == RaiteenPaa::Oikea  )
        alku += 10.0;
    else if( etelaTyyppi() == VaihdeOikea &&  raide()->etelainen()->paanTyyppi()!=RaiteenPaa::RaideRisteys && raide()->pohjoinen()->vaihde() == RaiteenPaa::Vasen )
        alku += 10.0;
    else if( pohjoisTyyppi() == VaihdeVasen &&  raide()->etelainen()->paanTyyppi()==RaiteenPaa::Vaihde && raide()->etelainen()->vaihde() == RaiteenPaa::Oikea )
        loppu -= 10.0;
    else if( pohjoisTyyppi() == VaihdeOikea &&  raide()->etelainen()->paanTyyppi()==RaiteenPaa::Vaihde && raide()->etelainen()->vaihde() == RaiteenPaa::Vasen )
        loppu -= 10.0;


    if( raide()->sahkoistetty() )
        painter->setPen( QPen(QBrush(Qt::darkGray),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    else
        painter->setPen( QPen(QBrush(Qt::darkBlue),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

    if( etelaTyyppi() == Paa && raide()->etelainen()->paanTyyppi() == RaiteenPaa::RaidePuskuri)
        painter->drawLine(QLineF(0.0, -4.0, 0.0, 4.0));
    if( pohjoisTyyppi() == Paa && raide()->pohjoinen()->paanTyyppi() == RaiteenPaa::RaidePuskuri)
        painter->drawLine(QLineF(pituus(), -4.0, pituus(), 4.0));


    painter->drawLine(alku, 0.0, loppu, 0.0);



}

bool RataKisko::aktiivinen(Kisko::PaanSuunta paassa, RataKisko *toinenKisko)
{
    if( raide()->etelainen()->paanTyyppi() == RaiteenPaa::RaideRisteys )
    {
        if( !toinenKisko)
            return true;    /* Raideristeys aina "aktiivinen" ellei nimenomaisesti... */
        if( paassa == EtelaPaa && ( etelaTyyppi()==VaihdeVasen || etelaTyyppi()==VaihdeOikea))
            return etelaTyyppi()==toinenKisko->pohjoisTyyppi();
        else if( paassa == PohjoisPaa && ( pohjoisTyyppi()==VaihdeVasen || pohjoisTyyppi()==VaihdeOikea ))
            return pohjoisTyyppi() == toinenKisko->etelaTyyppi();
    }

    if( paassa == EtelaPaa)
    {
        if( etelaTyyppi() == Paa || etelaTyyppi()==LiikennePaikanPaa || etelaTyyppi() == Valille )
            return true;
        else if( toinenKisko && etelainen() != toinenKisko->pohjoinen())
            return false;
        else if( etelaTyyppi() == VaihdeVasen )
            return raide()->pohjoinen()->vaihde() == RaiteenPaa::Vasen;
        else if( etelaTyyppi() == VaihdeOikea)
            return raide()->pohjoinen()->vaihde() == RaiteenPaa::Oikea;
    }
    else
    {
        if( pohjoisTyyppi() == Paa || pohjoisTyyppi() == Valille )
            return true;
        else if( toinenKisko && pohjoinen() != toinenKisko->etelainen())
            return false;
        else if( pohjoisTyyppi() == VaihdeVasen )
            return raide()->etelainen()->vaihde() == RaiteenPaa::Vasen;
        else if( pohjoisTyyppi() == VaihdeOikea)
            return raide()->etelainen()->vaihde() == RaiteenPaa::Oikea;
    }
    return true;

}

void RataKisko::esiopastinIlmoitus(RaiteenPaa::Opaste opaste, Opastin *opastimelta)
{
    // Oma opastin ilmoittaa opastinkäsitteen muutoksesta
    if( opastimelta == opastinPohjoiseen_)
    {
        RataKisko* seuraava = haeAktiivinenNaapuri( etelainen());
        if( seuraava )
            seuraava->esiopastinHaku(opaste, etelainen(), pituus());
    }
    else
    {
        RataKisko* seuraava = haeAktiivinenNaapuri( pohjoinen());
        if( seuraava )
            seuraava->esiopastinHaku(opaste, pohjoinen(), pituus());
    }


}



RataKisko *RataKisko::haeAktiivinenNaapuri(QPointF sijainnista)
{
    QList<QGraphicsItem*> lista = scene()->items(sijainnista);
    foreach( QGraphicsItem* item, lista)
    {
        RataKisko* kisko = qgraphicsitem_cast<RataKisko*>(item);

        if( kisko && kisko != this )
        {
            if( kisko->etelainen() == sijainnista && kisko->aktiivinen( EtelaPaa , this)  )
                return kisko;
            else if( kisko->pohjoinen() == sijainnista && kisko->aktiivinen( PohjoisPaa, this ) )
                return kisko;
        }
    }
    return 0;

}

Opastin *RataKisko::opastinSijainnissa(QPointF sijainnissa)
{
    if( sijainnissa == etelainen())
        return opastinEtelaan_;
    else if( sijainnissa == pohjoinen())
        return opastinPohjoiseen_;
    return 0;
}

void RataKisko::esiopastinHaku(RaiteenPaa::Opaste opaste, QPointF naapurilta, qreal metria)
{
    qreal yhteensa = metria + pituus();
    if( yhteensa > 5000)
        return;


    if( naapurilta == pohjoinen())
    {
        if( opastinPohjoiseen_ )    // On itsellään...
            opastinPohjoiseen_->asetaEsiOpastin(opaste);
        else
        {
            RataKisko* seuraava = haeAktiivinenNaapuri( etelainen());
            if( seuraava )
                seuraava->esiopastinHaku(opaste, etelainen(), yhteensa);
        }
    }
    else if( naapurilta == etelainen())
    {
        if( opastinEtelaan_ )    // On itsellään...
            opastinEtelaan_->asetaEsiOpastin(opaste);
        else
        {
            RataKisko* seuraava = haeAktiivinenNaapuri( pohjoinen());
            if( seuraava )
                seuraava->esiopastinHaku(opaste, pohjoinen(), yhteensa);
        }
    }

}

void RataKisko::kerroOpastimet()
{
    if( opastinPohjoiseen_)
        esiopastinIlmoitus(opastinPohjoiseen_->opaste(), opastinPohjoiseen_);
    if( opastinEtelaan_)
        esiopastinIlmoitus(opastinEtelaan_->opaste(), opastinEtelaan_);

}


