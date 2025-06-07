mkdir build
cd build
qmake6 ../CS206-SomeApplication.pro
make all -j$(nproc)
mkdir translations
cp ../translations/*.qm ./translations/
