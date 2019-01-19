rem call c:\users\micha\setqtmsc.bat
cd src
qmake -r cliphist2.pro
mingw32-make clean
mingw32-make release
cd release
mkdir ..\..\dist\packages\de.mneuroth.cliphist2\data
copy cliphist2.exe ..\..\dist\packages\de.mneuroth.cliphist2\data
cd ..\..\dist\packages\de.mneuroth.cliphist2\data
windeployqt --release --no-system-d3d-compiler cliphist2.exe
cd ..\..\..\..\dist
binarycreator -c config\config.xml -p packages Cliphist2Installer.exe
rem TODO --> version und datum in package.xml anpassen
rem TODO --> version an build und artifakte uebergeben
rem TODO --> make source package...
rem TODO --> copy artifacts to \\mimanas\...
