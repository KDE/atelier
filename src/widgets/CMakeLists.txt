include_directories(../dialogs)
include_directories(${QWT_INCLUDE_DIR})

set(widgets_SRCS
    gcodeeditorwidget.cpp
    bedextruderwidget.cpp
    videomonitorwidget.cpp
    atcoreinstancewidget.cpp
    thermowidget.cpp
    )

add_library(AtelierWidgets STATIC ${widgets_SRCS})

target_link_libraries(AtelierWidgets
    Qt5::Core
    Qt5::Widgets
    Qt5::SerialPort
    KF5::I18n
    KF5::TextEditor
    Qt5::Charts
    Qt5::Multimedia
    Qt5::MultimediaWidgets
    AtCore::AtCore
    ${QWT_LIBRARY}
    )

add_subdirectory(3dview)
