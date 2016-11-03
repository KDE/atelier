set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(atelier_SRCS
    main.cpp
    atelier.cpp
    mainwindow.cpp
    )

add_subdirectory(dialogs)
add_subdirectory(widgets)

add_executable(atelier ${atelier_SRCS})

target_link_libraries(atelier AtelierWidgets AtelierDialogs atcore)
