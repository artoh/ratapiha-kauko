/**************************************************************************.
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
**  9.1.2012
**************************************************************************/

#ifndef EDITORIRAIDE_H
#define EDITORIRAIDE_H

#include "editoriraiteenpaa.h"

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

   EditoriRaiteenPaa* pohjoinen() { return pohjoinen_; }
   EditoriRaiteenPaa* etelainen() { return etelainen_; }

   void talleta();

protected:
    QString liikennepaikka_;
    int raide_;
    int raideid_;

    EditoriRaiteenPaa* pohjoinen_;
    EditoriRaiteenPaa* etelainen_;

};

#endif // EDITORIRAIDE_H
