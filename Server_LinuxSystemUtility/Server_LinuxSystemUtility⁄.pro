QT       += core gui svg
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    GetSysInfoData.cpp \
    SpecialTrojanServer.cpp \
    SpecialTrojan.cpp \
    thirdpart/jsoncpp/jsoncpp.cpp
    #InfoStruct.cpp

INCLUDEPATH += \
    thirdpart\
    thirdpart/Simple-WebSocket-Server \
    thirdpart/openssl\
    thirdpart/openssl/include\
    thirdpart/asio

LIBS += \
    -ldl\
     -lpthread\
     -L$${PWD}/thirdpart/openssl/lib -lssl -lcrypto


#HEADERS += \
#    rdisk.hpp \
#    alarm.hpp \
#    ioctrl.hpp \
#    setup.hpp

HEADERS += \
    GetSysInfoData.h \
    GetSysInfoImpl.h \
    PublicData.h \
    SpecialTrojanServer.h \
    SpecialTrojan.h \
    InfoStruct.h \
    stdafx.h
