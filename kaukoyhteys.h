/**************************************************************************
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#ifndef KAUKOYHTEYS_H
#define KAUKOYHTEYS_H

#include <QObject>

#include <QSqlDatabase>

/** Muodostaa sql-yhteyden tietokantaan, sekä tcp-yhteyden palvelimeen
  */
class KaukoYhteys : public QObject
{
    Q_OBJECT
public:
    explicit KaukoYhteys(QObject *parent = 0);
    ~KaukoYhteys();
    
    /** Onko lukuyhteys tietokantaan, eli voiko näyttää ratapihan tilanteen*/
    bool onkoLukuyhteys();
    /** Onko käsky-yhteys palvelimeen, eli voiko antaa käskyjä asetinlaitteelle*/
    bool onkoKaskyyhteys();

signals:
    
public slots:

protected:
    QSqlDatabase tietokanta_;

    
};

#endif // KAUKOYHTEYS_H
