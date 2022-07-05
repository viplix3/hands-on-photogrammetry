#pragma once

#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <fstream>

class CameraCalibration {
private:
	const char* m_inputVideoFile;
	cv::Size m_boardPattern;
	float m_boardCellSize;
	double rootMeanSquaredError;

	cv::VideoCapture m_videoReader;

	std::vector<cv::Mat> m_selectedFrames;
	
	// Image (2D) coordinates for checkerboard points
	std::vector<std::vector<cv::Point2f>> m_imagePoints;
	// Object (3D) coordinates for checkerboard points
	std::vector<std::vector<cv::Point3f>> m_objectPoints;

	cv::Mat K = cv::Mat::eye(3, 3, CV_64F); // Camera Matrix
	cv::Mat distortionCoeff = cv::Mat::zeros(4, 1, CV_64F); // Camera distortion coefficients
	std::vector<cv::Mat> rotationVectors, translationVectors;

public:
	CameraCalibration(const char* videoFilePath, cv::Size boardPattern, float cellSize)
	: m_inputVideoFile(videoFilePath), m_boardPattern(boardPattern), m_boardCellSize(cellSize)
	{}

	bool openVideoFile();
	int numSelectedFrames();

	void selectCalibrationFramesFromVideo();
	void generate3DPointsForSelectedFrames();
	void calibrateCamera();
	
	void printCalibrationParameters();
	void saveCalibrationParameters(const char* filePath);
};