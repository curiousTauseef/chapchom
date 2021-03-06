# Check for cmake minimum required version (previously working with v2.8)
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

# ----------------------------------------------------------------------
# Project intialisation
# ----------------------------------------------------------------------

# Project name (this case library's name)
PROJECT(chapchom)
SET(${CMAKE_PROJECT_NAME}_MAJOR_VERSION 0)
SET(${CMAKE_PROJECT_NAME}_MINOR_VERSION 3)
SET(${CMAKE_PROJECT_NAME}_PATCH_LEVEL 0)

# Prints a lot of stuff
#SET(CMAKE_VERBOSE_MAKEFILE TRUE)

# ----------------------------------------------------------------------
# Output requested configuration
# ----------------------------------------------------------------------
MESSAGE( STATUS "" )
MESSAGE( STATUS "-----------------------------------------------------------------------" )
MESSAGE( STATUS "** GENERAL LIBRARY INFORMATION **" )
MESSAGE( STATUS "-----------------------------------------------------------------------" )
MESSAGE( STATUS "CHAPCHOM_LIB_TYPE:            " ${CHAPCHOM_LIB_TYPE} )
MESSAGE( STATUS "CHAPCHOM_BUILD_VERSION:       " ${CHAPCHOM_BUILD_VERSION} )
MESSAGE( STATUS "CHAPCHOM_BUILD_DEMOS:         " ${CHAPCHOM_BUILD_DEMOS} )
MESSAGE( STATUS "PROJECT_NAME:                 " ${PROJECT_NAME})
MESSAGE( STATUS "PROJECT_MAJOR_VERSION:        " ${chapchom_MAJOR_VERSION})
MESSAGE( STATUS "PROJECT_MINOR_VERSION:        " ${chapchom_MINOR_VERSION})
MESSAGE( STATUS "PROJECT_PATCH_LEVEL:          " ${chapchom_PATCH_LEVEL})
MESSAGE( STATUS "-----------------------------------------------------------------------" )

# ----------------------------------------------------------------------
# Read configuration file and extract values of variables
# ----------------------------------------------------------------------
MESSAGE( STATUS "" )
MESSAGE( STATUS "-----------------------------------------------------------------------" )
MESSAGE( STATUS "** READ VARIABLES FROM CONFIGURATION FILE: " ${CHAPCHOM_CONFIGURATION_FILE} " **")
MESSAGE( STATUS "-----------------------------------------------------------------------" )
file(STRINGS ${CHAPCHOM_CONFIGURATION_FILE} ConfigContents)
foreach(NameAndValue ${ConfigContents})
  # Strip leading spaces
  string(REGEX REPLACE "^[ ]+" "" NameAndValue ${NameAndValue})
  # Find variable name
  string(REGEX MATCH "^[^=]+" Name ${NameAndValue})
  # Find the value
  string(REPLACE "${Name}=" "" Value ${NameAndValue})
  # Set the variable
  set(${Name} "${Value}")
  MESSAGE( STATUS ${Name} " : " ${Value})
endforeach()

MESSAGE( STATUS "-----------------------------------------------------------------------" )

