/**
 * @file task-08-image-similarity.cpp
 * @author Paulo Santos (pauloxrms@gmail.com)
 * @brief Linearly merges two images, interactively.
 * @version 0.1
 * @date 22-08-2025
 *
 * @copyright Copyright (c) 2025 Paulo Santos
 *
 */

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <opencv2/opencv.hpp>

double calculateMSE(const cv::Mat &i1, const cv::Mat &i2)
{
	cv::Mat diff;
	cv::absdiff(i1, i2, diff);
	diff.convertTo(diff, CV_64F);
	diff = diff.mul(diff);
	return cv::mean(diff)[0];
}

double calculatePSNR(const cv::Mat &i1, const cv::Mat &i2, const double data_range = 1.0)
{
	const double mse = calculateMSE(i1, i2);
	if (mse == 0) {
		return INFINITY;
	}
	return 10.0 * log10(data_range * data_range / mse);
}

double calculateSSIM(const cv::Mat &i1, const cv::Mat &i2, const double c1 = 1e-8,
		     const double c2 = 1e-8)
{
	cv::Scalar mean1_s;
	cv::Scalar stddev1_s;
	cv::meanStdDev(i1, mean1_s, stddev1_s);
	const double mean1 = mean1_s[0];
	const double stddev1_sq = stddev1_s[0] * stddev1_s[0];

	cv::Scalar mean2_s;
	cv::Scalar stddev2_s;
	cv::meanStdDev(i2, mean2_s, stddev2_s);
	const double mean2 = mean2_s[0];
	const double stddev2_sq = stddev2_s[0] * stddev2_s[0];

	cv::Mat i1_sub_mean;
	cv::Mat i2_sub_mean;
	i1.convertTo(i1_sub_mean, CV_64F);
	i2.convertTo(i2_sub_mean, CV_64F);
	i1_sub_mean -= mean1;
	i2_sub_mean -= mean2;
	const cv::Mat product = i1_sub_mean.mul(i2_sub_mean);
	const double covariance = cv::mean(product)[0];

	const double num = (2 * mean1 * mean2 + c1) * (2 * covariance + c2);
	const double den = (mean1 * mean1 + mean2 * mean2 + c1) * (stddev1_sq + stddev2_sq + c2);
	return num / den;
}

double calculateNPCC(const cv::Mat &i1, const cv::Mat &i2)
{
	cv::Scalar mean1_s = cv::mean(i1);
	double mean1 = mean1_s[0];
	cv::Scalar mean2_s = cv::mean(i2);
	double mean2 = mean2_s[0];

	cv::Mat i1_sub_mean;
	cv::Mat i2_sub_mean;
	i1.convertTo(i1_sub_mean, CV_64F);
	i2.convertTo(i2_sub_mean, CV_64F);
	i1_sub_mean -= mean1;
	i2_sub_mean -= mean2;

	cv::Mat num_mat = i1_sub_mean.mul(i2_sub_mean);
	double num = cv::sum(num_mat)[0];

	cv::Mat den_left = i1_sub_mean.mul(i1_sub_mean);
	cv::Mat den_right = i2_sub_mean.mul(i2_sub_mean);

	double den_val = sqrt(cv::sum(den_left)[0] * cv::sum(den_right)[0]);

	if (den_val == 0.0) {
		if (num == 0.0) {
			return 1.0;
		}
		return 0.0;
	}

	return num / den_val;
}

int main(const int argc, char *argv[])
{
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <image1_path> <image2_path>" << std::endl;
		return -1;
	}

	cv::Mat i1 = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
	cv::Mat i2 = cv::imread(argv[2], cv::IMREAD_GRAYSCALE);

	if (i1.empty() || i2.empty()) {
		std::cerr << "Error: One or both images could not be loaded." << std::endl;
		return -1;
	}
	if (i1.size() != i2.size() || i1.channels() != 1 || i2.channels() != 1) {
		std::cerr << "Error: Inputs must be single-channel images of the same size."
			  << std::endl;
		return -1;
	}

	i1.convertTo(i1, CV_64F, 1.0 / 255.0);
	i2.convertTo(i2, CV_64F, 1.0 / 255.0);

	const double mse = calculateMSE(i1, i2);
	const double psnr = calculatePSNR(i1, i2);
	const double ssim = calculateSSIM(i1, i2);
	const double npcc = calculateNPCC(i1, i2);

	std::cout << "--- Image Similarity Metrics ---" << std::endl;
	std::cout << "MSE:  " << std::fixed << std::setprecision(6) << mse << std::endl;
	std::cout << "PSNR: " << std::fixed << std::setprecision(2) << psnr << " dB" << std::endl;
	std::cout << "SSIM: " << std::fixed << std::setprecision(6) << ssim << std::endl;
	std::cout << "NPCC: " << std::fixed << std::setprecision(6) << npcc << std::endl;

	return 0;
}
