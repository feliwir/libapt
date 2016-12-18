find_package(OpenGL REQUIRED)

set(HEADER_DIR ${CMAKE_SOURCE_DIR}/include/libapt)
set(PUBLIC_HEADERS  ${HEADER_DIR}/apt.hpp
					${HEADER_DIR}/bytecode.hpp
					${HEADER_DIR}/const.hpp
					${HEADER_DIR}/dat.hpp
					${HEADER_DIR}/error.hpp	
					${HEADER_DIR}/fileprovider.hpp
					${HEADER_DIR}/manager.hpp
					${HEADER_DIR}/scriptcontext.hpp
					${HEADER_DIR}/scriptfunction.hpp
					${HEADER_DIR}/texture.hpp
					${HEADER_DIR}/value.hpp)

file(GLOB SRC_FILES
	"*.c"
	"*.h"
    "*.hpp"
    "*.cpp"
)

add_library(libapt STATIC   ${SRC_FILES}
							${PUBLIC_HEADERS})

set_property(TARGET libapt PROPERTY CXX_STANDARD 14)

target_include_directories(libapt PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(libapt PRIVATE ${CMAKE_SOURCE_DIR}/deps/glm-0.9.8.3
										  ${OPENGL_INCLUDE_DIR})

target_link_libraries(libapt PUBLIC ${OPENGL_gl_LIBRARY})