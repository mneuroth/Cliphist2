rem call c:\users\micha\setqtmsc.bat
qmake -r cliphist2.pro
nmake clean
nmake release
cd release
mkdir ..\dist\packages\de.mneuroth.cliphist2\data
copy cliphist2.exe ..\dist\packages\de.mneuroth.cliphist2\data
cd ..\dist\packages\de.mneuroth.cliphist2\data
windeployqt --release --no-system-d3d-compiler cliphist2.exe
cd ..\..\..
binarycreator -c config\config.xml -p packages Cliphist2Installer.exe
rem TODO --> version und datum in package.xml anpassen
rem TODO --> version an build und artifakte uebergeben
rem TODO --> make source package...
rem TODO --> copy artifacts to \\mimanas\...
