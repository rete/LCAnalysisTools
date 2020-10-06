########################################################
# cmake file for building LCAnalysisTools
# @author Remi Ete, DESY
cmake_minimum_required( VERSION 3.14 FATAL_ERROR )
########################################################

# project name
project( LCAnalysisTools LANGUAGES CXX )

# project version
set( ${PROJECT_NAME}_VERSION_MAJOR 0 )
set( ${PROJECT_NAME}_VERSION_MINOR 0 )
set( ${PROJECT_NAME}_VERSION_PATCH 1 )

### DEPENDENCIES ############################################################

# options
option( INSTALL_DOC                  "Set to OFF to skip build/install Documentation" OFF )

find_package( ILCUTIL REQUIRED COMPONENTS streamlog ILCSOFT_CMAKE_MODULES )
find_package( LCIO REQUIRED )
find_package( Marlin 01.17 )
# find_package( ROOT 6.16 REQUIRED )
# find_package( MarlinMT REQUIRED )

include( ilcsoft_default_settings )
include( GNUInstallDirs )

# make the package library
file( GLOB library_sources source/src/*.cc )
add_library( ${PROJECT_NAME} SHARED ${library_sources} )
add_library( ${PROJECT_NAME}::Core ALIAS ${PROJECT_NAME} )
target_include_directories( ${PROJECT_NAME} BEFORE PUBLIC source/include )
target_include_directories( ${PROJECT_NAME} SYSTEM PRIVATE ${streamlog_INCLUDE_DIRS} ${LCIO_INCLUDE_DIRS} )
target_link_libraries( ${PROJECT_NAME} PUBLIC ${streamlog_LIBRARIES} ${LCIO_LIBRARIES} )
install( TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} )
# TODO: install include directories if needed....

# make Marlin processors library
file( GLOB processors_sources source/plugins/marlin/*.cc )
if( "${Marlin_FOUND}" AND "${processors_sources}" )
  add_library( ${PROJECT_NAME}Processors MODULE ${processors_sources} )
  add_library( ${PROJECT_NAME}::MarlinProcessors ALIAS ${PROJECT_NAME}Processors )
  target_include_directories( ${PROJECT_NAME}Processors SYSTEM PUBLIC ${Marlin_INCLUDE_DIRS} )
  target_link_libraries( ${PROJECT_NAME}Processors PUBLIC ${PROJECT_NAME}::Core ${Marlin_LIBRARIES} )
  install( TARGETS ${PROJECT_NAME}Processors LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} )  
endif()


# make MarlinMT processors library
# file( GLOB processors_sources source/plugins/marlin/*.cc )
# add_library( ${PROJECT_NAME}Processors MODULE ${processors_sources} )
# add_library( ${PROJECT_NAME}Processors ALIAS ${PROJECT_NAME}::MarlinProcessors )
# target_link_libraries( ${PROJECT_NAME}Processors PUBLIC ${PROJECT_NAME}::Core MarlinMT::Core )

### DOCUMENTATION ###########################################################
if( INSTALL_DOC )
    add_subdirectory( doc )
    install( CODE "EXECUTE_PROCESS( COMMAND ${CMAKE_BUILD_TOOL} doc)" )
endif()

# display some variables and write them to cache
DISPLAY_STD_VARIABLES()

# generate and install following configuration files
# GENERATE_PACKAGE_CONFIGURATION_FILES( ${PROJECT_NAME}Config.cmake ${PROJECT_NAME}ConfigVersion.cmake )
