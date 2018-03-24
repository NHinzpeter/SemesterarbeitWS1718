TEMPLATE = app
CONFIG += console c++11
CONFIG += app_bundle
CONFIG += qt
CONFIG += c++11

data.path = $$OUT_PWD/

data.files += $$PWD/*.dat

data.files += $$PWD/*.txt

data.files += $$PWD/*.xml

data.files += $$PWD/*.dtd

INSTALLS += data

SOURCES += main.cpp \
    nattlib3.cpp \
    nelementlib3.cpp \
    ntokenlib3.cpp \
    ntxtlib.cpp \
    clilib.cpp

DISTFILES += \
    daten.xml \
    daten.dtd \
    daten.txt \
    daten.xml \
    daten.dtd

HEADERS += \
    natt3.h \
    nelement3.h \
    ntoken3.h \
    ntxt.h
