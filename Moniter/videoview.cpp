#include "videoview.h"

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
	db.setDatabaseName("mysql");
	db.setHostName("192.168.43.250");
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
	fileName=QFileDialog::getOpenFileName(this,"Open video File","F://大创","*.*");
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
	QMessageBox qmb;
	
	
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
	 medianBlur(diff, diff, 3);//中值滤波
	 erode(diff, diff, element);
	 dilate(diff, diff, element2,Point(-1,-1),2);
	 erode(diff, diff, element,Point(-1,-1),2);
	 
     
	 //dilate(diff,diff,element2);
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
                                           //drawContours(img2, contours, -1, Scalar(0, 0, 255), 1, 8);  //绘制轮廓
     int x0 = 0, y0 = 0, w0 = 0, h0 = 0;
     for (int i = 0; i<contours.size(); i++)
     {
         approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);//对图像轮廓点进行多边形拟合：轮廓点组成的点集，输出的多边形点集，精度（即两个轮廓点之间的距离），输出多边形是否封闭
         boundRect[i] = boundingRect(Mat(contours_poly[i]));
         if (boundRect[i].width>150 && boundRect[i].width<800 && boundRect[i].height>200 && boundRect[i].height<800) {//轮廓筛选
             x0 = boundRect[i].x;
             y0 = boundRect[i].y;
             w0 = boundRect[i].width;
             h0 = boundRect[i].height;
             rectangle(result, Point(x0, y0), Point(x0 + w0, y0 + h0), Scalar(0, 255, 0), 2, 8, 0);
			 car_numbercount(x0,y0,w0,h0);
			 if(crosslane_judge==true) car_legaljudge(x0,y0,w0,h0);
		 }
	 }
             
      putText(result,"Car Number:"+intToString(CarNum),Point(1500,100), CV_FONT_HERSHEY_SIMPLEX, 1.5f, Scalar(0, 255, 0), 3);
	  if(red_light==true)
		  putText(result, "red light",Point(400,100), CV_FONT_HERSHEY_SIMPLEX, 1.5f, Scalar(255, 0, 0), 3);
	  else if (red_light == false)
		  putText(result, "green light", Point(400, 100), CV_FONT_HERSHEY_SIMPLEX, 1.5f, Scalar(0, 255, 0), 3);
	  line(result, Point(500, 650), Point(1550, 650), Scalar(0, 0, 255), 3, 8);//画红线
	  //line(result,hori_1,hori_2,Scalar(0,0,255),3);
	  if(is_judge==true){
         //line(result,Point(580,650),Point(300,1200),Scalar(0,0,255),3,8);
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
	url=url + current_date_time.toString("yyyyMMddhhmmss") + ".jpg";
	imwrite("F://大创/"+url.toStdString(),img);
	db.exec("INSERT INTO car_info (licence_id,url,time) VALUES('12345','"+url+"','"+date+"')");
	return false;
}


int videoview::car_numbercount(int x0, int y0, int w0, int h0)
{
	
	if ((y0 + h0 / 2 + 1) >= 600 && (y0 + h0 / 2 - 1) <= 700 && (x0 >= 500) && (x0 <= 1500) && w0 >= 200 && x0 >= 200) {//经过这条线（区间），车辆数量+1
		if (centers.size() >= 3) centers.erase(centers.begin());
		Point center = Point(x0 + w0 / 2, y0 + h0 / 2);
		bool isCalcued = false;
		for (int j = 0; j < centers.size(); j++) {

			ui.label_3->setText(QString::number(centers.size(), 10) + " " + QString::number(centers[j].x, 10) + " " + QString::number(centers[j].y, 10));
			if ((std::abs(center.y - centers[j].y) <= 30) && (std::abs(center.x - centers[j].x) <= 50)) {
				isCalcued = true;
				centers[j].y = center.y;
				centers[j].x = center.x;
				break;
			}
			else if (centers[j].y <= 620) centers.erase(centers.begin() + j);

		}
		if (isCalcued == false) {
			CarNum++;
			Rect rect(x0 - 50, y0 - 50, w0 + 50, h0 + 50);
			Mat img = frame.clone();
			Mat image_roi = img(rect);
			QString url = "pictures/car_count/";
			savetodb(image_roi, url);
			if (red_light == true) {
				 url = "pictures/illegals/run_redlights/";
				 savetodb(image_roi, url);
				 putText(result, "闯红灯" , Point(x0, y0), CV_FONT_HERSHEY_SIMPLEX, 1.5f, Scalar(0, 255, 0), 3);
			}
			centers.push_back(center);
		}
	}
        
         //putText(result, "CarNum=" + intToString(CarNum), org, CV_FONT_HERSHEY_SIMPLEX, 0.8f, Scalar(0, 255, 0), 2);
	  
	return 0;
}


