cmake_minimum_required(VERSION 3.16)
project(TP6 LANGUAGES CXX)

# Configuration C++ [cite: 9]
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets)
find_package(OpenGL REQUIRED)

set(SOURCES
    main.cpp
    mainwindow.cpp
    vrairplane.cpp
    vrbody.cpp
    vrcamera.cpp
    vrcube.cpp
    vrcurve.cpp
    vrdice.cpp
    vrhelix.cpp
    vrlight.cpp
    vrmesh.cpp
    vrmodel.cpp
    vrplane.cpp
    vrplanet.cpp
    vrscene.cpp
    vrskybox.cpp
    vrsphere.cpp
    vrsphericalcamera.cpp
    vrsurface.cpp
    vrterrain.cpp
    vrtexcube.cpp
    vrtorus.cpp
    vrwidget.cpp
)

set(HEADERS
    mainwindow.h
    vrairplane.h
    vrbody.h
    vrcamera.h
    vrcube.h
    vrcurve.h
    vrdice.h
    vrhelix.h
    vrlight.h
    vrmesh.h
    vrmodel.h
    vrplane.h
    vrplanet.h
    vrscene.h
    vrskybox.h
    vrsphere.h
    vrsphericalcamera.h
    vrsurface.h
    vrterrain.h
    vrtexcube.h
    vrtorus.h
    vrwidget.h
)

add_executable(TP6
    ${SOURCES}
    ${HEADERS}
    mainwindow.ui
    VRResources.qrc
)

target_include_directories(TP6 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# Liens vers les bibliothèques Qt et OpenGL [cite: 12]
target_link_libraries(TP6
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
    OpenGL::GL
)

# ============================================================
#  ASSIMP (Configuration portable)
# ============================================================
set(ASSIMP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../assimp")

if(EXISTS "${ASSIMP_ROOT}/include/assimp/Importer.hpp")
    target_include_directories(TP6 PRIVATE "${ASSIMP_ROOT}/include")
    target_link_libraries(TP6 PRIVATE "${ASSIMP_ROOT}/lib/libassimp.dll.a")
else()
    # Si Assimp n'est pas trouvé dans le dossier parent, on cherche via le système
    find_package(assimp REQUIRED)
    target_link_libraries(TP6 PRIVATE assimp::assimp)
endif()


file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/model"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