# ----------------------------------------------------------------------
# Do we want to compile using Armadillo?
# ----------------------------------------------------------------------
MESSAGE( STATUS "" )
MESSAGE( STATUS "-----------------------------------------------------------------------" )
MESSAGE( STATUS "** ARMADILLO'S LIBRARY INFORMATION **" )
MESSAGE( STATUS "-----------------------------------------------------------------------" )
IF (CHAPCHOM_USES_ARMADILLO)
  IF (CHAPCHOM_AUTO_FIND_ARMADILLO_PATHS)
    # Set the suggested folder where to look for Armadillo's library (where the
    # ArmadilloConfig.cmake) file lives
    LIST(APPEND CMAKE_PREFIX_PATH ${ARMADILLO_AUTO_FIND_FOLDER})
    # Try to find Armadillo to enable Armadillo wrappers
    FIND_PACKAGE(Armadillo REQUIRED)
    IF (ARMADILLO_FOUND)
      MESSAGE( STATUS "FOUND ARMADILLO_INCLUDE_DIRS:             " ${ARMADILLO_INCLUDE_DIRS} )
      MESSAGE( STATUS "FOUND ARMADILLO_LIBRARIES:                " ${ARMADILLO_LIBRARIES} )
      INCLUDE_DIRECTORIES(${ARMADILLO_INCLUDE_DIRS})
    ENDIF (ARMADILLO_FOUND)
  ELSE (CHAPCHOM_AUTO_FIND_ARMADILLO_PATHS)
    MESSAGE( STATUS "USING YOUR OWN ARMADILLO'S INCLUDE AND LIB PATHS")
    MESSAGE( STATUS "ARMADILLO_INCLUDE_DIRS:             " ${ARMADILLO_INCLUDE_DIRS} )
    MESSAGE( STATUS "ARMADILLO_LIBRARIES:                " ${ARMADILLO_LIBRARIES} )
    INCLUDE_DIRECTORIES(${ARMADILLO_INCLUDE_DIRS})
  ENDIF (CHAPCHOM_AUTO_FIND_ARMADILLO_PATHS)

  MESSAGE( STATUS "-----------------------------------------------------------------------" )
  MESSAGE( STATUS "" )
  MESSAGE( STATUS "-----------------------------------------------------------------------" )
  MESSAGE( STATUS "** THREADS'S LIBRARY INFORMATION **" )
  MESSAGE( STATUS "-----------------------------------------------------------------------" )
  # Find Threads packages (usually pthread)
  # Required by Armadillo
  FIND_PACKAGE(Threads REQUIRED)
  
ENDIF (CHAPCHOM_USES_ARMADILLO)

# ----------------------------------------------------------------------
# Do we want to compile using VTK? It should be a must
# ----------------------------------------------------------------------
MESSAGE( STATUS "" )
MESSAGE( STATUS "-----------------------------------------------------------------------" )
MESSAGE( STATUS "** VTK'S LIBRARY INFORMATION **" )
MESSAGE( STATUS "-----------------------------------------------------------------------" )
IF (CHAPCHOM_USES_VTK)
  IF (CHAPCHOM_AUTO_FIND_VTK_PATHS)
    # Set the suggested folder where to look for VTK's library (where the
    # VTKConfig.cmake) file lives
    LIST(APPEND CMAKE_PREFIX_PATH ${VTK_AUTO_FIND_FOLDER})
    # Try to find VTK to enable VTK wrappers
    FIND_PACKAGE(VTK 8.1 REQUIRED)
    IF (VTK_FOUND)
      MESSAGE( STATUS "FOUND VTK_INCLUDE_DIRS:             " ${VTK_INCLUDE_DIRS} )
      MESSAGE( STATUS "FOUND VTK_LIBRARIES:                " ${VTK_LIBRARIES} )
      INCLUDE_DIRECTORIES(${VTK_INCLUDE_DIRS})
    ENDIF (VTK_FOUND)
  ELSE (CHAPCHOM_AUTO_FIND_VTK_PATHS)
    MESSAGE( STATUS "USING YOUR OWN VTK'S INCLUDE AND LIB PATHS")
    MESSAGE( STATUS "VTK_INCLUDE_DIRS:             " ${VTK_INCLUDE_DIRS} )
    MESSAGE( STATUS "VTK_LIBRARIES:                " ${VTK_LIBRARIES} )
    INCLUDE_DIRECTORIES(${VTK_INCLUDE_DIRS})
  ENDIF (CHAPCHOM_AUTO_FIND_VTK_PATHS)
  
ENDIF (CHAPCHOM_USES_VTK)
MESSAGE( STATUS "-----------------------------------------------------------------------" )

# ----------------------------------------------------------------------

