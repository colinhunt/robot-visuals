cmake_minimum_required(VERSION 2.8.4)
project(modelviewer)

set(SOURCE_FILES model.cpp model.h Camera.cpp Camera.h GlTransformable.cpp GlTransformable.h TGALoader.h TGALoader.cpp)

add_executable(solidviewer solidviewer.cpp ${SOURCE_FILES})
add_executable(attachviewer attachviewer.cpp BvhParser.h BvhParser.cpp Skeleton.h Skeleton.cpp Articulator.h Articulator.cpp ${SOURCE_FILES})
add_executable(personviewer personviewer.cpp BvhParser.h BvhParser.cpp Skeleton.h Skeleton.cpp Articulator.h Articulator.cpp FrameTimer.h ${SOURCE_FILES})

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

target_link_libraries(solidviewer ${OPENGL_LIBRARIES})
target_link_libraries(attachviewer ${OPENGL_LIBRARIES})
target_link_libraries(personviewer ${OPENGL_LIBRARIES})
