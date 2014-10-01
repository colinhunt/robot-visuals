cmake_minimum_required(VERSION 2.8.4)
project(square)

add_executable(circle circle.cpp model.h model.cpp)

if(APPLE)
    find_library(COCOA Cocoa)
    find_library(OPENGL OpenGL)
    find_library(GLUT_L GLUT)
    mark_as_advanced(COCOA OPENGL GLUT_L)
    include_directories(/opt/X11/include /opt/local/include)
    LINK_DIRECTORIES(/opt/X11/lib /opt/local/lib)

    add_library(glew_static STATIC IMPORTED)
    set_target_properties(glew_static PROPERTIES
        IMPORTED_LOCATION /opt/local/lib/libGLEW.a)

    add_library(glut_static STATIC IMPORTED)
    set_target_properties(glut_static PROPERTIES
        IMPORTED_LOCATION /opt/X11/lib/libglut.3.dylib)

#    target_link_libraries(square ${COCOA} ${OPENGL} ${GLUT_L} glew_static glut_static)
    set(OPENGL_LIBRARIES ${COCOA} ${OPENGL} ${GLUT_L} glew_static glut_static)
else(APPLE)
    set(OPENGL_LIBRARIES glut GLEW GL GLU X11)
endif(APPLE)

target_link_libraries(circle ${OPENGL_LIBRARIES})
