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

    enum Opaste { Pimea, Seis, Aja, AjaSn, AjaVarovasti, EiOpastetta, Tyhja, SeisLevy, NopeusRajoitus, OdotaLahtolupaa, Lahtolupa, OpastinPuuttuu };
    enum OpastinTyyppi { EiOpastinta, PaaOpastin, SuojastusOpastin, RaideOpastin };
    enum PaanTyyppi { Suora, Vaihde, RaideRisteys, RaidePuskuri } ;

    enum VaihdeKasite { EiVaihdetta, Vasen, Oikea, Aukiajettu } ;
    enum SpKasite { SpPuuttuu = 0, SpSallii, SpSulkee } ;
    enum Suunta { Virhe=0, Etelaan, Pohjoiseen} ;

    enum Automaatio { EiAutomaatiota, AutomaatioKaytossa, AutomaatioViive, AutomaatioAktiivinen, Lapikulku, LapikulkuAktiivinen, LapikulkuViive };

    Opaste opaste() const;  // Näkyvä opastinkäsite
    Opaste opasteKasite() const { return opaste_; } // Asetettu opastinkäsite
    OpastinTyyppi opastin() const { return opastinTyyppi_; }

    PaanTyyppi paanTyyppi() const { return paanTyyppi_; }
    VaihdeKasite vaihde() const { return vaihdeTila_; }
    SpKasite raiteenSulku() const { return raiteenSulku_; }
    bool opastinSeis() const { return opastinSeis_; }   // Onko opastin asetettu näyttämään SEIS

    QString tilaTieto() const;  // Tilatieto tietokantaan

    bool kaannaVaihde();    // Kääntää vaihteen
    bool kaannaVaihde(VaihdeKasite tilaan);
    bool lukitseVaihde(VaihdeKasite tilaan);
    void aukiaja(); /* Vaihde aukiajettuun tilaan!*/
    bool aukiajonKaanto();  /* Käännetään aukiajettu vaihde vasemmalle*/

    bool paaOpastin() const { return opastin() == PaaOpastin; }
    bool raideOpastin() const { return opastin() == RaideOpastin; }
    bool suojastusOpastin() const { return opastin() == SuojastusOpastin; }

    void asetaPaanTyyppi(PaanTyyppi tyyppi);
    void asetaRaiteenSulku(bool onko);
    void asetaOpastin(OpastinTyyppi opastintyyppi);
    void asetaOpaste(Opaste opaste);
    void asetaAutomaationTila(Automaatio automaatio);
    Automaatio automaatioTila() const { return automaatio_; }

    void seis();    // Asettaa opastimen näyttämään SEIS
    void aja();     // Poistaa opastimen SEIS-tilan

    static QChar suuntakirjain(Suunta suunta);
    static Suunta suuntaKirjaimesta(QChar kirjain);
    static Suunta suuntaKirjaimesta(const QString& kirjain);

protected:
    /** Päivittää yhden kohteen*/
    void paivitysTehtava( const QString& lause);

    PaanTyyppi paanTyyppi_;

    VaihdeKasite vaihdeTila_;

    OpastinTyyppi opastinTyyppi_;
    Opaste opaste_;
    Automaatio automaatio_;

    bool opastinSeis_;  // FIR-FR762 Asetettu näyttämään SEIS

    SpKasite raiteenSulku_;
};

#endif // RAITEENPAA_H
