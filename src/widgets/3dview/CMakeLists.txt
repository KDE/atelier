
add_library(Atelier3Dqml STATIC
    axisgnomonentity.cpp
    cameracontroller.cpp
    fileloader.cpp
    gcodeto4d.cpp
    gridmesh.cpp
    linemesh.cpp
    linemeshgeometry.cpp
)
ecm_add_qml_module(Atelier3Dqml GENERATE_PLUGIN_SOURCE
    URI org.kde.atelier.viewer3d
    QML_FILES
        SceneEntity.qml
        Viewer3d.qml
    DEPENDENCIES
        Qt3D.Core
        Qt3D.Extras
)
target_link_libraries(Atelier3Dqml PRIVATE
    Qt::Core
    Qt::Gui
    Qt::3DCore
    Qt::3DExtras
    Qt::3DRender
)

set(3d_SRCS
    viewer3d.cpp
    viewer3d.qrc
)

add_library(Atelier3D STATIC ${3d_SRCS})

target_link_libraries(Atelier3D 
    Atelier3Dqml
    Atelier3Dqmlplugin
    Qt::Core
    Qt::Qml
    Qt::Quick
    Qt::Widgets
)
