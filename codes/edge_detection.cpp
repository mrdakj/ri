#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <numeric>

cv::Mat gaussian_blur(const cv::Mat& image)
{
	const int gauss_filter_size = 5;
	const std::vector<float> gauss
		{2.0/159, 4.0/159, 5.0/159, 4.0/159, 2.0/159, 
		4.0/159, 9.0/159, 12.0/159, 9.0/159, 4.0/159,
		5.0/159, 12.0/159, 15.0/159, 12.0/159, 5.0/159,
		4.0/159, 9.0/159, 12.0/159, 9.0/159, 4.0/159,
		2.0/159, 4.0/159, 5.0/159, 4.0/159, 2.0/159};

	cv::Mat output = cv::Mat(image.rows, image.cols, CV_8UC1, cv::Scalar(0));

	for (int i = 0; i < image.rows-gauss_filter_size+1; ++i) {
		for (int j = 0; j <= image.cols-gauss_filter_size; ++j) {
			std::vector<float> current;
			current.reserve(gauss_filter_size*gauss_filter_size);
			for (int x = 0; x < gauss_filter_size; ++x) {
				for (int y = 0; y < gauss_filter_size; ++y) {
					current.push_back(image.at<unsigned char>(i+x, j+y));
				}
			}

			output.at<unsigned char>(i + gauss_filter_size/2, j + gauss_filter_size/2) = std::inner_product(gauss.cbegin(), gauss.cend(), current.cbegin(), 0.0);
		}
	}

	int thickness = gauss_filter_size/2;
	// set borders
	for (int i = 0; i < output.rows; ++i) {
		for (int j = 0; j < thickness; ++j) {
			output.at<unsigned char>(i, j) = image.at<unsigned char>(i, j);
			output.at<unsigned char>(i, output.cols-1-j) = image.at<unsigned char>(i, output.cols-1-j);
		}
	}

	for (int j = 0; j < output.cols; j++) {
		for (int i = 0; i < thickness; i++) {
			output.at<unsigned char>(i, j) = image.at<unsigned char>(i, j); 
			output.at<unsigned char>(output.rows-1-i, j) = image.at<unsigned char>(output.rows-1-i, j); 
		}
	}

	return output;
}

int main(int argc, const char *argv[])
{
	if (argc != 2) {
		std::cerr << "Usage: ./binarization path_to_img" << std::endl;
		return 1;
	}

	// read image
	cv::Mat img = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

	float threshold = 0.35;
	const int L = 256;
	std::vector<std::vector<unsigned>> mi(img.rows, std::vector<unsigned>(img.cols, 0));
	auto output = gaussian_blur(img);

	unsigned maxm = 0;
	for (int i = 1; i < output.rows-1; i++) {
		for (int j = 1; j < output.cols-1; j++) {
			unsigned s =0;
			for (int x = -1; x <= 1; x++) {
				for (int y = -1; y <= 1; y++) {
					s+=std::abs(output.at<unsigned char>(i,j)-output.at<unsigned char>(i+x,j+y));
				}
			}
			mi[i][j] = 1000*s/(s+(L-1));
			maxm = std::max(maxm, mi[i][j]);
		}
	}

	unsigned limit = maxm*threshold;

	for (int i = 1; i < output.rows-1; ++i) {
		for (int j = 1; j < output.cols-1; j++) {
			output.at<unsigned char>(i,j) = (mi[i][j] < limit) ? 0 : 255;
		}
	}

	namedWindow("Display window", cv::WINDOW_AUTOSIZE);
	imshow("Display window", output);
	cv::waitKey(0);
	imwrite("edge_detection.png", output);

    return 0;
}
