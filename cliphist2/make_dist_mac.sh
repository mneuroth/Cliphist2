#!/bin/sh
echo "make distribution of cliphist2 for Mac OS X (*.app and *.dmg):"
#cp /Developer/Applications/Qt/translations/qt_de.qm .
cp /Users/min/Downloads/Programmierung/qt-everywhere-opensource-src-4.7.3/translations/qt_de.qm .
echo "call qmake"
qmake -spec macx-g++ cliphist2.pro CONFIG+=release CONFIG-=debug
echo "make app"
make
cp -r /Users/min/Downloads/Programmierung/qt-everywhere-opensource-src-4.7.3/src/gui/mac/qt_menu.nib cliphist2.app/Contents/Resources
cp cliphist2orig.clp cliphist2.app/Contents/Resources/cliphist2.clp
echo "make dmg"
rm cliphist2.dmg
macdeployqt cliphist2.app -dmg -verbose=3 -no-plugins
echo "clean up"
#rm qt_de.qm