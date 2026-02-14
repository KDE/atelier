
set(atelier_SRCS
    main.cpp
    mainwindow.cpp
)

add_subdirectory(dialogs)
add_subdirectory(widgets)

ecm_create_qm_loader(atelier_SRCS atelier)

if (NOT APPLE)
    add_executable(atelier ${atelier_SRCS} resources.qrc)
    install(TARGETS atelier RUNTIME DESTINATION bin)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++11")
    set(MACOSX_BUNDLE_DISPLAY_NAME "atelier")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.kde.atelier")
    set(MACOSX_BUNDLE_BUNDLE_NAME "atelier")
    set(MACOSX_BUNDLE_DISPLAY_NAME "atelier")
    set(MACOSX_BUNDLE_ICON_FILE atelier.icns)
    set(MACOSX_BUNDLE_INFO_STRING "Atelier")
    set(MACOSX_BUNDLE_COPYRIGHT "2016-2018 The Atelier Authors")
    set(APPICON ${CMAKE_CURRENT_SOURCE_DIR}/atelier.icns)
    set_source_files_properties(${APPICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    add_executable(atelier MACOSX_BUNDLE ${atelier_SRCS} resources.qrc ${APPICON})
    install(TARGETS atelier BUNDLE DESTINATION bin)
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
