/**
 * @file task-01-generate-an-image.cpp
 * @author Paulo Santos (pauloxrms@gmail.com)
 * @brief Gera uma image a partir de ruído gaussiano.
 * @version 0.1
 * @date 22-07-2025
 *
 * @copyright Copyright (c) 2024 Paulo Santos
 *
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <random>

cv::Mat generate_image(const int seed, const int width, const int height, const double mean,
		       const double std)
{
	// Is this equivalent to numpy?
	std::mt19937 gen(seed);
	std::normal_distribution distribution(mean, std);

	// Create an empty OpenCV Mat (image) with the specified dimensions and type (CV_32F for
	// float)
	cv::Mat image(height, width, CV_32F);

	// Populate the image with values from the normal distribution
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			image.at<float>(i, j) = static_cast<float>(distribution(gen));
		}
	}

	return image;
}

int main()
{
	// Test parameters
	constexpr int w = 3;
	constexpr int h = 5;

	cv::Mat image = generate_image(0, w, h, 128, 20);

	cv::namedWindow("Random Gaussian", cv::WINDOW_KEEPRATIO);
	cv::imshow("Random Gaussian", image / 255.0);
	cv::waitKey(0);

	// Expected test values (should be re-sampled???)
	cv::Mat test =
		(cv::Mat_<float>(5, 3) << 163.28104692, 136.00314417, 147.57475968, 172.81786398,
		 165.3511598, 108.4544424, 147.00176835, 124.97285583, 125.93562296, 136.21197004,
		 130.88087142, 157.08547014, 143.2207545, 130.43350033, 136.87726465);

	// Expected test values (re-sampling from cpp seeded random xD)
	cv::Mat test_cpp = (cv::Mat_<float>(5, 3) << 150.4559, 134.05611, 129.41719, 129.46083,
			    99.553482, 158.40138, 122.17212, 125.3382, 124.5386, 92.766968,
			    126.24654, 155.33759, 150.50629, 120.82009, 152.41216);

	bool passed = true;
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			if (constexpr double epsilon = 0.0001;
			    std::abs(image.at<float>(i, j) - test_cpp.at<float>(i, j)) >= epsilon) {
				passed = false;
				break;
			}
		}
		if (!passed) {
			break;
		}
	}

	if (passed) {
		std::cout << "Test passed!" << std::endl;
	} else {
		std::cout << "Test failed!" << std::endl;
		std::cout << "Generated Image:" << std::endl << image << std::endl;
		std::cout << "Expected Test Values:" << std::endl << test << std::endl;
	}

	return 0;
}
