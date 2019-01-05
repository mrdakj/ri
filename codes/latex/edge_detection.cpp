#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main(int argc, const char *argv[])
{
	if (argc != 3) {
		std::cerr << "Usage: ./binarization path_to_img threshold" << std::endl;
		return 1;
	}

	// read image
	cv::Mat img = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

	float threshold = std::atof(argv[2]);
	const int L = 256;
	std::vector<std::vector<float>> mi(img.rows, std::vector<float>(img.cols, 0));
	auto output = gaussian_blur(img);

	float maxm = 0;
	for (int i = 1; i < output.rows-1; i++) {
		for (int j = 1; j < output.cols-1; j++) {
			unsigned s = 0;
			for (int x = -1; x <= 1; x++) {
				for (int y = -1; y <= 1; y++) {
					s += std::abs(output.at<unsigned char>(i,j)-output.at<unsigned char>(i+x,j+y));
				}
			}
			mi[i][j] = (1.0*s)/(s+(L-1));
			maxm = std::max(maxm, mi[i][j]);
		}
	}

	for (int i = 1; i < output.rows-1; ++i) {
		for (int j = 1; j < output.cols-1; j++) {
			output.at<unsigned char>(i,j) = (mi[i][j]/maxm <  threshold) ? 0 : 255;
		}
	}

	namedWindow("Display window", cv::WINDOW_AUTOSIZE);
	imshow("Display window", output);
	cv::waitKey(0);
	imwrite("edge_detection.png", output);

	return 0;
}
