set(widgets_SRCS
    gcodeeditorwidget.cpp
    printerhostpositionvisualcontroller.cpp
    bedextruderwidget.cpp
    plotwidget.cpp
    pushgcodewidget.cpp
    ratescontrolwidget.cpp
    printprogresswidget.cpp
    )

add_library(AtelierWidgets STATIC ${widgets_SRCS})

target_link_libraries(AtelierWidgets Qt5::Core Qt5::Widgets Qt5::SerialPort KF5::Solid KF5::I18n KF5::TextEditor Qt5::Charts)
