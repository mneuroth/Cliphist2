echo "make distribution of cliphist2 for Windows (*.exe and Inno-Setup):"
echo "call qmake"
qmake -r cliphist2.pro CONFIG+=release
echo "make app"
mingw32-make Release
echo "call Inno-Setup to create setup file"
"C:\Programme\Inno Setup 5\iscc" dist\cliphist2.iss /Fcliphist2_setup /O./dist
