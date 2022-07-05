#include "camera_calib.h"

int CameraCalibration::numSelectedFrames() {
	return m_selectedFrames.size();
}

bool CameraCalibration::openVideoFile() {
	bool status;
	status = m_videoReader.open(m_inputVideoFile);
	return status;
}

void CameraCalibration::selectCalibrationFramesFromVideo() {
	cv::Mat currFrame;
	while(true) {
		m_videoReader >> currFrame;

		if(currFrame.empty()) {
			std::cout << "No image data found in stream" << std::endl;
			break;
		}

		std::cout << "Press Esc to exit frame selection, Space to check frame for selection" << std::endl;
		// Show current frame from video and keep if selected
		cv::imshow("Camera Calibration", currFrame);
		int key = cv::waitKey();

		if(key == 27) break;						// Esc key pressed, stop the frame selection procedure
		else if(key == 32) {						// Space key pressed
			std::vector<cv::Point2f> cornerPoints;
			bool patternWasFound = cv::findChessboardCorners(currFrame, m_boardPattern, cornerPoints);

			cv::Mat cornerDisplayImage = currFrame.clone();
			cv::drawChessboardCorners(cornerDisplayImage, m_boardPattern, cornerPoints, patternWasFound);
			cv::imshow("Camera Calibration", cornerDisplayImage);

			std::cout << "Press Space to skip the frame, Enter to select the frame" << std::endl;

			key = cv::waitKey();
			if(key == 32) continue; 								// Esc pressed, move on to next frame
			else if(key == 13)	{									// Enter pressed, keep the current frame
				m_selectedFrames.push_back(currFrame);
				m_imagePoints.push_back(cornerPoints);
			}
		}
	}
	m_videoReader.release();
}

void CameraCalibration::generate3DPointsForSelectedFrames() {
	std::vector<std::vector<cv::Point3f>> objectPointsVector(1);
	for(int currRow = 0; currRow < m_boardPattern.height; currRow++) {
		for(int currCol = 0; currCol < m_boardPattern.width; currCol++) {
		 	// Z-coordinate is 0 as all the points are on a plane
			objectPointsVector[0].push_back(cv::Point3f(m_boardCellSize * currCol, m_boardCellSize * currRow, 0));
		}
	}
	objectPointsVector.resize(m_imagePoints.size(), objectPointsVector[0]);
	m_objectPoints = objectPointsVector;
}

void CameraCalibration::calibrateCamera() {
	cv::Size imageSize = m_selectedFrames[0].size();

	rootMeanSquaredError = cv::calibrateCamera(m_objectPoints, m_imagePoints, imageSize,
														K, distortionCoeff, rotationVectors,
														translationVectors);
}

void CameraCalibration::printCalibrationParameters() {
	std::cout << "\n\n\n";
	std::cout << "Camera Calibration Results" << std::endl;
	std::cout << "Number of images used: " << m_selectedFrames.size() << std::endl;
	std::cout << "Root Mean Squared Error: " << rootMeanSquaredError << std::endl;
	std::cout << "Camera Matrix (K)" << std::endl << K.row(0) << "\n" << K.row(1) << "\n" << K.row(2) << std::endl;
	std::cout << "Distortion Coefficients (k1, k2, p1, p2, k3, ...): "
					<< " " << distortionCoeff.t() << std::endl;
}

void CameraCalibration::saveCalibrationParameters(const char* filePath) {
	std::ofstream calibrationReport(filePath);

	calibrationReport << "Camera Calibration Results" << std::endl;
	calibrationReport << "Number of images used: " << m_selectedFrames.size() << std::endl;
	calibrationReport << "Root Mean Squared Error: " << rootMeanSquaredError << std::endl;
	calibrationReport << "Camera Matrix (K)" << std::endl << K.row(0) << "\n" << K.row(1) << "\n" << K.row(2) << std::endl;
	calibrationReport << "Distortion Coefficients (k1, k2, p1, p2, k3, ...): "
					<< " " << distortionCoeff.t() << std::endl;
}