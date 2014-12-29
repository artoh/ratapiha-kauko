#include "kulkutie.h"

#include "raiteenpaa.h"
#include "opastin.h"

Kulkutie::Kulkutie(RaideTieto *mista, RaideTieto *minne)
    : mista_(mista), minne_(minne),
      lyhimmanKulkutienPituus_(20000),  // Kulkutien enimmäispituus 20 km
      onkoLyhinToissijaisella_(true),
      tila_(EIKULKUTIETA)
{

}

Kulkutie::~Kulkutie()
{

}

void Kulkutie::etsiKulkutie(Kulkutie::Suunta suunnasta)
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

    // Jos löytyi, vaihdetaan tilaa
    if( loytyikoKulkutie())
        tila_ = PERUSEHDOT;
}

void Kulkutie::lukitseKulkutielle()
{
    if( tila() != PERUSEHDOT )
        return;     // Pitää olla kulkutie valmiina

    for( int i = 0; i < valmisKulkutie_.count()-1; i++)
    {
        // Lukitaan kaikki paitsi viimeinen kulkutielle
        valmisKulkutie_.at(i)->raide()->lukitseKulkutielle(this, valmisKulkutie_.at(i), valmisKulkutie_.at(i+1)->liitettyPaa() );
    }
    // Lukitaan vielä viimeinenkin kulkutielle
    valmisKulkutie_.last()->raide()->lukitseKulkutielle(this, valmisKulkutie_.last(), valmisKulkutie_.last()->aktiivinenVastapaa());

    // Nyt pitäisi laittaa tämä kulkutie valvontaan odottamaan, että kaikki vaihteet
    // kääntyvät kulkutie-ehtojen mukaisiksi, jotta voidaan laittaa värejä opastimiin

    tila_ = LUKITAAN;

}

void Kulkutie::valvoKulkutie()
{

    bool kaikkiLukittu = true;
    foreach (RaiteenPaa* paa, valmisKulkutie_)
    {
        if( paa->raide()->onkoLukittuKulkutielle() != Ratapiha::ELEMENTTI_LUKITTU)
            kaikkiLukittu = false;
    }

    if( tila() == LUKITAAN && kaikkiLukittu)
    {
        // Nyt kulkutie on viimein valmis
        tila_ = VALMIS;
        laitaVarit();
    }

}

void Kulkutie::kulkutienEtsija(RaiteenPaa *paa, int taso, int pituus, bool toissijainen)
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
                QPair<RaiteenPaa*, RaiteenPaa*> vastaPaat = paa->raide()->mahdollisetVastapaat( paa, tyyppi());
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

QString Kulkutie::raiteet()
{
    QString lista;
    foreach (RaiteenPaa* paa, valmisKulkutie_)
    {
        lista.append(paa->raide()->raideTunnusTeksti() + " ");
    }
    return lista;
}


