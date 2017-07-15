set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(atelier_SRCS
    main.cpp
    mainwindow.cpp
    )

add_subdirectory(dialogs)
add_subdirectory(widgets)

add_executable(atelier ${atelier_SRCS})

ecm_create_qm_loader(atelier_SRCS atelier)

target_link_libraries(atelier AtelierWidgets AtelierDialogs AtCore::AtCore KF5::Solid KF5::XmlGui KF5::ConfigWidgets)

install(TARGETS atelier RUNTIME DESTINATION bin)
