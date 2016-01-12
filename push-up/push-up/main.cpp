#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <iostream>
using namespace std;
using namespace cv;
#define X1 keypoints1.at((matches.at(j).queryIdx)).pt.x
#define X2 keypoints2.at((matches.at(j).trainIdx)).pt.x
#define X3 keypoints3.at((matches2.at(j).queryIdx)).pt.x
#define X4 keypoints4.at((matches2.at(j).trainIdx)).pt.x

#define Y1 keypoints1.at((matches.at(j).queryIdx)).pt.y
#define Y2 keypoints2.at((matches.at(j).trainIdx)).pt.y
#define Y3 keypoints3.at((matches2.at(j).queryIdx)).pt.y
#define Y4 keypoints4.at((matches2.at(j).trainIdx)).pt.y
#define XTHRE 30
#define YTHRE 70
int main()
{
	int startNum = 0;
	int endNum = 5;
	Mat image1;//前一帧
	Mat image2 ;//当前帧
	Mat image0;//前两帧
	//获取摄像头  
	//CvCapture* capture = cvCreateCameraCapture(0);
	CvCapture *capture = cvCreateFileCapture("../video/5.mp4");
	IplImage* frame;
	int count = 0;
	int sig1 = -1;
	int sig0 = 0;
	int sig = 0;
	int push_up=0;
	int push_up_num=0;
	Mat image;
	Mat Dimage;
	Mat Uimage;
	int jump=3;
	while (1)
	{
		if (1 == sig || 3 == sig)
		{
			jump = 1;
		}
		for (int i = 0; i < jump; i++)
		{
			frame = cvQueryFrame(capture);
		}
		jump = 3;
		frame = cvQueryFrame(capture);
		Mat Mframe(frame, 0);
		if (0 == count)
		{
			resize(Mframe, image1, Size(320, 240));
			image2 = image1.clone();
			image = image1.clone();
			Dimage = image1.clone();
			Uimage = image1.clone();
			image0 = image1.clone();
			count++;
		}
		else
		{
			//image2.copyTo(image1);
			image0 = image1.clone();
			image1 = image2.clone();
			resize(Mframe, image2, Size(320, 240));
		}
		

		// 检测surf特征点
		vector<KeyPoint> keypoints1, keypoints2;
		SurfFeatureDetector detector(400);
		detector.detect(image1, keypoints1);
		detector.detect(image2, keypoints2);

		if (keypoints1.size() > 0 && keypoints2.size() > 0)
		{
			// 描述surf特征点
			SurfDescriptorExtractor surfDesc;
			Mat descriptros1, descriptros2;
			surfDesc.compute(image1, keypoints1, descriptros1);
			surfDesc.compute(image2, keypoints2, descriptros2);
			//imshow("descriptros1", descriptros1);
			//imshow("descriptros2", descriptros2);
			// 计算匹配点数
			BruteForceMatcher<L2<float>>matcher;
			vector<DMatch> matches;
			vector<DMatch> matche2;
			matcher.match(descriptros1, descriptros2, matches);
			//cout << "matches.begin(): "<<&matches.begin() << " matches.end():" << (int)&matches.end() << endl;
			/*
			//先检查出最稳定的特征点，并删除之
			std::nth_element(matches.begin(), matches.begin() + startNum, matches.end());
			matches.erase(matches.begin(), matches.begin() + startNum);

			//检查出最不稳定特征点，并删除之
			std::nth_element(matches.begin(), matches.begin() + endNum, matches.end());
			matches.erase(matches.begin() + endNum + 1, matches.end());
			*/
			// 画出匹配图
			/*Mat imageMatches;
			drawMatches(image1, keypoints1, image2, keypoints2, matche2,
			imageMatches, Scalar(255, 0, 0));
			*/
			//	namedWindow("image");
			//imshow("image1", image1);
			imshow("image", image0);


			//存放符合人体条件的点
			vector<DMatch> body;
			int sum = 0;
			for (int j = 0; j < matches.size(); j++)
			{
				if (((((X1 >= X2) && (X1 - X2)<XTHRE) || ((X2>X1) && (X2 - X1) < XTHRE))) &&
					((((Y1 >= Y2) && (Y1 - Y2)<YTHRE) || ((Y2>Y1) && (Y2 - Y1) < YTHRE))))
				{
					body.push_back(matches.at(j));
					sum += Y1 - Y2;
				}
				else
				{
					//cout << "X1:" << X1 << " X2:" << X2 << endl;
				}
			}
			cout << "特征总数：" << matches.size() << endl;
			cout << "有效特征：" << body.size() << endl;
			cout << "sum:" << sum << endl;

			if (0 < sum)
			{
				switch (sig)
				{
				case 0:
					sig1 = sig0;
					sig0 = sig;
					sig = 3;
					break;
				case 1:

					sig1 = sig0;
					sig0 = sig;
					sig = 3;
					break;
				case 2:
					sig1 = sig0;
					sig0 = sig;
					sig = 3;
					break;
				case 3:
					sig1 = sig0;
					sig0 = sig;
					sig = 4;
					break;
				case 4:
					sig1 = sig0;
					sig0 = sig;
					sig = 4;
					break;
				}
			}
			else if (0 > sum)
			{
				switch (sig)
				{
				case 0:
					sig1 = sig0;
					sig0 = sig;
					sig = 1;
					break;
				case 1:
					sig1 = sig0;
					sig0 = sig;
					sig = 2;
					break;
				case 2:
					sig1 = sig0;
					sig0 = sig;
					sig = 2;
					break;
				case 3:
					sig1 = sig0;
					sig0 = sig;
					sig = 1;
					break;
				case 4:
					sig1 = sig0;
					sig0 = sig;
					sig = 1;
				}
			}
			//最低点
			if (( ((4 == sig)&&((2==sig1)||0==sig1)) ))
			{


				vector<KeyPoint> keypoints3, keypoints4;
				SurfFeatureDetector detector(400);
				detector.detect(Uimage, keypoints3);
				detector.detect(image0, keypoints4);

				if (keypoints3.size() > 0 && keypoints4.size() > 0)
				{
					// 描述surf特征点
					SurfDescriptorExtractor surfDesc2;
					Mat descriptros3, descriptros4;
					surfDesc2.compute(Uimage, keypoints3, descriptros3);
					surfDesc2.compute(image0, keypoints4, descriptros4);

					// 计算匹配点数
					BruteForceMatcher<L2<float>>matcher2;
					vector<DMatch> matches2;

					matcher2.match(descriptros3, descriptros4, matches2);
					//存放符合人体条件的点
					vector<DMatch> body2;
					int sum2 = 0;
					for (int j = 0; j < matches2.size(); j++)
					{
						if (((((X3 >= X4) && (X4 - X3)<XTHRE) || ((X4>X3) && (X4 - X3) < XTHRE))) &&
							((((Y3 >= Y4) && (Y3 - Y4)<YTHRE) || ((Y4>Y3) && (Y4 - Y3) < YTHRE))))
						{
							body.push_back(matches2.at(j));
							sum2 += Y3 - Y4;
						}
					}
					if (sum2 <-100)
					{
						//cout << "sum3:" << sum3 << endl;
						push_up_num++;
						//push_up = sig;
						Dimage = image0.clone();
						if (0 == push_up_num % 2)
						{
							push_up_num++;
						}
						imshow("Dimage", Dimage);
					}
					cout << "sum2:" << sum2 << endl;
				}
			}

			//最高点
			else if (((2 == sig)) && (4 == sig1 || 0 == sig1))
			{
				vector<KeyPoint> keypoints3, keypoints4;
				SurfFeatureDetector detector(400);
				detector.detect(Dimage, keypoints3);
				detector.detect(image0, keypoints4);

				if (keypoints3.size() > 0 && keypoints4.size() > 0)
				{
					// 描述surf特征点
					SurfDescriptorExtractor surfDesc2;
					Mat descriptros3, descriptros4;
					surfDesc2.compute(Dimage, keypoints3, descriptros3);
					surfDesc2.compute(image0, keypoints4, descriptros4);

					// 计算匹配点数
					BruteForceMatcher<L2<float>>matcher2;
					vector<DMatch> matches2;

					matcher2.match(descriptros3, descriptros4, matches2);
					//存放符合人体条件的点
					vector<DMatch> body2;
					int sum2 = 0;
					for (int j = 0; j < matches2.size(); j++)
					{
						if (((((X3 >= X4) && (X4 - X3)<XTHRE) || ((X4>X3) && (X4 - X3) < XTHRE))) &&
							((((Y3 >= Y4) && (Y3 - Y4)<YTHRE) || ((Y4>Y3) && (Y4 - Y3) < YTHRE))))
						{
							body.push_back(matches2.at(j));
							sum2 += Y3 - Y4;
						}
					}
					if (sum2 >100)
					{
						//cout << "sum3:" << sum3 << endl;
						
						push_up_num++;
						//push_up = sig;
						Uimage = image0.clone();
						imshow("Uimage", Uimage);
						if (1 == push_up_num % 2)
						{
							push_up_num++;
						}
					}
					cout << "sum2:" << sum2 << endl;
				}
			}
			cout << "sig:" << sig << endl;
			cout << "sig0:" << sig0 << endl;
			cout << "sig1:" << sig1 << endl;
			
			cout << "俯卧撑个数：" << push_up_num << endl << endl;
			waitKey(0);
		}
	}
	
	return 0;
}