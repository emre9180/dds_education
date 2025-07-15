cmake_minimum_required(VERSION 3.14)
project(miniDDS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project directories
include_directories(include)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples) 
# Install headers
install(DIRECTORY include/ DESTINATION include) 