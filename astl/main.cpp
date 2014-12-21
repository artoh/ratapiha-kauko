
#include <QApplication>
#include <QLabel>
#include <QTime>
#include <QDebug>

#include "sqlasetinlaite.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);  

    QTime t;
    t.start();

    SqlAsetinlaite asetinlaite;
    asetinlaite.lataaRata();
 
    qDebug("Time elapsed: %d ms", t.elapsed());


    QLabel label("ASETINLAITE ASETINLAITE ASETINLAITE ASETINLAITE");
    label.show();
    
    return a.exec();  
}
