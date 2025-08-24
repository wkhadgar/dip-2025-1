/**
 * @file task-07-linear-combination.cpp
 * @author Paulo Santos (pauloxrms@gmail.com)
 * @brief Linearly merges two images, interactively.
 * @version 0.1
 * @date 22-08-2025
 *
 * @copyright Copyright (c) 2025 Paulo Santos
 *
 */

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

class linCombData
{
      public:
	cv::Mat i1;
	cv::Mat i2;
	cv::Mat output;
	int alpha_slider_value = 50;

	linCombData() = default;
};

auto onTrackbar(const int new_pos, void *lin_comb_data) -> void
{
	if (lin_comb_data == nullptr) {
		return;
	}

	auto *data = static_cast<linCombData *>(lin_comb_data);

	const double alpha = new_pos / 100.0;
	const double beta = 1.0 - alpha;

	cv::addWeighted(data->i1, alpha, data->i2, beta, 0.0, data->output);
	cv::imshow("Linear Combination", data->output);
}

int main(const int argc, char *argv[])
{
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <image1_path> <image2_path>" << std::endl;
		return -1;
	}

	linCombData lin_comb_data = {
		.i1 = cv::imread(argv[1], cv::IMREAD_COLOR),
		.i2 = cv::imread(argv[2], cv::IMREAD_COLOR),
		.alpha_slider_value = 50,
	};

	if (lin_comb_data.i1.empty() || lin_comb_data.i2.empty()) {
		std::cerr << "Error: One or both images could not be loaded. Check file paths."
			  << std::endl;
		return -1;
	}

	if (lin_comb_data.i1.size() != lin_comb_data.i2.size()) {
		std::cerr << "Error: Input images must have the same dimensions." << std::endl;
		return -1;
	}

	cv::namedWindow("I1", cv::WINDOW_KEEPRATIO);
	cv::namedWindow("I2", cv::WINDOW_KEEPRATIO);
	cv::namedWindow("Linear Combination", cv::WINDOW_KEEPRATIO);

	onTrackbar(0, &lin_comb_data);

	cv::createTrackbar("Combination Weight", "Linear Combination", nullptr, 100, onTrackbar,
			   &lin_comb_data);

	cv::imshow("I1", lin_comb_data.i1);
	cv::imshow("I2", lin_comb_data.i2);

	while (true) {
		if (const auto key = static_cast<char>(cv::waitKey(1));
		    // 'q' || 'Esc'
		    key == 'q' || key == 27) {
			break;
		}
	}

	cv::destroyAllWindows();

	return 0;
}
