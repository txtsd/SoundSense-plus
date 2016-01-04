QT                      +=  widgets multimedia svg

TARGET                   =  SoundSense+
TEMPLATE                 =  app
DESTDIR                  =  dist

OBJECTS_DIR              =  build
MOC_DIR                  =  build
RCC_DIR                  =  build
UI_DIR                   =  build

SOURCES                 +=  src/main.cpp \
                            src/gui.cpp \
                            src/model.cpp \
                            src/audio.cpp \
                            src/log.cpp

HEADERS                 +=  inc/gui.h \
                            inc/model.h \
                            inc/audio.h \
                            inc/log.h

RESOURCES               +=  icons.qrc

FORMS                   +=  ui/GUI.ui

CONFIG                  +=  c++14

# QMAKE_RESOURCE_FLAGS    +=  -no-compress
QMAKE_RESOURCE_FLAGS    +=  -threshold 10 -compress 9

# LIBS                    +=  -lre2
