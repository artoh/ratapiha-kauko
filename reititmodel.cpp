/**************************************************************************
Ratapiha - railway construction and simulation toy

(c) Arto Hyvättinen 2012
5.8.2012

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


**************************************************************************/

#include "reititmodel.h"

#include <QSqlQuery>


ReititModel::ReititModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    QSqlQuery liikennepaikkakysely("select liikennepaikka, nimi from liikennepaikka order by nimi");
    while( liikennepaikkakysely.next())
        liikennepaikat_.insert( liikennepaikkakysely.value(0).toString(), liikennepaikkakysely.value(1).toString() );

}

QVariant ReititModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case Liikennepaikka: return tr("Liikennepaikka");
        case Raide : return tr("Raide");
        case Aika : return tr("Lähtöaika");
        case Pysahtyy : return tr("Pysähdysaika");
        case Tapahtuma : return tr("Tapahtuma");
        case Suunta: return tr("Suunta");
        }
    }
    return QVariant();
}

QVariant ReititModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid())
        return QVariant();

    if( role == Qt::TextAlignmentRole )
    {
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else if( role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case Liikennepaikka:
            return tiedot_.at( index.row()).liikennepaikka() ;
        case Raide:
            return tiedot_.at( index.row()).raide();
        case Aika:
            return tiedot_.at( index.row()).lahtoAika();
        case Pysahtyy:
            return tiedot_.at( index.row()).pysahtyy();
        case Tapahtuma:
        {
            return (int) tiedot_.at( index.row()).tapahtumaTyyppi();
        }
        case Suunta:
            return (int) tiedot_.at( index.row()).suunta();
        }
    }
    return QVariant();
}

void ReititModel::valitseReitti(const QString &reittitunnus)
{
    beginResetModel();
    tiedot_.clear();

    reittiTunnus_ = reittitunnus;

    QSqlQuery kysely(  QString("select liikennepaikka, raide, lahtoaika, pysahtyy, tapahtuma, suunta "
                               "from aikataulu natural join liikennepaikka where reitti=\"%1\" "
                               "order by kmluku, lahtoaika ").arg(reittitunnus));
    while( kysely.next())
    {
        QString liikennepaikka = kysely.value(0).toString();
        int raide = kysely.value(1).toInt();
        QTime lahtoaika = kysely.value(2).toTime();
        int pysahtyy = kysely.value(3).toInt();
        QString tapahtumaKirjain = kysely.value(4).toString();
        RaiteenPaa::Suunta suunta = RaiteenPaa::suuntaKirjaimesta( kysely.value(5).toString());

        tiedot_.append( ReittiSuunnitteluTieto(liikennepaikka, raide, lahtoaika, pysahtyy, tapahtumaKirjain, suunta)  );
    }


    endResetModel();
}

void ReititModel::uusiReitti()
{
    beginResetModel();
    tiedot_.clear();
    reittiTunnus_ = QString();
    endResetModel();
}



int ReititModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : tiedot_.count();
}

int ReititModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 6 ;
}

Qt::ItemFlags ReititModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags liput = QAbstractTableModel::flags(index);
    if( index.isValid())
    {
        if( index.column() != 0)
            liput |= Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
    return liput;
}

bool ReititModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() || role != Qt::EditRole || index.column() < 1 )
        return false;

    switch( index.column())
    {
    case Raide:
        tiedot_[ index.row()].asetaRaide( value.toInt());
        break;
    case Aika:
        tiedot_[ index.row()].asetaLahtoaika( value.toTime());
        break;
    case Pysahtyy:
        tiedot_[ index.row()].asetaPysahdyksenKesto( value.toInt());
        break;
    case Suunta:
        tiedot_[ index.row()].asetaSuunta(value.toInt());
        break;
    case Tapahtuma:
        tiedot_[ index.row()].asetaTapahtumatyyppi(value.toInt());
        if( value.toInt() == ReittiSuunnitteluTieto::Ohittaa)
            tiedot_[ index.row()].asetaPysahdyksenKesto(0); // Ohittava juna ei pysähdy
        break;

    }
    emit dataChanged(index,index);
    emit muokattu(true);
    return true;
}

void ReititModel::lisaaPysahdys(const QString &liikennepaikka)
{
    // Ensin lisätään lähtöpaikka, sitten määräpaikka, ja välissä pysähdykset (näin toimii automatiikka ;)
    if( !tiedot_.count())
    {
        beginInsertRows( QModelIndex(),0,0);
        tiedot_.append( ReittiSuunnitteluTieto( liikennepaikka, 0, QTime(0,0), 0, "L", RaiteenPaa::Pohjoiseen) );
        endInsertRows();
    }
    else if( tiedot_.count() == 1)
    {
        // Lisätään saapuminen
        beginInsertRows( QModelIndex(), 1, 1);
        tiedot_.append( ReittiSuunnitteluTieto(liikennepaikka, 0, QTime(),0,"S", tiedot_.at(0).suunta() ));
        endInsertRows();
    }
    else
    {
        // Muuten lisätään toiseksi viimeiseksi
        beginInsertRows( QModelIndex(), tiedot_.count()-1, tiedot_.count()-1);
        tiedot_.insert( tiedot_.count()-1, ReittiSuunnitteluTieto(liikennepaikka, 40, QTime(), 0, "P", tiedot_.at( tiedot_.count()-1 ).suunta()) );
        endInsertRows();
    }
    emit muokattu(true);

}

void ReititModel::poistaRivi(int rivi)
{
    beginRemoveRows(QModelIndex(), rivi, rivi);
    tiedot_.removeAt(rivi);
    endRemoveRows();
    emit muokattu(true);
}

void ReititModel::peruMuokkaukset()
{
    if( !reittiTunnus().isEmpty())
    {
        valitseReitti( reittiTunnus_);  // Valitsee nykyisen reitin!
        emit muokattu(false);
    }
    else
        emit uusiHylatty(); // Tämä poistetaan!
}

void ReititModel::reitistaKopio()
{
    reittiTunnus_ = QString();  // Reitin kopio - tiedot jätetään, nimi muutetaan!
}

void ReititModel::tallenna(const QString &uusitunnus)
{
    // Poistetaan vanhat..
    if( !reittiTunnus().isEmpty())
        QSqlQuery poisto( QString("delete from aikataulu where reitti=\"%1\"").arg(reittiTunnus()));

    reittiTunnus_ = uusitunnus;
    foreach( ReittiSuunnitteluTieto tieto, tiedot_)
        tieto.lisaaTietokantaan(reittiTunnus());
    emit muokattu(false);
}


