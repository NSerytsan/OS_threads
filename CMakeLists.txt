cmake_minimum_required(VERSION 3.0.0)
project(OS_threads VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(matrix mult_matrix.c)
add_executable(concurrency concurrency.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
