cmake_minimum_required(VERSION 3.4)
project(CAL1516T2GE)

set(src_dir       src)
set(resources_dir resources)
file(GLOB_RECURSE sources ${src_dir}/*.cpp ${src_dir}/*.h ${src_dir}/*.hpp)
file(GLOB_RECURSE data    ${resources_dir}/*)

set(executable_name cal)
add_executable(${executable_name} ${sources} ${data})

target_compile_options(${executable_name} PUBLIC "-std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -pedantic -Wdouble-promotion -Wshadow -Wfloat-equal -Wcast-align -Wcast-qual -Wwrite-strings -Wconversion -Wsign-conversion -Wuseless-cast -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wdisabled-optimization -Wstack-protector -Winline -Wswitch-default -Wswitch-enum")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

file(COPY ${data} DESTINATION ${resources_dir})

list(APPEND
  CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/cmake"
)
find_package(CURL REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(CURLpp REQUIRED)

target_link_libraries(${executable_name}
  PUBLIC ${Boost_LIBRARIES}
  PUBLIC ${CURL_LIBRARIES}
  PUBLIC ${CURLPP_LIBRARIES}
)
