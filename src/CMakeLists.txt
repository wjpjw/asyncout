add_subdirectory(util)
aux_source_directory(. DIRSRCS)
add_library(asyncout SHARED ${DIRSRCS})
target_link_libraries(asyncout util)

