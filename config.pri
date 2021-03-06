# Config.pri file version 2.0. Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR = $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        profile {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/bb/system)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/bb/system)

            LIBS += -lbbsystem

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        } else {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/bb/system)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/bb/system)

            LIBS += -lbbsystem

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }

    }

    CONFIG(release, debug|release) {
        !profile {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/bb/system)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/bb/system)

            LIBS += -lbbsystem

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

simulator {
    CONFIG(debug, debug|release) {
        !profile {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/bb/system)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/bb/system)

            LIBS += -lbbsystem

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

config_pri_assets {
    OTHER_FILES += \
        $$quote($$BASEDIR/assets/AlbumListPage.qml) \
        $$quote($$BASEDIR/assets/ImageDetailsPage.qml) \
        $$quote($$BASEDIR/assets/ImageListPage.qml) \
        $$quote($$BASEDIR/assets/PreferrencesPage.qml) \
        $$quote($$BASEDIR/assets/images/album.png) \
        $$quote($$BASEDIR/assets/images/bb_action_saveas.png) \
        $$quote($$BASEDIR/assets/images/bb_action_share.png) \
        $$quote($$BASEDIR/assets/images/image.png) \
        $$quote($$BASEDIR/assets/main.qml)
}

config_pri_source_group1 {
    SOURCES += \
        $$quote($$BASEDIR/src/album.cpp) \
        $$quote($$BASEDIR/src/applicationui.cpp) \
        $$quote($$BASEDIR/src/category.cpp) \
        $$quote($$BASEDIR/src/communicator.cpp) \
        $$quote($$BASEDIR/src/image.cpp) \
        $$quote($$BASEDIR/src/main.cpp) \
        $$quote($$BASEDIR/src/picturecardpreview.cpp) \
        $$quote($$BASEDIR/src/settings.cpp)

    HEADERS += \
        $$quote($$BASEDIR/src/album.h) \
        $$quote($$BASEDIR/src/applicationui.hpp) \
        $$quote($$BASEDIR/src/category.h) \
        $$quote($$BASEDIR/src/communicator.h) \
        $$quote($$BASEDIR/src/image.h) \
        $$quote($$BASEDIR/src/picturecardpreview.h) \
        $$quote($$BASEDIR/src/settings.h)
}

INCLUDEPATH += $$quote($$BASEDIR/src)

CONFIG += precompile_header

PRECOMPILED_HEADER = $$quote($$BASEDIR/precompiled.h)

lupdate_inclusion {
    SOURCES += \
        $$quote($$BASEDIR/../src/*.c) \
        $$quote($$BASEDIR/../src/*.c++) \
        $$quote($$BASEDIR/../src/*.cc) \
        $$quote($$BASEDIR/../src/*.cpp) \
        $$quote($$BASEDIR/../src/*.cxx) \
        $$quote($$BASEDIR/../assets/*.qml) \
        $$quote($$BASEDIR/../assets/*.js) \
        $$quote($$BASEDIR/../assets/*.qs) \
        $$quote($$BASEDIR/../assets/images/*.qml) \
        $$quote($$BASEDIR/../assets/images/*.js) \
        $$quote($$BASEDIR/../assets/images/*.qs)

    HEADERS += \
        $$quote($$BASEDIR/../src/*.h) \
        $$quote($$BASEDIR/../src/*.h++) \
        $$quote($$BASEDIR/../src/*.hh) \
        $$quote($$BASEDIR/../src/*.hpp) \
        $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS = $$quote($${TARGET}.ts)
