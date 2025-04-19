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
    KF6::I18n
    KF6::TextEditor
    Qt::Charts
    Qt::Core
    Qt::Multimedia
    Qt::MultimediaWidgets
    Qt::Widgets
)

if(WIN32)
    target_compile_definitions(AtelierWidgets PRIVATE)
endif()

add_subdirectory(3dview)
