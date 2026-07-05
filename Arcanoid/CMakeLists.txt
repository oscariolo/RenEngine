project(Arcanoid)

# Find all Arcanoid source files
file(GLOB_RECURSE ARCANOID_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")

# Create the executable
add_executable(Arcanoid ${ARCANOID_SOURCES})

# Link the Engine
# Because Engine's includes are PUBLIC, Arcanoid automatically 
# knows where to find <Engine.h>, GLM, and GLFW headers
target_link_libraries(Arcanoid PRIVATE Engine)

# Copy the GLFW DLL next to the Arcanoid executable so it can run
add_custom_command(TARGET Arcanoid POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/vendor/glfw/lib/glfw3.dll"
    $<TARGET_FILE_DIR:Arcanoid>
)