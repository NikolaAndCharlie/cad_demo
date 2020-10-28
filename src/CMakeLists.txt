
add_subdirectory(3rdparty/dxflib)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 
  COMPONENTS 
    Core
    Widgets 
    Gui
    Sql
    Xml
  REQUIRED
)

option(ACCEPT_VCA_ARC_BUG "FindArcTool's endAngle is actual sweepAngle in VCA." ON)

add_executable(CADPlan WIN32)
target_sources(CADPlan
  PRIVATE
    main.cc
    dxf_importer.h
    dxf_importer.cc
    entity.h
    line_entity.h
    arc_entity.h
    circle_entity.h
    entity.cc
    line_entity.cc
    line_entity.cc
    circle_entity.cc
    arc_entity.cc
    entity_view.h
    entity_view.cc
    entity_model.h
    entity_model.cc
    main_window.h
    main_window.cc
)

if(ACCEPT_VCA_ARC_BUG)
  target_compile_definitions(CADPlan PUBLIC ACCEPT_VCA_ARC_ERROR)
endif()

target_link_libraries(CADPlan 
  dxflib
  Qt5::Core 
  Qt5::Widgets 
  Qt5::Gui
  Qt5::Sql
  Qt5::Xml
)

