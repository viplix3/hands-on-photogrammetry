#include <iostream>
#include "camera_calib.h"

bool checkInputParams(int argc, const char** argv) {
	if(argc <= 4 || argc > 6) {
		std::cout << "Insufficient parameters\n";
		std::cout << "Usage ./camera-calibration ${CHECKERBOARD_PATTERN_INPUT_VIDEO_PATH} ";
		std::cout << "${NUM_BOARD_CELLS_X} ${NUM_BOARD_CELLS_Y} ${BOARD_CELL_SIZE} ";
		std::cout << "{DISTORTION_CORRECTED_OUTPUT_VIDEO_PATH}" << std::endl;
		return false;
	}
	return true;
}

int main(int argc, const char** argv) {
	if(checkInputParams(argc, argv) == false)
		return -1;

	const char* inputVideoFile = argv[1];
	cv::Size boardPattern(std::stoi(argv[2]), std::stoi(argv[3]));
	float cellSize = (argc == 5) ? (std::stof(argv[4])) : 1.0f;
	const char* outputFileName = (argc == 6) ? (argv[5]) : "";

	CameraCalibration camCalibrator(inputVideoFile, boardPattern, cellSize);

	if(camCalibrator.openVideoFile() == false) {
		std::cout << "Cannot open the video file '" << inputVideoFile << "'" << std::endl;
	}

	camCalibrator.selectCalibrationFramesFromVideo();
	if(camCalibrator.numSelectedFrames() <= 10) {
		std::cout << "Number of selected frames are very less, need 10 frames for optimal parameter estimation\n";
		std::cout << "Exiting..." << std::endl;
		return -2;
	}
	else std::cout << "Number of selected frames for calibration: "
						<< camCalibrator.numSelectedFrames() << std::endl;
	
	camCalibrator.generate3DPointsForSelectedFrames();
	camCalibrator.calibrateCamera();
	camCalibrator.printCalibrationParameters();
	camCalibrator.saveCalibrationParameters("../../resources/CalibrationReport.txt");
	camCalibrator.reportReprojectionError();

	camCalibrator.distortionCorrection(outputFileName);

	return 0;
}