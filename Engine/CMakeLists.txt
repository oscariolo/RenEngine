project(Engine)

# Recursively find all C++ and Header files in Engine/src
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")

# Include the GLAD C file so it gets compiled into the engine
set(GLAD_SOURCE "${CMAKE_SOURCE_DIR}/vendor/glad/src/glad.c")

add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/reactphysics3d" "${CMAKE_BINARY_DIR}/vendor/reactphysics3d")

# Create the static library
add_library(Engine STATIC ${ENGINE_SOURCES} ${GLAD_SOURCE})

# Expose include directories
# PUBLIC means any project linking to 'Engine' also gets these include paths
target_include_directories(Engine PUBLIC 
    "src"
    "${CMAKE_SOURCE_DIR}/vendor/glfw/include"
    "${CMAKE_SOURCE_DIR}/vendor/glad/include"
    "${CMAKE_SOURCE_DIR}/vendor/glm"
    "${CMAKE_SOURCE_DIR}/vendor/reactphysics3d/include"
)

# Find system OpenGL
find_package(OpenGL REQUIRED)

# Link the pre-compiled GLFW library and OpenGL to the Engine
target_link_libraries(Engine PUBLIC 
    "${CMAKE_SOURCE_DIR}/vendor/glfw/lib/libglfw3dll.a"
    OpenGL::GL
    reactphysics3d
)

