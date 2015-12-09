cmake_minimum_required(VERSION 2.8.1)
project(book-manager)
enable_testing()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -DAUTOSAVE_SECONDS=1")


message("Build type is: ${CMAKE_BUILD_TYPE}")

add_subdirectory(tests)
add_subdirectory(src)
