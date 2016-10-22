project(atelier)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
endif()

find_package(Qt5 REQUIRED COMPONENTS
                Core
                Test
                Widgets
            )

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(atelier_SRCS
    src/atelier.cpp
    src/main.cpp
    )

add_executable(atelier ${atelier_SRCS} ${RESOURCES})
qt5_use_modules(atelier Core)
target_link_libraries(atelier Qt5::Core Qt5::Widgets)
