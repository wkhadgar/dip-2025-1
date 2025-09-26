/**
 * @file task-09-geometric-transformations.cpp
 * @author Paulo Santos (pauloxrms@gmail.com)
 * @brief Geometric transforms an image.
 * @version 0.1
 * @date 24-09-2025
 *
 * @copyright Copyright (c) 2025 Paulo Santos
 *
 */

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include <map>
#include <opencv2/opencv.hpp>

using TransformResults = std::map<std::string, cv::Mat>;

/**
 * @brief Applies various geometric transformations to a grayscale image.
 * * The logic mimics the coordinate mapping performed by the NumPy implementation.
 * OpenCV's cv::remap is used for most transformations, which requires map_x and map_y
 * matrices specifying the source coordinates for every destination pixel.
 *
 * @param img Input grayscale image (CV_8UC1).
 * @return A map containing the transformed images.
 */
TransformResults apply_geometric_transformations(const cv::Mat &img)
{
	if (img.empty() || img.channels() != 1) {
		throw std::invalid_argument(
			"Input image must be a non-empty grayscale (1-channel) image.");
	}

	TransformResults geo_dict;
	int N = img.rows;
	int M = img.cols;

	cv::Mat map_x;
	cv::Mat map_y;

	int tx = 200;
	int ty = 200;

	cv::Mat translated = cv::Mat::zeros(N + std::abs(ty), M + std::abs(tx), img.type());

	int start_y = std::max(0, ty);
	int start_x = std::max(0, tx);
	cv::Rect roi(start_x, start_y, M, N);

	img.copyTo(translated(roi));

	geo_dict["translated"] = translated;

	cv::Mat rotated = cv::Mat::zeros(M, N, img.type());
	map_x.create(M, N, CV_32FC1);
	map_y.create(M, N, CV_32FC1);

	for (int i = 0; i < rotated.rows; ++i) {
		for (int j = 0; j < rotated.cols; ++j) {
			map_x.at<float>(i, j) = static_cast<float>(i);
			map_y.at<float>(i, j) = static_cast<float>(M - 1 - j);
		}
	}
	cv::transpose(img, rotated);
	cv::flip(rotated, rotated, 1);

	geo_dict["rotated"] = rotated;
	double scale_x = 1.5;
	auto modified_M = static_cast<int>(M * scale_x);

	cv::Mat stretched = cv::Mat::zeros(N, modified_M, img.type());

	cv::resize(img, stretched, cv::Size(modified_M, N), 0, 0, cv::INTER_NEAREST);

	geo_dict["stretched"] = stretched;

	cv::Mat mirrored;
	cv::flip(img, mirrored, 1);

	geo_dict["mirrored"] = mirrored;

	map_x.create(N, M, CV_32FC1);
	map_y.create(N, M, CV_32FC1);

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			// Normalized coordinates [ -1, 1 ]
			double x_c = (j - M / 2.0) / (M / 2.0);
			double y_c = (i - N / 2.0) / (N / 2.0);

			// Radial distance
			double r_sq = x_c * x_c + y_c * y_c;

			// Distortion factor
			double k = 0.3;
			double factor = 1.0 + k * r_sq;

			double x_dist = x_c * factor;
			double y_dist = y_c * factor;

			// Convert back to original pixel coordinates [ 0, M-1 ]
			auto x_new = static_cast<float>((x_dist + 1.0) * (M / 2.0));
			auto y_new = static_cast<float>((y_dist + 1.0) * (N / 2.0));

			map_x.at<float>(i, j) = x_new;
			map_y.at<float>(i, j) = y_new;
		}
	}

	cv::Mat distorted = cv::Mat::zeros(N, M, img.type());

	cv::remap(img, distorted, map_x, map_y, cv::INTER_LINEAR, cv::BORDER_CONSTANT,
		  cv::Scalar(0));

	geo_dict["distorted"] = distorted;

	return geo_dict;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <image_path>" << std::endl;
		return 1;
	}

	// Load image in grayscale
	const cv::Mat example_image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

	if (example_image.empty()) {
		std::cerr << "Error: Could not load image at " << argv[1] << std::endl;
		return 1;
	}

	try {
		const TransformResults results = apply_geometric_transformations(example_image);

		// Show results
		cv::imshow("Original", example_image);
		cv::imshow("Translated", results.at("translated"));
		cv::imshow("Rotated", results.at("rotated"));
		cv::imshow("Stretched", results.at("stretched"));
		cv::imshow("Mirrored", results.at("mirrored"));
		cv::imshow("Distorted (Barrel)", results.at("distorted"));

		std::cout << "Original Size: " << example_image.cols << "x" << example_image.rows
			  << std::endl;
		std::cout << "Stretched Size: " << results.at("stretched").cols << "x"
			  << results.at("stretched").rows << std::endl;

		cv::waitKey(0);
		cv::destroyAllWindows();
	} catch (const std::exception &e) {
		std::cerr << "An error occurred: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
