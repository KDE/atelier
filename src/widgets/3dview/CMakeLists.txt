set(3d_SRCS
    gcodeto4d.cpp
    fileloader.cpp
    linemesh.cpp
    gridmesh.cpp
    linemeshgeometry.cpp
    viewer3d.cpp
) 

file(GLOB 3d_SRC_QML
    AnimatedEntity.qml
    viewer3d.qml
    )

qt5_add_resources(3dfiles_RCS viewer3d.qrc)

add_library(Atelier3D STATIC ${3d_SRCS} ${3dfiles_RCS} ${3d_SRC_QML})

target_link_libraries(Atelier3D 
    Qt5::Core 
    Qt5::Widgets
    Qt5::Quick  
    Qt5::Qml
    Qt5::3DCore
    Qt5::3DExtras
    Qt5::3DRender
    Qt5::3DInput
    )
