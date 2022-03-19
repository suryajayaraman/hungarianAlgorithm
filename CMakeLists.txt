project(hungarianAlgorithm)
cmake_minimum_required(VERSION 3.12)



add_executable(HUNGARIAN_ALGORITHM src/main.cpp src/hungarian.cpp)
target_include_directories(HUNGARIAN_ALGORITHM PUBLIC include)
