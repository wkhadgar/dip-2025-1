/**
* @file task-03-filtering-sp-noise.cpp
 * @author Paulo Santos (pauloxrms@gmail.com)
 * @brief Removes salt and pepper noise from image.
 * @version 0.1
 * @date 08-09-2025
 *
 * @copyright Copyright (c) 2025 Paulo Santos
 *
 */

#include <opencv2/opencv.hpp>
#include <vector>

#define KERNEL_SIZE 4

static auto get_neighboring_median(cv::Mat &img, const int x, const int y, const uint8_t pad)
	-> uint8_t
{
	std::vector<uint8_t> neighbors;

	for (int w_pad = -pad; w_pad <= pad; w_pad++) {
		for (int h_pad = -pad; h_pad <= pad; h_pad++) {
			neighbors.push_back(img.at<uint8_t>(x + w_pad, y + h_pad));
		}
	}

	std::sort(neighbors.begin(), neighbors.end());

	return neighbors[neighbors.size() / 2];
}

auto remove_salt_and_pepper_noise(const cv::Mat &img) -> cv::Mat
{
	constexpr uint8_t pad = KERNEL_SIZE / 2;

	cv::Mat pad_safe_img;
	cv::copyMakeBorder(img, pad_safe_img, pad, pad, pad, pad, cv::BORDER_REFLECT_101);

	cv::Mat denoised_img = img.clone();
	for (int i = pad; i < pad_safe_img.rows - pad; i++) {
		for (int j = pad; j < pad_safe_img.cols - pad; j++) {
			const auto median = get_neighboring_median(pad_safe_img, i, j, pad);
			denoised_img.at<uint8_t>(i - pad, j - pad) = median;
		}
	}

	return denoised_img;
}

int main()
{
	const cv::Mat img = cv::imread("head.png", cv::IMREAD_GRAYSCALE);
	const cv::Mat denoised_img = remove_salt_and_pepper_noise(img);

	cv::imwrite("out/head_filtered.png", denoised_img);

	return 0;
}
