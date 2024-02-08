set(PROJECT_NAME puzzlemoppet)

# Add flag to make console not appear on windows.
# Also makes WinMain() entry point instead of main().
if(WIN32)
    set(WIN32_APPLICATION_FLAG "WIN32")
else()
    set(WIN32_APPLICATION_FLAG "")
endif()

add_executable(${PROJECT_NAME} ${WIN32_APPLICATION_FLAG}
    CameraCollider.h
    Editor.cpp
    Editor.h
    EndLevelScreen.cpp
    EndLevelScreen.h
    Enums.h
    Events.cpp
    Events.h
    FinalScenePlayerProxy.h
    get_lines.h
    GridBasedCharacterController.h
    GUIPane.h
    level_stats.cpp
    level_stats.h
    Level.cpp
    Level.h
    main.cpp
    MainState.cpp
    MainState.h
    Map.cpp
    Map.h
    options.cpp
    options.h
    Positioner.cpp
    Positioner.h
    RotateToAnimator.h
    SimpleMenu.cpp
    SimpleMenu.h
    StartScreen.cpp
    StartScreen.h
    volume.cpp
    volume.h
)

target_link_libraries(${PROJECT_NAME} Litha)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
