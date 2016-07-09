#ifndef ORIGINALVIBE_H
#define ORIGINALVIBE_H
#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<vector>
using namespace cv;

class OriginalVibe{
public:
	//构造函数
	OriginalVibe(int _numberSamples, int _minMatch,int _distanceThreshold,int _updateFactor,int _neighborWidth,int _neighborHeight):numberSamples(_numberSamples),minMatch(_minMatch),distanceThreshold(_distanceThreshold),updateFactor(_updateFactor),neighborWidth(_neighborWidth),neighborHeight(_neighborHeight){};	
	~OriginalVibe(){};
	//操作成员变量
	void setUpdateFactor(int _updateFactor);
	//灰度图像
	void originalVibe_Init_GRAY(const Mat &firstFrame);
	void originalVibe_ClassifyAndUpdate_GRAY(const Mat &frame,OutputArray &_segmentation);
	//RGB三通道
	void originalVibe_Init_BGR(const Mat & firstFrame);
	void originalVibe_ClassifyAndUpdate_BGR(const Mat &frame,OutputArray &_segmentation);

private:
	//背景模型
	const int numberSamples;
	std::vector<Mat>  backgroundModel;
	//像素点的分类判断的参数
	const int minMatch;
	int distanceThreshold;
	//背景模型更新概率
	 int updateFactor;
	//8-领域(3 x 3)
	const int neighborWidth;
	const int neighborHeight;
	//前景和背景分割
	const static  unsigned char BACK_GROUND;
	const static  unsigned char FORE_GROUND;	
	//BGR的距离计算
	int distanceL1(const  Vec3b &src1,const  Vec3b &src2);
	float distanceL2(const Vec3b &src1,const	Vec3b &src2);
};
#endif
