#pragma once
#include <opencv2/objdetect/objdetect.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <qfiledialog.h>
#include <qtimer.h>
#include <qmessagebox.h>
#include <vector>
#define PI 3.1415926535
class linefinder
{
public:
	//linefinder(void);
	~linefinder(void);  
    cv::Mat img;  
    //包含被检测直线的端点的向量  
    std::vector<cv::Vec4i> lines;  
  
    //累加器分辨率参数  
    double deltaRho;  
    double deltaTheta;  
      
    //确认直线之前必须受到的最小投票数  
    int minVote;  
  
    //直线的最小长度  
    double minLength;  
    //直线上允许的最大空隙  
    double maxGap;  
    linefinder() :deltaRho(1), deltaTheta(PI / 180), minVote(5), minLength(0.0), maxGap(0.0) {}  
    void setAccResolution(double dRho, double dTheta)  
    {  
        deltaRho = dRho;  
        deltaTheta = dTheta;  
    }  
    void setminVote(int minv)  
    {  
        minVote = minv;  
    }  
    void setLengthAndGap(double length, double gap)  
    {  
        minLength = length;  
        maxGap = gap;  
    }  
    std::vector<cv::Vec4i> findLines(cv::Mat& binary)  
    {  
        lines.clear();  
        cv::HoughLinesP(binary, lines, deltaRho, deltaTheta, minVote, minLength, maxGap);  
		//cv::HoughLines(binary,lines,deltaRho,deltaTheta,minVote);
        return lines;  
    }  
    void drawDetectedLines(cv::Mat& image, cv::Scalar color = cv::Scalar(255, 0, 0))  
    {  
        std::vector<cv::Vec4i>::const_iterator it = lines.begin();  
        while (it!=lines.end())  
        {  
            cv::Point pt1((*it)[0], (*it)[1]);  
            cv::Point pt2((*it)[2], (*it)[3]);  
            cv::line(image, pt1, pt2, color);  
            it++;  
        }  
    }  
};  

