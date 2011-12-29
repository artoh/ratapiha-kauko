/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#ifndef KISKOSCENE_H
#define KISKOSCENE_H

#include <QGraphicsScene>

class KiskoScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit KiskoScene(QObject *parent = 0);

    /** Palauttaa skenen kuvaaman näkymän id:n */
    virtual int nakyma() = 0;
    
signals:
    
public slots:
    
};

#endif // KISKOSCENE_H
