cmake_minimum_required(VERSION 3.0.0)
project(OS_threads VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(serial_mult_matrix serial_mult_matrix.c matrix.c)
add_executable(parallel_mult_matrix parallel_mult_matrix.c matrix.c)
add_executable(race_condition race_condition.c)
add_executable(mutex mutex.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
