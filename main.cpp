#include<iostream>
#include<opencv2/opencv.hpp>
#include<math.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
	Mat src, white, dst, color_dst;
	src = imread("sample02.png");
	if (!src.data) return -1;

	cv::namedWindow("src");
	cv::imshow("src", src);

	src.convertTo(src, CV_64FC3);
	Mat3f src3f;
	src3f = Mat3f(src);

	for (int y = 0; y < src.rows; ++y) {
		for (int x = 0; x < src.cols; ++x) {
			if (src3f(y, x)(0) > 200 &&
				src3f(y, x)(1) > 200 &&
				src3f(y, x)(2) > 200) {
				for (int c = 0; c < 3; ++c) {
					src3f(y, x)(c) = 255;
				}
			}
			else {
				for (int c = 0; c < 3; ++c) {
					src3f(y, x)(c) = 0;
				}
			}
		}
	}

	src = Mat(src3f);
	src.convertTo(src, CV_8UC3);

	Canny(src, dst, 180, 220, 3);
	cv::namedWindow("canny");
	cv::imshow("canny", dst);
	cvtColor(dst, color_dst, CV_GRAY2BGR);

#if 0
	vector<Vec2f> lines;
	HoughLines(dst, lines, 1, CV_PI / 180, 100);

	for (size_t i = 0; i < lines.size(); ++i)
	{
		float rho = lines[i][0];
		float theta = lines[i][1];
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b * rho;
		Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
		Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
		line(color_dst, pt1, pt2, Scalar(0, 0, 255), 3, 8);
	}
#else
	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI / 180, 80, 30, 10);
	for (size_t i = 0; i < lines.size(); i++) 
	{
		line(color_dst, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(0, 0, 255), 3, 8);
	}
#endif
	namedWindow("source", 1);
	imshow("source", src);

	namedWindow("Detected Lines", 1);
	imshow("Detected Lines", color_dst);
	waitKey(0);

	return 0;
}