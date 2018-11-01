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

qt5_add_resources(3dfiles_RCS viewer3d.qrc)

add_library(Atelier3D STATIC ${3d_SRCS} ${3dfiles_RCS})

target_link_libraries(Atelier3D 
    Qt5::Core 
    Qt5::Qml
    Qt5::Quick  
    Qt5::Widgets
    Qt5::3DCore
    Qt5::3DExtras
    Qt5::3DRender
    Qt5::3DInput
)
