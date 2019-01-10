echo "make distribution of cliphist2 for Windows (*.exe and Inno-Setup):"
echo "call qmake"
qmake -r cliphist2.pro CONFIG+=release
echo "make app"
mingw32-make Release
echo "compress executable"
upx -9 release\cliphist2.exe
echo "call Inno-Setup to create setup file"
"C:\Programme\Inno Setup 5\iscc" dist\cliphist2_static.iss /Fcliphist2_setup /O./dist
rem "C:\Programme\Inno Setup 5\iscc" dist\cliphist2.iss /Fcliphist2_setup /O./dist
