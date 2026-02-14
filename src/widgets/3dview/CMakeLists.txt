set(3d_SRCS
    axisgnomonentity.cpp
    bedproperties.cpp
    cameracontroller.cpp
    fileloader.cpp
    gcodeto4d.cpp
    gridmesh.cpp
    linemesh.cpp
    linemeshgeometry.cpp
    viewer3d.cpp
)

add_library(Atelier3D STATIC ${3d_SRCS})
ecm_add_qml_module(Atelier3D GENERATE_PLUGIN_SOURCE
    URI org.kde.atelier.viewer3d
    QML_FILES
        SceneEntity.qml
        Viewer3d.qml
    RESOURCES
        axisgnomon.gltf
        axisgnomon.bin
        axisgnomon.vert
)

target_link_libraries(Atelier3D PRIVATE
    Qt::Core
    Qt::Qml
    Qt::Quick
    Qt::Widgets
    Qt::3DCore
    Qt::3DExtras
    Qt::3DRender
    Qt::3DInput
)
