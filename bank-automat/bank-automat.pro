QT       += core gui

QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bill100widget.cpp \
    bill10widget.cpp \
    bill20widget.cpp \
    bill500widget.cpp \
    bill50widget.cpp \
    bitcoin.cpp \
    main.cpp \
    mainwindow.cpp \
    menu.cpp \
    transactions.cpp \
    transfer.cpp \
    withdraw.cpp

HEADERS += \
    bill100widget.h \
    bill10widget.h \
    bill20widget.h \
    bill500widget.h \
    bill50widget.h \
    bitcoin.h \
    mainwindow.hpp \
    menu.h \
    transactions.h \
    transfer.h \
    withdraw.h

FORMS += \
    bill100widget.ui \
    bill20widget.ui \
    bill500widget.ui \
    bill50widget.ui \
    bitcoin.ui \
    mainwindow.ui \
    menu.ui \
    transactions.ui \
    withdraw.ui \
    bill10widget.ui \
    transfer.ui
    
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pics.qrc
