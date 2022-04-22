project(hungarianAlgorithm)
cmake_minimum_required(VERSION 3.12)

set(CMAKE_BUILD_TYPE Debug)


# find dependent packages
find_package (Eigen3 3.3 REQUIRED)

# create Executable
set(SOURCES 
	src/main.cpp 
	src/hungarian.cpp
   )	

add_executable(HUNGARIAN_ALGORITHM ${SOURCES})
target_include_directories(HUNGARIAN_ALGORITHM PUBLIC include ${EIGEN_INCLUDE_DIR})
target_link_libraries(HUNGARIAN_ALGORITHM Eigen3::Eigen)
