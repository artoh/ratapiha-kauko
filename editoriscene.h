/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#ifndef EDITORISCENE_H
#define EDITORISCENE_H

#include "kiskoscene.h"

#include <QMap>

class EditoriRaide;


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
    bool naytetaankoNopeusRajoitus() const { return naytaNopeusRajoitus_; }

    EditoriRaide* haeRaide(const QString& liikennepaikka, int raide);


signals:
    
public slots:
    void naytaNopeusRajoitus(bool nayta);

private:
    // Hakee radalle liikennepaikkojen sijainnit
    void haeLiikennePaikat();

    int nakyma_;
    bool naytaNopeusRajoitus_;
    
    QMap<QString,EditoriRaide*> raiteet_;
};

#endif // EDITORISCENE_H
