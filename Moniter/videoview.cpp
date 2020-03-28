#include "videoview.h"
#include <qdebug.h>

videoview::videoview(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	cvInitFont(&font,CV_FONT_HERSHEY_COMPLEX,1.0,1.0,0,2,8);
	CarNum=0;
	is_judge=false;
	crosslane_judge=true;
	timer=new QTimer(this);
	db=QSqlDatabase::addDatabase ("QODBC"); 
	db.setDatabaseName("monitor");
	db.setHostName("fifi");
	if(!db.open ())  
    {  
        QMessageBox qmb;
	    qmb.warning(this,"",QStringLiteral("fail"),QMessageBox::Yes);
    }
}

videoview::~videoview()
{
	
}


void videoview::on_openfile_clicked(void) //打开视频
{
	int number=0;
	is_scaned=false;
	CarNum=0;
	is_judge=false;
	crosslane_judge=false;
	QString fileName;
	fileName=QFileDialog::getOpenFileName(this,"Open video File","../video","*.*");
	timer->stop();
	
	capture.open(fileName.toStdString());
	if(!capture.isOpened()) ui.label_2->setText(QStringLiteral("打开失败"));
	if (capture.isOpened())
    {
		
        double rate= capture.get(CV_CAP_PROP_FPS);//获取帧速率
        capture >> frame;
        if (!frame.empty())
        {
            image1 = Mat2QImage(frame);
			image1 = image1.scaled(ui.label_2->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            ui.label_2->setPixmap(QPixmap::fromImage(image1));
			timer->setInterval(1000/rate);   //set timer match with FPS
            connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
            timer->start();
        }
    }


}
void videoview::nextFrame()
{
    capture >> frame;
    if (!frame.empty())
    {
        image1 = Mat2QImage(frame);
		image1 = image1.scaled(ui.label_2->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui.label_2->setPixmap(QPixmap::fromImage(image1));
        //this->update();
    }

}


QImage videoview::Mat2QImage(Mat cvImg)
{
	QImage qImg;
    if(cvImg.channels()==3)                             //3 channels color image
    {

        cv::cvtColor(cvImg,cvImg,CV_BGR2RGB);
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }
    else if(cvImg.channels()==1)                    //grayscale image
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_Indexed8);
    }
    else
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }

    return qImg;
}

