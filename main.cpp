#include "cv.h"
#include "highgui.h"
#include "originalVibe.h"
#include<iostream>
using namespace std;
using namespace cv;
int main(int, char*argv[])
{
	/*视频流的输入*/
	VideoCapture cap(argv[1]);
	if(!cap.isOpened()) 
		return -1;
	

	/*视频帧图像*/
	Mat frame;

	/*前景-背景检测及显示窗口*/
	Mat seg;	

	/*创建vibe背景建模的对象*/
	OriginalVibe vibe(20,2,20,16,3,3);
	Mat frameGray;
	
	int number =0;
	for(;;)
	{
		cap >> frame;
		if(! frame.data)
			break;
	
		number++;
		if(number == 1)
		{
			vibe.originalVibe_Init_BGR( frame );
			continue;
		}
		else
		{
			vibe.originalVibe_ClassifyAndUpdate_BGR(frame,seg);
			medianBlur(seg,seg,5);	
			imshow("segmentation", seg);
		}
		imshow("frame",frame);
		if(waitKey(10) >= 0) 
			break;
	}

	return 0;
}
