set(PROJECT_NAME puzzlemoppet-config)

add_executable(${PROJECT_NAME}
main.cpp
)

target_link_libraries(${PROJECT_NAME} Litha)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
