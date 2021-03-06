add_executable(apt_player   main.cpp
                            player.cpp 
                            player.hpp
                            provider.cpp
                            provider.hpp)
set_property(TARGET apt_player PROPERTY CXX_STANDARD 14)

if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    find_package(Boost COMPONENTS system filesystem REQUIRED)
    target_link_libraries(apt_player ${Boost_LIBRARIES})
    target_include_directories(apt_player PRIVATE ${Boost_INCLUDE_DIRS})
endif()

target_link_libraries(apt_player libapt libbig glfw)
target_include_directories(apt_player PRIVATE glfw 
                        ${LIBAPT_ROOT}/deps/cxxopts/include)
add_custom_command(TARGET apt_player POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/apt $<TARGET_FILE_DIR:apt_player>)