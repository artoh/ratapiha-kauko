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
    
    int nakyma() { return nakyma_; }

signals:
    
public slots:

private:
    int nakyma_;
    
};

#endif // EDITORISCENE_H
