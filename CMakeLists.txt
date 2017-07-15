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

#Atelier Dependencies
find_package(AtCore REQUIRED COMPONENTS
                AtCore
            )

find_package(KF5 REQUIRED COMPONENTS
                Solid
                I18n
                XmlGui
                ConfigWidgets
                TextEditor
            )

find_package(Qt5 REQUIRED COMPONENTS
                Core
                Test
                Widgets
                SerialPort
                Charts
            )

include(ECMPoQmTools)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})
add_subdirectory(src)
add_subdirectory(deploy)

if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ecm_install_po_files_as_qm(po)
endif()

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
