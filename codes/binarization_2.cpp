#include <iostream>
#include <vector>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, const char *argv[])
{
	if (argc != 2) {
		std::cerr << "Usage: ./binarization path_to_img" << std::endl;
		return 1;
	}

	// read image
	cv::Mat img = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
	cv::Mat img_binary = cv::Mat(img.rows, img.cols, CV_8UC1, cv::Scalar(255));

	// weights
	std::vector<std::vector<float>> w1(img.rows, std::vector<float>(img.cols, 0));
	// centroids
	std::pair<float, float> c;

	// init weights
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			w1[i][j] = img.at<unsigned char>(i,j)/255.0;
		}
	}

	// stopping criteria
	float eps = 0;

	do {
		// calculate centroids
		std::pair<float, float> c1_fraction{0,0};
		std::pair<float, float> c2_fraction{0,0};

		for (int i = 0; i < img.rows; i++) {
			for (int j = 0; j < img.cols; j++) {
				c1_fraction.first += w1[i][j]*w1[i][j]*img.at<unsigned char>(i,j);
				c1_fraction.second += w1[i][j]*w1[i][j];
				c2_fraction.first += (1-w1[i][j])*(1-w1[i][j])*img.at<unsigned char>(i,j);
				c2_fraction.second += (1-w1[i][j])*(1-w1[i][j]);
			}
		}

		auto old_c = c;
		c = {c1_fraction.first/c1_fraction.second, c2_fraction.first/c2_fraction.second};
		eps = (old_c.first-c.first)*(old_c.first-c.first) + (old_c.second-c.second)*(old_c.second-c.second);

		// update weights
		for (int i = 0; i < img.rows; i++) {
			for (int j = 0; j < img.cols; j++) {
				float d1 = (img.at<unsigned char>(i,j)-c.first)*(img.at<unsigned char>(i,j)-c.first);
				float d2 = (img.at<unsigned char>(i,j)-c.second)*(img.at<unsigned char>(i,j)-c.second);
				w1[i][j] = d2/(d2+d1);
			}
		}
	} while(eps > 1);

	// cluster pixels based on weights 
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			img_binary.at<unsigned char>(i,j) = (w1[i][j] > 0.5) ? 255 : 0;
		}
	}

	// show and save binary image
	namedWindow("Display window", cv::WINDOW_AUTOSIZE);
	imshow("Display window", img_binary);
	cv::waitKey(0);
	imwrite("fcm.png", img_binary);

    return 0;
}
