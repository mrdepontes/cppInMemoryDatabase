cmake_minimum_required(VERSION 3.0.0)
project(MAIN VERSION 0.1.0)

include(CTest)
enable_testing()
set(BOOST_ROOT "path_to_boost\\boost_1_78_0")
set(BOOST_LIBRARYDIR "path_to_boost\\boost_1_78_0\\stage\\lib")

link_directories(${BOOST_ROOT})
link_directories(${BOOST_LIBRARYDIR})

set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_MULTITHREADED ON)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# https://cmake.org/cmake/help/latest/policy/CMP0074.html
cmake_policy(SET CMP0074 NEW)
find_package(Boost 1.78.0 REQUIRED)

add_subdirectory(
    lib
    )

if(Boost_FOUND)
  message(STATUS "Found Boost use static libraries. ex <boost/json.hpp>")
  include_directories(${Boost_INCLUDE_DIRS})
  add_executable(${PROJECT_NAME} main.cpp)
  
  target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC ${Boost_LIBRARIES}
    )

  target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC IN_PLACE_DATABASE
        )

    target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
endif()  

