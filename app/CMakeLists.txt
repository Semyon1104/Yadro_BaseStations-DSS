add_executable(computer_club_app sample.cpp)

target_link_libraries(computer_club_app 
    PRIVATE computer_club_lib
)