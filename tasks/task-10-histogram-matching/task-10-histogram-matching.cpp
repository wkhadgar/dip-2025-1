/**
 * @file task-10-histogram-matching.cpp
 * @author Paulo Santos (pauloxrms@gmail.com)
 * @brief Performs a histogram match of an image, based on a reference.
 * @version 0.1
 * @date 24-09-2025
 *
 * @copyright Copyright (c) 2025 Paulo Santos
 *
 */

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include <opencv2/opencv.hpp>

cv::Mat matchHistogramSingleChannel(const cv::Mat &source_channel, const cv::Mat &reference_channel)
{
	if (source_channel.type() != CV_8UC1 || reference_channel.type() != CV_8UC1) {
		throw std::invalid_argument(
			"Channels must be single-channel 8-bit grayscale (CV_8UC1).");
	}

	int histSize = 256;
	float range[] = {0, 256};
	const float *histRange = {range};
	bool uniform = true;
	bool accumulate = false;

	cv::Mat hist_src;
	cv::Mat hist_ref;
	cv::calcHist(&source_channel, 1, nullptr, cv::Mat(), hist_src, 1, &histSize, &histRange,
		     uniform, accumulate);
	cv::calcHist(&reference_channel, 1, nullptr, cv::Mat(), hist_ref, 1, &histSize, &histRange,
		     uniform, accumulate);

	cv::Mat cdf_src(hist_src.size(), hist_src.type());
	cv::Mat cdf_ref(hist_ref.size(), hist_ref.type());

	auto total_pixels = static_cast<double>(source_channel.total());

	/* CDF for source */
	float sum_src = 0;
	for (int i = 0; i < histSize; ++i) {
		sum_src += hist_src.at<float>(i);
		cdf_src.at<float>(i) = static_cast<float>(sum_src / total_pixels);
	}

	/* CDF for reference */
	float sum_ref = 0;
	for (int i = 0; i < histSize; ++i) {
		sum_ref += hist_ref.at<float>(i);
		cdf_ref.at<float>(i) = static_cast<float>(sum_ref / total_pixels);
	}

	cv::Mat lut(1, 256, CV_8UC1);
	int j = 0;
	for (int i = 0; i < 256; ++i) {
		while (j < 255 && cdf_ref.at<float>(j) < cdf_src.at<float>(i)) {
			j++;
		}
		lut.at<uchar>(i) = static_cast<uchar>(j);
	}

	cv::Mat matched_channel;
	cv::LUT(source_channel, lut, matched_channel);

	return matched_channel;
}

cv::Mat match_histograms_rgb(const cv::Mat &source_img, const cv::Mat &reference_img)
{
	if (source_img.empty() || reference_img.empty() || source_img.channels() != 3 ||
	    reference_img.channels() != 3) {
		throw std::invalid_argument("Both images must be non-empty and 3-channel (RGB).");
	}

	cv::Mat src_8u;
	cv::Mat ref_8u;
	source_img.convertTo(src_8u, CV_8UC3);
	reference_img.convertTo(ref_8u, CV_8UC3);

	std::vector<cv::Mat> source_channels;
	cv::split(src_8u, source_channels);
	std::vector<cv::Mat> reference_channels;
	cv::split(ref_8u, reference_channels);

	std::vector<cv::Mat> matched_channels(3);

	for (int i = 0; i < 3; ++i) {
		matched_channels[i] =
			matchHistogramSingleChannel(source_channels[i], reference_channels[i]);
	}

	cv::Mat matched_img;
	cv::merge(matched_channels, matched_img);

	return matched_img;
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <source_image_path> <reference_image_path>"
			  << std::endl;
		return 1;
	}

	const cv::Mat source_img = cv::imread(argv[1], cv::IMREAD_COLOR);
	const cv::Mat reference_img = cv::imread(argv[2], cv::IMREAD_COLOR);

	if (source_img.empty()) {
		std::cerr << "Error: Could not load source image." << std::endl;
		return 1;
	}

	if (reference_img.empty()) {
		std::cerr << "Error: Could not load reference image." << std::endl;
		return 1;
	}

	try {
		const cv::Mat matched_img = match_histograms_rgb(source_img, reference_img);

		cv::imshow("Source Image", source_img);
		cv::imshow("Reference Image", reference_img);
		cv::imshow("Matched Image", matched_img);

		cv::waitKey(0);
		cv::destroyAllWindows();
	} catch (const std::exception &e) {
		std::cerr << "An error occurred: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
