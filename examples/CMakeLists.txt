cmake_minimum_required(VERSION 4.1.1)

project(doletisExamples LANGUAGES C CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(NAME doletis SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# ---- C++ examples ----

file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")
foreach(EXAMPLE_SRC ${sources})
  get_filename_component(EXAMPLE_NAME ${EXAMPLE_SRC} NAME_WE)

  add_executable(${EXAMPLE_NAME} ${EXAMPLE_SRC})
  set_property(TARGET ${EXAMPLE_NAME} PROPERTY CXX_STANDARD 23)
  target_link_libraries(${EXAMPLE_NAME} doletis::doletis)
endforeach()

# ---- C (-fblocks) examples ----

include(CheckCCompilerFlag)
check_c_compiler_flag("-fblocks" DOLETIS_HAS_C_BLOCKS)
find_path(DOLETIS_BLOCK_HEADER Block.h)
if(APPLE)
  set(DOLETIS_BLOCKS_LIBS "")
  set(DOLETIS_CAN_BUILD_C ${DOLETIS_HAS_C_BLOCKS})
else()
  find_library(DOLETIS_BLOCKS_RUNTIME_LIB BlocksRuntime)
  if(DOLETIS_HAS_C_BLOCKS AND DOLETIS_BLOCK_HEADER AND DOLETIS_BLOCKS_RUNTIME_LIB)
    set(DOLETIS_BLOCKS_LIBS ${DOLETIS_BLOCKS_RUNTIME_LIB})
    set(DOLETIS_CAN_BUILD_C TRUE)
  else()
    set(DOLETIS_CAN_BUILD_C FALSE)
  endif()
endif()

if(DOLETIS_CAN_BUILD_C)
  file(GLOB_RECURSE c_sources "${CMAKE_CURRENT_SOURCE_DIR}/c/*.c")
  foreach(C_SRC ${c_sources})
    get_filename_component(C_NAME ${C_SRC} NAME_WE)

    add_executable(c_${C_NAME} ${C_SRC})
    set_property(TARGET c_${C_NAME} PROPERTY C_STANDARD 23)
    target_compile_options(c_${C_NAME} PRIVATE -fblocks -Wall -Wextra -Werror)
    target_link_libraries(c_${C_NAME} PRIVATE doletis::doletis ${DOLETIS_BLOCKS_LIBS})
  endforeach()
endif()
