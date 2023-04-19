set(PRJ_BASE_NAME "myAmp")
project(${PRJ_BASE_NAME}_Xputty)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(X11 REQUIRED)
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(XPUTTY "xputty")

set(PROJECT_BINARY_DIR "${PROJECT_BINARY_DIR}/../${PROJECT_NAME}.lv2")

set(SOURCE_FILES
    ${PRJ_BASE_NAME}.c
    ${PRJ_BASE_NAME}.ttl
    manifest.ttl
)
set(UI_SOURCE_FILES
    ${PROJECT_NAME}.cpp
    ${PROJECT_NAME}.hpp
)
file(COPY ${PRJ_BASE_NAME}.ttl manifest.ttl DESTINATION ${PROJECT_BINARY_DIR})

add_library(${PROJECT_NAME} MODULE ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PRJ_BASE_NAME}")
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

add_library(${PROJECT_NAME}UI MODULE ${UI_SOURCE_FILES})
set_target_properties(${PROJECT_NAME}UI PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME}UI PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")
set_target_properties(${PROJECT_NAME}UI PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

if(XPUTTY_FOUND)
    target_include_directories(${PROJECT_NAME}UI PRIVATE ${XPUTTY_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME}UI PRIVATE ${XPUTTY_LIBRARIES})
else()
#   set(XPUTTY_FILES
#
#   )
#   add_library(XPUTTY ${XPUTTY_FILES} STATIC)
    set(XPUTTY_BASEDIR "${CMAKE_CURRENT_SOURCE_DIR}/libxputty/libxputty")
    find_library(XPUTTY NAMES "xputty" PATHS "${XPUTTY_BASEDIR}")
    target_include_directories(${PROJECT_NAME}UI PRIVATE "${XPUTTY_BASEDIR}/include")
    target_link_libraries(${PROJECT_NAME}UI PRIVATE ${XPUTTY})
endif()

target_include_directories(${PROJECT_NAME}UI PRIVATE ${X11_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME}UI PRIVATE ${CAIRO_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}UI PRIVATE ${X11_LIBRARIES})
target_link_libraries(${PROJECT_NAME}UI PRIVATE ${CAIRO_LIBRARIES})
target_link_libraries(${PROJECT_NAME}UI PRIVATE Threads::Threads)
