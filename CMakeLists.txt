project(atelier)
cmake_minimum_required(VERSION 3.0)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(KDECompilerSettings)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMInstallIcons)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
endif()

#Import AtCore from KDE Anonimous GIT
include(ExternalProject)

ExternalProject_Add(atcore_import
    GIT_REPOSITORY  git://anongit.kde.org/atcore.git
    GIT_TAG "master"
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/atcore"
    BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/atcore"
)

#Inlude like Static Library
add_library(atcore STATIC IMPORTED)
add_dependencies(atcore atcore_import)

#Atelier Dependencies
find_package(KF5 REQUIRED COMPONENTS
                Solid
            )

find_package(Qt5 REQUIRED COMPONENTS
                Core
                Test
                Widgets
                SerialPort
            )

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
