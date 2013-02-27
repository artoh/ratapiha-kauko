#include "junataulumodel.h"

#include <QSqlRecord>
#include <QDateTime>
#include <QSqlQuery>

#include <QDebug>
#include <QBrush>

JunaTauluModel::JunaTauluModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    paivita();
}

int JunaTauluModel::rowCount(const QModelIndex &parent) const
{
    if( parent == QModelIndex())
        return model_.rowCount();
    return 0;
}

int JunaTauluModel::columnCount(const QModelIndex &parent) const
{
    if( parent == QModelIndex())
        return 10;
    return 0;
}

QVariant JunaTauluModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid())
        return QVariant();

    if( role == Qt::TextAlignmentRole)
        return int( Qt::AlignLeft | Qt::AlignVCenter);
    else if( role == Qt::DisplayRole)
    {
        QSqlRecord tietue = model_.record(index.row());

        switch(index.column())
        {
        case JunaNro:
        {
            QString jnro = tietue.value(0).toString();
            if( jnro.startsWith('H') || jnro.startsWith('T') || jnro.startsWith('P') || jnro.startsWith('S'))
                return jnro.mid(1);
            else
                return jnro;
        }
        case JunaTyyppi:
        {
            QString jnro = tietue.value(0).toString();
            if( jnro.startsWith('H'))
                return tr("Taajamajuna");
            else if( jnro.startsWith('T'))
                return tr("Tavarajuna");
            else if( jnro.startsWith('P'))
                return tr("Pikajuna");
            else if( jnro.startsWith('S'))
                return tr("Pendolino");
            return QString();
        }
        case Reitti:
            return tietue.value(1);

        case Lahtoaika:
            return tietue.value(2);

        case Mista:
            return tietue.value(3);

        case MistaRaide:
            return tietue.value(4);

        case Saapumisaika:
        {
            QTime lahtoaika = tietue.value(2).toTime();
            QTime matkaaika = tietue.value(5).toTime();

            QDateTime lahtoDt( QDate(2000,1,1), lahtoaika);
            QDateTime perilla = lahtoDt.addSecs( QTime(0,0).secsTo(matkaaika) );

            return perilla.time();
        }
        case Minne:
            return tietue.value(6);

        case MinneRaide:
            return tietue.value(7);

        case Vaunuja:
            return tietue.value(8);
        }

    }
    else if( role == Qt::ForegroundRole )
    {
        if( index.column() == JunaTyyppi)
        {
            QSqlRecord tietue = model_.record(index.row());
            QString jnro = tietue.value(0).toString();
            if( jnro.startsWith('H'))
                return QBrush(Qt::black);
            else if( jnro.startsWith('T'))
                return QBrush(Qt::blue);
            else if( jnro.startsWith('P'))
                return QBrush(Qt::red);
            else if( jnro.startsWith('S'))
                return QBrush(Qt::green);
            return QBrush(Qt::darkGray);

        }
        else
            return QBrush(Qt::black);

    }

    return QVariant();

}

QVariant JunaTauluModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role == Qt::DisplayRole)
    {
        if( orientation == Qt::Vertical)
            return section; // rivin numero
        else
        {
            switch(section)
            {
            case JunaNro : return tr("Juna n:o");
            case JunaTyyppi: return tr("Laji");
            case Reitti: return tr("Reitti");
            case Lahtoaika: return tr("Lähtöaika");
            case Mista: return tr("Mistä");
            case MistaRaide: return tr("Raiteelta");
            case Saapumisaika: return tr("Saapuu");
            case Minne: return tr("Minne");
            case MinneRaide: return tr("Raiteelle");
            case Vaunuja: return tr("Vaunuja");
            }
        }
    }
    return QVariant();
}

QString JunaTauluModel::junaNumero(int rivi) const
{
    QSqlRecord tietue = model_.record(rivi );
    return tietue.value(0).toString();
}


void JunaTauluModel::paivita()
{
    QString kysymys = "select junanro, juna.reitti, lahtee,  mista.nimi, lahto.raide,  saapuminen.saapumisaika as matkaaika, minne.nimi, saapuminen.raide, vaunuja from juna, aikataulu as lahto, aikataulu as saapuminen, liikennepaikka as mista, liikennepaikka as minne "
            "where juna.reitti=lahto.reitti and juna.reitti=saapuminen.reitti and lahto.tapahtuma='L' and  saapuminen.tapahtuma='S' and mista.liikennepaikka=lahto.liikennepaikka  and minne.liikennepaikka=saapuminen.liikennepaikka ";

   beginResetModel();

    QSqlQuery kysely(kysymys);
    model_.setQuery(kysymys);

//    qDebug() << kysymys << model_.rowCount() << "riviä ";

    endResetModel();

}
