cmake_minimum_required(VERSION 3.0.0)
project(anomaly_detection_util VERSION 0.1.0)

add_library(anomaly_detection_util SHARED anomaly_detection_util.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

