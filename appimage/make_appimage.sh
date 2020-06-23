#!/bin/sh
rm -rf vcpkg
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install irrlicht ode openal-soft scnlib
cd ..
rm -rf build
mkdir build
rm linuxdeploy-x86_64.AppImage
wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
chmod +x linuxdeploy-x86_64.AppImage
cd build
cmake ../.. -DCMAKE_TOOLCHAIN_FILE=$PWD/../vcpkg/scripts/buildsystems/vcpkg.cmake -DDATA_DIR=/usr/share/puzzlemoppet/Puzzle/data -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
make -j `nproc --all` install DESTDIR=AppDirPuzzle
make -j `nproc --all` install DESTDIR=AppDirConfig
cd ..
rm PuzzleMoppet*.AppImage
./linuxdeploy-x86_64.AppImage --appdir build/AppDirPuzzle --output appimage -d puzzlemoppet.desktop -i ../icons/main.png
./linuxdeploy-x86_64.AppImage --appdir build/AppDirConfig --output appimage -d config.desktop -i ../icons/config.png
GIT_HASH=$(git rev-parse --verify HEAD --short)
mv PuzzleMoppet-*.AppImage PuzzleMoppet-$GIT_HASH.AppImage
mv PuzzleMoppetConfig-*.AppImage PuzzleMoppetConfig-$GIT_HASH.AppImage
