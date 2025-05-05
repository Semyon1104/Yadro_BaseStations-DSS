cmake_minimum_required(VERSION 3.10)
project(Computer_club)

# Установка стандарта C++ и настроек Runtime Library
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    # Установка одинакового типа Runtime Library для всех компонентов
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

include_directories("${PROJECT_SOURCE_DIR}/include")

# Настройка Google Test перед добавлением поддиректории
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(3rdparty/googletest)

add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(test)