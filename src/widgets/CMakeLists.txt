include_directories(../dialogs)

set(widgets_SRCS
    atcoreinstancewidget.cpp
    bedextruderwidget.cpp
    gcodeeditorwidget.cpp
    thermowidget.cpp
    qcgaugewidget.cpp
    videomonitorwidget.cpp
    welcomewidget.cpp
)

add_library(AtelierWidgets STATIC ${widgets_SRCS})

target_link_libraries(AtelierWidgets
    AtCore::AtCore
    KF5::I18n
    KF5::TextEditor
    Qt5::Charts
    Qt5::Core
    Qt5::Multimedia
    Qt5::MultimediaWidgets
    Qt5::Widgets
)

if(WIN32)
    target_compile_definitions(AtelierWidgets PRIVATE)
endif()

add_subdirectory(3dview)
