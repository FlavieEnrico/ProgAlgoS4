# ---Choose CMake version---
cmake_minimum_required(VERSION 3.20)

# ---Choose project name---
project(Les-Petits-Boids)

# ---Declare source files---
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*)
file(GLOB_RECURSE COMMON_FILES CONFIGURE_DEPENDS src-common/*)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${COMMON_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE src)

# ---Choose C++ version---
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# ---Choose warning level---
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors)
endif()

# ---Maybe enable warnings as errors---
set(WARNINGS_AS_ERRORS OFF CACHE BOOL "ON iff you want to treat warnings as errors")

if(WARNINGS_AS_ERRORS)
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /WX)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
    endif()
endif()

# ---Setup Testing---
include(FetchContent)
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest
    GIT_TAG b7c21ec5ceeadb4951b00396fc1e4642dd347e5f
)
FetchContent_MakeAvailable(doctest)
target_link_libraries(${PROJECT_NAME} PRIVATE doctest::doctest)

# ---Add p6 library---
FetchContent_Declare(
    p6
    GIT_REPOSITORY https://github.com/julesfouchy/p6
    GIT_TAG f96a287c0874fffb9fa6e3794c4a5c151069dc0c
)
FetchContent_MakeAvailable(p6)
target_link_libraries(${PROJECT_NAME} PRIVATE p6::p6)


# ---Set the output folder---
set_target_properties(${PROJECT_NAME} PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

# ---Copy the assets and the shaders to the output folder (where the executable is created)---
Cool__target_copy_folder(${PROJECT_NAME} assets)
Cool__target_copy_folder(${PROJECT_NAME} shaders)