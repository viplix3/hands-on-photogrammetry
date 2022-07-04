add_library(
	calibration-utils
	camera_calib.cxx
)

target_include_directories(calibration-utils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include/")