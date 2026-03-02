cmake_minimum_required(VERSION 4.1.1)

project(doletisExamples LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)


CPMAddPackage(NAME doletis SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# ---- Create standalone executable ----


file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")
foreach(EXAMPLE_SRC ${sources})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SRC} NAME_WE)

    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SRC})
    set_property(TARGET ${EXAMPLE_NAME} PROPERTY CXX_STANDARD 23)
    target_link_libraries(${EXAMPLE_NAME} doletis::doletis)

endforeach()
