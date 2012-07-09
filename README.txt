About Puzzle Moppet
===================

A challenging 3D puzzle game where you must guide the Moppet through the vast 
and eternal void of space by solving the various and beautiful puzzles thrown 
at you.

This project is an effort to make the original Puzzle Moppet open source, 
replace art assets and continue the game development. Videos, details and 
binaries can still be found at original developer's website: 
http://garnetgames.com/puzzlemoppet.


License Information
-------------------

Please see specific license files in /puzzlemoppet/licenses.

Except otherwise noted, source files are released under WTFPL, version 2.0. 
Game assets are released under a free license, usually CCBYSA3 or GPLv3+. Asset 
files created by Daniel Frith (original author) are released under WTFPL.

Requirements
------------

. CMake
. Irrlicht 1.6.1, http://downloads.sourceforge.net/irrlicht/irrlicht-1.6.1.zip
. ODE, http://www.ode.org/
. OpenAL, http://connect.creativelabs.com/openal/default.aspx

If in GNU/Linux search for the required libraries using your package manager.

Distribution specific instructions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
. Ubuntu:
sudo apt-get install build-essential cmake libode-dev libopenal-dev libxxf86vm-dev libirrlicht-dev

. General:
If your package manager does not provide version 1.6.1 you need to compile it yourself:

Download the Irrlicht files, version 1.6.1, unzip and place in /puzzlemoppet/thirdparty.                                                                                                                   
If in GNU Linux search for the required libraries using your package manager.

> cd <project dir>/puzzlemoppet/thirdparty/irrlicht-1.6.1/source/Irrlicht/
> make

Building the Game
-----------------

Open a terminal/console window, navigate to the project folder and change into the 
/puzzlemoppet directory. Run CMake and then Make:

> cmake .
> make

Other cmake options
~~~~~~~~~~~~~~~~~~~
"cmake ." for normal build
"cmake -DCMAKE_BUILD_TYPE=Debug ." if you need lines number in gdb
"cmake -DCMAKE_BUILD_TYPE=Release ." will give you -O2 optimisation for release builds

Running the game
----------------
cd $BASEDIR/puzzlemoppet/bin/
./puzzlemoppet

Running the editor
~~~~~~~~~~~~~~~~~~
./puzzlemoppet -e levelname.lev
cf http://www.desura.com/games/puzzle-moppet/news/puzzle-moppet-level-editor for more info.

Running a specific level
~~~~~~~~~~~~~~~~~~~~~~~~
./puzzlemoppet levelname.lev

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
