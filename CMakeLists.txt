cmake_minimum_required(VERSION 3.10)
project(CellNinjia_desktop)

# Compiler options
set(CMAKE_CXX_STANDARD 14)  # Qt requires C++14
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Set executable output directory to project root directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Find Qt5 components
find_package(Qt5 REQUIRED COMPONENTS Core Widgets SerialPort Charts)

# Enable automatic MOC, UIC and RCC generation
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Include directories
include_directories(src include/asn1)

# GUI-specific include directories (only for GUI target)
# gui/include will be added specifically to the GUI target

# Find and link libserial (for CLI version)
find_library(SERIAL_LIB serial PATHS /usr/lib/x86_64-linux-gnu)

# Add local libcellninjia_asn1.so library
set(ASN1_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcellninjia_asn1.so)

# ============================================================================
# CLI APPLICATION (Original command-line version)
# ============================================================================

# Source files for CLI version
set(SOURCES
    src/main.cpp
    src/decode_preprocess.cpp
)

# Create CLI executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries to CLI executable
if(SERIAL_LIB)
    target_link_libraries(${PROJECT_NAME} 
        ${SERIAL_LIB}
        ${ASN1_LIB}
    )
else()
    message(WARNING "libserial not found, CLI version will have limited functionality")
    target_link_libraries(${PROJECT_NAME} ${ASN1_LIB})
endif()

# Set runtime library path for CLI executable
set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH_USE_LINK_PATH TRUE
    INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# ============================================================================
# GUI APPLICATION (Qt-based graphical interface)
# ============================================================================

# GUI source files (all contained in gui/ folder)
set(GUI_SOURCES
    gui/src/main.cpp
    gui/src/mainwindow.cpp
    gui/src/cellninjia_core.cpp
    gui/src/chartwidget.cpp
    gui/src/offlinedecoderdialog.cpp
    gui/src/advancedfilter.cpp
    gui/src/loganalysis.cpp
    gui/src/loganalysisworker.cpp
    # Shared decode functionality from src/
    src/decode_preprocess.cpp
)

# GUI header files (all contained in gui/include/ folder)
set(GUI_HEADERS
    gui/include/mainwindow.h
    gui/include/cellninjia_core.h
    gui/include/chartwidget.h
    gui/include/offlinedecoderdialog.h
    gui/include/advancedfilter.h
    gui/include/loganalysis.h
)

# Create Qt GUI executable
add_executable(CellNinjia_GUI ${GUI_SOURCES} ${GUI_HEADERS})

# Add GUI-specific include directories
target_include_directories(CellNinjia_GUI PRIVATE gui/include)

# Link Qt libraries to GUI executable
target_link_libraries(CellNinjia_GUI
    Qt5::Core
    Qt5::Widgets 
    Qt5::SerialPort
    Qt5::Charts
    ${ASN1_LIB}
)

# Set runtime library path for GUI executable
set_target_properties(CellNinjia_GUI PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH_USE_LINK_PATH TRUE
    INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# ============================================================================
# DECODER UTILITY (Command-line decoder tool)
# ============================================================================

# Decoder source files
set(DECODER_SOURCES
    tools/decoder/decode.cpp
    src/decode_preprocess.cpp
)

# Create Decoder executable
add_executable(decoder ${DECODER_SOURCES})

# Link libraries to Decoder executable
if(SERIAL_LIB)
    target_link_libraries(decoder
        ${SERIAL_LIB}
        ${ASN1_LIB}
    )
else()
    target_link_libraries(decoder ${ASN1_LIB})
endif()

# Set runtime library path for Decoder executable
set_target_properties(decoder PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH_USE_LINK_PATH TRUE
    INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)