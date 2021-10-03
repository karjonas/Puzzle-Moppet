$base_path = Get-Location
$curr_hash = (git rev-parse HEAD).Substring(0,8)
$out_dir = "Puzzle_Moppet-$curr_hash"
echo "Output dir: $out_dir"
rmdir -r build
mkdir build
rmdir -r vcpkg
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg.exe install irrlicht ode openal-soft --triplet x64-windows
cd ../build
cmake ../../ -DDATA_DIR="./data" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="$base_path\vcpkg\scripts\buildsystems\vcpkg.cmake"
devenv PuzzleMoppet.sln /Build Release
cd ..
rmdir -r $out_dir
mkdir $out_dir
cp -r "build/projects/ConfigApp/bin/Release/*" "$out_dir/"
cp -r "build/projects/Puzzle/bin/Release/*" "$out_dir/"
cp -r "../data" "$out_dir/"
cp -r "../licenses" "$out_dir/"
Compress-Archive -Path "$out_dir" -DestinationPath "$out_dir.zip"
