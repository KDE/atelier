cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(atelier)

set(QT_MIN_VERSION "6.7")
set(KF_MIN_VERSION "6.10")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(KDECompilerSettings NO_POLICY_SCOPE)
remove_definitions(
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_FOREACH
    -DQT_NO_KEYWORDS
)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMInstallIcons)
include(ECMDeprecationSettings)
ecm_set_disabled_deprecation_versions(
  QT 6.8.0
  KF 6.12.0
)

if (POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(KDE_APPLICATIONS_VERSION_MAJOR "0")
set(KDE_APPLICATIONS_VERSION_MINOR "70")
set(KDE_APPLICATIONS_VERSION_MICRO "0")
set(KDE_APPLICATIONS_VERSION "${KDE_APPLICATIONS_VERSION_MAJOR}.${KDE_APPLICATIONS_VERSION_MINOR}.${KDE_APPLICATIONS_VERSION_MICRO}")

#Atelier Dependencies
find_package(AtCore REQUIRED COMPONENTS
                AtCore
                AtCoreWidgets
            )

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
                I18n
                XmlGui
                ConfigWidgets
                TextEditor
            )

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS
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
    find_package(Qt6Test ${QT_MIN_VERSION} CONFIG REQUIRED)
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

include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.cpp src/*.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})


feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
