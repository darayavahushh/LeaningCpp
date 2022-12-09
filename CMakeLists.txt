cmake_minimum_required(VERSION 3.0)

set(myproject "homework3")
# Project name
project(${myproject})

# Specify the required C++ standard
if(CMAKE_VERSION GREATER "3.1.2") 
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
else()
    if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    endif()    
endif()

add_library(lib INTERFACE)

add_executable(${myproject} hw.cpp)

target_link_libraries(${myproject} PUBLIC lib)

target_include_directories(${myproject} 
 PRIVATE src INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
	$<INSTALL_INTERFACE:inc>)

