set(widgets_SRCS
    toolbarwidget.cpp
    gcodeeditorwidget.cpp
    temporaryprintercontrolwidget.cpp
    )

add_library(AtelierWidgets STATIC ${widgets_SRCS})

target_link_libraries(AtelierWidgets Qt5::Core Qt5::Widgets Qt5::SerialPort KF5::Solid)
