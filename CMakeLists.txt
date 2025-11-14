cmake_minimum_required(VERSION 3.10)

project(PuzzleMoppet)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_FOR_INSTALL "OFF" CACHE BOOL "Should the game search for its datas in install path")
set(DATA_DIR ${PROJECT_SOURCE_DIR}/data CACHE STRING "Directory for game data")
set(BUILD_FOR_APPIMAGE "OFF" CACHE BOOL "Building for AppImage")


if (BUILD_FOR_INSTALL)
	set(DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/puzzlemoppet")
endif (BUILD_FOR_INSTALL)

add_definitions(-DDATA_DIR="${DATA_DIR}")

# find the absolute litha engine root directory
set(rootDir ${CMAKE_HOME_DIRECTORY})

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
set(projectLibs Litha)

# Project properties
if (exeExtension)
	set(projectProperties PROPERTIES SUFFIX ${exeExtension})
endif (exeExtension)

########### PLATFORM SPECIFIC PROJECTS CONFIG ############

# Hide console window in Windows.
if (WIN32)
	set(projectProperties ${projectProperties} WIN32_EXECUTABLE true)
endif (WIN32)


########### BUILD ALL PROJECTS ############
add_subdirectory(projects)

########### INSTALLATION DESTINATION ############
install(DIRECTORY ${PROJECT_SOURCE_DIR}/data/levels DESTINATION share/puzzlemoppet)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/data/media DESTINATION share/puzzlemoppet)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/data/perfectscores DESTINATION share/puzzlemoppet)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/data/tutorialtexts DESTINATION share/puzzlemoppet)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/data/tutorialtexts_mac DESTINATION share/puzzlemoppet)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/licenses/ DESTINATION share/licences/puzzlemoppet)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/icons DESTINATION share/puzzlemoppet)




