cmake_minimum_required(VERSION 3.10)
project(TradeMatrix LANGUAGES CXX)

# --------------------------------------------------------------------
# Build configuration
# --------------------------------------------------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default build type if not specified (Release / Debug)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# --------------------------------------------------------------------
# Include and source directories
# --------------------------------------------------------------------
include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SRC_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# --------------------------------------------------------------------
# Create executable
# --------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${SRC_FILES})

# --------------------------------------------------------------------
# Optional: link with pthread and filesystem if needed
# --------------------------------------------------------------------
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

# For GCC < 9, may need to link filesystem explicitly
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
endif()

# --------------------------------------------------------------------
# Compiler detection + flags
# --------------------------------------------------------------------
message(STATUS "===========================================")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Using Clang compiler: ${CMAKE_CXX_COMPILER}")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -Wpedantic
        -O2
        -Wshadow -Wconversion
        -Wnon-virtual-dtor -Wold-style-cast
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Using GCC compiler: ${CMAKE_CXX_COMPILER}")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -Wpedantic
        -O2
        -Wshadow -Wconversion
        -Wno-unused-parameter
        -fdiagnostics-color=always
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "Using Microsoft Visual C++ compiler")
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4 /permissive- /EHsc
    )
else()
    message(WARNING "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "===========================================")
