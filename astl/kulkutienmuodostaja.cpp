#include "kulkutienmuodostaja.h"
#include "kulkutienmuodostajanelementti.h"

#include "raiteenpaa.h"
#include "opastin.h"

KulkutienMuodostaja::KulkutienMuodostaja(RaideTieto *mista, RaideTieto *minne)
    : mista_(mista), minne_(minne), tyyppi_(tyyppi),
      parasKulkutie_(0),
      lyhimmanKulkutienPituus_(20000),  // Kulkutien enimmäispituus 20 km
      onkoLyhinToissijaisella_(true)
{

}

KulkutienMuodostaja::~KulkutienMuodostaja()
{

}

void KulkutienMuodostaja::etsiKulkutie(KulkutienMuodostaja::Suunta suunnasta)
{
    if( suunnasta == EISUUNTAA)
    {
        // Jos haetaan molemmista suunnista, hajotetaan kahdeksi hakupyynnöksi
        etsiKulkutie(POHJOISEEN);
        etsiKulkutie(ETELAAN);
        return;
    }

    // Ensimmäisenä alkuehtona aloittavan ja lopettavan raiteen pitää olla suora raide
    if(     mista()->raideTyyppi() != RaideTieto::RAIDE ||
            minne()->raideTyyppi() != RaideTieto::RAIDE)
        return;

    // Haetaan pää, jossa pitäisi olla kulkutien aloittava opastin
    RaiteenPaa *paa = 0;
    if( suunnasta == ETELAAN)
        paa = mista()->raiteenPaa(RaiteenPaa::E);
    else if( suunnasta == POHJOISEEN)
        paa = mista()->raiteenPaa(RaiteenPaa::P);

    if( !paa || !paa->liitettyPaa())
        return;

    if( !alkuEhdot(paa))
        return;

    // Tehdään etsintä upouudella rekursiivisella funktiolla

    kulkutienEtsija(paa->liitettyPaa(), 0, 0, false);

    // Nyt lähdetään muodostamaan kulkuteitä elementin avulla
    // Lopputuloksena parhaaksi kulkutieksi on tullut kulkutie,
    // joka johtaa nopeiten perille asti
    // new KulkutienMuodostajanElementti(paa, 0, this);
}

void KulkutienMuodostaja::ollaanMaalissa(KulkutienMuodostajanElementti *elementti)
{
    // Ensin tarkistetaan, että tämä kelpaa maaliraiteeksi
    if( loppuEhdot(elementti->raiteenPaa()->aktiivinenVastapaa()))
    {
        // Tämä on paras elementti tähän saakka
        parasKulkutie_ = elementti;
    }

}




bool KulkutienMuodostaja::ollaankoLyhimmalla(KulkutienMuodostajanElementti *elementti)
{
    if( !kulkutie())    // Jos ei vielä yhtään, niin ollaan aina
        return true;
    if( !kulkutie()->toissijainen() && elementti->toissijainen())
        return false;   // Jos olisi toissijainen, ja ensisijainen löydetty jo
    if( elementti->pituus() > kulkutie()->pituus())
        return false;   // Ollaan jo pidemmällä kuin paras

    return true;    // Vaikuttaa lupaavalta, kannattaa jatkaa.
}


void KulkutienMuodostaja::kulkutienEtsija(RaiteenPaa *paa, int taso, int pituus, bool toissijainen)
{
    // Kulkutietä etsivä rekursiivinen funktio, joka korvaa erilliset elementtioliot

    int pituudella = pituus + paa->raide()->pituus();

    bool toissijaisella = ( toissijainen ||  paa->kulkutieLajit() == RaiteenPaa::VAINVAIHTO ||
                           paa->liitettyPaa()->kulkutieLajit() == RaiteenPaa::VAINVAIHTO);

    // Tästä jatketaan eteenpäin vain, jos ollaan edelleen parhaalla kulkutiellä
    if( pituudella < lyhimmanKulkutienPituus_ &&
            ( onkoLyhinToissijaisella_ || !toissijaisella ))
    {
        // Tutkitaan perusehdot
        if( kulkutieEhdot( paa ))
        {
            // Kelpaa kulkutielle, lisätään tutkittavien listaan
            tutkittavaKulkutie_.append(paa);

            // Ollaanko jo maalissa ???
            if( paa->raide() == minne())
            {
                // Maalissa ollaan!
                // Tarkistetaan maaliehdot
                if( loppuEhdot(paa->aktiivinenVastapaa()))
                {
                    // Kopioidaan kulkutieluettelo valmiiksi
                    valmisKulkutie_ = tutkittavaKulkutie_;
                    lyhimmanKulkutienPituus_ = pituudella;
                    onkoLyhinToissijaisella_ = toissijaisella;
                }
            }
            // Ellei, tutkitaan myös eteenpäin, ellei liian iso taso
            else if( taso < 20)
            {
                // Jatketaan eteenpäin, jos saa jatkaa
                QPair<RaiteenPaa*, RaiteenPaa*> vastaPaat = raiteenPaa->raide()->mahdollisetVastapaat( muodostaja->tyyppi());
                if( vastaPaat.first)
                {
                    RaiteenPaa *seuraava = vastaPaat.first->liitettyPaa();
                    if( seuraava )
                        kulkutienEtsija(seuraava, taso + 1, pituudella, toissijaisella );
                }
                if( vastaPaat.second)
                {
                    RaiteenPaa *seuraava = vastaPaat.second->liitettyPaa();
                    if( seuraava )
                        kulkutienEtsija(seuraava, taso + 1, pituudella, toissijaisella );
                }

            }
            tutkittavaKulkutie_.removeLast();   // Otetaan viimeinen elementti pois...
        }
    }

}

QString KulkutienMuodostaja::raiteet()
{
    QString lista;
    foreach (RaiteenPaa* paa, valmisKulkutie_)
    {
        lista.append(paa->raide()->raideTunnusTeksti() + " ");
    }
    return lista;
}


