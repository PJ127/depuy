cmake_minimum_required( VERSION 2.8 )

project( TestApplication )

include (CMakeInclude.txt)

set( DESIRED_QT_VERSION 4 )
find_package( Qt4 ${QT_VERSION} COMPONENTS QtCore QtGui QtXml QtOpenGL REQUIRED )
include( ${QT_USE_FILE} )

set( fqv_name "win32_x86_msvc_1500" )

# EXTERNAL_LIBS_INSTALL_PREFIX
set( EXTERNAL_LIBS_INSTALL_PREFIX "D:/Dev/DevSurgivisio/ExternalLibs/trunk" CACHE PATH  "" )


# GLEW
set( GLEW_VERSION "1.5.7" CACHE STRING "Glew version" FORCE )
set( GLEW_INSTALL_PREFIX	"${EXTERNAL_LIBS_INSTALL_PREFIX}/glew/${GLEW_VERSION}")

if( NOT EXISTS ${GLEW_INSTALL_PREFIX} )
    message( SEND_ERROR "Glew required, but not found in '${EXTERNAL_LIBS_INSTALL_PREFIX}'" )
else()
    set( GLEW_INCLUDE_DIR ${GLEW_INSTALL_PREFIX}/include/${fqv_name})
    set( GLEW_LIB_DIR ${GLEW_INSTALL_PREFIX}/lib/${fqv_name})
	set( GLEW_BIN_DIR ${GLEW_INSTALL_PREFIX}/bin)
    set( __lib_list glew32 )
    fn_find_external_libs( GLEW ${GLEW_LIB_DIR} ${__lib_list} )
endif()

# GLUT
set( GLUT_VERSION "3.7.6" CACHE STRING "Glut version" FORCE )
set( GLUT_INSTALL_PREFIX	"${EXTERNAL_LIBS_INSTALL_PREFIX}/glut/${GLUT_VERSION}")

if( NOT EXISTS ${GLUT_INSTALL_PREFIX} )
    message( SEND_ERROR "Glut required, but not found in '${EXTERNAL_LIBS_INSTALL_PREFIX}'" )
else()
	set( GLUT_INCLUDE_DIR ${GLUT_INSTALL_PREFIX}/include )
	set( GLUT_LIB_DIR ${GLUT_INSTALL_PREFIX}/lib/${fqv_name} )
    set( __lib_list glut32 )
    fn_find_external_libs( GLUT ${GLUT_LIB_DIR} ${__lib_list} )
endif()


list( APPEND EXTERNAL_LIBS_INCLUDE_DIR 							${QT_INCLUDE_DIR} 	${GLEW_INCLUDE_DIR} ${GLUT_INCLUDE_DIR} )
list( APPEND EXTERNAL_LIBS_LIBRARIES 	glu32.lib opengl32.lib	${QT_LIBRARIES} 	${GLEW_LIBRARIES}   ${GLUT_LIBRARIES})

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${EXTERNAL_LIBS_INCLUDE_DIR} )

file( GLOB_RECURSE header_list *.h )
file( GLOB_RECURSE source_list *.cpp )
file( GLOB_RECURSE ui_list *.ui )

# Et tu moc
find_mocable_files( mocable_list ${header_list} )
qt4_wrap_cpp( moc_list ${mocable_list} )
qt4_wrap_ui( generated_ui_list ${ui_list} )
qt4_add_resources( generated_qrc_list ${qrc_list} )

# Et tu source_group
source_group_custom( ${CMAKE_CURRENT_SOURCE_DIR} "Header Files" ${header_list} )
source_group_custom( ${CMAKE_CURRENT_SOURCE_DIR} "Source Files" ${source_list} )
source_group_custom( ${CMAKE_CURRENT_SOURCE_DIR} "QT-UI Files" ${ui_list} )
source_group_custom( ${CMAKE_CURRENT_SOURCE_DIR} "Resource Files" ${qrc_list} )
source_group_custom( ${CMAKE_CURRENT_BINARY_DIR} "Generated QT Files" ${moc_list} )
source_group_custom( ${CMAKE_CURRENT_BINARY_DIR} "Generated QT Files" ${generated_ui_list} )
source_group_custom( ${CMAKE_CURRENT_BINARY_DIR} "Generated QT Files" ${generated_qrc_list} )


# Et tu exclude from build
exclude_inltpls( ${source_list} )

add_executable( ${PROJECT_NAME} ${header_list} ${source_list} ${moc_list} ${generated_ui_list} ${generated_qrc_list})

target_link_libraries( ${PROJECT_NAME} ${EXTERNAL_LIBS_LIBRARIES})

include (CMakeImportDlls.txt)