void videoview::on_calculate_cars_clicked(){
	if(capture.isOpened()) {
		timer->stop();
		double rate= capture.get(CV_CAP_PROP_FPS);//获取帧速率
           capture >> frame;
		   //qmb.warning(this,"",QStringLiteral("duru"),QMessageBox::Yes);
           if(!frame.empty()){
			 is_scaned=true;
			 threadDetect(frame);
             binaryResult = BinaryDetect(frame, frame);
			 image = Mat2QImage(binaryResult);
			 image = image.scaled(ui.label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			 tmp = frame.clone();
			 ui.label->setPixmap(QPixmap::fromImage(image));
			 image1=Mat2QImage(result);
	         image1 = image1.scaled(ui.label_2->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
             ui.label_2->setPixmap(QPixmap::fromImage(image1));
			 //timer1 = new QTimer(this);
             timer->setInterval(1000/rate); //set timer match with FPS
             connect(timer, SIGNAL(timeout()), this, SLOT(nextResult()));
             timer->start();
            }    
		}
		//capture.release();
	}


Mat videoview::BinaryDetect(Mat frame1, Mat frame2)
{
     Mat gray1, gray2;
     cvtColor(frame1, gray1, CV_BGR2GRAY);
     cvtColor(frame2, gray2, CV_BGR2GRAY);
	 Mat diff;
     absdiff(gray1, gray2, diff);//计算两帧之间的差的绝对值
	 threshold(diff, diff, 25, 255, CV_THRESH_BINARY);//图象二值化
	 Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
     Mat element2 = getStructuringElement(MORPH_RECT, Size(19, 19));
	 medianBlur(diff, diff, 9);//中值滤波
	 
	 dilate(diff, diff, element2,Point(-1,-1),2);
	 erode(diff, diff, element,Point(-1,-1),2);
	 return diff;
 }



void videoview::nextResult(void)
{
	QMessageBox qmb;
	//qmb.warning(this,"",QStringLiteral("next"),QMessageBox::Yes);
	capture >> frame;
	//if(frame.empty()) qmb.warning(this,"",QStringLiteral("dianji"),QMessageBox::Yes);
	if (!frame.empty()){
	binaryResult = BinaryDetect(tmp, frame);
	image = Mat2QImage(binaryResult);
    image = image.scaled(ui.label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui.label->setPixmap(QPixmap::fromImage(image));
	//tmpimg = Mat2QImage(binaryResult);
    //tmpimg = tmpimg.scaled(ui.label_3->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	 //ui.label_3->setPixmap(QPixmap::fromImage(tmpimg));
	//Mat binary=binaryResult;
	result=moveDetect(binaryResult,frame);
	image1=Mat2QImage(result);
	image1 = image1.scaled(ui.label_2->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui.label_2->setPixmap(QPixmap::fromImage(image1));
	tmp = frame.clone();
	}
}


Mat videoview::moveDetect(Mat diff, Mat frame)
{
	result=frame.clone();
	std::vector<std::vector<Point>> contours;
    std::vector<Vec4i> hierarcy;
    findContours(diff, contours, hierarcy, CV_RETR_EXTERNAL,  CV_CHAIN_APPROX_SIMPLE, Point(0, 0));//查找轮廓,此处diff被涂改
    std::vector<std::vector<Point>>contours_poly(contours.size());
    std::vector<Rect> boundRect(contours.size()); //定义外接矩形集合
     int x0 = 0, y0 = 0, w0 = 0, h0 = 0;
     for (int i = 0; i<contours.size(); i++)
     {
		 //对图像轮廓点进行多边形拟合：轮廓点组成的点集，输出的多边形点集，精度（即两个轮廓点之间的距离），输出多边形是否封闭
         approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
         boundRect[i] = boundingRect(Mat(contours_poly[i]));
         if (boundRect[i].width>100 && boundRect[i].width<800 && boundRect[i].height>100 && boundRect[i].height<800) {//轮廓筛选
             x0 = boundRect[i].x;
             y0 = boundRect[i].y;
             w0 = boundRect[i].width;
             h0 = boundRect[i].height;
             rectangle(result, Point(x0, y0), Point(x0 + w0, y0 + h0), Scalar(0, 255, 0), 2, 8, 0);

			 car_numbercount(x0,y0,w0,h0);
//if(crosslane_judge==true) car_legaljudge(x0,y0,w0,h0);
		 }
	 }
             
      putText(result,"Car Number:"+intToString(CarNum),Point(1500,100), CV_FONT_HERSHEY_SIMPLEX, 1.5f, Scalar(0, 255, 0), 3);

	  //运动轨迹
	  for (int i = 0; i < cars.size(); i++) {
		   for (int j = 0; j < cars[i].centers.size() - 1; j++) {
			   //qDebug() << "line" << endl;
			   line(result, cars[i].centers[j], cars[i].centers[j+1], 
				   cars[i].is_runRedLight||cars[i].is_cross?Scalar(0, 0, 255):Scalar(0,255,0), 3, 8);
			}
		  
	  }

	  /*if(red_light==true)
		  putText(result, "red light",Point(1500,200), CV_FONT_HERSHEY_SIMPLEX, 1.5f, Scalar(0, 0, 255), 3);
	  else if (red_light == false)
		  putText(result, "green light", Point(1500, 200), CV_FONT_HERSHEY_SIMPLEX, 1.5f, Scalar(0, 255, 0), 3);*/
	  putText(result, lightstatus.c_str(), Point(1500, 200), CV_FONT_HERSHEY_SIMPLEX, 1.5f, Scalar(0, 255, 0), 3);

	  line(result, Point(500, 650), Point(1550, 650), Scalar(0, 0, 255), 3, 8);//画红线
	  if(is_judge==true){
		  for(int j=0;j<verti.size();j+=2){
			  line(result,verti[j],verti[j+1],Scalar(0,0,255),3);
			  savetodb_line(verti[j],verti[j+1]);
		  }
	  }
	  
	 return result;
}

std::string videoview::intToString(int number)
 {
     //this function has a number input and string output
     std::stringstream ss;
     ss << number;
     return ss.str();
 }


bool videoview::savetodb(Mat img, QString url)
{
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
	std::string name = current_date_time.toString("yyyyMMddhhmmss").toStdString() + ".jpg";
	imwrite("../" + url.toStdString() + name, img);
	bool is_fail = false;
	/*string pre = "python F://dachuang/lic_re/PR.py ";
	const char *com = (pre+"F:/monitor/" + url.toStdString() + name).c_str();
	
	printf("%s\n",com);
	
	FILE *fp;
	fp = _popen(com, "w");
	if (fp == NULL) {

		is_fail = true;
		printf("%s\n", "fail");

	}
	_pclose(fp);*/
	
	//ifstream fin("../lic_re/rere.txt");
	string nResult;
	//if (!is_fail) {
		//getline(fin, nResult);
	//}
	//else {
		nResult = "12345";
	//}
	//std::cout << nResult.size();
	//fin.close();
	mf.upload(url.toStdString(),name);
	url=url + current_date_time.toString("yyyyMMddhhmmss") + ".jpg";
	QString l = QString::fromStdString(nResult);
	db.exec("INSERT INTO car_info (license_id,picture_url,action_time) VALUES('"+l+"','"+url+"','"+date+"')");
	return false;
}


int videoview::car_numbercount(int x0, int y0, int w0, int h0)
{	
	if ((y0 + h0 / 2 + 1) >= 550 && (y0 + h0 / 2 - 1) <= 900 && (x0 >= 200) && (x0 <= 1500) && 
			w0 >= 180 && h0 >= 180) {//经过这个区间，车辆数量+1
		Point center = Point(x0 + w0 / 2, y0 + h0 / 2);
		bool isCalcued = false;
		for (int j = 0; j < cars.size(); j++) {

			//qDebug()<<j<<" "<<cars[j].centers.back().x<<" "<<cars[j].centers.back().y;
			if ((std::abs(center.y - cars[j].centers.back().y) <= 150) 
				&& (std::abs(center.x - cars[j].centers.back().x) <= 100)) {
				isCalcued = true;
				cars[j].centers.push_back(center);
				cars[j].centers.push_back(center);
				car_legaljudge(x0, y0, w0, h0, j);
				break;
			}
		}
		if (isCalcued == false) {
			CarNum++;
			Rect rect(x0 - 50, y0 - 50, w0 + 50, h0 + 50);
			Mat img = frame.clone();
			Mat image_roi = img(rect);
			QString url = "pictures/car_count/";
			printf("%d %d %d %d\n", x0, y0, w0, h0);
			savetodb(image_roi, url);
			carObject car;
			car.centers.push_back(center);
			cars.push_back(car);
		}
	}
	else {
		for (int j = 0; j < cars.size(); j++) {
			Point center = Point(x0 + w0 / 2, y0 + h0 / 2);
			if ((std::abs(center.y - cars[j].centers.back().y) <= 150) 
				&& (std::abs(center.x - cars[j].centers.back().x) <= 100)) {
				cars.erase(cars.begin() + j);
				break;
			}
		}
	} 
	return 0;
}


void videoview::on_lane_finder_clicked(void)
{
    is_judge=true;
	if(is_scaned==false){
		threadDetect(frame);
	}
	else {
		is_scaned = false;
		verti.clear();
		Point pt1, pt2, pt3, pt4, pt5, pt6;
		pt1.x = 625; pt1.y = 655;
		verti.push_back(pt1);
		pt2.x = 462; pt2.y = 1079;
		verti.push_back(pt2);
		pt3.x = 1072; pt3.y = 655;
		verti.push_back(pt3);
		pt4.x = 1239; pt4.y = 1079;
		verti.push_back(pt4);
	}
	
}


int videoview::car_legaljudge(int x0, int y0, int w0, int h0,int j)
{
	//闯红灯判断
	int centersize = cars[j].centers.size();
	bool is_legal = true;
	switch (light_state) {
		case 0:
			break;
		case 1://仅允许直行
			if ((cars[j].centers[0].x-x0>100)&&(x0<650)&&(y0 + h0 / 2) <= 700 && (y0 + h0 / 2) >= 600 ) {
				is_legal = false;
			}
			break;
		case 2://仅允许左转
			if ((y0 + h0 / 2) <= 700 && (y0 + h0 / 2) >= 600 && x0 >= 300) {
				is_legal = false;
			}
			break;
		case 3://不许通行
			if ((y0 + h0 / 2) <= 700 && (y0 + h0 / 2) >= 600 && x0 <= 1500) {
				is_legal = false;
			}
			break;
	}
	if (is_legal==false && cars[j].is_runRedLight == false) {
		cars[j].is_runRedLight = true;
		Rect rect(x0 - 50, y0 - 50, w0 + 50, h0 + 50);
		Mat img = frame.clone();
		Mat image_roi = img(rect);
		QString url = "pictures/illegals/run_redlights/";
		savetodb_illegal(image_roi, url, QStringLiteral("run redlight"));
	}
//越线判断
	if (crosslane_judge == true) {
		for (int i = 0; i < verti.size(); i += 2) { //遍历检查是否越线
			if (x0 + 250 < ((verti[i].x + verti[i + 1].x) / 2) && x0 + w0 - 30 > ((verti[i].x + verti[i + 1].x) / 2)
				&& (y0 + h0 > verti[i].y) && cars[j].is_cross == false) {
				cars[j].is_cross = true;
				Rect rect(x0 - 50, y0 - 50, w0 + 50, h0 + 50);
				Mat img = frame.clone();
				Mat image_roi = img(rect);
				QString url = "pictures/illegals/cross_lane/";
				savetodb_illegal(image_roi, url, QStringLiteral("cross line"));
			}
		}
	}
        

	return 0;
}


int videoview::threadDetect(Mat frame)
{  
	  verti.clear();
	  Mat gray;
      cvtColor(frame, gray, CV_BGR2GRAY);
	 threshold(gray, gray, 120, 255, CV_THRESH_BINARY);//图象二值化
	  Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
     Mat element2 = getStructuringElement(MORPH_RECT, Size(19, 19));
	  medianBlur(gray, gray, 3);//中值滤波
	 erode(gray, gray, element);
	 erode(gray, gray, element,Point(-1,-1),2);
	 finder.setLengthAndGap(250, 15);  
     finder.setminVote(60);  
     std::vector<Vec4i> lines = finder.findLines(gray); 
	 std::vector<Vec4i>::const_iterator it = lines.begin();  
        while (it!=lines.end())  
        {  
            cv::Point pt1((*it)[0], (*it)[1]);  
            cv::Point pt2((*it)[2], (*it)[3]);  
			
			 if(std::abs(pt2.x-pt1.x)<=300&&std::abs(pt1.y-pt2.y)>=200&&pt1.y>=500&&pt2.y>=500){
				bool is_exist=false;
				for(int i=0;i<verti.size();i+=2){
					if (pt1.x-verti[i].x<=100){
						is_exist=true;
						if(getlength(pt1,pt2)>getlength(verti[i],verti[i+1])){
							 if(pt1.y>pt2.y) {
								 verti[i]=pt2;
								 verti[i+1]=pt1;
				             }
				             else{
					           verti[i+1]=pt2;
							   verti[i]=pt1;
				             }
						}
						break;
					}
				}
				if(is_exist==false){
				    if(pt1.y>pt2.y) {
					    verti.push_back(pt2);verti.push_back(pt1);
				    }
				    else{
					    verti.push_back(pt1);verti.push_back(pt2);
				    }
			    }
			}
            it++;  
        }
	  //image1=Mat2QImage(gray);
	 //image1 = image1.scaled(ui.label_3->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    // ui.label_3->setPixmap(QPixmap::fromImage(image1));

	return 0;
}


double videoview::getlength(Point p1, Point p2)
{
	return std::sqrt(std::pow(p1.x-p2.x,2)+std::pow(p1.y-p2.y,2));
}


void videoview::on_cross_lane_clicked(void)
{
	crosslane_judge=true;
}


void videoview::savetodb_illegal(Mat img,QString url,QString type)
{
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
	std::string name = current_date_time.toString("yyyyMMddhhmmss").toStdString() + ".jpg";
	imwrite("../" + url.toStdString() + name, img);
	//string pre = "python F://dachuang/lic_re/PR.py ";
	//const char *com = (pre + "F:/monitor/" + url.toStdString() + name).c_str();
	//printf("%s\n", com);
	bool is_fail = false;
	//FILE *fp;
	//fp = _popen(com, "r");
	/*if (fp == NULL) {

		is_fail = true;
		printf("%s\n", "fail");

	}
	_pclose(fp);*/
    //ifstream fin("F://dachuang/lic_re/rere.txt");
	string nResult;
	//if (!is_fail) {
		//getline(fin, nResult);
	//}
	//else {
		nResult = "00000";
	//}
	//getline(fin, nResult);
	//std::cout << nResult.size();
	//fin.close();
	mf.upload(url.toStdString(), name);
	url = url + current_date_time.toString("yyyyMMddhhmmss") + ".jpg";
	QString l = QString::fromStdString(nResult);
	db.exec("INSERT INTO illegals (license_id,picture_url,action_time,action_type) VALUES('" + l + "','" +url+"','"+date+"','"+type+"')");
	//QString instruct = "INSERT INTO illegals (license_id,picture_url,action_time,action_type) VALUES('12345','" + url + "','" + date + "','" + type + "')";
	//QMessageBox qmb;
	//qmb.warning(this, "",instruct , QMessageBox::Yes);

}


int videoview::savetodb_line(Point point1, Point point2)
{
	QString x1=QString::number(point1.x,10);
	QString x2=QString::number(point2.x,10);
	QString y1=QString::number(point1.y,10);
	QString y2=QString::number(point2.y,10);
	db.exec("INSERT INTO line_info (x1,y1,x2,y2) VALUES('"+x1+"','"+y1+"','"+x2+"','"+y2+"')");
	return 0;
}


/*void videoview::on_change_light_clicked()
{
	if (red_light == false) {
		ui.change_light->setText(QStringLiteral("设置为绿灯"));
		red_light = true;
	}
	else {
		ui.change_light->setText(QStringLiteral("设置为红灯"));
		red_light = false;
	}
	
}*/

void videoview::on_light_kind_changed()
{
	light_state = ui.light_kind->currentIndex();
	switch (light_state) {
	case 0:
		lightstatus = "pass";
		break;
	case 1:
		lightstatus = "straight permitted";
		break;
	case 2:
		lightstatus = "left permitted";
		break;
	case 3:
		lightstatus = "fobidden";
		break;
	}

	
}
