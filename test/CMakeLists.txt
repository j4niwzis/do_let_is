cmake_minimum_required(VERSION 4.1.1 FATAL_ERROR)

project(doletisTests LANGUAGES C CXX)

# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
  NAME googletest
  GITHUB_REPOSITORY google/googletest
  GIT_TAG release-1.12.1
  VERSION 1.12.1
  OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt"
)
CPMAddPackage("gh:TheLartians/Format.cmake@1.7.3")

if(TEST_INSTALLED_VERSION)
  find_package(doletis REQUIRED)
else()
  CPMAddPackage(NAME doletis SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

# ---- Create C++ test binary ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} gtest gtest_main doletis::doletis)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23)

enable_testing()

add_test(${PROJECT_NAME} ${PROJECT_NAME})

add_executable(doletisPureCxxTests ${CMAKE_CURRENT_SOURCE_DIR}/pure/pure_iso_cxx.cc)
set_target_properties(doletisPureCxxTests PROPERTIES CXX_STANDARD 23 CXX_EXTENSIONS OFF)
target_compile_options(doletisPureCxxTests PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors -Werror)
target_link_libraries(doletisPureCxxTests PRIVATE doletis::doletis)
add_test(doletisPureCxxTests doletisPureCxxTests)

# ---- Create C (-fblocks) test binary ----

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
check_c_compiler_flag("-fblocks" DOLETIS_HAS_C_BLOCKS)
check_cxx_compiler_flag("-fblocks" DOLETIS_HAS_CXX_BLOCKS)
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
  file(GLOB c_test_sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/c/*.c)
  add_executable(doletisCTests ${c_test_sources})
  set_target_properties(doletisCTests PROPERTIES C_STANDARD 23)
  target_compile_options(doletisCTests PRIVATE -fblocks -Wall -Wextra -Werror)
  target_link_libraries(doletisCTests PRIVATE doletis::doletis ${DOLETIS_BLOCKS_LIBS})
  add_test(doletisCTests doletisCTests)

  if(DOLETIS_HAS_CXX_BLOCKS)
    add_executable(doletisInteropTests ${CMAKE_CURRENT_SOURCE_DIR}/interop/c_in_cxx_interop.cc)
    set_target_properties(doletisInteropTests PROPERTIES CXX_STANDARD 23)
    target_compile_options(doletisInteropTests PRIVATE -fblocks -Wall -Wextra -Wno-unused-parameter -Werror)
    target_link_libraries(doletisInteropTests PRIVATE doletis::doletis ${DOLETIS_BLOCKS_LIBS})
    add_test(doletisInteropTests doletisInteropTests)

    add_executable(doletisCxxBlocksTests ${CMAKE_CURRENT_SOURCE_DIR}/interop/cxx_use_blocks.cc)
    set_target_properties(doletisCxxBlocksTests PROPERTIES CXX_STANDARD 23)
    target_compile_options(doletisCxxBlocksTests PRIVATE -fblocks -Wall -Wextra -Wno-unused-parameter -Werror)
    target_link_libraries(doletisCxxBlocksTests PRIVATE doletis::doletis ${DOLETIS_BLOCKS_LIBS})
    add_test(doletisCxxBlocksTests doletisCxxBlocksTests)
  endif()
endif()

# ---- code coverage ----

if(ENABLE_TEST_COVERAGE)
  target_compile_options(doletis INTERFACE -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(doletis INTERFACE -fprofile-arcs -ftest-coverage)
endif()
