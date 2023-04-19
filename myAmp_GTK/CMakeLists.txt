set(PRJ_BASE_NAME "myAmp")
project(${PRJ_BASE_NAME}_GTK)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)

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
target_include_directories(${PROJECT_NAME}UI PRIVATE ${GTK2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}UI PRIVATE ${GTK2_LIBRARIES})
target_link_libraries(${PROJECT_NAME}UI PRIVATE Threads::Threads)
