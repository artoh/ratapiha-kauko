/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "editorikisko.h"
#include "editoriscene.h"
#include <QPainter>
#include <QPen>
#include <QList>
#include <QSqlQuery>

#include <QDebug>
#include <QSqlError>

EditoriKisko::EditoriKisko(EditoriScene *skene, const QLineF &viiva, int kiskoid, const QString &liikennepaikka, int raide, const QString &kiskodata, int sn) :
    Kisko(viiva, kiskoid, liikennepaikka, raide, kiskodata, sn), skene_(skene),
    valittu_(false)
{

    skene->addItem(this);
    paidenTarkistusToimet();
}

void EditoriKisko::paidenTarkistusToimet(bool tallenna)
{
    // Tarkistetaan kaverit
    tarkistaPaanTyypit();

    // Ja tarkistakoot vielä naapuritkin uudemman kerran
    QList<QGraphicsItem*> tormaajat = collidingItems();
    foreach( QGraphicsItem* item, tormaajat)
    {
        if( EditoriKisko* ekisko = qgraphicsitem_cast<EditoriKisko*>(item))
        {
            ekisko->tarkistaPaanTyypit();
            if( tallenna)       // Siltä varalta, että päässä on jotain muuttunut!
                ekisko->talletaKisko();
        }
    }
    if( tallenna )
        talletaKisko();
}

QRectF EditoriKisko::boundingRect() const
{
    return QRectF(-5.0, -10.0, pituus()+10.0, 20.0);
}

void EditoriKisko::valitse(bool onko)
{
    valittu_ = onko;
    update(boundingRect());
}

void EditoriKisko::asetaLiikennepaikka(const QString &lyhenne)
{
    liikennepaikka_ = lyhenne;
    update(boundingRect());

}

void EditoriKisko::asetaRaide(int raide)
{
    raide_ = raide;
    update(boundingRect());
}

void EditoriKisko::levitaRaiteenAsetus()
{
    if( raide() < 1 || liikennePaikka().isEmpty() )
       return;  // Ei ole levitettävää

    if( etelaTyyppi() > 9 || pohjoisTyyppi() > 9)
    {

        QPointF sijainti;
        if( etelaTyyppi() > 9)
            sijainti = etelainen();
        else if( pohjoisTyyppi() > 9)
            sijainti = pohjoinen();

        QList<QGraphicsItem*> lista = collidingItems();

        foreach( QGraphicsItem* item, lista)
        {
            EditoriKisko* kisko = qgraphicsitem_cast<EditoriKisko*>(item);
            if( kisko && kisko!=this && ( kisko->etelainen() == sijainti || kisko->pohjoinen()==sijainti ) )
            {
                // Tämä on vaihdekaveri, eli tälle asetetaan sama liikennepaikka ja raide.
                kisko->asetaRaide( raide());
                kisko->asetaLiikennepaikka( liikennePaikka() );
            }
        }
    }
    paidenTarkistusToimet(true);
}

