project(mySimplePolySynth)

set(PROJECT_BINARY_DIR "${PROJECT_BINARY_DIR}/../${PROJECT_NAME}.lv2")

set(SOURCE_FILES
    ${PROJECT_NAME}.ttl
    ${PROJECT_NAME}.cpp
    manifest.ttl
    BMap.hpp
    Envelope.hpp
    Key.hpp
    KeyStatus.hpp
    Limit.hpp
    LinearFader.hpp
    Waveform.hpp
)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-ffast-math)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/fp:fast)
endif()

add_library(${PROJECT_NAME} MODULE ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

file(COPY ${PROJECT_NAME}.ttl manifest.ttl DESTINATION ${PROJECT_BINARY_DIR})
