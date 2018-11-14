#ifndef VIDEOVIEW_H
#define VIDEOVIEW_H

#include <QtWidgets/QWidget>
#include "ui_videoview.h"
#include <opencv2/objdetect/objdetect.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <qfiledialog.h>
#include <qtimer.h>
#include <qmessagebox.h>
#include <vector>
#include<qdatetime.h>
#include <Windows.h>
#include "linefinder.h"


#include <QSqlQuery>  
#include <QVariant>  
#include <QDebug>  
#include <QtSql>  
#include <QSqlError>  
using namespace cv;

class videoview : public QWidget
{
	Q_OBJECT

public:
	videoview(QWidget *parent = 0);
	VideoCapture capture;
	~videoview();
	Mat frame;
	Mat tmp;
	QImage tmpimg;
	Mat result,binaryResult;
	QImage image,image1;
	QTimer *timer;
	int CarNum;
	CvFont font;
	std::vector<Point> centers;
	std::vector<Point> illegalcenters;
	Range R1,R2;
	bool is_judge;
	 QSqlDatabase db;
	 linefinder finder;
	 Point hori_1,hori_2;
	 std::vector<Point> verti;
	 bool is_scaned;
	 bool crosslane_judge;
	 bool red_light = false;
private:
	Ui::videoviewClass ui;
private slots:
	void on_openfile_clicked(void);
	void on_calculate_cars_clicked(void);
	void on_lane_finder_clicked(void);
	void nextFrame(void);
	void nextResult(void);
	Mat moveDetect(Mat frame1, Mat frame2);
	Mat BinaryDetect(Mat frame1, Mat frame2);
    void on_cross_lane_clicked(void);
	void on_change_light_clicked();
public:
	QImage Mat2QImage(Mat cvImg);
	std::string intToString(int number);
	
	bool savetodb(Mat img,QString url);
	int car_numbercount(int x0, int y0, int w0, int h0);
	int car_legaljudge(int x0, int y0, int w0, int h0);
	int threadDetect(Mat frame);
	double getlength(Point p1, Point p2);
	void savetodb_illegal(Mat img);
	int savetodb_line(Point point1, Point point2);
	
};

#endif // VIDEOVIEW_H
