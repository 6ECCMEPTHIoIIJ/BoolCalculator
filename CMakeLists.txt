cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME "bool_calculator")
project(${PROJECT_NAME} VERSION "0.3.0" LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED on)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_subdirectory("sources")

target_include_directories(${PROJECT_NAME} PRIVATE "include" "tests")
set_target_properties(${PROJECT_NAME}
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/debug"
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/release"
                      RUNTIME_OUTPUT_NAME_DEBUG "${PROJECT_NAME}_debug")