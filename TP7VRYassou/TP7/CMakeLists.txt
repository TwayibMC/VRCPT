cmake_minimum_required(VERSION 3.16)

# Nom du projet (RVTP7)
project(RVTP7 LANGUAGES CXX)

# Configuration C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Activation automatique de Qt (MOC, UI, RCC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Recherche des paquets Qt6 (ou Qt5 si Qt6 absent)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets)
find_package(OpenGL REQUIRED)

# --- LISTE DES FICHIERS SOURCE (.cpp) ---
# Basé sur votre structure actuelle
set(SOURCES
    main.cpp
    mainwindow.cpp
    vrairplane.cpp
    vrbb8.cpp
    vrbody.cpp
    vrcamera.cpp
    vrlight.cpp
    vrmesh.cpp
    vrmodel.cpp
    vrscene.cpp
    vrsphere.cpp
    vrsphericalcamera.cpp
    vrstereocamera.cpp
    vrsubjectivecamera.cpp
    vrsurface.cpp
    vrtorus.cpp
    vrwidget.cpp
)

# --- LISTE DES FICHIERS EN-TÊTES (.h) ---
set(HEADERS
    mainwindow.h
    vrairplane.h
    vrbb8.h
    vrbody.h
    vrcamera.h
    vrlight.h
    vrmesh.h
    vrmodel.h
    vrscene.h
    vrsphere.h
    vrsphericalcamera.h
    vrstereocamera.h
    vrsubjectivecamera.h
    vrsurface.h
    vrtorus.h
    vrwidget.h
)

# Création de l'exécutable
add_executable(RVTP7
    ${SOURCES}
    ${HEADERS}
    mainwindow.ui
    VRResources.qrc
)

# Inclusion du dossier courant pour les headers
target_include_directories(RVTP7 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# Liens vers les bibliothèques Qt et OpenGL
target_link_libraries(RVTP7
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
    OpenGL::GL
)

# ============================================================
#  CONFIGURATION ASSIMP (Adaptée à votre dossier)
# ============================================================
# On suppose que le dossier assimp est au même niveau que le dossier du projet
set(ASSIMP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../assimp")

if(EXISTS "${ASSIMP_ROOT}/include/assimp/Importer.hpp")
    message(STATUS "Assimp trouvé dans : ${ASSIMP_ROOT}")
    target_include_directories(RVTP7 PRIVATE "${ASSIMP_ROOT}/include")

    # Lien vers la librairie statique (.a) pour la compilation
    target_link_libraries(RVTP7 PRIVATE "${ASSIMP_ROOT}/lib/libassimp.dll.a")

    # --- PARTIE MAGIQUE : COPIE AUTOMATIQUE DE LA DLL ---
    # Copie libassimp-5.dll à côté de l'exe après la compilation
    add_custom_command(TARGET RVTP7 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ASSIMP_ROOT}/bin/libassimp-5.dll"
        "$<TARGET_FILE_DIR:RVTP7>"
        COMMENT "Copie de la DLL Assimp vers le dossier de build..."
    )
else()
    message(WARNING "Dossier Assimp local introuvable, tentative via le système...")
    find_package(assimp REQUIRED)
    target_link_libraries(RVTP7 PRIVATE assimp::assimp)
endif()

# ============================================================
#  COPIE DES RESSOURCES (Modèles et Shaders)
# ============================================================

# Copie automatique du dossier "model" dans le dossier de build
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/model" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

# Copie automatique des fichiers shaders (.vsh et .fsh)
file(GLOB SHADERS "*.vsh" "*.fsh")
if(SHADERS)
    file(COPY ${SHADERS} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()
