#-------------------------------------------------
#
# Project created by QtCreator 2018-11-26T16:02:58
#
#-------------------------------------------------

QT       += core gui svg    network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client_LinuxSystemUtility
TEMPLATE = app

#RC_FILE = main.rc

RC_ICONS = image/horse_icon.ico

INCLUDEPATH += forms    \
    thirdpart    \
    bussines    \
    thirdpart/asio  \
    thirdpart/fmt   \
    thirdpart/jsoncpp   \
    thirdpart/Simple-WebSocket-Server   \
    thirdpart/openssl/include


#take_mark:this porject doesn't support boost1.7
INCLUDEPATH += $$BOOST_ROOT
LIBS += -L$${BOOST_ROOT}  /usr/local/opt/boost1.69/lib
INCLUDEPATH += /usr/local/opt/boost1.69/include


LIBS +=-L$${PWD}/thirdpart/openssl/libs_linux -lssl -lcrypto # take:add
#LIBS +=-L$${PWD}/htmlReport -lhtml
win32 {
LIBS += -lws2_32 -lmswsock \
    -L$${PWD}/thirdpart/openssl/libs -lssl -lcrypto
}

SOURCES += main.cpp \
    bussines/FileLogCheckHttp.cpp \
    bussines/webserviceclientworker.cpp \
    cequthread.cpp \
    cstateoperation.cpp \
    forms/BluetoothForms.cpp \
    forms/CheckInternetConnectforms.cpp \
    forms/FileSystemeScanForms.cpp \
    forms/InternetHistoryForms.cpp \
    forms/MountFileSystemForms.cpp \
    forms/NetworkAdapterForms.cpp \
    forms/OperationSysteminfoForms.cpp \
    forms/SoftwareCheckOfSystemInstallationForms.cpp \
    forms/SysemAccountForms.cpp \
    forms/SystemModuleSecurityCheck.cpp \
    forms/USBHardWareForms.cpp \
    forms/aboutforms.cpp \
    forms/USBHistoryEventsScanForms.cpp \
    forms/filelogforms.cpp \
    forms/filelogformssetting.cpp \
    forms/iconfont.cpp \
    forms/mainwindows.cpp \
    forms/processanalysisforms.cpp \
    forms/tipmessagebox.cpp \
    thirdpart/fmt/format.cpp \
    thirdpart/jsoncpp/jsoncpp.cpp \
    thirdpart/sconv/sconv.c \
    bussines/utils.cpp \
    forms/html.cpp \
    forms/getcheckfileinfo.cpp


HEADERS += \
    bussines/FileLogCheckHttp.h \
    bussines/webserviceclientworker.h \
    cequthread.h \
    cstateoperation.h \
    forms/BluetoothForms.h \
    forms/CheckInternetConnectforms.h \
    forms/FileSystemeScanForms.h \
    forms/InternetHistoryForms.h \
    forms/NetworkAdapterForms.h \
    forms/OperationSysteminfoForms.h \
    forms/SoftwareCheckOfSystemInstallationForms.h \
    forms/SysemAccountForms.h \
    forms/SystemModuleSecurityCheck.h \
    forms/USBHardWareForms.h \
    forms/aboutforms.h \
    forms/USBHistoryEventsScanForms.h \
    forms/filelogforms.h \
    forms/filelogformssetting.h \
    forms/iconfont.h \
    forms/mainwindows.h \
    forms/processanalysisforms.h \
    forms/tipmessagebox.h \
    public.h \
    thirdpart/fmt/format.h \
    thirdpart/jsoncpp/json/json-forwards.h \
    thirdpart/jsoncpp/json/json.h \
    thirdpart/sconv/codec.hpp \
    thirdpart/sconv/gbkuni30.h \
    thirdpart/sconv/sconv.h \
    bussines/utils.h \
    forms/MountFileSystemForms.h \
    forms/html.h \
    forms/getcheckfileinfo.h

FORMS += \
    forms/BluetoothForms.ui \
    forms/CheckInternetConnectforms.ui \
    forms/FileSystemeScanForms.ui \
    forms/InternetHistoryForms.ui \
    forms/MountFileSystemForms.ui \
    forms/NetworkAdapterForms.ui \
    forms/OperationSysteminfoForms.ui \
    forms/SoftwareCheckOfSystemInstallationForms.ui \
    forms/SysemAccountForms.ui \
    forms/SystemModuleSecurityCheck.ui \
    forms/USBHardWareForms.ui \
    forms/USBHistoryEventsScanForms.ui \
    forms/aboutforms.ui \
    forms/USBHistoryEventsScanForms.ui \
    forms/filelogforms.ui \
    forms/filelogformssetting.ui \
    forms/mainwindows.ui \
    forms/processanalysisforms.ui \
    forms/tipmessagebox.ui \
    forms/getcheckfileinfo.ui

RESOURCES += \
    source.qrc
