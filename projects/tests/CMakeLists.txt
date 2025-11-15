set(projectName tests)

set(sourceFiles
    main.cpp
)

add_executable(${projectName} ${sourceFiles})

if(DEFINED projectProperties AND projectProperties)
    set_target_properties(${projectName} ${projectProperties})
endif()

if(DEFINED projectLibs AND projectLibs)
    target_link_libraries(${projectName} ${projectLibs})
endif()
