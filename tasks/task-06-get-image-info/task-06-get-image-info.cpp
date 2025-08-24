/**
 * @file task-06-get-image-info.cpp
 * @author Paulo Santos (pauloxrms@gmail.com)
 * @brief Gets information from an image.
 * @version 0.1
 * @date 22-08-2025
 *
 * @copyright Copyright (c) 2025 Paulo Santos
 *
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include <opencv2/opencv.hpp>

/**
 * @class ImageInfo
 * @brief A class to hold image metadata and statistics.
 *
 * This class provides a structured way to store and retrieve
 * information about an image, similar to the dictionary in the Python script.
 */
class ImageInfo
{
      public:
	int width = 0;
	int height = 0;
	std::string dtype;
	int depth = 0;
	size_t nbytes = 0;
	std::map<std::string, std::map<std::string, double, std::less<>>, std::less<>> statistics;

	ImageInfo() = default;

	[[nodiscard]] std::string toString() const
	{
		std::stringstream ss;
		ss << "{\n";
		ss << "  \"width\": " << width << ",\n";
		ss << "  \"height\": " << height << ",\n";
		ss << R"(  "dtype": ")" << dtype << "\",\n";
		ss << "  \"depth\": " << depth << ",\n";
		ss << "  \"nbytes\": " << nbytes << ",\n";
		ss << "  \"statistics\": {\n";
		for (const auto &[channel_idx, data] : statistics) {
			ss << "    \"" << channel_idx << "\": {\n";
			for (const auto &[stat, value] : data) {
				ss << "      \"" << stat << "\": " << value << ",\n";
			}
			ss << "    },\n";
		}
		ss << "  }\n";
		ss << "}";
		return ss.str();
	}
};

/**
 * @brief Extracts metadata and statistical information from an image.
 *
 * @param image Input image (grayscale or color).
 * @return A populated ImageInfo object.
 * @throws std::invalid_argument if the input is not a valid image.
 */
ImageInfo getImageInfo(const cv::Mat &image)
{
	if (image.empty()) {
		throw std::invalid_argument("Input must be a valid image.");
	}

	ImageInfo info;
	info.width = image.cols;
	info.height = image.rows;
	info.dtype = "CV_8U";
	info.depth = image.channels();
	info.nbytes = image.total() * image.elemSize();

	if (info.depth == 1) {
		double minVal;
		double maxVal;
		cv::Point minLoc;
		cv::Point maxLoc;
		cv::minMaxLoc(image, &minVal, &maxVal, &minLoc, &maxLoc);

		cv::Scalar mean;
		cv::Scalar stddev;
		cv::meanStdDev(image, mean, stddev);

		// Calculate median
		cv::Mat flatImage = image.reshape(1, 1);
		std::vector pixels(flatImage.begin<uchar>(), flatImage.end<uchar>());
		std::sort(pixels.begin(), pixels.end());
		double median = pixels[pixels.size() / 2];

		info.statistics["single_channel"]["min"] = minVal;
		info.statistics["single_channel"]["max"] = maxVal;
		info.statistics["single_channel"]["mean"] = mean[0];
		info.statistics["single_channel"]["std_dev"] = stddev[0];
		info.statistics["single_channel"]["median"] = median;

		// Note: Histogram calculation is done in main for demonstration purposes
	} else {
		// Multi-channel
		std::vector<cv::Mat> channels;
		cv::split(image, channels);

		for (size_t i = 0; i < channels.size(); ++i) {
			double minVal;
			double maxVal;
			cv::Point minLoc;
			cv::Point maxLoc;
			cv::minMaxLoc(channels[i], &minVal, &maxVal, &minLoc, &maxLoc);

			cv::Scalar mean;
			cv::Scalar stddev;
			cv::meanStdDev(channels[i], mean, stddev);

			// Calculate median for each channel
			cv::Mat flatChannel = channels[i].reshape(1, 1);
			std::vector pixels(flatChannel.begin<uchar>(), flatChannel.end<uchar>());
			std::sort(pixels.begin(), pixels.end());
			double median = pixels[pixels.size() / 2];

			std::string channel_name = "channel_" + std::to_string(i);
			info.statistics[channel_name]["min"] = minVal;
			info.statistics[channel_name]["max"] = maxVal;
			info.statistics[channel_name]["mean"] = mean[0];
			info.statistics[channel_name]["std_dev"] = stddev[0];
			info.statistics[channel_name]["median"] = median;
		}
	}

	return info;
}

int main(const int argc, char **argv)
{
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <image_path>" << std::endl;
		return -1;
	}

	const std::string image_path = argv[1];
	const cv::Mat image = cv::imread(image_path, cv::IMREAD_UNCHANGED);

	if (image.empty()) {
		std::cerr << "Could not read the image: " << image_path << std::endl;
		return -1;
	}

	try {
		const ImageInfo info = getImageInfo(image);
		std::cout << "Image Info for: " << image_path << "\n";

		constexpr int histSize = 256;
		constexpr std::array<float, 2> range = {0, 256};
		const float *histRange = {(range.data())};
		constexpr bool uniform = true;
		constexpr bool accumulate = false;

		if (info.depth == 1) {
			cv::Mat hist;
			cv::calcHist(&image, 1, nullptr, cv::Mat(), hist, 1, &histSize, &histRange,
				     uniform, accumulate);
			std::cout << "Histogram for single channel:\n" << hist.t() << std::endl;
		} else {
			std::vector<cv::Mat> channels;
			cv::split(image, channels);
			std::vector<cv::Mat> hists(info.depth);
			for (int i = 0; i < info.depth; ++i) {
				cv::calcHist(&channels[i], 1, nullptr, cv::Mat(), hists[i], 1,
					     &histSize, &histRange, uniform, accumulate);
				std::cout << "Histogram for channel " << i << ":\n"
					  << hists[i].t() << std::endl;
			}
		}

		std::cout << "\nStatistics (excluding histogram):\n" << std::endl;
		for (const auto &[channel_idx, data] : info.statistics) {
			std::cout << channel_idx << " statistics:\n";
			for (const auto &[stat, value] : data) {
				std::cout << "  " << stat << ": " << value << "\n";
			}
		}

	} catch (const std::invalid_argument &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}
