#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <iostream>
using namespace std;
using namespace cv;
#define X1 keypoints1.at((matches.at(j).queryIdx)).pt.x
#define X2 keypoints2.at((matches.at(j).trainIdx)).pt.x
#define X3 keypoints3.at((matches3.at(j).queryIdx)).pt.x
#define X4 keypoints2.at((matches3.at(j).trainIdx)).pt.x

#define Y1 keypoints1.at((matches.at(j).queryIdx)).pt.y
#define Y2 keypoints2.at((matches.at(j).trainIdx)).pt.y
#define Y3 keypoints3.at((matches3.at(j).queryIdx)).pt.y
#define Y4 keypoints2.at((matches3.at(j).trainIdx)).pt.y
#define XTHRE 30
#define YTHRE 70
int main()
{
	int startNum = 0;
	int endNum = 5;
	Mat image1;
	Mat image2 ;
	//��ȡ����ͷ  
	//CvCapture* capture = cvCreateCameraCapture(0);
	CvCapture *capture = cvCreateFileCapture("../video/7.mp4");
	IplImage* frame;
	int count = 0;
	int sig1 = -1;
	int sig0 = 0;
	int sig = 0;
	int push_up=0;
	int push_up_num=0;
	Mat image;
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
			count++;
		}
		else
		{
			//image2.copyTo(image1);
			image1 = image2.clone();
			resize(Mframe, image2, Size(320, 240));
		}
		

		// ���surf������
		vector<KeyPoint> keypoints1, keypoints2;
		SurfFeatureDetector detector(400);
		detector.detect(image1, keypoints1);
		detector.detect(image2, keypoints2);

		if (keypoints1.size() > 0 && keypoints2.size() > 0)
		{
			// ����surf������
			SurfDescriptorExtractor surfDesc;
			Mat descriptros1, descriptros2;
			surfDesc.compute(image1, keypoints1, descriptros1);
			surfDesc.compute(image2, keypoints2, descriptros2);
			//imshow("descriptros1", descriptros1);
			//imshow("descriptros2", descriptros2);
			// ����ƥ�����
			BruteForceMatcher<L2<float>>matcher;
			vector<DMatch> matches;
			vector<DMatch> matche2;
			matcher.match(descriptros1, descriptros2, matches);
			//cout << "matches.begin(): "<<&matches.begin() << " matches.end():" << (int)&matches.end() << endl;
			/*
			//�ȼ������ȶ��������㣬��ɾ��֮
			std::nth_element(matches.begin(), matches.begin() + startNum, matches.end());
			matches.erase(matches.begin(), matches.begin() + startNum);

			//������ȶ������㣬��ɾ��֮
			std::nth_element(matches.begin(), matches.begin() + endNum, matches.end());
			matches.erase(matches.begin() + endNum + 1, matches.end());
			*/
			// ����ƥ��ͼ
			/*Mat imageMatches;
			drawMatches(image1, keypoints1, image2, keypoints2, matche2,
			imageMatches, Scalar(255, 0, 0));
			*/
			//	namedWindow("image");
			//imshow("image1", image1);
			imshow("image", image1);


			//��ŷ������������ĵ�
			vector<DMatch> body;
			double sum = 0;
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
			cout << "����������" << matches.size() << endl;
			cout << "��Ч������" << body.size() << endl;
			cout << "sum:" << sum << endl;

			if (50 < sum)
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
			else if (-50 > sum)
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
			if (( ((4 == sig)&&((2==sig1)||0==sig1)) || ((2 == sig))&&(4==sig1||0==sig1)) /*&& (sig != push_up)*/)
			{
				// ���surf������
				vector<KeyPoint> keypoints3;
				SurfFeatureDetector detector3(400);
				detector3.detect(image, keypoints3);
				

				if (keypoints3.size() > 0)
				{
					// ����surf������
					SurfDescriptorExtractor surfDesc3;
					Mat descriptros3;
					surfDesc3.compute(image, keypoints3, descriptros3);
					
					//imshow("descriptros1", descriptros1);
					//imshow("descriptros2", descriptros2);
					// ����ƥ�����
					BruteForceMatcher<L2<float>>matcher3;
					vector<DMatch> matches3;
					
					matcher.match(descriptros3, descriptros2, matches3);
					Mat imageMatches;
					drawMatches(image1, keypoints3, image2, keypoints2, matches3,
						imageMatches, Scalar(255, 0, 0));
					//imshow("match", imageMatches);
					vector<DMatch> body3;
					double sum3 = 0;

					for (int j = 0; j < matches3.size(); j++)
					{
						//X3;
						

						if (((((X3 >= X4) && (X3 - X4)<XTHRE) || ((X4>X3) && (X4 - X3) < XTHRE))) &&
							((((Y3 >= Y4) && (Y3 - Y4)<YTHRE) || ((Y4>Y3) && (Y4 - Y3) < YTHRE))))
						{
							
							
							body3.push_back(matches3.at(j));
							sum3 += Y3 - Y4;
							//cout << "Y3:" << Y3 << " Y4:" << Y4 << " sum3:" << sum3 << endl;

						}
					}
					if (sum3>200 || sum3 < -200)
					{
						//cout << "sum3:" << sum3 << endl;
						push_up_num++;
						//push_up = sig;
						image = image1.clone();
						//imshow("mat", image);
					}
					cout << "sum3:" << sum3 << endl;
				}	
				
			}
			cout << "sig:" << sig << endl;
			cout << "sig0:" << sig0 << endl;
			cout << "sig1:" << sig1 << endl;
			
			cout << "���ԳŸ�����" << push_up_num << endl << endl;
			waitKey(30);
		}
	}
	
	return 0;
}