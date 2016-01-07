#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <iostream>
using namespace std;
using namespace cv;
int main()
{
	int startNum = 0;
	int endNum = 5;
	Mat image1 = imread("../d1.jpg");
	Mat image2 = imread("../d2.jpg");
	imshow("image1",image1);
	imshow("image2", image2);

	for (int i = 0; i < 14; i++)
	{
		
		// 检测surf特征点
		vector<KeyPoint> keypoints1, keypoints2;
		SurfFeatureDetector detector(400);
		detector.detect(image1, keypoints1);
		detector.detect(image2, keypoints2);
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
		matcher.match(descriptros1, descriptros2, matches);
		//cout << "matches.begin(): "<<&matches.begin() << " matches.end():" << (int)&matches.end() << endl;

		//先检查出最稳定的特征点，并删除之
		std::nth_element(matches.begin(), matches.begin() + startNum, matches.end());
		matches.erase(matches.begin(), matches.begin() + startNum);

		//检查出最不稳定特征点，并删除之
		std::nth_element(matches.begin(), matches.begin() + endNum, matches.end());
		matches.erase(matches.begin() + endNum + 1, matches.end());
		// 画出匹配图
		Mat imageMatches;
		drawMatches(image1, keypoints1, image2, keypoints2, matches,
			imageMatches, Scalar(255, 0, 0));

		namedWindow("image");
		imshow("image", imageMatches);
		waitKey();
		startNum+=5;
	}
	return 0;
}