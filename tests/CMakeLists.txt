cmake_minimum_required(VERSION 3.16)
project(OpenCompositeGazeTests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
find_package(Python REQUIRED COMPONENTS Interpreter)
set(REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(GLOB OPENVR_HEADERS CONFIGURE_DEPENDS "${REPO_ROOT}/OpenVRHeaders/openvr-*")
add_custom_command(
    OUTPUT "${GENERATED_DIR}/interfaces/vrtypes.h"
    COMMAND "${Python_EXECUTABLE}" "${REPO_ROOT}/scripts/split_headers.py"
        "${REPO_ROOT}/OpenVRHeaders" "${GENERATED_DIR}"
    DEPENDS "${REPO_ROOT}/scripts/split_headers.py" ${OPENVR_HEADERS}
    VERBATIM)
add_executable(eye_gaze_test eye_gaze_test.cpp "${GENERATED_DIR}/interfaces/vrtypes.h")
target_include_directories(eye_gaze_test PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}" "${REPO_ROOT}/libs/openxr-sdk/include")
enable_testing()
add_test(NAME eye_gaze COMMAND eye_gaze_test)
