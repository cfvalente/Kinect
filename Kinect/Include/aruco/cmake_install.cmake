# Install script for directory: C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/src

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "main")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/bin/Debug/aruco125.lib")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/bin/Release/aruco125.lib")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/bin/MinSizeRel/aruco125.lib")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/bin/RelWithDebInfo/aruco125.lib")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "main")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "main")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/bin/Debug/aruco125.dll")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/bin/Release/aruco125.dll")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/bin/MinSizeRel/aruco125.dll")
  ELSEIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/bin/RelWithDebInfo/aruco125.dll")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "main")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "main")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/aruco" TYPE FILE FILES
    "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/src/aruco.h"
    "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/src/arucofidmarkers.h"
    "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/src/ar_omp.h"
    "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/src/board.h"
    "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/src/boarddetector.h"
    "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/src/cameraparameters.h"
    "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/src/chromaticmask.h"
    "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/src/cvdrawingutils.h"
    "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/src/exports.h"
    "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/src/highlyreliablemarkers.h"
    "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/src/marker.h"
    "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/src/markerdetector.h"
    "C:/Users/Caio/Downloads/aruco-1.2.5/aruco-1.2.5/src/subpixelcorner.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "main")

