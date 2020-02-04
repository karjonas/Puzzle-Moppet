set(PROJECT_NAME puzzlemoppet)

add_executable(${PROJECT_NAME}
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

target_link_libraries(${PROJECT_NAME} Litha)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
