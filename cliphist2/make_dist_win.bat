echo "make distribution of cliphist2 for Windows (*.exe and Inno-Setup):"
echo "call qmake"
qmake cliphist2.pro CONFIG+=release
echo "make app"
mingw32-make
echo "call Inno-Setup to create setup file"
"C:\Programme\Inno Setup 5\iscc" dist\cliphist2.iss /Fcliphist2_setup /O./dist
