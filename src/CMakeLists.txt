set(atelier_SRCS
    main.cpp
    atelier.cpp
    toolbarwidget.cpp
    leftwidget.cpp
    )

set(interface_SRCS
    mainwindow.cpp
    connectsettings.cpp
    gcodeeditor.cpp
    )

add_library(AtelierInterface STATIC ${interface_SRCS})

target_link_libraries(AtelierInterface Qt5::Core Qt5::Widgets Qt5::SerialPort)

add_executable(atelier ${atelier_SRCS})

target_link_libraries(atelier AtelierInterface)
