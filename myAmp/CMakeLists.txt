set(PRJ_NAME "myAmp")
project(${PRJ_NAME}_NoUI)

set(PROJECT_BINARY_DIR "${PROJECT_BINARY_DIR}/../${PRJ_NAME}.lv2")

set(SOURCE_FILES
    ${PRJ_NAME}.ttl
    ${PRJ_NAME}.c
    manifest.ttl
)
add_library(${PROJECT_NAME} MODULE ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PRJ_NAME}")
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

file(COPY ${PRJ_NAME}.ttl manifest.ttl DESTINATION ${PROJECT_BINARY_DIR})
