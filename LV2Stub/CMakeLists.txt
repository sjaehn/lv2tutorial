project(LV2Stub)

set(PROJECT_BINARY_DIR "${PROJECT_BINARY_DIR}/../${PROJECT_NAME}.lv2")

set(SOURCE_FILES
    ${PROJECT_NAME}.ttl
    manifest.ttl
    src/LV2Plugin.cpp
    src/LV2Plugin.hpp
    src/Ports.hpp
    src/Urids.hpp
)
add_library(${PROJECT_NAME} MODULE ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

file(COPY ${PROJECT_NAME}.ttl manifest.ttl DESTINATION ${PROJECT_BINARY_DIR})