# ----------------------------------------------------------------------
# Modify compilation variables
# ----------------------------------------------------------------------
IF(${CHAPCHOM_BUILD_VERSION} STREQUAL DEBUG)
  SET(CHAPCHOM_C_FLAGS "-O0 -g -Wall -Wshadow")
  SET(CHAPCHOM_CXX_FLAGS "-O0 -g -Wall -Wshadow -std=c++11")
ELSEIF(${CHAPCHOM_BUILD_VERSION} STREQUAL RELEASE)
  SET(CHAPCHOM_C_FLAGS "-O3 -Wall -Wshadow -DNDEBUG")
  SET(CHAPCHOM_CXX_FLAGS "-O3 -Wall -Wshadow -DNDEBUG -std=c++11")
ELSEIF(${CHAPCHOM_BUILD_VERSION} STREQUAL PROFILING)
  SET(CHAPCHOM_C_FLAGS "-O0 -pg -Wall -Wshadow")
  SET(CHAPCHOM_CXX_FLAGS "-O0 -pg -Wall -Wshadow -std=c++11")
  MESSAGE( WARNING "You are compiling with profiling ENABLED, this will SIGNIFICANTLY increase execution times. If you do not want profiling to be ENABLED then choose another version when asked for library version" ) 
ELSE(${CHAPCHOM_BUILD_VERSION} STREQUAL DEBUG)
  SET(CHAPCHOM_C_FLAGS "-O0 -g -Wall -Wshadow")
  SET(CHAPCHOM_CXX_FLAGS "-O0 -g -Wall -Wshadow -std=c++11")
ENDIF(${CHAPCHOM_BUILD_VERSION} STREQUAL DEBUG)

#IF(${CHAPCHOM_BUILD_VERSION} STREQUAL DEBUG)
#  SET(CHAPCHOM_C_FLAGS "-O0 -g -Wall -Wextra -Wshadow -Wconversion -pedantic")
#  SET(CHAPCHOM_CXX_FLAGS "-O0 -g -Wall -Wextra -Wshadow -Wconversion -pedantic -std=c++11")
#ELSEIF(${CHAPCHOM_BUILD_VERSION} STREQUAL RELEASE)
#  SET(CHAPCHOM_C_FLAGS "-O3 -Wall -Wextra -Wshadow -Wconversion -pedantic -DNDEBUG")
#  SET(CHAPCHOM_CXX_FLAGS "-O3 -Wall -Wextra -Wshadow -Wconversion -pedantic -DNDEBUG -std=c++11")
#ELSEIF(${CHAPCHOM_BUILD_VERSION} STREQUAL PROFILING)
#  SET(CHAPCHOM_C_FLAGS "-O0 -pg -Wall -Wshadow -Wconversion -Wextra -pedantic")
#  SET(CHAPCHOM_CXX_FLAGS "-O0 -pg -Wall -Wshadow -Wconversion -Wextra -pedantic -std=c++11")
#  MESSAGE( WARNING "You are compiling with profiling ENABLED, this will SIGNIFICANTLY increase execution times. If you do not want profiling to be ENABLED then choose another version when asked for library version" ) 
#ELSE(${CHAPCHOM_BUILD_VERSION} STREQUAL DEBUG)
#  SET(CHAPCHOM_C_FLAGS "-O0 -g -Wall -Wextra -Wshadow -Wconversion -pedantic")
#  SET(CHAPCHOM_CXX_FLAGS "-O0 -g -Wall -Wextra -Wshadow -Wconversion -pedantic -std=c++11")
#ENDIF(${CHAPCHOM_BUILD_VERSION} STREQUAL DEBUG)

# Use this if you require to add more libraries (do not forget to
# uncomment the respective line below to set linker variables)
#SET(CHAPCHOM_LINKER_FLAGS "-lmath")

# ----------------------------------------------------------------------
# Append extra definitions based on configuration files
# ----------------------------------------------------------------------
SET(CHAPCHOM_RANGE_CHECK_DEFINITION "-DCHAPCHOM_RANGE_CHECK")
IF (CHAPCHOM_RANGE_CHECK)
  SET(CHAPCHOM_CXX_FLAGS "${CHAPCHOM_CXX_FLAGS} ${CHAPCHOM_RANGE_CHECK_DEFINITION}" )
