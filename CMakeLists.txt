cmake_minimum_required(VERSION 3.0.0)
project(runfile VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(GCC_COVERAGE_COMPILE_FLAGS "-pthread -std=c++11")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
add_executable(runfile MainTrain.cpp CLI.cpp  SimpleAnomalyDetector.cpp anomaly_detection_util.cpp timeseries.cpp  minCircle.cpp HybridAnomalyDetector.cpp Server.cpp )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

