#!/bin/sh
echo "make distribution of cliphist2 for Mac OS X (*.app and *.dmg):"
echo "using QT distribution in directory: " $QT_DIR
cp $QT_DIR/translations/qt_de.qm .
echo "call qmake"
qmake -spec macx-g++ cliphist2.pro CONFIG+=release CONFIG-=debug
echo "make app"
make
# workaround for qt_menu.nib (see: https://bugreports.qt.nokia.com//browse/QTBUG-5952)
cp -r $QT_SRC_DIR/src/gui/mac/qt_menu.nib cliphist2.app/Contents/Resources
cp cliphist2.clp cliphist2.app/Contents/Resources/cliphist2.clp
echo "make dmg"
rm cliphist2.dmg
$QT_DIR/bin/macdeployqt cliphist2.app -dmg -verbose=3 -no-plugins
echo "clean up"
#rm qt_de.qm