 
install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/atelierui.rc"
    DESTINATION ${KXMLGUI_INSTALL_DIR}/${PROJECT_NAME}/)

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/atelier.menu"
    RENAME atelier
    DESTINATION share/menu)
    
install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/atelier.png"
    DESTINATION share/pixmaps)
    
install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/org.kde.atelier.desktop"
    DESTINATION share/applications)

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/org.kde.atelier.appdata.xml"
    DESTINATION ${KDE_INSTALL_METAINFODIR})
