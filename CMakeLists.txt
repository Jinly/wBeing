cmake_minimum_required (VERSION 2.6)

project (wBeing)

set (SOURCE_EXE source/main.cpp source/gui.cpp source/world.cpp
				source/animal.cpp)
include_directories (source)

# GTK2
find_package (GTK2 REQUIRED)
include_directories (${GTK2_INCLUDE_DIRS})

# wBeing
add_executable (wBeing ${SOURCE_EXE} ${GTK2_LIBRARIES})
target_link_libraries(wBeing ${GTK2_LIBRARIES})