void videoview::on_lane_finder_clicked(void)
{
    is_judge=true;
	//if(is_scaned==false){
	//threadDetect(frame);
	//}
	//is_scaned=false;
	verti.clear();
	Point pt1,pt2,pt3,pt4,pt5,pt6;
	pt1.x=625;pt1.y=655;
	verti.push_back(pt1);
	pt2.x=462;pt2.y=1079;
	verti.push_back(pt2);
	pt3.x=1072;pt3.y=655;
	verti.push_back(pt3);
	pt4.x=1239;pt4.y=1079;
	verti.push_back(pt4);
	
}


int videoview::car_legaljudge(int x0, int y0, int w0, int h0)
{
	QMessageBox qmb;
	//qmb.warning(this,"",QString::number(x0,10)+QStringLiteral(" y:")+QString::number(y0,10)+QString::number(x0+w0,10)+" "+QString::number(y0+h0,10),QMessageBox::Yes);
	bool isCalcued=true;
	Point center;
	for(int i=0;i<verti.size();i+=2){ //遍历检查是否越线
		if(x0+250<((verti[i].x+verti[i+1].x)/2)&&x0+w0-30>((verti[i].x+verti[i+1].x)/2)&&(y0+h0>verti[i].y)){
			//qmb.warning(this,"",QStringLiteral("越线"),QMessageBox::Yes);
			 if(illegalcenters.size()>=4) illegalcenters.erase(illegalcenters.begin());
				 center=Point(x0+w0/2,y0+h0/2); //若越线记录车辆中点坐标
				 isCalcued=false;
				 for(int j=0;j<illegalcenters.size();j++){ //检查是否已被记录
					  //ui.label_3->setText(QString::number(centers.size(),10)+" "+QString::number(centers[j].x,10)+" "+QString::number(centers[j].y,10));
					 if((std::abs(center.y-illegalcenters[j].y)<=60)&&(std::abs(center.x-illegalcenters[j].x)<=80)){
						 isCalcued=true;
						 illegalcenters[j].y=center.y;
						 illegalcenters[j].x=center.x;
						 break;
					 }
					 else if(illegalcenters[j].y<=620) illegalcenters.erase(illegalcenters.begin()+j);
					 
				 }
		}
	}
                 if(isCalcued==false) {
					 Rect rect(x0-50, y0-50,w0+50,h0+50);
					 //ui.label_3->setText(" "+QString::number(x0,10)+" "+QString::number(y0,10)+QString::number(x0+w0,10)+" "+QString::number(y0+h0,10));
					 //QMessageBox qmb;
					 //qmb.warning(this,"",QString::number(x0,10)+QStringLiteral(" y:")+QString::number(y0,10)+QString::number(x0+w0,10)+" "+QString::number(y0+h0,10),QMessageBox::Yes);
					 Mat img=frame.clone();
                     Mat image_roi = img(rect);
					 savetodb_illegal(image_roi);
					 illegalcenters.push_back(center);

					 //putText(result,"x:"+intToString(center.x)+"y"+intToString(center.y),Point(1500,200), CV_FONT_HERSHEY_SIMPLEX, 1.5f, Scalar(0, 255, 0), 3);
					 //QMessageBox qmb;
					 //qmb.warning(this,"",QString::number(center.x,10)+QStringLiteral(" y:")+QString::number(center.y,10),QMessageBox::Yes);
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
	 //dilate(gray, gray, element2,Point(-1,-1),1);
	 erode(gray, gray, element,Point(-1,-1),2);
	 //Canny(frame, gray, 350, 400); 
	 finder.setLengthAndGap(250, 15);  
     finder.setminVote(60);  
     //Mat contours;  
     //Canny(gray, contours, 350, 400);  
     std::vector<Vec4i> lines = finder.findLines(gray); 
	 std::vector<Vec4i>::const_iterator it = lines.begin();  
        while (it!=lines.end())  
        {  
            cv::Point pt1((*it)[0], (*it)[1]);  
            cv::Point pt2((*it)[2], (*it)[3]);  
			
			 if(std::abs(pt2.x-pt1.x)<=300&&std::abs(pt1.y-pt2.y)>=200&&pt1.y>=500&&pt2.y>=500){
				//line(result,pt1,pt2,Scalar(0,255,0),3);
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
				//line(frame,pt1,pt2,Scalar(0,0,255));
			    }
			}
            it++;  
        }
	  //finder.drawDetectedLines(frame);  
	  image1=Mat2QImage(gray);
	 image1 = image1.scaled(ui.label_3->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
     ui.label_3->setPixmap(QPixmap::fromImage(image1));

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


void videoview::savetodb_illegal(Mat img)
{
	QDateTime current_date_time =QDateTime::currentDateTime();
	QString url="D://pictures/img"+current_date_time.toString("yyyyMMddhhmmss")+".jpg";
	QString date=current_date_time.toString("yyyy-MM-dd hh:mm:ss");
	imwrite(url.toStdString(),img);
	db.exec("INSERT INTO car_crossline (licence_id,url,time) VALUES('12345','"+url+"','"+date+"')");
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


void videoview::on_change_light_clicked()
{
	if (red_light == false) {
		ui.change_light->setText(QStringLiteral("设置为绿灯"));
		red_light = true;
	}
	else {
		ui.change_light->setText(QStringLiteral("设置为红灯"));
		red_light = false;
	}
	
}
