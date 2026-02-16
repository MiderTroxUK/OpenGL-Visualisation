QT += core gui opengl widgets openglwidgets

CONFIG += c++17 cmdline

SOURCES += \
        glslprogram.cpp \
        glutils.cpp \
        main.cpp \
        mainview.cpp \
        mainwindow.cpp \
        scenebasic.cpp

HEADERS += \
    glslprogram.h \
    glutils.h \
    mainview.h \
    mainwindow.h \
    scene.h \
    scenebasic.h

# Include Paths
INCLUDEPATH += $$PWD/include
INCLUDEPATH += "C:/glm-master" 

# Library Links
unix|windows: LIBS += -L$$PWD/./ -lopengl32 -lglu32 -lglew32

# Copy shaders to output directory after build
win32 {
    COPY = xcopy /y /s /i
    DIR_SRC = $$shell_path($$PWD/shader)
    DIR_DST_ROOT = $$shell_path($$OUT_PWD/shader)
    DIR_DST_DBG = $$shell_path($$OUT_PWD/debug/shader)
    DIR_DST_REL = $$shell_path($$OUT_PWD/release/shader)
    
    QMAKE_POST_LINK += $$COPY $$DIR_SRC $$DIR_DST_ROOT & $$COPY $$DIR_SRC $$DIR_DST_DBG & $$COPY $$DIR_SRC $$DIR_DST_REL
}

SUBDIRS += \
    square.pro

DISTFILES += \
    .qmake.stash \
    glew32.dll \
    glew32.lib

FORMS += \
    mainwindow.ui
