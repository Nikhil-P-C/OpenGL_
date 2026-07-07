cmake_minimum_required(VERSION 3.10)
project(OpenGL_Example)
set(CMAKE_CXX_STANDARD 17)


add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew-2.3.1/build/cmake
)
add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw-3.4
)

add_executable(Engine 
        src/main.cpp
        src/Shader.cpp
        src/stb_image.c
        )
add_custom_command(TARGET Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:Engine>/res
)
target_compile_definitions(Engine PRIVATE GLEW_STATIC)
# set_target_properties(Engine PROPERTIES
#     VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
# )
target_include_directories(Engine PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw-3.4/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew-2.3.1/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
    )

target_link_libraries(Engine PRIVATE glfw glew_s)