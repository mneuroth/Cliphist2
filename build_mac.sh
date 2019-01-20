export PATH=$PATH:/Users/min/Qt55/5.9.5/clang_64/bin:/Users/min/Qt55/Tools/QtInstallerFramework/3.0/bin
cd src
qmake -r cliphist2.pro
rm -rf cliphist2.app/
make clean
make -j 4
macdeployqt cliphist2.app/
mkdir ../dist/packages/de.mneuroth.cliphist2/data
cp -R cliphist2.app ../dist/packages/de.mneuroth.cliphist2/data/
binarycreator --offline-only -c ../dist/config/config.xml -p ../dist/packages Cliphist2Installer
# TODO --> version und datum in package.xml anpassen
# TODO --> version an build und artifakte uebergeben
# TODO --> make source package...
# TODO --> copy artifacts to \\mimanas\...
