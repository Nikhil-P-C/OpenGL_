cmake_minimum_required(VERSION 3.10)
project(OpenGL_Example)
set(CMAKE_CXX_STANDARD 17)


add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew-2.3.1/build/cmake
)
add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw-3.4
)

add_executable(Engine main.cpp)
target_compile_definitions(Engine PRIVATE GLEW_STATIC)

target_include_directories(Engine PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw-3.4/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew-2.3.1/include
    )

target_link_libraries(Engine PRIVATE glfw glew_s)