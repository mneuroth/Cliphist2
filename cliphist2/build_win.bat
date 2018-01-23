rem call c:\users\micha\setqtmsc.bat
qmake -r cliphist2.pro
nmake clean
nmake release
cd release
windeployqt --release --no-system-d3d-compiler cliphist2.exe