#!/bin/sh
echo "make binary distribution of cliphist2 for Debian/Ubuntu (*.deb):"
echo "call qmake"
qmake -unix PREFIX=/usr cliphist2.pro CONFIG+=release
#echo "make application"
#make   # will be done by making deb package in next step
echo "make *.deb"
dh_make -s -c gpl -e michael.neuroth@freenet.de -f cliphist2-1.0.0.tar.gz
cp dist/rules debian/rules
cp dist/copyright debian/copyright
cp dist/control debian/control
dpkg-buildpackage -rfakeroot -b
      