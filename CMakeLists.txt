cmake_minimum_required(VERSION 3.10)
project(canny-cpu)

find_package(OpenCV REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES  main.cpp canny.hpp canny.cpp io.hpp io.cpp)

add_executable(canny_cpu ${SOURCE_FILES})

target_link_libraries(canny_cpu PRIVATE ${OpenCV_LIBS})