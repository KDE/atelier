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

    viewer3d.qrc
)

add_library(Atelier3D STATIC ${3d_SRCS})

target_link_libraries(Atelier3D 
    Qt::Core
    Qt::Qml
    Qt::Quick
    Qt::Widgets
    Qt::3DCore
    Qt::3DExtras
    Qt::3DRender
    Qt::3DInput
)
