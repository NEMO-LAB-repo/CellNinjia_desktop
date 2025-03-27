cmake_minimum_required(VERSION 3.10)
project(CellNinjia_desktop)

# Compiler options
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Set executable output directory to project root directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Include directories
include_directories(. asn1_group)

# Find and link libserial
find_library(SERIAL_LIB serial PATHS /usr/lib/x86_64-linux-gnu REQUIRED)

# Add local libcellninjia_asn1.so library
set(ASN1_LIB ${CMAKE_CURRENT_SOURCE_DIR}/libcellninjia_asn1.so)

# Source files for original project
set(SOURCES
    main.cpp
    decode_preprocess.cpp
)

# Create original executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries to original executable
target_link_libraries(${PROJECT_NAME} 
    ${SERIAL_LIB}
    ${ASN1_LIB}
)

# Set runtime library path for original executable
set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH_USE_LINK_PATH TRUE
    INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Decoder source files
set(DECODER_SOURCES
    Decoder/decode.cpp
    decode_preprocess.cpp
)

# Create Decoder executable
add_executable(decoder ${DECODER_SOURCES})

# Link libraries to Decoder executable
target_link_libraries(decoder
    ${SERIAL_LIB}
    ${ASN1_LIB}
)

# Set runtime library path for Decoder executable
set_target_properties(decoder PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH_USE_LINK_PATH TRUE
    INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}"
)