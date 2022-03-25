#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void main(){
	Mat img, hsv, mask, blur;
	VideoCapture cap(0);

	//canny
	int can_lo = 50, can_hi = 200;
	//selektor warna kuning
	int h_low = 0, s_low = 127, v_low = 140;
	int h_hi = 26, s_hi = 255, v_hi = 255;

	//slider untuk mengatur range nilai HSV
	namedWindow("slider");
	createTrackbar("hue low : ", "slider", &h_low, 255);
	createTrackbar("sat low : ", "slider", &s_low, 255);
	createTrackbar("val low : ", "slider", &v_low, 255);
	createTrackbar("hue hi : ", "slider", &h_hi, 255);
	createTrackbar("sat hi : ", "slider", &s_hi, 255);
	createTrackbar("val hi : ", "slider", &v_hi, 255);

	//slider canny
	namedWindow("canny");
	createTrackbar("canny lo : ", "canny", &can_lo, 1000);
	createTrackbar("canny hi : ", "canny", &can_hi, 1000);

	while(true){
		Scalar low(h_low, s_low, v_low);
		Scalar hi(h_hi, s_hi, v_hi);
		cap.read(img);
		cvtColor(img, hsv, COLOR_BGR2HSV);
		inRange(hsv, low, hi, mask);
		Canny(mask, mask, can_lo, can_hi, 3);
		
		vector<Vec2f> lines;
		HoughLines(mask, lines, 1, CV_PI / 180, 150, 0, 0);

		for (size_t i = 0; i < lines.size(); i++)
		{
			float rho = lines[i][0], theta = lines[i][1];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a * rho, y0 = b * rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(img, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);
		}
		
		imshow("image ori", img);
		imshow("image edge", mask);
		waitKey(1);
	}
}
