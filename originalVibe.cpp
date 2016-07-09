#include"originalVibe.h"
#include<iostream>
const unsigned char OriginalVibe::BACK_GROUND = 0;
const unsigned char OriginalVibe::FORE_GROUND = 255;

//操作成员变量
void OriginalVibe::setUpdateFactor(int _updateFactor)
{
	this->updateFactor = _updateFactor;
}
//第一种方法：最原始的vibe灰度通道
void OriginalVibe::originalVibe_Init_GRAY(const Mat &firstFrame)
{
	int height = firstFrame.rows;
	int width = firstFrame.cols;
	//背景模型分配内存
	backgroundModel.clear();
	for(int index = 0;index < this->numberSamples;index++)
	{
		backgroundModel.push_back(Mat::zeros(height,width,CV_8UC1));
	}
	//随机数
	RNG rng;
	int cshift;
	int rshift;
	for(int r = 0;r < height ;r++)
	{
		for(int c = 0;c < width ; c++)
		{
			if( c < neighborWidth/2 || c > width - neighborWidth/2 -1|| r < neighborHeight/2 || r > height - neighborHeight/2 -1)
			{
				/*随机数的生成方式有很多种*/
				/*
				cshift = randu<int>()%neighborWidth - neighborWidth/2;
				rshift = randu<int>()%neighborHeight - neighborHeight/2;
				*/	
				cshift = rand()%neighborWidth - neighborWidth/2;
				rshift = rand()%neighborHeight - neighborHeight/2;
				
				for(std::vector<Mat>::iterator it = backgroundModel.begin();it != backgroundModel.end();it++)
				{
					for(;;)
					{
						/*
						cshift = rng.uniform(-neighborWidth/2,neighborWidth/2 + 1);
						rshift = rng.uniform(-neighborHeight/2,neighborHeight/2 +1 );
						*/		
						cshift = abs(randu<int>()%neighborWidth) - neighborWidth/2;
						rshift = abs(randu<int>()%neighborHeight) - neighborHeight/2;
						
						if(!(cshift == 0 && rshift==0))
							break;
					}	
					if(c + cshift < 0 || c + cshift >=width)
						cshift *= -1;
					if(r + rshift < 0 || r + rshift >= height)
						rshift *= -1;
					(*it).at<uchar>(r,c) = firstFrame.at<uchar>(r+rshift,c+cshift);
				}
			}
			else
			{
				for(std::vector<Mat>::iterator it = backgroundModel.begin();it != backgroundModel.end();it++)
				{
					for(;;)
					{
						/*
						cshift = rng.uniform(-neighborWidth/2,neighborWidth/2 + 1);
						rshift = rng.uniform(-neighborHeight/2,neighborHeight/2 +1 );
						*/
						cshift = abs(randu<int>()%neighborWidth) - neighborWidth/2;
						rshift = abs(randu<int>()%neighborHeight) - neighborHeight/2;
						if(!(cshift == 0 && rshift == 0))
							break;
					}
					(*it).at<uchar>(r,c) = firstFrame.at<uchar>(r+rshift,c+cshift);
				}
			}
		}
	}
}
void OriginalVibe::originalVibe_ClassifyAndUpdate_GRAY(const Mat &frame,OutputArray &_segmentation)
{
	int width = frame.cols;
	int height = frame.rows;
	int rshift;
	int cshift;
	_segmentation.create(frame.size(),CV_8UC1);
	Mat segmentation = _segmentation.getMat();
	
	RNG rng;
	for(int r = 0; r < height;r++)
	{
		for(int c = 0;c < width ;c++)
		{
			int count = 0;
			unsigned char pixel = frame.at<uchar>(r,c);
			//让pixel和背景模板中backgroundModel进行比较
			for(std::vector<Mat>::iterator it = backgroundModel.begin();it != backgroundModel.end();it++)
			{
				if( abs( int(pixel) - int( (*it).at<uchar>(r,c)) )  < (this->distanceThreshold) )
				{
					count++;
					//循环到一定阶段，判断count的值是否大于 minMatch,更新背景模型
					if( count >= this->minMatch)
					{
						int random = rng.uniform(0,this->updateFactor);
						if(random == 0)
						{
							int updateIndex = rng.uniform(0,this->numberSamples);
							backgroundModel[updateIndex].at<uchar>(r,c) = pixel;
						}
						random = rng.uniform(0,this->updateFactor);
						if(random == 0)
						{
							if(c < neighborWidth/2 || c > width - neighborWidth/2-1 || r < neighborHeight/2 || r > height - neighborHeight/2-1)
							{
								for(;;)
								{
									/*
									 cshift = rng.uniform(-neighborWidth/2,neighborWidth/2 + 1);
									 rshift = rng.uniform(-neighborHeight/2,neighborHeight/2 +1 );
									*/
									cshift = abs(randu<int>()%neighborWidth) - neighborWidth/2;
									rshift = abs(randu<int>()%neighborHeight) - neighborHeight/2;
									if(!(cshift == 0 && rshift ==0))
										break;
								}		
								if(c + cshift < 0 || c + cshift >=width)
									cshift *= -1;
								if(r + rshift < 0 || r + rshift >= height)
									rshift *= -1;
								int updateIndex = rng.uniform(0,this->numberSamples);
								backgroundModel[updateIndex].at<uchar>(r+rshift,c+cshift) = pixel;
							}
							else
							{
								for(;;)
								{
									/*
									cshift = rng.uniform(-neighborWidth/2,neighborWidth/2 + 1);
									rshift = rng.uniform(-neighborHeight/2,neighborHeight/2 +1 );
									*/
									cshift = abs(randu<int>()%neighborWidth) - neighborWidth/2;
									rshift = abs(randu<int>()%neighborHeight) - neighborHeight/2;
									if(!(cshift == 0 && rshift==0))
										break;
								}								
								int updateIndex = rng.uniform(0,this->numberSamples);
								backgroundModel[updateIndex].at<uchar>(r+rshift,c+cshift) = pixel;
							}						
						}
						segmentation.at<uchar>(r,c) = this ->BACK_GROUND;
						break;
					}
				}
			}
			if( count < this->minMatch)	
			segmentation.at<uchar>(r,c) = this->FORE_GROUND;
		}
	}
}


