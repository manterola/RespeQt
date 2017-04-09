# -------------------------------------------------
# Project created by QtCreator 2009-11-22T14:13:00
# Last Update: Nov 29, 2014
# -------------------------------------------------
#
# Copyright 2015,2016 Joseph Zatarski
#
# This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
# However, the years for these copyrights are unfortunately unknown. If you
# know the specific year(s) please let the current maintainer know.
#
#CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += VERSION=\\\"R4-RC2\\\"
TARGET = RespeQt
TEMPLATE = app
CONFIG += qt
QT += core gui network widgets printsupport serialport
CONFIG += mobility
MOBILITY = bearer
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtZlib
SOURCES += main.cpp \
    mainwindow.cpp \
    sioworker.cpp \
    optionsdialog.cpp \
    aboutdialog.cpp \
    diskimage.cpp \
    diskimagepro.cpp \
    folderimage.cpp \
    miscdevices.cpp \
    createimagedialog.cpp \
    diskeditdialog.cpp \
    autoboot.cpp \
    autobootdialog.cpp \
    atarifilesystem.cpp \
    miscutils.cpp \
    textprinterwindow.cpp \
    cassettedialog.cpp \
    docdisplaywindow.cpp \
    bootoptionsdialog.cpp \
    network.cpp \
    logdisplaydialog.cpp \
    respeqtsettings.cpp \
    pclink.cpp \
    printers/baseprinter.cpp \
    printers/textprinter.cpp \
    printers/atari1027.cpp \
    atascii.cpp \
    drivewidget.cpp \
    infowidget.cpp \
    serialport-Qt.cpp \
    printerwidget.cpp \
    atasciiinternational.cpp \
    printers/atariprinter.cpp \
    printers/nativeprintersupport.cpp \
    printers/atari1020.cpp \
    printers/necp6.cpp \
    printers/epsonfx80.cpp
win32:LIBS += -lwinmm -lz
unix:LIBS += -lz
win32:SOURCES += serialport-win32.cpp
unix:SOURCES += serialport-unix.cpp
HEADERS += mainwindow.h \
    serialport.h \
    sioworker.h \
    optionsdialog.h \
    aboutdialog.h \
    diskimage.h \
    diskimagepro.h \
    folderimage.h \
    miscdevices.h \
    createimagedialog.h \
    diskeditdialog.h \
    autoboot.h \
    autobootdialog.h \
    atarifilesystem.h \
    miscutils.h \
    textprinterwindow.h \
    cassettedialog.h \
    docdisplaywindow.h \
    bootoptionsdialog.h \
    network.h \
    logdisplaydialog.h \
    respeqtsettings.h \
    pclink.h \
    printers/baseprinter.h \
    printers/textprinter.h \
    printers/atari1027.h \
    atascii.h \
    drivewidget.h \
    infowidget.h \
    serialport-Qt.h \
    printerwidget.h \
    atasciiinternational.h \
    printers/atariprinter.h \
    printers/nativeprintersupport.h \
    printers/atari1020.h \
    printers/necp6.h \
    printers/epsonfx80.h
win32:HEADERS += serialport-win32.h
unix:HEADERS += serialport-unix.h
FORMS += mainwindow.ui \
    optionsdialog.ui \
    aboutdialog.ui \
    createimagedialog.ui \
    diskeditdialog.ui \
    autobootdialog.ui \
    textprinterwindow.ui \
    cassettedialog.ui \
    docdisplaywindow.ui \
    bootoptionsdialog.ui \
    logdisplaydialog.ui \
    drivewidget.ui \
    infowidget.ui \
    printerwidget.ui
RESOURCES += icons.qrc \
    atarifiles.qrc \
    i18n.qrc \
    documentation.qrc \
    fonts.qrc
OTHER_FILES += \
    license.txt \
    history.txt \
    atascii_read_me.txt \
    RespeQt.rc \
    about.html \
    compile.txt \
TRANSLATIONS = \
    i18n/respeqt_de.ts \
    i18n/respeqt_es.ts \
    i18n/qt_pl.ts \
    i18n/qt_tr.ts \
    i18n/qt_ru.ts \
    i18n/qt_sk.ts \
    i18n/qt_de.ts \
    i18n/qt_es.ts \
    i18n/respeqt_de.ts \
    i18n/respeqt_es.ts \
    i18n/respeqt_pl.ts \
    i18n/respeqt_ru.ts \
    i18n/respeqt_sk.ts \
i18n/respeqt_tr.ts

RC_FILE = RespeQt.rc
