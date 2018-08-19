include_directories(../src) 
add_executable(example main.cpp)
target_link_libraries(example asyncout pthread)
