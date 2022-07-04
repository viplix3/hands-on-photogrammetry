cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME camera-calibration)
project("${PROJECT_NAME}" VERSION 1.0.0)

find_package(OpenCV REQUIRED PATHS /usr/local/include/opencv4)
include_directories("${OpenCV_INCLUDE_DIRS}")

add_subdirectory(calibration_utils)

add_executable(
	camera-calibration
	main.cxx
)

target_link_libraries(camera-calibration PRIVATE calibration-utils "${OpenCV_LIBS}")