#!/bin/sh
echo "make binary distribution of cliphist2 for SuSE/Fedora (*.rpm):"
cd ..
tar -cf cliphist2-1.0.0.tar cliphist2-1.0.0/*
gzip cliphist2-1.0.0.tar
cp cliphist2-1.0.0.tar.gz /usr/src/packages/SOURCES
cd cliphist2-1.0.0
#echo "call qmake"
#qmake -unix PREFIX=/usr cliphist2.pro CONFIG+=release
#echo "make application"
#make   # will be done by making deb package in next step
echo "make *.rpm"
rpm -ba cliphist2.spec      
# rpm is created in: /usr/src/packages/RPMS/i586
