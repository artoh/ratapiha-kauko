#ifndef RATALAITE_H
#define RATALAITE_H


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
    Ratalaite(int laitetunnus);

    /**
     * @brief Ratalaitteen tunnus
     * @return Laitteen 20-bittinen laitetunnus
     */
    int laitetunnus() const { return laitetunnus_; }

    /**
     * @brief Muodostaa laitetunnuksen
     * @param raidetunnus Raiteen tunnus (16 bittiä)
     * @param laiteosa Laitetyypin tunnus (4 bittiä)
     * @return Laitetunnus (20 bittiä)
     */
    static int muodostaLaitetunnus(int raidetunnus, int laiteosa);

    /**
     * @brief Asetinlaitteen antaman komennon käsittely
     * @param komento 8-bittinen käskysanoma
     */
    virtual void komento(int komento);

    /**
     * @brief Lähettää tilaviestin asetinlaitteelle
     * @param viesti Lähetettävä tilaviesti
     */
    void lahetaViesti( int viesti);

private:
    int laitetunnus_;

};

#endif // RATALAITE_H
