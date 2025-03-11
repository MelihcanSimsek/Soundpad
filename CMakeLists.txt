cmake_minimum_required(VERSION 3.16)

project(CustomSoundpad VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Multimedia)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appCustomSoundpad
    main.cpp
)

qt_add_qml_module(appCustomSoundpad
    URI CustomSoundpad
    VERSION 1.0
    QML_FILES
        Main.qml
        QML_FILES ImageButton.qml
        RESOURCES res.qrc
        QML_FILES Soundpad.qml
        SOURCES AppDbContext.h AppDbContext.cpp
        SOURCES SoundInfo.h SoundInfo.cpp
        SOURCES SoundpadController.h SoundpadController.cpp
        SOURCES Config.h Config.cpp
        QML_FILES VolumeDialog.qml
        QML_FILES AddDialog.qml
        SOURCES AudioMixer.h AudioMixer.cpp
        QML_FILES Wheel.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appCustomSoundpad PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appCustomSoundpad
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appCustomSoundpad
    PRIVATE Qt6::Quick Qt6::Multimedia
)

include(GNUInstallDirs)
install(TARGETS appCustomSoundpad
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
