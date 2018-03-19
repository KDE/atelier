set(dialogs_SRCS
    profilesdialog.cpp
    choosefiledialog.cpp
    )
add_library(AtelierDialogs STATIC ${dialogs_SRCS})
target_link_libraries(AtelierDialogs Qt5::Widgets Qt5::SerialPort KF5::I18n)
