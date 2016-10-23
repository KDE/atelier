set(atelier_SRCS
    main.cpp
    atelier.cpp
    toolbarwidget.cpp
    leftwidget.cpp
    )

set(interface_UI_SRCS
    mainwindow.ui
    connectsettings.ui
    gcodeeditor.ui
    )

qt5_wrap_ui(interface_UI_SRCS ${interface_UI})

set(interface_SRCS
    mainwindow.cpp
    connectsettings.cpp
    gcodeeditor.cpp
    ${interface_UI_SRCS}
    )

add_library(AtelierInterface STATIC ${interface_SRCS} ${interface_UI_SRCS})

target_link_libraries(AtelierInterface Qt5::Core Qt5::Widgets Qt5::SerialPort)

add_executable(atelier ${atelier_SRCS} ${interface_UI_SRCS} )

target_link_libraries(atelier AtelierInterface)
