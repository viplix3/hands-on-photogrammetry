#include <iostream>
#include "camera_calib.h"

bool checkInputParams(int argc, const char** argv) {
	if(argc <= 3 || argc > 5) {
		std::cout << "Insufficient parameters\n";
		std::cout << "Usage ./camera-calibration ${PATH_TO_CHECKERBOARD_PATTERN_IMAGES} ";
		std::cout << "${BOARD_CELL_SIZE} ${NUM_BOARD_CELLS_X} ${NUM_BOARD_CELLS_Y}" << std::endl;
		return false;
	}
	return true;
}

int main(int argc, const char** argv) {
	if(checkInputParams(argc, argv) == false)
		return -1;

	const char* inputVideoFile = argv[1];
	cv::Size boardPattern(std::stoi(argv[2]), std::stoi(argv[3]));
	float cellSize = (argc == 4) ? (std::stof(argv[4])) : 1.0f;

	CameraCalibration camCalibrator(inputVideoFile, boardPattern, cellSize);

	if(camCalibrator.openVideoFile() == false) {
		std::cout << "Cannot open the video file '" << inputVideoFile << "'" << std::endl;
	}

	camCalibrator.selectCalibrationFramesFromVideo();
	if(camCalibrator.numSelectedFrames() <= 10) {
		std::cout << "Number of selected frames are very less, need 10 frames for optimal parameter estimation";
		std::cout << "Exiting..." << std::endl;
		return -2;
	}
	else std::cout << "Number of selected frames for calibration: "
						<< camCalibrator.numSelectedFrames() << std::endl;
	
	camCalibrator.generate3DPointsForSelectedFrames();
	camCalibrator.calibrateCamera();
	camCalibrator.printCalibrationParameters();
	camCalibrator.saveCalibrationParameters("../../resources/CalibrationReport.txt");

	return 0;
}