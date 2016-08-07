###############################################################################
# CMake module to search for SOCI library
#
# WARNING: This module is experimental work in progress.
#
# This module defines:
#  SOCI_INCLUDE_DIRS        = include dirs to be used when using the soci library
#  SOCI_LIBRARY             = full path to the soci library
#  SOCI_VERSION             = the soci version found (not yet. soci does not provide that info.)
#  SOCI_FOUND               = true if soci was found
#
# This module respects:
#  LIB_SUFFIX         = (64|32|"") Specifies the suffix for the lib directory
# Currently this module searches for the following version numbers:
# 3_1
#
# For each component you specify in find_package(), the following variables are set.
#
#  SOCI_${COMPONENT}_PLUGIN = full path to the soci plugin
#  SOCI_${COMPONENT}_FOUND
#
# Copyright (c) 2011 Michael Jansen <info@michael-jansen.biz>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
# Changed by Alex Davydow - to work with 3_1 version
###############################################################################
#
### Global Configuration Section
#
SET(_SOCI_ALL_PLUGINS    mysql odbc postgresql sqlite3)
SET(_SOCI_REQUIRED_VARS  SOCI_INCLUDE_DIR SOCI_LIBRARY)

#
### FIRST STEP: Find the soci headers.
#
FIND_PATH(
    SOCI_INCLUDE_DIR soci.h
    PATH "/usr/local" "/usr/lib" "/usr/lib64" "/usr/include" "/mingw/local" "/mingw/lib" "/mingw/include" "/usr/local/include"
    PATH_SUFFIXES "" "soci"
    DOC "Soci (http://soci.sourceforge.net) include directory")
MARK_AS_ADVANCED(SOCI_INCLUDE_DIR)

SET(SOCI_INCLUDE_DIRS ${SOCI_INCLUDE_DIR})

#
### SECOND STEP: Find the soci core library. Respect LIB_SUFFIX
#
SET(SOCI_VERSION "3_1")

FIND_LIBRARY(
    SOCI_LIBRARY
    NAMES soci_core soci_core_${SOCI_VERSION} soci_core-gcc-3_0
    HINTS ${SOCI_INCLUDE_DIR}/.. 
    PATH_SUFFIXES lib64 lib lib${LIB_SUFFIX})
MARK_AS_ADVANCED(SOCI_LIBRARY)

GET_FILENAME_COMPONENT(SOCI_LIBRARY_DIR ${SOCI_LIBRARY} PATH)
MARK_AS_ADVANCED(SOCI_LIBRARY_DIR)

#
### THIRD STEP: Find all installed plugins if the library was found
#
IF(SOCI_INCLUDE_DIR AND SOCI_LIBRARY)

    MESSAGE(STATUS "Soci found: Looking for plugins")
    FOREACH(plugin IN LISTS _SOCI_ALL_PLUGINS)

        FIND_LIBRARY(
            SOCI_${plugin}_PLUGIN
            NAMES soci_${plugin} soci_${plugin}_${SOCI_VERSION} soci_${plugin}-gcc-3_0
            HINTS ${SOCI_INCLUDE_DIR}/..
            PATH_SUFFIXES lib64 lib lib${LIB_SUFFIX})
        MARK_AS_ADVANCED(SOCI_${plugin}_PLUGIN)

        IF(SOCI_${plugin}_PLUGIN)
            MESSAGE(STATUS "    * Plugin ${plugin} found ${SOCI_${plugin}_LIBRARY}.")
            SET(SOCI_${plugin}_FOUND True)
        ELSE()
            MESSAGE(STATUS "    * Plugin ${plugin} not found.")
            SET(SOCI_${plugin}_FOUND False)
        ENDIF()

    ENDFOREACH()

    #
    ### FOURTH CHECK: Check if the required components were all found
    #
    FOREACH(component ${Soci_FIND_COMPONENTS})
        IF(${SOCI_${component}_FOUND})
            # Does not work with NOT ... . No idea why.
        ELSE()
            MESSAGE(SEND_ERROR "Required component ${component} not found.")
        ENDIF()
    ENDFOREACH()

ENDIF()

#
### ADHERE TO STANDARDS
#
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Soci DEFAULT_MSG ${_SOCI_REQUIRED_VARS})

