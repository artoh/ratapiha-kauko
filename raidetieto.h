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

    enum Kulkutietyyppi { EiKulkutieta, Junakulkutie, Vaihtokulkutie, Linjasuojastus, Varattukulkutie } ;
    enum KulkutieTila { Valmis, Varattu, Virhetila } ;

    RaideTieto();

    void paivita( int akseleita, const QString& junanumero, const QString& tila,
                  const QString& etelatila, const QString& pohjoistila,
                  const QString& kulkutietila);

    bool sahkoistetty() const { return sahkoistetty_; }
    bool valvottu() const { return valvottu_; }
    bool onkoVaihde(); /** Onko vaihde */

    int akseleita() const { return akseleita_; }
    QString junanumero() const { return junanumero_; }
    Kulkutietyyppi kulkutieTyyppi() const { return kulkutietyyppi_; }
    KulkutieTila kulkutieTila() const { return kulkutietila_; }
    RaiteenPaa::Suunta suunta() const { return suunta_; }

    RaiteenPaa* pohjoinen()  { return &pohjoinen_; }
    RaiteenPaa* etelainen()  { return &etelainen_; }

protected:
    RaiteenPaa pohjoinen_;
    RaiteenPaa etelainen_;

    bool sahkoistetty_;
    bool valvottu_;

    int akseleita_;
    QString junanumero_;

    Kulkutietyyppi kulkutietyyppi_;
    KulkutieTila kulkutietila_;
    RaiteenPaa::Suunta suunta_;
};

#endif // RAIDETIETO_H
