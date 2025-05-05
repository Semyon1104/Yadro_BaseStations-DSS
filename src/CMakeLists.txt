add_library(computer_club_lib STATIC computer_club.cpp)

target_include_directories(computer_club_lib 
    PUBLIC "${PROJECT_SOURCE_DIR}/include"
)