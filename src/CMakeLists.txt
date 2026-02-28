
add_subdirectory(dialogs)
add_subdirectory(widgets)

add_executable(atelier
    main.cpp
    mainwindow.cpp
    resources.qrc
)

if (APPLE)
    set(MACOSX_BUNDLE_DISPLAY_NAME "atelier")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.kde.atelier")
    set(MACOSX_BUNDLE_BUNDLE_NAME "atelier")
    set(MACOSX_BUNDLE_DISPLAY_NAME "atelier")
    set(MACOSX_BUNDLE_INFO_STRING "Atelier")
    set(MACOSX_BUNDLE_COPYRIGHT "2016-2018 The Atelier Authors")
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
