#!/bin/sh
echo "make binary distribution of cliphist2 for SuSE/Fedora (*.rpm):"
echo "create source archive"
./make_src_tar.sh
cp cliphist2-1.0.0.tar.gz /usr/src/packages/SOURCES
#echo "call qmake"
#qmake -unix PREFIX=/usr cliphist2.pro CONFIG+=release
#echo "make application"
#make   # will be done by making deb package in next step
echo "make *.rpm"
rpm -ba dist/cliphist2.spec      
# rpm is created in: /usr/src/packages/RPMS/i586
