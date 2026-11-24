cmake_minimum_required(VERSION 3.10.0)
project(ThisGame VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

include(GNUInstallDirs)

add_subdirectory(glfw)
add_library(glad STATIC
    glad/src/glad.c
)
find_package(OpenGL REQUIRED)

target_include_directories(glad PUBLIC
    glad/include
)

add_executable(ThisGame
    main.cpp
    src/FilePaths.cpp
    src/FileReader.cpp
    src/Shader.cpp
    src/VBO.cpp
    src/EBO.cpp
    src/VAO.cpp
    src/stb_image.cpp
    src/Image.cpp
    src/Mesh.cpp
    src/Vector.cpp
    src/Camera.cpp
    src/Object.cpp
    
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

target_compile_definitions(ThisGame PRIVATE
    PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(ThisGame PRIVATE glfw glad OpenGL::GL)

if(MINGW)
    target_link_options(ThisGame PRIVATE -static-libgcc -static-libstdc++)
endif()

target_include_directories(ThisGame PRIVATE
    include
    src
    stb
    glm
    imgui
)

install(DIRECTORY assets
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS ThisGame
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)