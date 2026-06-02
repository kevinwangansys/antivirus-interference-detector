cmake_minimum_required(VERSION 3.20)
project(AVInterferenceDetectorPoC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific settings
if(MSVC)
    add_compile_options(/W4 /WX /permissive-)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Zi")
else()
    add_compile_options(-Wall -Wextra -Werror)
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# ProcessMonitorDemo
add_executable(ProcessMonitorDemo 
    src/ProcessMonitorDemo.cpp
)
target_link_libraries(ProcessMonitorDemo PRIVATE
    kernel32
    user32
    psapi
)

# AVDetectionDemo
add_executable(AVDetectionDemo 
    src/AVDetectionDemo.cpp
)
target_link_libraries(AVDetectionDemo PRIVATE
    kernel32
    user32
    advapi32
    ole32
    oleaut32
    wbemuuid
    wmiutils
)

# PerformanceMetricsDemo
add_executable(PerformanceMetricsDemo 
    src/PerformanceMetricsDemo.cpp
)
target_link_libraries(PerformanceMetricsDemo PRIVATE
    kernel32
    user32
    psapi
)

# Set output directory
set_target_properties(ProcessMonitorDemo AVDetectionDemo PerformanceMetricsDemo
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Testing
enable_testing()

# Add tests
add_test(
    NAME ProcessMonitorDemo_Help
    COMMAND ProcessMonitorDemo
)

add_test(
    NAME AVDetectionDemo_Run
    COMMAND AVDetectionDemo
)

add_test(
    NAME PerformanceMetricsDemo_Help
    COMMAND PerformanceMetricsDemo
)
