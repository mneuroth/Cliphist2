#!/bin/sh
echo "make distribution of cliphist2 for Mac OS X (*.app and *.dmg):"
cp /Developer/Applications/Qt/translations/qt_de.qm .
echo "call qmake"
qmake -macx -spec macx-g++ cliphist2.pro CONFIG+=release CONFIG-=debug
echo "make app"
make
echo "make dmg"
# rm cliphis2.dmg
macdeployqt cliphist2.app -dmg
echo "clean up"
#rm qt_de.qm
