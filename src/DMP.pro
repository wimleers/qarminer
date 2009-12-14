# -------------------------------------------------
# Project created by QtCreator 2009-12-14T15:27:37
# -------------------------------------------------
QT += testlib
QT -= gui
TARGET = DMP
CONFIG += console
CONFIG -= app_bundle
CONFIG+= x86
TEMPLATE = app
SOURCES += main.cpp \
    arffparser.cpp \
    fpnode.cpp \
    fptree.cpp
HEADERS += arffparser.h \
    fpnode.h \
    fptree.h
