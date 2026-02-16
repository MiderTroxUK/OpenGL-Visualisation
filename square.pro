QT += core gui opengl widgets openglwidgets

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        glslprogram.cpp \
        glutils.cpp \
        main.cpp \
        mainview.cpp \
        scenebasic.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .qmake.stash \
    Makefile \
    Makefile.Debug \
    Makefile.Release \
    glew32.dll \
    glew32.lib \
    instruction.txt

HEADERS += \
    glslprogram.h \
    glutils.h \
    mainview.h \
    scene.h \
    scenebasic.h

INCLUDEPATH += $$PWD/include
INCLUDEPATH += C:/glm-master

unix|windows: LIBS += -L$$PWD/./ -lopengl32 -lglu32 -lglew32

# Added copy script for shaders
# Copy shaders to output directory after build
win32 {
    COPY = xcopy /y /s /i
    DIR_SRC = $$shell_path($$PWD/shader)
    DIR_DST_ROOT = $$shell_path($$OUT_PWD/shader)
    DIR_DST_DBG = $$shell_path($$OUT_PWD/debug/shader)
    DIR_DST_REL = $$shell_path($$OUT_PWD/release/shader)
    
    QMAKE_POST_LINK += $$COPY $$DIR_SRC $$DIR_DST_ROOT & $$COPY $$DIR_SRC $$DIR_DST_DBG & $$COPY $$DIR_SRC $$DIR_DST_REL
}
