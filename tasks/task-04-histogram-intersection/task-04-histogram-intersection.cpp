/**
 * @file task-04-histogram-intersection.cpp
 * @author Paulo Santos (pauloxrms@gmail.com)
 * @brief Computes the histogram intersection between two images.
 * @version 0.1
 * @date 09-08-2025
 *
 * @copyright Copyright (c) 2025 Paulo Santos
 *
 */

#include <opencv2/opencv.hpp>
#include <iostream>

auto compute_histogram_intersection(const cv::Mat &img_a, const cv::Mat &img_b) -> float
{
	cv::Mat hist_a;
	cv::Mat hist_b;
	constexpr int hist_size[] = {256};
	float range[] = {0.f, 256.f};
	const float *ranges[] = {range};


	cv::calcHist(&img_a, 1, nullptr, cv::Mat(), hist_a, 1, hist_size, ranges, true, false);
	cv::normalize(hist_a, hist_a, 1.0, 0.0, cv::NORM_L1);

	cv::calcHist(&img_b, 1, nullptr, cv::Mat(), hist_b, 1, hist_size, ranges, true, false);
	cv::normalize(hist_b, hist_b, 1.0, 0.0, cv::NORM_L1);

	float sum = 0.0;
	for (int i = 0; i < 256; i++) {
		sum += MIN(hist_a.at<float>(i), hist_b.at<float>(i));
	}

	// For extra safety, but very improbable case here
	if (sum > 1.0) {
		sum = 1.0;
	} else if (sum < 0) {
		sum = 0.0;
	}

	return sum;
}

int main()
{
	const cv::Mat img = cv::imread("beans.png", cv::IMREAD_GRAYSCALE);
	cv::Mat img2;
	cv::medianBlur(img, img2, 3);

	std::cout << "Intersection = " << compute_histogram_intersection(img, img2) << std::endl;

	return 0;
}
