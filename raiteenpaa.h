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
    RaiteenPaa();
    void paivitaTila(const QString& tila);

    enum OpastinKasite { Puuttuu, Seis, Aja, AjaVarovasti } ;
    enum PaanTyyppi { Suora, Vaihde, RaideRisteys, RaidePuskuri } ;
    enum VaihdeKasite { EiVaihdetta, Vasen, Oikea } ;
    enum SpKasite { SpPuuttuu, SpSallii, SpSulkee } ;

    OpastinKasite paaOpastin() const { return paaOpastin_; }
    OpastinKasite raideOpastin() const { return raideOpastin_; }
    OpastinKasite suojastusOpastin() const { return suojastusOpastin_; }

    PaanTyyppi paanTyyppi() const { return paanTyyppi_; }
    VaihdeKasite vaihde() const { return vaihdeTila_; }
    SpKasite raiteenSulku() const { return raiteenSulku_; }

    QString tilaTieto() const;  // Tilatieto tietokantaan

    bool kaannaVaihde();    // Kääntää vaihteen


protected:
    /** Päivittää yhden kohteen*/
    void paivitysTehtava( const QString& lause);

    PaanTyyppi paanTyyppi_;

    VaihdeKasite vaihdeTila_;

    OpastinKasite paaOpastin_;
    OpastinKasite raideOpastin_;
    OpastinKasite suojastusOpastin_;

    SpKasite raiteenSulku_;
};

#endif // RAITEENPAA_H
