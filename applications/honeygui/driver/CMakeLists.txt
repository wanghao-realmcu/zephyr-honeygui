# Copyright (c) 2024 Realtek Semiconductor Corp.
# SPDX-License-Identifier: Apache-2.0


message(STATUS "User driver currect direction is: ${CMAKE_CURRENT_SOURCE_DIR}")


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

