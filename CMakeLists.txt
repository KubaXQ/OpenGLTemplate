#Określamy minimalną wersję CMake wymaganą do zbudowania projektu
cmake_minimum_required(VERSION 3.20)

# W MSVC używamy statycznej wersji bibliotek czasu wykonywania (CRT)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")




#Ustawiamy nazwe projektu
project(OpenGlTemplate)

# Dołączamy moduł FetchContent do pobierania zewnętrznych bibliotek
include(FetchContent)

# Definiujemy zewnętrzne biblioteki, które CMake ma pobrać
FetchContent_Declare(
    GLEW
    URL https://github.com/nigels-com/glew/releases/download/glew-2.3.1/glew-2.3.1-win32.zip
    SOURCE_SUBDIR  "build/cmake"
)

FetchContent_Declare(
    GLFW
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_Declare(
    GLM
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
)

# Pobieramy biblioteki i dodajemy je do procesu budowania
FetchContent_MakeAvailable(GLEW)
FetchContent_MakeAvailable(GLFW)
FetchContent_MakeAvailable(GLM)

# Tworzymy zmienne ze ścieżkami do plików GLEW i GLM
set(GLEW_LIBRARY "${glew_SOURCE_DIR}/lib/Release/x64/glew32s.lib")

set(GLEW_INCLUDE
    "${glew_SOURCE_DIR}/include"
)
set(GLM_INCLUDE
    "${glm_SOURCE_DIR}/glm"
)


#Tworzymy plik wykonywalny z plikow zrodlowych
add_executable(OpenGLTemplate src/main.cpp src/Mesh.cpp src/Camera.cpp src/Shader.cpp src/Window.cpp)

# Definiujemy makro GLEW_STATIC wymagane przy statycznym linkowaniu GLEW
target_compile_definitions(OpenGLTemplate PRIVATE GLEW_STATIC)

#dodajemy sciezki do plikow naglowkowych dla projektu
target_include_directories(OpenGLTemplate PRIVATE
    include
    ${GLEW_INCLUDE}
    ${GLM_INCLUDE}
)

#dodajemy biblioteki do projektu
target_link_libraries(OpenGLTemplate PRIVATE
    ${GLEW_LIBRARY}
    glfw
    glm::glm
    opengl32
)

# Po zbudowaniu projektu kopiujemy katalog Shaders do folderu z plikiem wykonywalnym
add_custom_command(TARGET OpenGLTemplate POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Shaders"
        "$<TARGET_FILE_DIR:OpenGLTemplate>/Shaders"
)
