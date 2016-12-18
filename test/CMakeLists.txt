add_executable(apt_test main.cpp)
set_property(TARGET apt_test PROPERTY CXX_STANDARD 14)
target_link_libraries(apt_test libapt glfw)
target_include_directories(apt_test PRIVATE glfw)
add_custom_command(TARGET apt_test POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/apt $<TARGET_FILE_DIR:apt_test>)