void EditoriKisko::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    // Valitun kiskon taakse laatikko
    if( valittu_)
    {
        painter->setBrush( QBrush( QColor(255,128,0,125) ) );
        painter->setPen( Qt::NoPen);
        painter->drawRect( QRectF( -3.0, -6.0, pituus()+6, 12 ) );
    }

    painter->setPen( QPen(QBrush(Qt::green),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->drawLine(0.0, 0.0, 2.5, 0.0);

    painter->setFont( QFont("Helvetica",2,QFont::Bold));
    painter->drawText(QRectF(0.0, 1.0, 3.0, 4.0), QString("%1").arg( etelaTyyppi() ), QTextOption(Qt::AlignLeft));

    painter->setPen( QPen(QBrush(Qt::darkGreen),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    painter->drawLine(pituus()-2.5, 0.0, pituus(), 0.0);
    painter->drawText(QRectF(pituus()-4.0, 1.0, 3.0, 4.0), QString("%1").arg( pohjoisTyyppi() ), QTextOption(Qt::AlignRight));



    if( raide() > 0 && !liikennePaikka().isEmpty())
    {
        // Raidetunnus laitetaan kiskon ylle
        if( etelapaaTyyppi_ > 9 || pohjoispaaTyyppi_ > 9)
        {
            painter->setFont( QFont("Helvetica",2,QFont::DemiBold));
            painter->setPen( QPen(Qt::blue));   // Vaihde tai RR
            if( etelaTyyppi() == VaihdeJatkos || pohjoisTyyppi() == VaihdeJatkos)
            {
                painter->setFont( QFont("Helvetica",4,QFont::Bold));
                painter->drawText(QRectF(0.0, -9.0, pituus(), 5.0), QString("%1").arg(raide(),3,10,QChar('0')), QTextOption(Qt::AlignCenter));
            }
            else if( etelaTyyppi() == VaihdeVasen )
                painter->drawText(QRectF(0.0, -7.0, pituus(), 4.0), QString("%1c").arg(raide(),3,10,QChar('0')), QTextOption(Qt::AlignCenter));
            else if( etelaTyyppi() == VaihdeOikea )
                painter->drawText(QRectF(0.0, -7.0, pituus(), 4.0), QString("%1d").arg(raide(),3,10,QChar('0')), QTextOption(Qt::AlignCenter));
            else if( pohjoisTyyppi() == VaihdeVasen )
                painter->drawText(QRectF(0.0, -7.0, pituus(), 4.0), QString("%1b").arg(raide(),3,10,QChar('0')), QTextOption(Qt::AlignCenter));
            else if( pohjoisTyyppi() == VaihdeOikea )
                painter->drawText(QRectF(0.0, -7.0, pituus(), 4.0), QString("%1a").arg(raide(),3,10,QChar('0')), QTextOption(Qt::AlignCenter));
        }
        else
        {
            painter->setFont( QFont("Helvetica",4,QFont::Bold));
            painter->setPen( QPen(Qt::black));
            painter->drawText(QRectF(0.0, -9.0, pituus(), 5.0), QString("%1").arg(raide(),3,10,QChar('0')), QTextOption(Qt::AlignCenter));
        }
        painter->setPen( QPen(QBrush(Qt::black),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    }
    else
    {
        // Ei raidetunnusta
        painter->setPen( QPen(QBrush(Qt::magenta),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    }

    // Virheelliset punaisella !
    if( etelaTyyppi() == Virhe || pohjoisTyyppi() == Virhe )
        painter->setPen( QPen(QBrush(Qt::red),2.0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));


    painter->drawLine(2.5, 0.0, pituus()-2.5, 0.0);

}


void EditoriKisko::talletaKisko()
{

    if( !raide() || liikennePaikka().isEmpty() || skene_->nakyma()<0)
        return;     // Ei tallennettavaa!

    QSqlQuery kysely;
    if( kiskoid_ == 0)
    {
        // Kiskoa ei ole vielä talletettu, eli se pitää lisätä
        kysely.prepare("INSERT INTO kisko (nakyma, liikennepaikka, raide, etela_x, etela_y, pohjoinen_x, pohjoinen_y, sn, kiskotieto )"
                       "VALUES (:nakyma, :liikennepaikka, :raide, :etela_x, :etela_y, :pohjoinen_x, :pohjoinen_y, :sn, :kiskotieto)");

        kysely.bindValue(":nakyma",skene_->nakyma());
        kysely.bindValue(":liikennepaikka",liikennePaikka());
        kysely.bindValue(":raide",raide());
        kysely.bindValue(":etela_x",viiva().x1());
        kysely.bindValue(":etela_y",0-viiva().y1());
        kysely.bindValue(":pohjoinen_x",viiva().x2());
        kysely.bindValue("pohjoinen_y",0-viiva().y2());
        if( sn())
            kysely.bindValue(":sn",sn());
        else
            kysely.bindValue( ":sn", QVariant(QVariant::Int));  // NULL jos ei ole sn-asetusta
        kysely.bindValue(":kiskotieto",kiskoTietoTalletettavaksi());
        kysely.exec();
        kiskoid_ = kysely.lastInsertId().toInt();

    }
    else
    {
        QString kysymys = QString("UPDATE kisko SET nakyma=%1, liikennepaikka=\"%2\", raide=%3, etela_x=%4, etela_y=%5, pohjoinen_x=%6, pohjoinen_y=%7, sn=%8, kiskotieto=\"%10\" where kisko=%11")
                .arg( skene_->nakyma()  ).arg(liikennePaikka()).arg( raide()).arg(viiva().x1()).arg(viiva().y1()).arg(viiva().x2()).arg(viiva().y2()).arg(sn()).arg(kiskoTietoTalletettavaksi()).arg(kiskoid_ );
        kysely.exec(kysymys);
    }

    qDebug() << kysely.lastQuery();
    qDebug() << kysely.lastError().text();

}


QString EditoriKisko::kiskoTietoTalletettavaksi() const
{
    // Sitten pitäisi vielä kyhätä kiskotietolauselma ;)
    QString kiskotieto;

    // Kiskonpään tiedot
    switch(etelaTyyppi())
    {
    case Paa :
        kiskotieto.append("E* ");
        break;
    case VaihdeJatkos:
        kiskotieto.append("E= ");
        break;
    case VaihdeVasen:
        kiskotieto.append("E- ");
        break;
    case VaihdeOikea:
        kiskotieto.append("E+ ");
        break;
    case Virhe:
    case Valille:
        break;
    }

    switch(pohjoisTyyppi())
    {
    case Paa :
        kiskotieto.append("P* ");
        break;
    case VaihdeJatkos:
        kiskotieto.append("P= ");
        break;
    case VaihdeVasen:
        kiskotieto.append("P- ");
        break;
    case VaihdeOikea:
        kiskotieto.append("P+ ");
        break;
    case Virhe:
    case Valille:
        break;
    }

    // Laiturin tiedot
    if( laituri() == LaituriVasemmalla )
        kiskotieto.append("Lv ");
    else if(laituri() == LaituriOikealla)
        kiskotieto.append("Lo ");

    // Vielä: Nr, Nj, Kj, Kt, Kv, EoE, EoP

    return kiskotieto;
}
