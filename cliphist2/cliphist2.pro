# -------------------------------------------------
# Project created by QtCreator 2009-09-12T14:27:03
# -------------------------------------------------
TARGET = cliphist2
TEMPLATE = app
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets gui
win32:RC_FILE = cliphist2.rc
win32:QMAKE_LFLAGS += -static-libgcc # remove dependency to libgcc_s_dw2-1.dll

# win32:QMAKE_CXX_FLAGS+= -static-libgcc
# CONFIG = qt warn_on debug thread static
#CONFIG = qt release thread static 
SOURCES += main.cpp \
    cliphistwindow.cpp \
    edititem.cpp
HEADERS += cliphistwindow.h \
    edititem.h
FORMS += cliphistwindow.ui \
    edititem.ui
TRANSLATIONS = cliphist2_de.ts \
    cliphist2_nl.ts
unix:!macx { 
    isEmpty( PREFIX ):PREFIX = /usr
    isEmpty( DESKTOPDIR ):DESKTOPDIR = /usr/share/applications
    isEmpty( ICONDIR ):ICONDIR = /usr/share/pixmaps
    DEFINES += PREFIX=\\\"$${PREFIX}\\\"
    target.path = $${PREFIX}/bin
    INSTALLS += target
    language.path = $${PREFIX}/share/cliphist2
    language.files += cliphist2_de.qm \
        cliphist2_nl.qm \
        qt_de.qm
    INSTALLS += language
    desktop.path = $${DESKTOPDIR}
    desktop.files = cliphist2.desktop
    INSTALLS += desktop
    icon.path = $${ICONDIR}
    icon.files = cliphist2_512x512.png
    INSTALLS += icon
}

# images.path += /usr/share/cliphist2
# images.files += pixmaps/cliphist2_64x64.png
# INSTALLS += images
macx { 
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
    
    # #universal ? --> install on power book ?
#    CONFIG += x86 #\
#        ppc
    
    # QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.5u.sdk
    # QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.5
    target.path = /Applications
    INSTALLS = target
    utilities.path = Contents/Resources
    utilities.files += cliphist2_de.qm \
        cliphist2_nl.qm \
        qt_de.qm 
    QMAKE_BUNDLE_DATA += utilities
    INSTALLS += utilities
    ICON = cliphist2.icns
    QMAKE_INFO_PLIST = dist/Info.plist
}
os2 {
    QMAKE_LFLAGS += -static-libgcc
    RC_FILE = cliphist2_OS2.rc
}

RESOURCES += cliphist2.qrc

OTHER_FILES += \
    android/version.xml \
    android/AndroidManifest.xml \
    android/res/drawable/icon.png \
    android/res/drawable/logo.png \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/layout/splash.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/version.xml \
    android/AndroidManifest.xml \
    android/res/drawable/icon.png \
    android/res/drawable/logo.png \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/layout/splash.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/version.xml \
    android/AndroidManifest.xml \
    android/res/drawable/icon.png \
    android/res/drawable/logo.png \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/layout/splash.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/version.xml
