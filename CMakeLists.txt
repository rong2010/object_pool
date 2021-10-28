cmake_minimum_required(VERSION 3.0)
project(object_pool)
set(TEST_NAME test)
add_executable(${TEST_NAME} src/test.cc src/object_pool.h)
target_link_libraries(${TEST_NAME}
    pthread
)




