#ifndef KAUKOKAYTONNAKYMA_H
#define KAUKOKAYTONNAKYMA_H

#include <QString>
#include <QStringList>
#include <QList>

#include "raidetieto.h"

/**
 * @brief Kaukokäytössä katsottava ohjausnäkymä radasta
 */
class KaukokaytonNakyma
{
public:  

    KaukokaytonNakyma(const QString& nimi);
    ~KaukokaytonNakyma();

    void lisaaTeksti(int x, int y, const QString& teksti);
    void lisaaKisko(RaideTieto *raide, int etelaX, int etelaY, int pohjoinenX, int pohjoinenY, int kiskotietokoodi);

    QString nimi() const { return nakymaNimi_; }
    QStringList teksti() const { return nakymaTeksti_; }
    QList<RaideTieto*> raiteet() const { return raiteet_; }

protected:
    enum NakymaKiskoBitit
    {
        EPAA = 0x1,
        EMIINUS = 0x2,
        EPLUS = 0x4,
        PPAA = 0x10,
        PMIINUS = 0x20,
        PPLUS = 0x40,
        LAITURI_VASEN = 0x100,
        LAITURI_OIKEA = 0x200,
        NAYTA_RAIDETUNNUS = 0x400,
        NAYTA_JUNATUNNUS = 0x800
    };

    QString nakymaNimi_;
    QStringList nakymaTeksti_;
    QList<RaideTieto*> raiteet_;

};

#endif // KAUKOKAYTONNAKYMA_H