//第三种方法：BGR通道
void OriginalVibe::originalVibe_Init_BGR(const Mat & fristFrame)
{
	int height = fristFrame.rows;
	int width = fristFrame.cols;
	//背景模型分配内存
	backgroundModel.clear();
	for(int index = 0;index  < this->numberSamples;index++)
	{
		backgroundModel.push_back( Mat::zeros(height,width,CV_8UC3) );
	}
	//随机数
	RNG rng;
	int cshift;
	int rshift;
	for(int r =0 ; r < height; r++)
	{
		for(int c = 0;c < width ;c++)
		{
			if( c < neighborWidth/2 || c > width - neighborWidth/2 -1|| r < neighborHeight/2 || r > height - neighborHeight/2 -1 )
			{
				/*
				 初始化背景模型：开始
				*/
				for(vector<Mat>::iterator iter = backgroundModel.begin(); iter != backgroundModel.end();iter++)
				{
					for(;;)
					{
						cshift = abs(randu<int>()%neighborWidth) - neighborWidth/2;
						rshift = abs(randu<int>()%neighborHeight) - neighborHeight/2;
						if(!(cshift == 0 && rshift==0))
							break;
					}
					if(c + cshift < 0 || c + cshift >=width)
						cshift *= -1;
					if(r + rshift < 0 || r + rshift >= height)
						rshift *=-1;
					(*iter).at<Vec3b>(r,c) = fristFrame.at<Vec3b>(r+rshift,c+cshift);
				}
			}
			/*初始化背景模型：结束*/
			else
			{
				/*******初始化背景模型：开始******/
				for(vector<Mat>::iterator iter = backgroundModel.begin(); iter != backgroundModel.end();iter++)
				{
					for(;;)
					{
						cshift = abs(randu<int>()%neighborWidth) - neighborWidth/2;
						rshift = abs(randu<int>()%neighborHeight) - neighborHeight/2;
						if( !(cshift == 0 && rshift==0) )
							break;
					}
					(*iter).at<Vec3b>(r,c) = fristFrame.at<Vec3b>(r+rshift,c+cshift);
				}
				/*****初始化背景模型：结束 ******/
			}	
		}
	}
}

