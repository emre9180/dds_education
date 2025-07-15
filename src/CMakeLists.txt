file(GLOB SOURCES "*.cpp")
add_library(miniDDS STATIC ${SOURCES})
target_include_directories(miniDDS PUBLIC ${CMAKE_SOURCE_DIR}/include) 