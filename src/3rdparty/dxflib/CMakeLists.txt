add_library(dxflib STATIC)
target_sources(dxflib   
  PRIVATE
    dl_attributes.h
    dl_codes.h
    dl_creationadapter.h
    dl_creationinterface.h
    dl_dxf.h
    dl_entities.h
    dl_exception.h
    dl_extrusion.h
    dl_writer.h
    dl_writer_ascii.h
    dl_dxf.cpp
    dl_writer_ascii.cpp
)


