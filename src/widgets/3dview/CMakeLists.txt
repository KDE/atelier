
add_library(Atelier3Dqml STATIC
)
ecm_add_qml_module(Atelier3Dqml GENERATE_PLUGIN_SOURCE
    URI org.kde.atelier.viewer3d
    QML_FILES
        SceneEntity.qml
        Viewer3d.qml
)

set(3d_SRCS
    axisgnomonentity.cpp
    cameracontroller.cpp
    fileloader.cpp
    gcodeto4d.cpp
    gridmesh.cpp
    linemesh.cpp
    linemeshgeometry.cpp
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
    Qt::3DCore
    Qt::3DExtras
    Qt::3DRender
    Qt::3DInput
)
