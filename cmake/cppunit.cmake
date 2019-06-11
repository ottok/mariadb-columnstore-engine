include_guard(GLOBAL)

#[[
find_path(CPPUNIT_INCLUDE_DIR cppunit/Test.h)
find_library(CPPUNIT_LIBRARY NAMES cppunit)

if(CPPUNIT_INCLUDE_DIR AND CPPUNIT_LIBRARY)

if(
]]
set(CPPUNIT_DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/.cppunit)
set(CPPUNIT_DIR ${CMAKE_CURRENT_BINARY_DIR}/cppunit)

list(APPEND ADDITIONAL_MAKE_CLEAN_FILES ${CPPUNIT_DOWNLOAD_DIR};${CPPUNIT_DIR})

configure_file(${CMAKE_CURRENT_LIST_DIR}/cppunit.CMakeLists.txt ${CPPUNIT_DOWNLOAD_DIR}/CMakeLists.txt @ONLY)

execute_process(COMMAND ${CMAKE_COMMAND} .
  WORKING_DIRECTORY ${CPPUNIT_DOWNLOAD_DIR})

execute_process(COMMAND ${CMAKE_COMMAND} --build .
  WORKING_DIRECTORY ${CPPUNIT_DOWNLOAD_DIR})

list(APPEND CMAKE_PREFIX_PATH ${CPPUNIT_DIR}/lib/cmake/cppunit)

find_package(cppunit)

