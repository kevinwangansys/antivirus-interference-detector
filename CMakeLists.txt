cmake_minimum_required(VERSION 3.20)
project(AVInterferenceDetector VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific settings
if(MSVC)
    add_compile_options(/W4 /WX /permissive-)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Zi /DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od")
else()
    add_compile_options(-Wall -Wextra -Werror -pedantic)
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
endif()

# Project directories
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Include directories
include_directories(${INCLUDE_DIR})

# Core library
add_library(AVInterferenceDetectorCore STATIC
    # Core components
    ${SOURCE_DIR}/core/ProcessMonitor.cpp
    ${SOURCE_DIR}/core/AVDetector.cpp
    ${SOURCE_DIR}/core/AVActivityMonitor.cpp
    ${SOURCE_DIR}/core/PerformanceCollector.cpp
    ${SOURCE_DIR}/core/InterferenceAnalyzer.cpp
    ${SOURCE_DIR}/core/ReportGenerator.cpp
    # Models
    ${SOURCE_DIR}/models/AVEvent.cpp
    ${SOURCE_DIR}/models/ProcessEvent.cpp
    ${SOURCE_DIR}/models/Metrics.cpp
    # Utils
    ${SOURCE_DIR}/utils/EventLogHelper.cpp
    ${SOURCE_DIR}/utils/RegistryHelper.cpp
    ${SOURCE_DIR}/utils/WMIHelper.cpp
)

target_include_directories(AVInterferenceDetectorCore PUBLIC ${INCLUDE_DIR})

# Link Windows libraries
target_link_libraries(AVInterferenceDetectorCore PRIVATE
    kernel32
    user32
    advapi32
    psapi
    ole32
    oleaut32
    wbemuuid
    wmiutils
    wevtapi
)

# CLI executable
add_executable(AVDetector 
    ${SOURCE_DIR}/cli/main.cpp
    ${SOURCE_DIR}/cli/CommandHandler.cpp
)
target_link_libraries(AVDetector PRIVATE AVInterferenceDetectorCore)

# Set output directories
set_target_properties(AVInterferenceDetectorCore AVDetector
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# PoC targets (optional)
add_subdirectory(poc/cpp)

# Testing
enable_testing()
add_subdirectory(tests)

# Installation rules
install(TARGETS AVDetector DESTINATION bin)
install(DIRECTORY ${INCLUDE_DIR}/ DESTINATION include)

# Summary
message(STATUS "")
message(STATUS "============ AV Interference Detector ============")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "")
message(STATUS "Core Library: AVInterferenceDetectorCore")
message(STATUS "CLI Application: AVDetector")
message(STATUS "PoC Demos: Enabled")
message(STATUS "")
message(STATUS "Build Output: ${CMAKE_BINARY_DIR}/bin")
message(STATUS "==================================================")
