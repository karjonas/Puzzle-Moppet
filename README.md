Puzzle Moppet
===================

![Screenshots](banner.png)

A challenging 3D puzzle game where you must guide the Moppet through the vast 
and eternal void of space by solving the various and beautiful puzzles thrown 
at you.

This project is an effort to make the original Puzzle Moppet open source, 
replace art assets and continue the game development. Videos, details and 
binaries can still be found at original developer's website: 
http://garnetgames.com/puzzlemoppet.

Requirements
------------

- CMake
- git
- vcpkg, https://github.com/microsoft/vcpkg/
- Irrlicht, http://irrlicht.sourceforge.net/downloads/
- ODE, http://www.ode.org/
- OpenAL, http://connect.creativelabs.com/openal/default.aspx

Distribution specific instructions
----------------------------------

Ubuntu:

    sudo apt-get install build-essential cmake libxxf86vm-dev libopenal-dev git curl unzip tar libgl1-mesa-dev

Arch Linux:

    sudo pacman -S openal cmake make gcc git

Dependencies
------------

Install vcpkg and the dependencies:

    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    ./vcpkg install irrlicht ode openal-soft scnlib


Building the Game
-----------------
Open a terminal/console window, navigate to the project folder. Replace <VCPKG_DIR> with the vcpkg path, then run CMake and then Make:

    mkdir build
    cd build
    cmake .. -DCMAKE_TOOLCHAIN_FILE=<VCPKG_DIR>/scripts/buildsystems/vcpkg.cmake
    make

To "install" the game (i.e. use make install):

    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_FOR_INSTALL=ON .
    make
    sudo make install

or

    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$HOME/somewhere -DBUILD_FOR_INSTALL=ON .
    make
    make install

Other cmake options
-------------------
- ```cmake . ``` For normal build

- ```cmake -DCMAKE_BUILD_TYPE=Debug .``` If you want debug symbols

- ```cmake -DCMAKE_BUILD_TYPE=Release . ``` will give you -O2 optimisation for release builds

- ```CMAKE_INSTALL_PREFIX``` :
	Install directory used by install.
	If ```make install``` is invoked or INSTALL is built, this directory is
	pre-pended onto all install directories.  This variable defaults to
	/usr/local on UNIX and c:/Program Files on Windows.

- ```BUILD_FOR_INSTALL```: Should the game search for its datas in install path

Running the game
----------------

    ./projects/Puzzle/bin/puzzlemoppet

Running the editor
------------------

Run the executable with command line parameter "-e" followed by a level name to invoke the editor:

    ./projects/Puzzle/bin/puzzlemoppet -e level_name.lev

where level_name.lev is either one of the levels in the "projects\Puzzle\levels\levels" directory or a completely new name to create a new level).

Controls:

- Mouse wheel to toggle between block type selection and editing
- Left mouse to add a block.
- Right mouse to remove a block.
- Arrow keys or WSAD to fly around the level.
- "S" to save, but the level is auto saved on exit anyway.
- Esc or Alt+F4 to exit.

To run a specific level omit the -e flag:

    ./projects/Puzzle/bin/puzzlemoppet level_name.lev

License Information
-------------------

Please see specific license files in /puzzlemoppet/licenses.

Except otherwise noted, source files are released under WTFPL, version 2.0. 
Game assets are released under a free license, usually CCBYSA3 or GPLv3+. Asset 
files created by Daniel Frith (original author) are released under WTFPL.
