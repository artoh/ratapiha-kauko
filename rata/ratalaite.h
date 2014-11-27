#ifndef RATALAITE_H
#define RATALAITE_H

#include <QtGlobal>

/**
 * @brief Kantaluokka radalla oleville laitteille, jotka yhteydessä asetinlaitteeseen
 *
 * Ratalaitteilla on 20-bittinen laitetunnus. Laitteet ottavat vastaan 8-bittisiä käskyjä,
 * ja lähettävät 8-bittisiä vastaussanomia.
 *
 * Laitteita ovat esim. vaihteet ja opastimet
 */
class Ratalaite
{
public:
    Ratalaite(quint32 laitetunnus);

    /**
     * @brief Ratalaitteen tunnus
     * @return Laitteen 20-bittinen laitetunnus
     */
    quint32 laitetunnus() { return laitetunnus; }

    /**
     * @brief Muodostaa laitetunnuksen
     * @param raidetunnus Raiteen tunnus (16 bittiä)
     * @param laiteosa Laitetyypin tunnus (4 bittiä)
     * @return Laitetunnus (20 bittiä)
     */
    static quint32 muodostaLaitetunnus(int raidetunnus, int laiteosa);

    /**
     * @brief Asetinlaitteen antaman komennon käsittely
     * @param komento 8-bittinen käskysanoma
     */
    virtual void komento(int komento) { ; }

protected:
    /**
     * @brief Lähettää tilaviestin asetinlaitteelle
     * @param viesti Lähetettävä tilaviesti
     */
    void lahetaViesti( quint8 viesti);

private:
    quint32 laitetunnus_;

};

#endif // RATALAITE_H
