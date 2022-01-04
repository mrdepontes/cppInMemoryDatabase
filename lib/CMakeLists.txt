cmake_minimum_required(VERSION 3.0.0)
project(IN_PLACE_DATABASE VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(IN_PLACE_DATABASE IN_PLACE_DATABASE.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
