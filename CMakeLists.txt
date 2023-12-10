cmake_minimum_required(VERSION 3.16)
project(BondPricer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_SHARED_MODULE_PREFIX "")

find_package(Boost COMPONENTS python3 REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

file(GLOB BondPricing_src src/*.cpp)
add_library(BondPricing MODULE
    ${BondPricing_src}
)
target_include_directories(BondPricing PRIVATE
    /usr/include/python3.8
    ${Python_INCLUDE_DIRS}
    include/
)   
target_link_libraries(BondPricing
    ${Boost_LIBRARIES}
    ${Python_LIBRARIES}
)

target_compile_options(BondPricing PRIVATE -Wall -Wno-undef -O3)
