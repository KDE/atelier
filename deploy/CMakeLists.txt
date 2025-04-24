ecm_install_icons(ICONS
    "${CMAKE_CURRENT_SOURCE_DIR}/256-apps-org.kde.atelier.png"
    DESTINATION ${KDE_INSTALL_ICONDIR})

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/org.kde.atelier.desktop"
    DESTINATION ${KDE_INSTALL_APPDIR})

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/org.kde.atelier.appdata.xml"
    DESTINATION ${KDE_INSTALL_METAINFODIR})
