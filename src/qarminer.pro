# -------------------------------------------------
# Project created by QtCreator 2009-12-14T15:27:37
# -------------------------------------------------
QT += testlib
QT -= gui
CONFIG += console
CONFIG -= app_bundle
CONFIG += x86
TEMPLATE = app
SOURCES += main.cpp \
    arffparser.cpp \
    fpnode.cpp \
    fptree.cpp \
    fpgrowth.cpp \
    typedefs.cpp \
    ruleminer.cpp \
    intervaltransformer.cpp
HEADERS += arffparser.h \
    fpnode.h \
    fptree.h \
    fpgrowth.h \
    typedefs.h \
    ruleminer.h \
    intervaltransformer.h

# Disable qDebug() output when in release mode.
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

# Add a DEBUG define when in debug mode.
CONFIG(debug, debug|release):DEFINES += DEBUG

# Enable compiler optimizations when building in release mode.
QMAKE_CXXFLAGS_RELEASE = -O3 \
    -funroll-loops \
    -fstrict-aliasing
CONFIG += debug_and_release
CONFIG(debug, debug|release):TARGET = qarminer_debug
else:TARGET = qarminer
