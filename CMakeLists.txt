cmake_minimum_required(VERSION 3.24)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

project(VulkanDay1)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Package Dependencies
find_package(Vulkan REQUIRED COMPONENTS volk shaderc_combined)
find_package(SDL3 CONFIG REQUIRED)

# Include Directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source Files
file(GLOB_RECURSE USER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# Executable Target
add_executable(${PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${USER_SOURCES}
)

# Compiler Options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /std:c++latest)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    Vulkan::Vulkan
    Vulkan::volk
    Vulkan::shaderc_combined
    SDL3::SDL3
)

# copy assets folder to build directory after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMENT "Copying assets folder to build directory"
)
