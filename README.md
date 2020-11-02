# Moniter
使用帧差法对交通监控视频
已基本实现车辆的计数、用户通过按钮设置红绿灯以实现闯红灯检测和简单的越线检测，将检测结果截图保存至服务器，并在数据库中存储图像的位置。

## 程序安装
首先需安装openCV3， 用Visual Studio 打开此项目，添加Opencv的库至依赖库，在VS中安装QT5，修改myftp中的服务器地址，具体可以查询网上教程<https://blog.csdn.net/liang19890820/article/details/53931813>，运行即可。

## 程序使用步骤：
1. 本程序尚未与摄像头连接，仅作为算法测试使用，待处理视频保存在电脑中。因此首先需选择视频并打开。程序获取视频后，检查视频的帧速率，并且以此为标准设定定时器，处理并显示视频的每一帧图像。
2. 在打开视频后，可选择“车道检测”功能使用Hough变换检测车道，同时将车道信息存入数据库。
3. 对连续的两帧图像使用运动物体检测算法，将算法产生的图像输入运动物体标记跟踪算法。
4. 运动物体标记跟踪算法检测所有轮廓并判定其所属对象的性质。若为新对象，则在原图像的相应位置截图，并调用车牌识别程序识别车辆的车牌，而后实时将图片上传至服务器，将车辆信息存入数据库，同时新建车辆对象、设置其初始位置，将其存至表示视野中所有车辆的全局变量中；否则在与之对应的车辆对象的位置链表头部添加新的坐标信息。
5. 检查车辆对象是否存在闯红灯行为。当前的允许通行状态与真实道路对应，包括：允许左转和直行、仅允许直行、仅允许左转、禁止通行等四种状态。将当前允许通行状态、车辆坐标、车辆轨迹性质结合判断车辆是否违章，若违章则截图、识别车牌、上传图片并将信息存入对应数据库。
6. 检查车辆对象是否存在越线行为。将车道线坐标、车辆坐标结合判断，若车辆越过车道线，则将车辆对象相应状态值设为真。若有越线则截图、识别车牌、上传图片并将信息存入数据库。
7. 在原始图像中标记车道线、车辆和车辆相应的行为并显示。


## 当前问题：
1. 红灯时，车辆右转并不违反交通规则，程序不能体现这一点
2. 精确度较低，漏测、误测情况普遍。

## 程序截图演示
![image](https://github.com/fania98/Moniter/blob/master/pic/show.JPG)

