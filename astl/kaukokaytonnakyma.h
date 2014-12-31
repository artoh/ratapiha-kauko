#ifndef KAUKOKAYTONNAKYMA_H
#define KAUKOKAYTONNAKYMA_H

#include <QString>
#include <QStringList>

#include <QByteArray>

#include <QList>

#include "raidetieto.h"

/**
 * @brief Kaukokäytössä katsottava ohjausnäkymä radasta
 */
class KaukokaytonNakyma
{
public:  

    KaukokaytonNakyma(const QString& nimi);

    KaukokaytonNakyma(int nakymaId, const QString& nimi, const QString& tekstit);

    ~KaukokaytonNakyma();

    int nakymaId() const { return nakymaId_; }
    QString nimi() const { return nakymaNimi_; }
    QString teksti() const { return nakymaTeksti_; }

    void lisaaRaide(RaideTieto *raide);

    QList<RaideTieto*> raiteet() const { return raiteet_; }

protected:

    int nakymaId_;
    QString nakymaNimi_;
    QString nakymaTeksti_;

    QList<RaideTieto*> raiteet_;

};

#endif // KAUKOKAYTONNAKYMA_H
