/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#ifndef EDITORISCENE_H
#define EDITORISCENE_H

#include "kiskoscene.h"


class EditoriScene : public KiskoScene
{
    Q_OBJECT
public:
    explicit EditoriScene(QObject *parent = 0);
    
    /** Hakee sanotun näkymän editoitavaksi */
    bool haeNakyma(int nakyma);

    /** Aloittaa uuden näkymän muokkaamisen*/
    void uusiNakyma(const QString& nimi);

    int nakyma() { return nakyma_; }

signals:
    
public slots:

private:
    // Hakee radalle liikennepaikkojen sijainnit
    void haeLiikennePaikat();

    int nakyma_;
    
};

#endif // EDITORISCENE_H
