set(dialogs_SRCS
    choosefiledialog.cpp
    profilesdialog.cpp
)

add_library(AtelierDialogs STATIC ${dialogs_SRCS})

target_link_libraries(AtelierDialogs 
    AtCore::AtCore
    KF6::I18n
    Qt::Widgets
)
