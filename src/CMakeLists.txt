find_package(OpenGL REQUIRED)

set(HEADER_DIR ${LIBAPT_ROOT}/include/libapt)
set(PUBLIC_HEADERS  ${HEADER_DIR}/apt.hpp
					${HEADER_DIR}/const.hpp
					${HEADER_DIR}/dat.hpp
					${HEADER_DIR}/error.hpp	
					${HEADER_DIR}/fileprovider.hpp
					${HEADER_DIR}/manager.hpp
					${HEADER_DIR}/rendertarget.hpp
					${HEADER_DIR}/texture.hpp)

file(GLOB_RECURSE SRC_FILES
	"*.c"
	"*.h"
    "*.hpp"
    "*.cpp"
)

add_library(libapt STATIC   ${SRC_FILES}
							${PUBLIC_HEADERS})

set_property(TARGET libapt PROPERTY CXX_STANDARD 14)
set_property(TARGET libapt PROPERTY PREFIX "")
target_include_directories(libapt PUBLIC ${LIBAPT_ROOT}/include)
target_include_directories(libapt PRIVATE ${LIBAPT_ROOT}/deps/glm
										  ${LIBAPT_ROOT}/deps/gli
										  ${OPENGL_INCLUDE_DIR})

target_link_libraries(libapt PUBLIC ${OPENGL_gl_LIBRARY})

set_property(TARGET libapt PROPERTY POSITION_INDEPENDENT_CODE ON)