ENDIF (CHAPCHOM_RANGE_CHECK)

SET(CHAPCHOM_USES_DOUBLE_PRECISION_DEFINITION "-DTYPEDEF_REAL_IS_DOUBLE")
IF (CHAPCHOM_USES_DOUBLE_PRECISION)
  SET(CHAPCHOM_CXX_FLAGS "${CHAPCHOM_CXX_FLAGS} ${CHAPCHOM_USES_DOUBLE_PRECISION_DEFINITION}" )
ENDIF (CHAPCHOM_USES_DOUBLE_PRECISION)

SET(CHAPCHOM_ARMADILLO_DEFINE "-DCHAPCHOM_USES_ARMADILLO")
IF (CHAPCHOM_USES_ARMADILLO)
  SET(CHAPCHOM_CXX_FLAGS "${CHAPCHOM_CXX_FLAGS} ${CHAPCHOM_ARMADILLO_DEFINE}" )
ENDIF (CHAPCHOM_USES_ARMADILLO)

SET(CHAPCHOM_VTK_DEFINE "-DCHAPCHOM_USES_VTK")
IF (CHAPCHOM_USES_VTK)
  SET(CHAPCHOM_CXX_FLAGS "${CHAPCHOM_CXX_FLAGS} ${CHAPCHOM_VTK_DEFINE}" )
ENDIF (CHAPCHOM_USES_VTK)

# ----------------------------------------------------------------------
# Set modified compilation variables
# ----------------------------------------------------------------------
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CHAPCHOM_C_FLAGS}" )
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CHAPCHOM_CXX_FLAGS}" )

# Uncomment this line if you added extra libraries
#SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

# ----------------------------------------------------------------------
# Include testing (this function already calls enable_testing())
# ----------------------------------------------------------------------
INCLUDE(CTest)

# ----------------------------------------------------------------------
# Output configuration
# ----------------------------------------------------------------------
MESSAGE( STATUS "" )
MESSAGE( STATUS "-----------------------------------------------------------------------" )
MESSAGE( STATUS "** GENERAL COMPILATION FLAGS **" )
MESSAGE( STATUS "-----------------------------------------------------------------------" )
# Output compilers versions
MESSAGE( STATUS "CMAKE_C_COMPILER:             " ${CMAKE_C_COMPILER} )
MESSAGE( STATUS "CMAKE_CXX_COMPILER:           " ${CMAKE_CXX_COMPILER} )

# Output configuration flags
# The flags passed to the C-compiler
MESSAGE( STATUS "CMAKE_C_FLAGS:                " ${CMAKE_C_FLAGS} )
# The flags passed to the C++ compiler
MESSAGE( STATUS "CMAKE_CXX_FLAGS:              " ${CMAKE_CXX_FLAGS} )
MESSAGE( STATUS "CMAKE_EXE_LINKER_FLAGS:       " ${CMAKE_EXE_LINKER_FLAGS} )

# Output some extra info.
MESSAGE( STATUS "CMAKE_VERBOSE_MAKEFILE:       " ${CMAKE_VERBOSE_MAKEFILE} )

MESSAGE( STATUS "-----------------------------------------------------------------------" )

# Add external source files sub-directories
ADD_SUBDIRECTORY(external_src)

# Add sub-directories with source files
ADD_SUBDIRECTORY(src)

# Add private folders (note that any source code in here is not
# compiled but only added to the make list)
ADD_SUBDIRECTORY(private)

# Once all source code has been compiled add sub-directories for demos
ADD_SUBDIRECTORY(demos)

MESSAGE( STATUS "" )
MESSAGE( STATUS "-----------------------------------------------------------------------" )
MESSAGE( STATUS "-----------------------------------------------------------------------" )
MESSAGE( STATUS "" )

