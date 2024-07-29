# - Find STB_IMAGE
# Find the STB_IMAGE C++ library
#
# This module sets the following variables:
#  stb_image_FOUND - set to true if the library is found
#  stb_image_SOURCE_DIR - source directory for found library

file(GLOB STB_SEARCH_PATHS /usr/include/)
find_path(STB_IMAGE_INCLUDE_DIR_1 NAMES stb_image.h PATHS ${STB_SEARCH_PATHS})

if(STB_IMAGE_INCLUDE_DIR_1)
    set(stb_image_SOURCE_DIR "${STB_IMAGE_INCLUDE_DIR_1}" CACHE PATH "stb_image include directory")
else ()
    find_path(STB_IMAGE_INCLUDE_DIR_2
            NAMES stb_image.h
            PATHS ${STB_SEARCH_PATHS}
            PATH_SUFFIXES stb/)

    if(STB_IMAGE_INCLUDE_DIR_2)
        set(stb_image_SOURCE_DIR "${STB_IMAGE_INCLUDE_DIR_2}/stb" CACHE PATH "stb_image include directory")
    endif ()
endif ()

# Checks 'REQUIRED'.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(stb_image REQUIRED_VARS stb_image_SOURCE_DIR)

mark_as_advanced(stb_image_SOURCE_DIR)