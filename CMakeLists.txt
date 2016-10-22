project(atelier)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
endif()

find_package(Qt5 REQUIRED COMPONENTS
                Core
                Test
                Widgets
                SerialPort
            )

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(atelier_SRCS
    main.cpp
    atelier.cpp
    toolbarwidget.cpp
    leftwidget.cpp
    )

set(interface_UI_SRCS
    mainwindow.ui
    )

qt5_wrap_ui(interface_UI_SRCS ${interface_UI})

set(interface_SRCS
    mainwindow.cpp
    ${interface_UI_SRCS}
    )

add_library(AtelierInterface STATIC ${interface_SRCS} ${interface_UI_SRCS})

target_link_libraries(AtelierInterface Qt5::Core Qt5::Widgets Qt5::SerialPort)

add_executable(atelier ${atelier_SRCS} ${interface_UI_SRCS} )

target_link_libraries(atelier AtelierInterface)
