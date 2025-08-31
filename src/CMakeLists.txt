
add_library(fillist STATIC fillist.cpp fillist.hpp)
add_executable(example example.cpp)


target_link_libraries(example PRIVATE
	fillist

)
