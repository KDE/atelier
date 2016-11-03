set(dialogs_SRCS
    generalsettingsdialog.cpp
    connectsettingsdialog.cpp
    )
add_library(AtelierDialogs STATIC ${dialogs_SRCS})
target_link_libraries(AtelierDialogs Qt5::Widgets Qt5::SerialPort)
