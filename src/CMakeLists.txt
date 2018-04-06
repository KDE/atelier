set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTORCC ON)
set(atelier_SRCS
    main.cpp
    mainwindow.cpp
    )

add_subdirectory(dialogs)
add_subdirectory(widgets)

add_executable(atelier ${atelier_SRCS} resources.qrc)

ecm_create_qm_loader(atelier_SRCS atelier)

target_link_libraries(atelier AtelierWidgets AtelierDialogs Atelier3D AtCoreWidgets AtCore::AtCore KF5::XmlGui KF5::ConfigWidgets)

install(TARGETS atelier RUNTIME DESTINATION bin)
