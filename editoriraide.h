/**************************************************************************.
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
**  9.1.2012
**************************************************************************/

#ifndef EDITORIRAIDE_H
#define EDITORIRAIDE_H

#include "raiteenpaa.h"

#include <QString>

/**
 Editorin käyttämä raidetietue.

*/
class EditoriRaide
{
public:
   EditoriRaide(const QString& liikennepaikka, int raide, int raideid,
                const QString& raidetila=QString(), const QString& etelatila=QString(), const QString& pohjoistila=QString());
   ~EditoriRaide();

   RaiteenPaa* pohjoinen() { return pohjoinen_; }
   RaiteenPaa* etelainen() { return etelainen_; }

   bool sahkoistetty() const { return sahkoistetty_; }
   bool valvottu() const { return valvottu_; }

   void asetaMaareet(bool sahkoistetty=true, bool valvottu=true);
   void talleta();

protected:
    QString liikennepaikka_;
    int raide_;
    int raideid_;

    RaiteenPaa* pohjoinen_;
    RaiteenPaa* etelainen_;

    bool sahkoistetty_;
    bool valvottu_;

};

#endif // EDITORIRAIDE_H
