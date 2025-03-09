cmake_minimum_required(VERSION 3.4...3.18)
project(quick_cliques)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

set(SOURCES
    src/CliqueTools.cpp
    src/MemoryManager.cpp
    src/Algorithm.cpp
    src/TomitaAlgorithm.cpp
    src/AdjacencyListAlgorithm.cpp
    src/HybridAlgorithm.cpp
    src/DegeneracyAlgorithm.cpp
    src/DegeneracyTools.cpp
    src/Tools.cpp
    src/python_bindings.cpp
)

pybind11_add_module(quick_cliques_cpp ${SOURCES})

target_include_directories(quick_cliques_cpp PRIVATE src)

# Define preprocessor macros
target_compile_definitions(quick_cliques_cpp PRIVATE
    PRINT_CLIQUES_ONE_BY_ONE
    ALLOW_ALLOC_ZERO_BYTES
    RETURN_CLIQUES_ONE_BY_ONE
)

if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(quick_cliques_cpp PRIVATE -O2 -Winline)
endif()

install(TARGETS quick_cliques_cpp DESTINATION .)
