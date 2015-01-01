QT       += gui
QT       += widgets
QT       += network

TARGET = kauko

TEMPLATE = app


SOURCES += \ 
    kauko/kaukoraide.cpp \
    kauko/kaukoraiteenpaa.cpp \
    kauko/kaukokisko.cpp \
    kauko/kaukoscene.cpp \
    kauko/main.cpp \
    kauko/kaukoraidevaihde.cpp

HEADERS += ratapiha.h \
    kauko/kaukoraide.h \
    kauko/kaukoraiteenpaa.h \
    kauko/kaukokisko.h \
    kauko/kaukoscene.h \
    kauko/kaukoraidevaihde.h

RESOURCES +=
