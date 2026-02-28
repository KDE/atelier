
set(atelier_SRCS
    main.cpp
    mainwindow.cpp
)

add_subdirectory(dialogs)
add_subdirectory(widgets)

if (NOT APPLE)
    add_executable(atelier ${atelier_SRCS} resources.qrc)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++11")
    set(MACOSX_BUNDLE_DISPLAY_NAME "atelier")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.kde.atelier")
    set(MACOSX_BUNDLE_BUNDLE_NAME "atelier")
    set(MACOSX_BUNDLE_DISPLAY_NAME "atelier")
    set(MACOSX_BUNDLE_INFO_STRING "Atelier")
    set(MACOSX_BUNDLE_COPYRIGHT "2016-2018 The Atelier Authors")
    add_executable(atelier MACOSX_BUNDLE ${atelier_SRCS} resources.qrc)
endif()

target_link_libraries(atelier 
    Atelier3D
    Atelier3Dplugin
    AtelierDialogs
    AtelierWidgets
    AtCore::AtCore
    AtCore::AtCoreWidgets
    KF6::ConfigWidgets
    KF6::XmlGui
)

ecm_add_app_icon(atelier ICONS 256-apps-org.kde.atelier.png)

install(TARGETS atelier ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

ecm_install_icons(ICONS 256-apps-org.kde.atelier.png DESTINATION ${KDE_INSTALL_ICONDIR})
