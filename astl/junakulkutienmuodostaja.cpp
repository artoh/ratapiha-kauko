#include "junakulkutienmuodostaja.h"
#include "opastin.h"

JunaKulkutienMuodostaja::JunaKulkutienMuodostaja(RaideTieto *mista, RaideTieto *minne)
    : KulkutienMuodostaja(mista, minne)
{

}

JunaKulkutienMuodostaja::~JunaKulkutienMuodostaja()
{

}

bool JunaKulkutienMuodostaja::alkuEhdot(RaiteenPaa *paa)
{
    // Opastimen pitää pystyä näyttämään AJA-opaste
    Opastin *opastin = paa->opastin();

    if( !opastin)
        return false;

    return (opastin->tyyppitieto() & Opastin::VIHREA );
}

bool JunaKulkutienMuodostaja::kulkutieEhdot(RaiteenPaa *paa)
{
    if( paa->kulkutieLajit() == RaiteenPaa::VAINVAIHTO ||
            paa->liitettyPaa()->kulkutieLajit() == RaiteenPaa::VAINVAIHTO)
        return false; // Ei kelpaa junakulkutielle

    return true;
}

bool JunaKulkutienMuodostaja::loppuEhdot(RaiteenPaa *maaliPaa)
{
    // Päätyttävä raidepuskuriin tai opastimeen, joka voi
    // näyttää punaista
    if( !maaliPaa->liitettyPaa() )
        return true;

    Opastin *opastin = maaliPaa->opastin();
    if( opastin && ( opastin->tyyppitieto() & Opastin::PUNAINEN )
            return true;

    return false;
}

