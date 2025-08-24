/**
 * @file task-05-load-image-from-url.cpp
 * @author Paulo Santos (pauloxrms@gmail.com)
 * @brief Loads an image from an URL.
 * @version 0.1
 * @date 22-08-2025
 *
 * @copyright Copyright (c) 2025 Paulo Santos
 *
 */

#include <iostream>
#include <vector>
#include <string>

#include <curl/curl.h>
#include <opencv2/opencv.hpp>

struct ImageData {

	std::vector<char> data;

	void write_data(void *contents, const size_t size)
	{
		this->data.insert(this->data.end(), static_cast<char *>(contents),
				  static_cast<char *>(contents) + size);
	}
};

static auto curl_write_cb(void *contents, const size_t size, const size_t nmemb, void *userp)
	-> size_t
{
	size_t rsize = size * nmemb;
	auto *mem = static_cast<ImageData *>(userp);
	mem->write_data(contents, rsize);
	return rsize;
}

/**
 * Loads an image from a given URL using libcurl and OpenCV's.
 *
 * @param url Direct link to the image file.
 * @param flags OpenCV imread flags (e.g., cv::IMREAD_COLOR or cv::IMREAD_GRAYSCALE).
 * @return The loaded image as a cv::Mat object.
 */
auto load_image_from_url(const std::string &url, const int flags = cv::IMREAD_COLOR) -> cv::Mat
{
	ImageData chunk;

	curl_global_init(CURL_GLOBAL_ALL);
	CURL *curl_handle = curl_easy_init();

	if (!curl_handle) {
		throw std::runtime_error("Could not initialize curl.");
	}

	curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, curl_write_cb);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, static_cast<void *>(&chunk));

	CURLcode res = curl_easy_perform(curl_handle);

	curl_easy_cleanup(curl_handle);
	curl_global_cleanup();

	if (res != CURLE_OK) {
		const std::string error_msg =
			"curl_easy_perform() failed: " + std::string(curl_easy_strerror(res));
		throw std::runtime_error(error_msg);
	}

	if (chunk.data.empty()) {
		throw std::runtime_error("No data received from URL.");
	}

	cv::Mat image = cv::imdecode(chunk.data, flags);

	if (image.empty()) {
		throw std::runtime_error("Could not decode image from URL.");
	}

	return image;
}

int main(const int argc, char *argv[])
{
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <image_url> [--grayscale]" << std::endl;
		return 1;
	}

	const std::string url = argv[1];
	int flag = cv::IMREAD_COLOR;
	if (argc > 2 && std::string(argv[2]) == "--grayscale") {
		flag = cv::IMREAD_GRAYSCALE;
	}

	std::cout << "[INFO] Downloading image from: " << url << std::endl;

	try {
		const cv::Mat img = load_image_from_url(url, flag);
		std::cout << "[INFO] Image loaded successfully." << std::endl;

		cv::namedWindow("Image from URL", cv::WINDOW_KEEPRATIO);
		cv::imshow("Image from URL", img);
		cv::waitKey(0);
		cv::destroyAllWindows();
	} catch (const std::exception &e) {
		std::cerr << "[ERROR] " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
