Puzzle Moppet
===================

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
- Irrlicht  http://irrlicht.sourceforge.net/downloads/
- ODE, http://www.ode.org/
- OpenAL, http://connect.creativelabs.com/openal/default.aspx

If in GNU/Linux search for the required libraries using your package manager.

Distribution specific instructions
----------------------------------
Ubuntu:

    sudo apt-get install build-essential cmake libode-dev libopenal-dev libxxf86vm-dev libirrlicht-dev

Arch Linux:

To build and install automatically use the PKGBUILD available at https://aur.archlinux.org/packages/puzzlemoppet-git

If you want to build it manually then install the dependencies:

    sudo pacman -S ode irrlicht openal cmake


Building the Game
-----------------
Open a terminal/console window, navigate to the project folder and change into the 
/puzzlemoppet directory. Run CMake and then Make:

    cmake .
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

    cd $BASEDIR/puzzlemoppet/bin/
    ./puzzlemoppet

Running the editor
------------------

Run the executable with command line parameter "-e" followed by a level name to invoke the editor:

    ./puzzlemoppet -e level_name.lev

where level_name.lev is either one of the levels in the "projects\Puzzle\levels\levels" directory or a completely new name to create a new level).

Controls:

- Mouse wheel to toggle between block type selection and editing
- Left mouse to add a block.
- Right mouse to remove a block.
- Arrow keys or WSAD to fly around the level.
- "S" to save, but the level is auto saved on exit anyway.
- Esc or Alt+F4 to exit.

To run a specific level omit the -e flag:

    ./puzzlemoppet level_name.lev

License Information
-------------------

Please see specific license files in /puzzlemoppet/licenses.

Except otherwise noted, source files are released under WTFPL, version 2.0. 
Game assets are released under a free license, usually CCBYSA3 or GPLv3+. Asset 
files created by Daniel Frith (original author) are released under WTFPL.

Other Notes (from Daniel Frith)
-------------------------------

Some minor patches were made to the Irrlicht 1.6.1 source code which have since 
been *lost*. The ones I remember are:

- An opengl tweak to fix the sea's transparency in the final scene of the game, 
in COpenGLMaterialRenderer.h:

    COpenGLMaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER::OnSetMaterial
    
    change 
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR) 
    to
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
        

- In GNU Linux, changing to use XRandR rather than XVidMode fixed an occasional 
fullscreen freezing problem.

- Some modifications to the OS X device, to do with it crashing at certain screen 
sizes, see irrlicht.sourceforge.net/forum/viewtopic.php?t=40211#p254941

