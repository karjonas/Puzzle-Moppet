add_executable(puzzlemoppet
main.cpp
Level.cpp
Map.cpp
Events.cpp
Editor.cpp
#MonsterCharacterController.cpp
#MainMenu.cpp
Positioner.cpp
#RoundedGUISkin.cpp
MainState.cpp
StartScreen.cpp
SimpleMenu.cpp
EndLevelScreen.cpp
level_stats.cpp
volume.cpp
options.cpp
)

target_link_libraries(puzzlemoppet Litha)

install(TARGETS puzzlemoppet RUNTIME DESTINATION bin)
