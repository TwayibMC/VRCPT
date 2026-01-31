cmake_minimum_required(VERSION 3.16)

project(VRProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)

set(PROJECT_SOURCES
        main.cpp
        vrwidget.cpp
        vrwidget.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(VRProject
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        VRResources.qrc
        README.md
        mainwindow.h mainwindow.cpp mainwindow.ui
        vrcamera.h vrcamera.cpp
        vrbody.h vrbody.cpp
        vrcube.h vrcube.cpp
        vrplane.h vrplane.cpp
        vrsphere.cpp vrsphere.h vrsurface.cpp vrsurface.h vrtorus.cpp vrtorus.h
        vrsphericalcamera.h vrsphericalcamera.cpp
        vrlight.h vrlight.cpp
        vrdice.h vrdice.cpp
        vrskybox.h vrskybox.cpp
        vrscene.h vrscene.cpp
        vrcurve.cpp vrcurve.h
        vrhelix.cpp vrhelix.h
        vrmesh.h vrmesh.cpp
        vrmodel.h vrmodel.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET VRProject APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(VRProject SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(VRProject
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(VRProject PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(VRProject PRIVATE Qt6::OpenGLWidgets)


find_library(ASSIMP_LIBRARY NAMES assimp assimp-vc143-mtd PATHS ../assimp/lib)
find_path(ASSIMP_INCLUDE_DIR NAMES assimp/Importer.hpp PATHS ../assimp/include)
include_directories(AFTER ${ASSIMP_INCLUDE_DIR})
target_link_libraries(VRProject PRIVATE ${ASSIMP_LIBRARY})

#target_link_libraries(${TARGET_NAME} OpenGL::GL)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER fr.unica.VRProject)
endif()
set_target_properties(VRProject PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include_directories(${PROJECT_SOURCE_DIR})

include(GNUInstallDirs)
install(TARGETS VRProject
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(VRProject)
endif()




