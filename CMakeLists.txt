
cmake_minimum_required(VERSION 2.8)
 
PROJECT(ContourPointsWidget)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(ContourPointsWidget MACOSX_BUNDLE ContourPointsWidget.cxx )
 
target_link_libraries(ContourPointsWidget ${VTK_LIBRARIES})
