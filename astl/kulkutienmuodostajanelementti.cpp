#include "kulkutienmuodostajanelementti.h"
#include "kulkutienmuodostaja.h"
#include "raiteenpaa.h"
#include "raidetieto.h"

KulkutienMuodostajanElementti::KulkutienMuodostajanElementti(RaiteenPaa *raiteenPaa, KulkutienMuodostajanElementti *vanhempi, KulkutienMuodostaja *muodostaja)
    : raiteenPaa_(raiteenPaa),
      vanhempi_(vanhempi),
      muodostaja_(muodostaja)
{
    // Päivitetään tiedot
    if( vanhempi )
    {
        taso_ = vanhempi->taso() + 1;
        pituus_ = vanhempi->pituus() + raiteenPaa->raide()->pituus();
        toissijainen_ = vanhempi->toissijainen();
    }
    else
    {
        taso_ = 0;
        pituus_ = raiteenPaa->raide()->pituus();
        toissijainen_ = false
    }

    // Selvitetään, onko tämä reitti "toissijainen"
    if( !toissijainen() && (raiteenPaa->kulkutieLajit() == RaiteenPaa::TOISSIJAINEN ||
            raiteenPaa->liitettyPaa()->kulkutieLajit() == RaiteenPaa::TOISSIJAINEN))
        toissijainen_ = true;


    // Jos tämä ei ole lyhin reitti, jätetään sikseen
    if( muodostaja->ollaankoLyhimmalla(this))
    {
        // Tutkitaan kulkutielajin ehdot
        if( muodostaja->kulkutieEhdot(raiteenPaa))
        {

            // Sitten selvitetään, ollaanko jo maalissa
            if( raiteenPaa->raide() == muodostaja->minne())
            {
                muodostaja->ollaanMaalissa(this);
            }
            else if( taso() < 20 )
            {
                // Muuten jatketaan eteenpäin, jos saa jatkaa
                QPair<RaiteenPaa*, RaiteenPaa*> vastaPaat = raiteenPaa->raide()->mahdollisetVastapaat( muodostaja->tyyppi());
                if( vastaPaat.first)
                {
                    RaiteenPaa *seuraava = vastaPaat.first->liitettyPaa();
                    if( seuraava )
                        new KulkutienMuodostajanElementti(seuraava, this, muodostaja);
                }
                if( vastaPaat.second)
                {
                    RaiteenPaa *seuraava = vastaPaat.second->liitettyPaa();
                    if( seuraava )
                        new KulkutienMuodostajanElementti(seuraava, this, muodostaja);
                }
            } // Taso pienempi kuin 20

        } // Kulkutie-ehdot hyväksyttä

    } // Ollaanko lyhimmällä



}

KulkutienMuodostajanElementti::~KulkutienMuodostajanElementti()
{

}

