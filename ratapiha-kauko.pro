
QT  += core qui sql

TARGET = ratapiha-kauko
TEMPLATE = app

HEADERS += \
    kaukoikkuna.h \
    kaukoyhteys.h \
    kaukokisko.h \
    kaukoscene.h \
    editorikisko.h \
    kisko.h \
    kiskoscene.h \
    editoriscene.h \
    editoriikkuna.h \
    editoriview.h \
    ruudukko.h \
    mittanauha.h

SOURCES += \
    kaukoikkuna.cpp \
    main.cpp \
    kaukoyhteys.cpp \
    kaukokisko.cpp \
    kaukoscene.cpp \
    editorikisko.cpp \
    kisko.cpp \
    kiskoscene.cpp \
    editoriscene.cpp \
    editoriikkuna.cpp \
    editoriview.cpp \
    ruudukko.cpp \
    mittanauha.cpp

RESOURCES += \
    ratapiha-kauko.qrc

OTHER_FILES += \
    pic/viivain.png \
    pic/ikkuna-uusi.png \
    pic/valintanuoli.png \
    pic/teksti.png \
    pic/pyyhekumi.png \
    pic/muokkaa.png \
    pic/kyna.png \
    pic/laituri-oikea.png \
    pic/laituri-vasen.png \
    pic/nayta-junanumerot.png \
    pic/nayta-raidenumerot.png


