/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 9.1.2012
**************************************************************************/

#ifndef EDITORIRAITEENPAA_H
#define EDITORIRAITEENPAA_H

#include <QString>


class EditoriRaiteenPaa
{
public:
    EditoriRaiteenPaa(const QString& tilatieto = QString());

    enum PaanTyyppi { Suora, Vaihde, RaideRisteys, RaidePuskuri } ;

    /** Tietokantaan talletettava tilatieto */
    QString tilaTieto() const;

    PaanTyyppi paanTyyppi() const { return paanTyyppi_; }
    bool paaOpastin() const { return paaOpastin_; }
    bool raideOpastin() const { return raideOpastin_; }
    bool suojastusOpastin() const { return suojastusOpastin_; }

    void asetaPaanTyyppi(PaanTyyppi tyyppi);
    void asetaPaaOpastin(bool onko);
    void asetaRaideOpastin(bool onko);
    void asetaSuojastusOpastin(bool onko);

protected:
    PaanTyyppi paanTyyppi_;
    bool paaOpastin_;
    bool raideOpastin_;
    bool suojastusOpastin_;


};

#endif // EDITORIRAITEENPAA_H
