cmake_minimum_required(VERSION 3.15)

project(ReplayAnalyzer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(.)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.c")

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/*.hpp" "src/*.h" "src/*.h" "src/*.hpp")

add_library(ReplayAnalyzer SHARED ${SOURCES} ${HEADERS})

set_target_properties(ReplayAnalyzer PROPERTIES
    PREFIX ""
    OUTPUT_NAME "ReplayAnalyzerDLL"
)

if(MSVC)
    target_compile_options(ReplayAnalyzer PUBLIC /W4)
else()
    target_compile_options(ReplayAnalyzer PUBLIC -m32 -O3 -Wall -Wextra)
endif()