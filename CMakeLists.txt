cmake_minimum_required(VERSION 3.10)
project(log-plugin LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Target architecture handling
if(NOT DEFINED TARGET_ARCH OR TARGET_ARCH STREQUAL "")
    set(TARGET_ARCH "x86")
endif()

# Platform-specific compiler flags
if(UNIX)
    # Add -m32 flag only for x86 architecture on Unix platforms
    if(TARGET_ARCH STREQUAL "x86")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
    endif()

    # Linux-specific flags
    add_compile_definitions(LINUX)
    add_compile_options(-w -fshort-wchar)
    set(CMAKE_C_FLAGS_RELEASE "-O3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
elseif(WIN32)
    # Windows-specific definitions
    add_compile_definitions(WIN32 _WINDOWS _USRDLL LOG_PLUGIN_EXPORTS)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
endif()

# Include directories
include_directories(
    libs/sdk
    libs/sdk/amx
    libs/fmt/include
)

# Source files
set(SOURCES
    src/main.cpp
    src/PluginLog.cpp
    src/PluginLog.hpp
    src/LogManager.cpp
    src/LogManager.hpp
    src/natives.cpp
    src/natives.hpp
    src/SampConfigReader.cpp
    src/SampConfigReader.hpp
    src/Singleton.hpp
    src/sdk.hpp
    src/version.hpp
    libs/sdk/amxplugin.cpp
)

# Create shared library
add_library(log-plugin SHARED ${SOURCES})

add_subdirectory(libs/log-core)
target_link_libraries(log-plugin log-core)
target_include_directories(log-plugin PUBLIC ./libs/log-core/include)

# add_subdirectory(libs/fmt)
target_link_libraries(log-plugin fmt)

include(CheckIncludeFile)

check_include_file(alloca.h HAVE_ALLOCA_H)
if(HAVE_ALLOCA_H)
    add_definitions(-DHAVE_ALLOCA_H)
endif()

check_include_file(inttypes.h HAVE_INTTYPES_H)
if(HAVE_INTTYPES_H)
    add_definitions(-DHAVE_INTTYPES_H)
endif()

check_include_file(malloc.h HAVE_MALLOC_H)
if(HAVE_MALLOC_H)
    add_definitions(-DHAVE_MALLOC_H)
endif()

check_include_file(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

add_definitions(-DPAWN_CELL_SIZE=32)

# Windows-specific configuration
if(WIN32)
    # Set module definition file for Win32 platform
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set_target_properties(log-plugin PROPERTIES
            LINK_FLAGS "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/src/plugin.def"
        )
    endif()

    # Set character set to Unicode
    target_compile_definitions(log-plugin PRIVATE UNICODE _UNICODE)

    # MSVC-specific definitions
    if(MSVC)
        target_compile_definitions(log-plugin PRIVATE
            _CRT_SECURE_NO_WARNINGS
            NOMINMAX
            _WIN32_WINNT=0x0600
            _SCL_SECURE_NO_WARNINGS
						_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
        )
    endif()
endif()

# Set output name
if(UNIX)
    set_target_properties(log-plugin PROPERTIES
        PREFIX ""
        OUTPUT_NAME "log-plugin"
        SUFFIX ".so"
    )
elseif(WIN32)
    set_target_properties(log-plugin PROPERTIES
        OUTPUT_NAME "log-plugin"
    )
endif()
