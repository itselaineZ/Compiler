QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/PARSER/ITEM.cpp \
    src/PARSER/ITEMSET.cpp \
    src/PARSER/LEXICAL.cpp \
    src/PARSER/PARSER.cpp \
    src/PARSER/REDUCT.cpp \
    src/PARSER/STACKSTORE.cpp \
    src/SEMANTICS/ATTRIBUTE.cpp \
    src/SEMANTICS/NAMEITEM.cpp \
    src/SEMANTICS/PROCITEM.cpp \
    src/SEMANTICS/QUATERNION.cpp \
    src/SEMANTICS/SEMANTICS.cpp \
    src/SEMANTICS/SYMBOL.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/outcpp.cpp

HEADERS += \
    include/PARSER/ITEM.h \
    include/PARSER/ITEMSET.h \
    include/PARSER/LEXICAL.h \
    include/PARSER/PARSER.h \
    include/PARSER/REDUCT.h \
    include/PARSER/STACKSTORE.h \
    include/PARSER/TERM.h \
    include/SEMANTICS/ARRAYITEM.h \
    include/SEMANTICS/ATTRIBUTE.h \
    include/SEMANTICS/LAYERITEM.h \
    include/SEMANTICS/NAMEITEM.h \
    include/SEMANTICS/PROCITEM.h \
    include/SEMANTICS/QUATERNION.h \
    include/SEMANTICS/SEMANTICS.h \
    include/SEMANTICS/SYMBOL.h \
    include/base.h \
    include/mainwindow.h \
    include/ourclass.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    gram_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
