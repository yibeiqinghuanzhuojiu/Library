QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += debug
QT +=sql


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addbookdialog.cpp \
    addmuchbooksdialog.cpp \
    changeuserdetaildialog.cpp \
    common.cpp \
    connectDatabase.cpp \
    logindialog.cpp \
    main.cpp \
    library.cpp \
    registerdialog.cpp \
    userdetaildialog.cpp

HEADERS += \
    addbookdialog.h \
    addmuchbooksdialog.h \
    bookDef.h \
    changeuserdetaildialog.h \
    common.h \
    connectDatabase.h \
    library.h \
    logindialog.h \
    registerdialog.h \
    userDef.h \
    userdetaildialog.h

FORMS += \
    addbookdialog.ui \
    addmuchbooksdialog.ui \
    changeuserdetaildialog.ui \
    library.ui \
    logindialog.ui \
    registerdialog.ui \
    userdetaildialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
