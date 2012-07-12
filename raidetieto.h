/**************************************************************************
** (c) Arto Hyvättinen 2012
** Gnu Public Licence 2 - No Warranty !!!
** 19.2.2011
**
** Ratapiha - kauko
**************************************************************************/

#ifndef RAIDETIETO_H
#define RAIDETIETO_H

#include "raiteenpaa.h"

class RaideTieto
{
public:

    enum Kulkutietyyppi { Junakulkutie, Vaihtokulkutie , EiKulkutieta} ;

    RaideTieto();

    void paivita( int akseleita, int junanumero, const QString& tila,
                  const QString& etelatila, const QString& pohjoistila);

    bool sahkoistetty() const { return sahkoistetty_; }
    bool valvottu() const { return valvottu_; }
    bool onkoVaihde(); /** Onko vaihde */

    int akseleita() const { return akseleita_; }
    int junanumero() const { return junanumero_; }
    Kulkutietyyppi kulkutie() const { return kulkutie_; }

    RaiteenPaa* pohjoinen()  { return &pohjoinen_; }
    RaiteenPaa* etelainen()  { return &etelainen_; }

protected:
    RaiteenPaa pohjoinen_;
    RaiteenPaa etelainen_;

    bool sahkoistetty_;
    bool valvottu_;

    int akseleita_;
    int junanumero_;

    Kulkutietyyppi kulkutie_;
};

#endif // RAIDETIETO_H
