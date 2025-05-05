enable_testing()

add_executable(computer_club_tests tests.cpp)

# Явная настройка Runtime Library для тестов
if(MSVC)
    set_target_properties(computer_club_tests PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )
endif()

target_link_libraries(computer_club_tests
    PRIVATE 
    computer_club_lib 
    gtest 
    gtest_main
)