QT = core gui

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        helpers/opencvhelper.cpp \
        main.cpp

win32{
    LIBS += -L$$PWD\libs\lib -lopencv_core451d -lopencv_imgproc451d -lopencv_highgui451d -lopencv_imgcodecs451d -lopencv_videoio451d -lopencv_video451d -lopencv_calib3d451d -lopencv_photo451d -lopencv_features2d451d

    INCLUDEPATH += $$PWD\libs\include
    DEPENDPATH += $$PWD\libs\include
}

unix{
    ## Opencv4
    INCLUDEPATH += /usr/include/opencv4
    DEPENDPATH += /usr/include/opencv4
    # Include all libs of opencv4
    LIBS += $(shell pkg-config opencv4 --libs)
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    helpers/opencvhelper.h

RESOURCES += \
    images.qrc
