#ifndef RATALAITE_H
#define RATALAITE_H

class RataScene;

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
    Ratalaite(int laitetunnus = 0);

    /**
     * @brief Asettaa laitetunnuksen ja rekisteröi laitteen
     * @param laitetunnus Laitteen 20-bittinen laitetunnus
     */
    void asetaLaitetunnus(int laitetunnus);

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
     * @brief Tätä kutsutaan, kun viiveellinen toiminto valmis
     *
     * Esimerkiksi vaihdetta käännettäessä saadaan ajastimelta viesti halutun
     * ajan jälkeen
     */
    virtual void viiveValmis(int viesti);


    /**
     * @brief Lähettää tilaviestin asetinlaitteelle
     * @param viesti Lähetettävä tilaviesti
     */
    void lahetaViesti( int viesti) const;

    /**
     * @brief Pyytää viiveValmis-viestiä halutun viiveen jälkeen
     * @param Viiveen pituus simulaatiosekunteina
     * @param Viesti viiveen kuluttua
     */
    void viiveToiminto(int kesto, int viesti) const;

    /**
     * @brief Yhdistää skeneen laitetoimintoja varten
     * @param skene
     */
    static void asetaSkene(RataScene *skene);
private:
    int laitetunnus_;

    static RataScene *skene__;

};

#endif // RATALAITE_H
