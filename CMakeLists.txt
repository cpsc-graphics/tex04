cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)

set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SFML)

find_package(OpenGL REQUIRED)

add_executable(main 
    src/main.cpp
    src/ship.cpp
    src/asteroids.cpp
    src/opengl_loader.cpp
)
target_compile_features(main PRIVATE cxx_std_17)

# Tell compiler where to find gl.h
target_include_directories(main PRIVATE 
    include
)

target_link_libraries(main 
    PRIVATE 
        SFML::Graphics
        OpenGL::GL
)

# Handle shaders: Copy them to the build output directory so they can be loaded at runtime

# 1. Collect all shader files so CMake monitors them for changes
file(GLOB SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*")

# 2. Create a custom target that runs the copy command
add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:main>/shaders"
    SOURCES ${SHADER_FILES}
    COMMENT "Copying shaders to build directory..."
)

# 3. Force your executable to depend on the copy target
add_dependencies(main copy_shaders)




