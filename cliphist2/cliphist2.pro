# -------------------------------------------------
# Project created by QtCreator 2009-09-12T14:27:03
# -------------------------------------------------
TARGET = cliphist2
TEMPLATE = app
ICON = cliphist2.icns
win32:RC_FILE = cliphist2.rc

# CONFIG = qt warn_on debug thread static
SOURCES += main.cpp \
    cliphistwindow.cpp \
    edititem.cpp
HEADERS += cliphistwindow.h \
    edititem.h
FORMS += cliphistwindow.ui \
    edititem.ui

TRANSLATIONS = cliphist_de.ts \
               cliphist_nl.ts
