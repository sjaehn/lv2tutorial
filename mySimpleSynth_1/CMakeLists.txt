set(PRJ_BASE_NAME "mySimpleSynth")
project(${PRJ_BASE_NAME}_1)

set(PROJECT_BINARY_DIR "${PROJECT_BINARY_DIR}/../${PROJECT_NAME}.lv2")

set(SOURCE_FILES
    ${PRJ_BASE_NAME}.ttl
    ${PRJ_BASE_NAME}.cpp
    manifest.ttl
    Envelope.hpp
    Key.hpp
    KeyStatus.hpp
    Limit.hpp
    LinearFader.hpp
    Waveform.hpp
)
add_library(${PROJECT_NAME} MODULE ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PRJ_BASE_NAME}")
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

file(COPY ${PRJ_BASE_NAME}.ttl manifest.ttl DESTINATION ${PROJECT_BINARY_DIR})
