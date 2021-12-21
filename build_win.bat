rem call c:\users\micha\setqtmsc.bat
rem set VERSION=%1
cd src
qmake -r cliphist2.pro
nmake clean
nmake release
#nmake release CFLAGS=-DVERSION=%1
cd release
mkdir ..\..\dist\packages\de.mneuroth.cliphist2\data
copy cliphist2.exe ..\..\dist\packages\de.mneuroth.cliphist2\data
copy cliphist2_256x256.ico ..\..\dist\packages\de.mneuroth.cliphist2\data
copy cliphist2_512x512.ico ..\..\dist\packages\de.mneuroth.cliphist2\data
copy cliphist2_512x512.png ..\..\dist\packages\de.mneuroth.cliphist2\data
cd ..\..\dist\packages\de.mneuroth.cliphist2\data
windeployqt --release --no-system-d3d-compiler cliphist2.exe
cd ..\..\..\..\dist
binarycreator -c config\config.xml -p packages Cliphist2Installer.exe
rem TODO --> version und datum in package.xml anpassen
rem TODO --> version an build und artifakte uebergeben
rem TODO --> make source package...
rem TODO --> copy artifacts to \\mimanas\...