float OriginalVibe::distanceL2(const Vec3b & src1,const  Vec3b& src2)
{
	return pow( pow(src1[0]-src2[0],2.0) +pow(src1[1]-src2[1],2.0) + pow(src1[2] - src2[2],2.0),0.5);
}
int OriginalVibe::distanceL1(const Vec3b & src1,const  Vec3b& src2)
{
	return abs(src1[0]-src2[0])+abs(src1[1] - src2[1])+abs(src1[2]-src2[2]) ;
}

void OriginalVibe::originalVibe_ClassifyAndUpdate_BGR(const Mat &frame,OutputArray &_segmentation)
{//*编号1
	int height = frame.rows;
	int width = frame.cols;
	int cshift;
	int rshift;
	_segmentation.create(frame.size(),CV_8UC1);
	Mat segmentation = _segmentation.getMat();

	RNG rng;

	for(int r =0 ;r < height; r++)
	{//编号1-1
		for(int c = 0;c < width ;c++)
		{//编号1-1-1
			int count = 0;
			Vec3b pixel = frame.at<Vec3b>(r,c);
			for( vector<Mat>::iterator iter = backgroundModel.begin() ;iter != backgroundModel.end(); iter++)
			{//编号1-1-1-1
				//
				//
				if( distanceL1(pixel,(*iter).at<Vec3b>(r,c)) < 4.5*this->distanceThreshold )
				{
					count++;
					if(count >= this->minMatch)
					{
						//第一步:更新模型update
						/**********开始更新模型*************/
						int random = rng.uniform(0,this->updateFactor);
						if(random == 0)
						{
							int updateIndex = rng.uniform(0,this->numberSamples);
							backgroundModel[updateIndex].at<Vec3b>(r,c) = pixel;
						}

						random = rng.uniform(0,this->updateFactor);
						if(random == 0)
						{
							/****************************************/
							if( c < neighborWidth/2 || c > width - neighborWidth/2-1 || r < neighborHeight/2 || r > height - neighborHeight/2-1 )
							{
								for(;;)
								{
									cshift = abs(randu<int>()%neighborWidth) - neighborWidth/2;
									rshift = abs(randu<int>()%neighborHeight) - neighborHeight/2;
									if(!(cshift == 0 && rshift==0))
										break;
								}
								if(c + cshift < 0 || c + cshift >=width)
									cshift*=-1;
								if(r + rshift < 0 || r + rshift >= height)
									rshift*=-1;
								int updateIndex = rng.uniform(0,this->numberSamples);
								backgroundModel[updateIndex].at<Vec3b>(r+rshift,c+cshift) = pixel;
							}
							else
							{
								for(;;)
								{
									cshift = abs(rand()%neighborWidth) - neighborWidth/2;
									rshift = abs(rand()%neighborHeight) - neighborHeight/2;
									if(!(cshift == 0 && rshift==0))
										break;
								}
								int updateIndex = rng.uniform(0,this->numberSamples);
								backgroundModel[updateIndex].at<Vec3b>(r+rshift,c+cshift) = pixel;
							}
							/****************************************/
						}
						/*
						*********结束更新模型************
						*/
						//第二步：分类classify
						segmentation.at<uchar>(r,c) = this->BACK_GROUND;
						break;
					}
				}
			}//编号1-1-1-1
			if(count < this->minMatch)//classify
				segmentation.at<uchar>(r,c) = this->FORE_GROUND;
		}//编号1-1-1
	}//编号1-1

}//*编号1


