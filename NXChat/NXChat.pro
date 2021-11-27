QT += quick quickcontrols2 widgets websockets sql network

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        accountconfig.cpp \
        enum_define.cpp \
        globalconfig.cpp \
        httpuploadmanager.cpp \
        lineengine.cpp \
        model/contactmodel.cpp \
        model/eventmodel.cpp \
        model/groupinvitedmodel.cpp \
        model/groupmodel.cpp \
        model/sessionmodel.cpp \
        modelitem.cpp \
        util/fileutil.cpp \
        main.cpp \
        submodules/hsluv-c/src/hsluv.c \
        util/quicklist.cpp

RESOURCES += qml.qrc \
    res.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


HEADERS += \
    accountconfig.h \
    clipboard.h \
    clipboard_image_provider.h \
    enum_define.h \
    globalconfig.h \
    httpuploadmanager.h \
    lineengine.h \
    filetype_image_provider.h \
    model/contactmodel.h \
    model/eventmodel.h \
    model/groupinvitedmodel.h \
    model/groupmodel.h \
    model/sessionmodel.h \
    util/fileutil.h \
    util/quicklist.h \
    utils.h \
    submodules/hsluv-c/src/hsluv.h \
    modelitem.h
