rem call c:\users\micha\setqtmsc.bat
qmake -r cliphist2.pro
nmake clean
nmake release
cd release
mkdir deploy
copy cliphist2.exe deploy
cd deploy
windeployqt --release --no-system-d3d-compiler cliphist2.exe
