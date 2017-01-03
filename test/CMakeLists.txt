add_executable(apt_player   main.cpp
                            player.cpp 
                            player.hpp
                            provider.cpp
                            provider.hpp)
set_property(TARGET apt_player PROPERTY CXX_STANDARD 14)
target_link_libraries(apt_player libapt libbig glfw)
target_include_directories(apt_player PRIVATE glfw 
                        ${LIBAPT_ROOT}/deps/cxxopts/src)
add_custom_command(TARGET apt_player POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/apt $<TARGET_FILE_DIR:apt_player>)