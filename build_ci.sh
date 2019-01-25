cd src
if [[ $CXX == clang* ]]; then
    qmake "QMAKE_CXX = clang++" "QMAKE_LINK=clang++" cliphist2.pro
else
    qmake cliphist2.pro
fi
make 
ls -lrta
cd .obj
ls -lrt
 