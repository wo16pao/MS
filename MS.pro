#-------------------------------------------------
#
# Project created by QtCreator 2020-02-28T13:08:25
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        widget.cpp \
    admin.cpp \
    aunt.cpp \
    admin/admin_addinfo.cpp \
    admin/admin_adddormitory.cpp \
    admin/admin_adddean.cpp \
    admin/admin_addarchive.cpp

HEADERS += \
        widget.h \
    admin.h \
    aunt.h \
    admin/admin_addinfo.h \
    admin/admin_adddormitory.h \
    admin/admin_adddean.h \
    admin/admin_addarchive.h

FORMS += \
        widget.ui \
    admin.ui \
    aunt.ui \
    admin/admin_addinfo.ui \
    admin/admin_adddormitory.ui \
    admin/admin_adddean.ui \
    admin/admin_addarchive.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
