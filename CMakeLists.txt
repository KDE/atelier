cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(atelier)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(KDECompilerSettings)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMInstallIcons)

if (POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(QT_MIN_VERSION "5.9.0")
set(KF5_DEP_VERSION "5.30.0")
set(KDE_APPLICATIONS_VERSION_MAJOR "1")
set(KDE_APPLICATIONS_VERSION_MINOR "0")
set(KDE_APPLICATIONS_VERSION_MICRO "0")
set(KDE_APPLICATIONS_VERSION "${KDE_APPLICATIONS_VERSION_MAJOR}.${KDE_APPLICATIONS_VERSION_MINOR}.${KDE_APPLICATIONS_VERSION_MICRO}")

#Atelier Dependencies
find_package(AtCore REQUIRED COMPONENTS
                AtCore
                AtCoreWidgets
            )

find_package(Qwt6 REQUIRED)

find_package(KF5 ${KF5_DEP_VERSION} REQUIRED COMPONENTS
                I18n
                XmlGui
                ConfigWidgets
                TextEditor
            )

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED COMPONENTS
                Core
                Widgets
                SerialPort
                Charts
                Quick
                Qml
                3DCore
                3DExtras
                3DRender
                3DInput
                Multimedia
                MultimediaWidgets
            )

if(BUILD_TESTING)
    find_package(Qt5Test ${QT_MIN_VERSION} CONFIG REQUIRED)
endif()

# config.h
configure_file (config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/src/config.h)

include(ECMPoQmTools)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})
add_subdirectory(src)
add_subdirectory(deploy)

if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ecm_install_po_files_as_qm(po)
endif()

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
