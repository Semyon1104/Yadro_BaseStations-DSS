cmake_minimum_required(VERSION 3.10)
project(Computer_club)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(test)