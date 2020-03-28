#pragma once
#include <vector>
#include <opencv2/objdetect/objdetect.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
using namespace std;
class carObject 
{
public:
	carObject();
	~carObject();
	vector<cv::Point> centers;
	bool is_runRedLight=false;
	bool is_cross = false;
};

