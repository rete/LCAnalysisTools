########################################################
# cmake file for building LCAnalysisTools documentation
# @author Remi Ete, Desy
########################################################

if( NOT INSTALL_DOC )
  return()
endif()

set( INSTALL_DOXYGEN_DIR ${CMAKE_INSTALL_DOCDIR}/${PROJECT_VERSION}/Doxygen )

# build documentation out-of-source
file( GLOB DOX_INPUT_FILE_LIST
  ${PROJECT_SOURCE_DIR}/source/include/LCAnalysisTools/*.h 
  ${PROJECT_SOURCE_DIR}/source/src/*.cc
)

list( JOIN DOX_INPUT_FILE_LIST " " DOX_INPUT_FILES )
set( DOX_PROJECT_NAME ${PROJECT_NAME} )
set( DOX_PROJECT_NUMBER ${${PROJECT_NAME}_VERSION} )
set( DOX_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/docbuild )
set( DOX_DOXYFILE_DIR ${PROJECT_SOURCE_DIR}/doc )
set( DOX_DOXYFILE ${DOX_DOXYFILE_DIR}/Doxyfile )
set( DOX_INPUT ${DOX_INPUT_FILES} )

# Generate the Doxyfile for Doxygen and Mkdocs
configure_file( 
  ${PROJECT_SOURCE_DIR}/doc/Doxyfile.in 
  ${DOX_DOXYFILE}
  @ONLY
)

find_package( Doxygen REQUIRED )

# custom command to build documentation
add_custom_command(
    OUTPUT  "${DOX_OUTPUT_DIRECTORY}/html/index.html" #"${DOC_BIN_DIR}/latex/index.tex"
    COMMAND ${DOXYGEN_EXECUTABLE}
    WORKING_DIRECTORY ${DOX_DOXYFILE_DIR}
    COMMENT "Building Doxygen documentation..."
    DEPENDS ${DOX_DOXYFILE} CMakeLists.txt ${DOX_INPUT_FILE_LIST}
)
add_custom_target( doxygen DEPENDS "${DOX_OUTPUT_DIRECTORY}/html/index.html" ) # "${DOC_BIN_DIR}/latex/index.tex" )
# build documentation before 'make install'
# FIXME: This is ugly. There might be a better way to do it.
# Look at other package for inspiration...
install( CODE "EXECUTE_PROCESS( COMMAND ${CMAKE_BUILD_TOOL} doxygen)" )
install( DIRECTORY ${DOX_OUTPUT_DIRECTORY}/html DESTINATION ${INSTALL_DOXYGEN_DIR} )

