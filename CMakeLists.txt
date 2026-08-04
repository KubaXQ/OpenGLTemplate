#Mowimy Cmakeowi jaka minimalna wersje trzeba uzyc
cmake_minimum_required(VERSION 3.20)


set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

#Tworzymy projekt o nazwie OpenGlTemplate
project(OpenGlTemplate)

#tworzymy zmienna dzieki czemu skracamy pisanie kodu nizej przy dolaczanniu bibliotek
set(EXTERNAL_LIBS "${CMAKE_SOURCE_DIR}/External_Libs")
set(GLEW_INCLUDE "${EXTERNAL_LIBS}/GLEW/include")
set(GLFW_INCLUDE "${EXTERNAL_LIBS}/GLFW/include")
set(GLM_INCLUDE "${EXTERNAL_LIBS}/GLM")

set(GLEW_LIBRARY "${EXTERNAL_LIBS}/GLEW/lib/Release/x64/glew32s.lib")
set(GLFW_LIBRARY "${EXTERNAL_LIBS}/GLFW/lib-vc2022/glfw3_mt.lib")

#Tworzymy program wykonywalny exe o nazwie OpenGlTemplate (taki obiekt)
add_executable(OpenGLTemplate src/main.cpp src/Mesh.cpp src/Camera.cpp src/Shader.cpp src/Window.cpp)

target_compile_definitions(OpenGLTemplate PRIVATE GLEW_STATIC)

#dodajemy pliki naglowkowe zeby pliki zrodlowe mogly sie skompilowac
target_include_directories(OpenGLTemplate PRIVATE
    include
    ${GLEW_INCLUDE}
    ${GLFW_INCLUDE}
    ${GLM_INCLUDE}

)

#dolaczamy biblioteki
target_link_libraries(OpenGLTemplate PRIVATE

${GLEW_LIBRARY}
${GLFW_LIBRARY}
opengl32

)

add_custom_command(TARGET OpenGLTemplate POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Shaders"
        "$<TARGET_FILE_DIR:OpenGLTemplate>/Shaders"
)
