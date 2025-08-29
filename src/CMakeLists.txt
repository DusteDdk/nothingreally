
add_library(writer STATIC filist.cpp filist.hpp)
add_executable(test test.cpp)


target_link_libraries(test PRIVATE
	writer

)
