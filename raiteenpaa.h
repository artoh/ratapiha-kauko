/**************************************************************************
** (c) Arto Hyvättinen 2012
** Gnu Public Licence 2 - No Warranty !!!
** 19.2.2011
**
** Ratapiha - kauko
**************************************************************************/

#ifndef RAITEENPAA_H
#define RAITEENPAA_H

#include <QString>

class RaiteenPaa
{
public:
    RaiteenPaa(const QString& tila = QString());
    void paivitaTila(const QString& tila);

    enum Opaste { Pimea, Seis, Aja, AjaSn, AjaVarovasti, EiOpastetta, Tyhja };
    enum OpastinTyyppi { EiOpastinta, PaaOpastin, SuojastusOpastin, RaideOpastin };
    enum PaanTyyppi { Suora, Vaihde, RaideRisteys, RaidePuskuri } ;

    enum VaihdeKasite { EiVaihdetta, Vasen, Oikea } ;
    enum SpKasite { SpPuuttuu = 0, SpSallii, SpSulkee } ;

    Opaste opaste() const { return opaste_; }
    OpastinTyyppi opastin() const { return opastinTyyppi_; }

    PaanTyyppi paanTyyppi() const { return paanTyyppi_; }
    VaihdeKasite vaihde() const { return vaihdeTila_; }
    SpKasite raiteenSulku() const { return raiteenSulku_; }

    QString tilaTieto() const;  // Tilatieto tietokantaan

    bool kaannaVaihde();    // Kääntää vaihteen

    bool paaOpastin() const { return opastin() == PaaOpastin; }
    bool raideOpastin() const { return opastin() == RaideOpastin; }
    bool suojastusOpastin() const { return opastin() == SuojastusOpastin; }

    void asetaPaanTyyppi(PaanTyyppi tyyppi);
    void asetaRaiteenSulku(bool onko);
    void asetaOpastin(OpastinTyyppi opastintyyppi);


protected:
    /** Päivittää yhden kohteen*/
    void paivitysTehtava( const QString& lause);

    PaanTyyppi paanTyyppi_;

    VaihdeKasite vaihdeTila_;

    OpastinTyyppi opastinTyyppi_;
    Opaste opaste_;

    SpKasite raiteenSulku_;
};

#endif // RAITEENPAA_H
