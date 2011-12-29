/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#ifndef KAUKOSCENE_H
#define KAUKOSCENE_H

#include "kiskoscene.h"
#include <QMap>

class KaukoKisko;

class KaukoScene : public KiskoScene
{
    Q_OBJECT
public:
    explicit KaukoScene(QObject *parent = 0);
    
    /** Hakee sanotun näkymän esiin */
    bool haeNakyma(int nakyma);

    /** Onko tiedonsiirtovirhettä, jolloin tila piirretään magentalla */
    bool onkoVikatilassa() { return true; }

    int nakyma() { return nakyma_; }

signals:
    
public slots:

private:
    int nakyma_;
    QMap<int,KaukoKisko*> kiskot_;
    
};

#endif // KAUKOSCENE_H