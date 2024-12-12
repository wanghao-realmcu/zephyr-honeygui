# Copyright (c) 2024 Realtek Semiconductor Corp.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.10)

# Define a variable for the library name

file(GLOB SOURCES "*.c" "*.cpp")
file(GLOB HEADERS "*.h" "*.hpp")


# Determine if we should create a library
if((NOT HEADERS) AND (NOT SOURCES))
	message(STATUS "No source or header files found in ${CMAKE_CURRENT_SOURCE_DIR}.")
endif()

if(HEADERS AND (NOT SOURCES))
	zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(SOURCES AND (NOT HEADERS))
	zephyr_library_sources(${SOURCES})
endif()

if(SOURCES AND HEADERS)
	zephyr_library_sources(${SOURCES})
	zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
endif()

# Process subdirectories regardless
file(GLOB SUBDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach(subdir ${SUBDIRS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeLists.txt)
            add_subdirectory(${subdir})
        else()
            message(STATUS "Skipping directory ${subdir} as it does not contain a CMakeLists.txt file.")
        endif()
    endif()
endforeach()
