/**
 * @file task-02-salt-and-pepper.cpp
 * @author Paulo Santos (pauloxrms@gmail.com)
 * @brief Generates a salt and pepper noised image.
 * @version 0.1
 * @date 29-07-2025
 *
 * @copyright Copyright (c) 2024 Paulo Santos
 *
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <random>

cv::Mat generate_image(const int height, const int width, const float salt_prob,
		       const float pepper_prob)
{
	// Create an empty OpenCV Mat (image) with the specified dimensions and type (CV_32F for
	// float)
	cv::Mat image(height, width, CV_32F);

	// Populate the image with 0.5
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			image.at<float>(i, j) = static_cast<float>(0.5);
		}
	}

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			const float salt_rand =
				static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
			const float pepper_rand =
				static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
			if (salt_rand < salt_prob) {
				image.at<float>(i, j) = static_cast<float>(1);
			} else if (pepper_rand < pepper_prob) {
				image.at<float>(i, j) = static_cast<float>(-1);
			}
		}
	}

	return image;
}

int main()
{
	// Test parameters
	constexpr int w = 100;
	constexpr int h = 100;
	constexpr float salted_prob = 0.1;
	constexpr float peppered_prob = 0.1;

	cv::Mat image = generate_image(h, w, salted_prob, peppered_prob);

	cv::namedWindow("Random Salt&Pepper", cv::WINDOW_KEEPRATIO);
	cv::imshow("Random Salt&Pepper", image);
	cv::waitKey(0);

	int salted = 0;
	int peppered = 0;
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			if (image.at<float>(i, j) == 1.0) {
				salted++;
			} else if (image.at<float>(i, j) == -1.0) {
				peppered++;
			}
		}
	}

	bool salt_passed = false;
	bool pepper_passed = false;
	const int expected_salt = h * w * salted_prob;
	const int expected_pepper = h * w * peppered_prob;
	if ((expected_salt - (0.1 * expected_salt)) < salted ||
	    salted < (expected_salt + (0.1 * expected_salt))) {
		salt_passed = true;
	}
	if ((expected_pepper - (0.1 * expected_pepper)) < salted ||
	    salted < (expected_pepper + (0.1 * expected_pepper))) {
		pepper_passed = true;
	}

	if (salt_passed && pepper_passed) {
		std::cout << "Test passed!" << std::endl;
	} else {
		std::cout << "Test failed!" << std::endl;
	}
	std::cout << "Salgados:" << std::endl << salted << std::endl;
	std::cout << "Apimentados:" << std::endl << peppered << std::endl;

	return 0;
}
