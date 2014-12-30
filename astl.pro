 
QT	+= gui
QT      += widgets
QT	+= network
QT	+= sql

TARGET = astl

TEMPLATE = app

SOURCES += astl/main.cpp \
    astl/raidetieto.cpp \
    astl/raiteenpaa.cpp \
    astl/opastin.cpp \
    astl/suoranraiteenpaa.cpp \
    astl/raide.cpp \
    astl/vaihde.cpp \
    astl/risteysvaihde.cpp \
    astl/raideristeys.cpp \
    astl/asetinlaite.cpp \
    astl/sqlasetinlaite.cpp \
    astl/asetinlaitepaneeli.cpp \
    astl/ratayhteys.cpp \
    astl/kaskytulkki.cpp \
    astl/kantaristeys.cpp \
    astl/kulkutie.cpp \
    astl/junakulkutie.cpp \
    astl/kaannettavanelementintila.cpp \
    astl/kaukoyhteys.cpp

HEADERS += \
    astl/raidetieto.h \
    astl/raiteenpaa.h \
    astl/opastin.h \
    astl/suoranraiteenpaa.h \
    astl/raide.h \
    astl/vaihde.h \
    astl/risteysvaihde.h \
    astl/raideristeys.h \
    astl/asetinlaite.h \
    astl/sqlasetinlaite.h \
    astl/asetinlaitepaneeli.h \
    astl/ratayhteys.h \
    astl/kaskytulkki.h \
    astl/kantaristeys.h \
    astl/kulkutie.h \
    astl/junakulkutie.h \
    astl/kaannettavanelementintila.h \
    ratapiha.h \
    astl/kaukoyhteys.h

FORMS += \
    astl/asetinlaitepaneeli.ui

