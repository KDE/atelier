set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(atelier_SRCS
    main.cpp
    atelier.cpp
    mainwindow.cpp
    )

set(widgets_SRCS
    connectsettingswidget.cpp
    toolbarwidget.cpp
    leftwidget.cpp
    gcodeeditorwidget.cpp
    )

add_library(AtelierWidgets STATIC ${widgets_SRCS})

target_link_libraries(AtelierWidgets Qt5::Core Qt5::Widgets Qt5::SerialPort)

add_executable(atelier ${atelier_SRCS})

target_link_libraries(atelier AtelierWidgets)
