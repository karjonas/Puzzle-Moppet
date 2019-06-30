project(LithaEngine)

cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS)

# find-pkg is used to select -DdSINGLE or -DdDOUBLE for ODE
INCLUDE(FindPkgConfig)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")

set(BUILD_FOR_INSTALL "OFF" CACHE BOOL "Should the game search for its datas in install path")
set(DATA_DIR ${PROJECT_SOURCE_DIR}/data CACHE PATH "Directory for game data")
add_definitions(-DDATA_DIR="${DATA_DIR}")

if (BUILD_FOR_INSTALL)
	add_definitions(-DDATA_DIR="${CMAKE_INSTALL_PREFIX}/share/puzzlemoppet")
endif (BUILD_FOR_INSTALL)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	# On Linux, force it to search the exe directory for .so files.
	# (Slackware at least was not searching the exe directory)
	set(CMAKE_INSTALL_RPATH "$ORIGIN")
	set(CMAKE_BUILD_WITH_INSTALL_RPATH true)
	pkg_check_modules(ODE REQUIRED ode)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")


# find the absolute litha engine root directory
set(rootDir ${CMAKE_HOME_DIRECTORY})


########## COMPILE TARGETS #######

# specify the directory to output libraries and executables

if (WIN32)
	set(lithaTargetName "Win32-gcc")
	set(irrlichtLibDir "Win32-gcc")
	set(exeExtension ".exe")
endif (WIN32)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(lithaTargetName "Linux")
	set(irrlichtLibDir "Linux")
	set(exeExtension "")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ODE_CFLAGS}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${ODE_CFLAGS}")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${ODE_CFLAGS}")
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(lithaTargetName "MacOSX")
	set(irrlichtLibDir "MacOSX")
	set(exeExtension "")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch i386")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -arch i386")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -arch i386")
endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")



########## COMMON LIBS ###########

# (things required by both litha engine and individual projects)
# (actually some of these probably aren't required by both...)

# Irrlicht
find_package(Irrlicht REQUIRED)
message ("Using Irrlicht version " ${IRRLICHT_VERSION})
if (${IRRLICHT_VERSION} MATCHES "1.6.1")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIRRLICHT_VERSION=161")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DIRRLICHT_VERSION=161")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DIRRLICHT_VERSION=161")
endif (${IRRLICHT_VERSION} MATCHES "1.6.1")

include_directories(${rootDir} ${rootDir}/include ${IRRLICHT_INCLUDE_DIRS})


########## LITHA ENGINE CONFIG ########

# Litha engine static library output directory
set(LIBRARY_OUTPUT_PATH ${rootDir}/lib/${lithaTargetName})

# Compile litha engine...
# (specific libraries needed only by Litha engine are set in this directory's CMakeLists.txt)
add_subdirectory(source)



########### PROJECTS CONFIG ############

# Project executable output path
set(EXECUTABLE_OUTPUT_PATH bin)

# Link to Litha Engine
link_directories(${rootDir}/lib/${lithaTargetName})

# Project libraries
set(projectLibs Litha ${IRRLICHT_LIBRARIES})

# Project properties
if (exeExtension)
	set(projectProperties PROPERTIES SUFFIX ${exeExtension})
endif (exeExtension)



########### PLATFORM SPECIFIC PROJECTS CONFIG ############

# Hide console window in Windows.
if (WIN32)
	set(projectProperties ${projectProperties} WIN32_EXECUTABLE true)
endif (WIN32)

# Linux needs GL for some reason.
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(projectLibs ${projectLibs} GL Xrandr X11)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")



########### BUILD ALL PROJECTS ############
add_subdirectory(projects)

########### INSTALLATION DESTINATION ############
install(DIRECTORY 
  ${PROJECT_SOURCE_DIR}/data
  DESTINATION share/puzzlemoppet/Puzzle)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/licenses/ DESTINATION share/licences/puzzlemoppet)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/icons DESTINATION share/puzzlemoppet)




