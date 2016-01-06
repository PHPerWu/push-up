#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

using namespace cv;
int main()
{
	int PointNum = 100;
	Mat image1 = imread("../c2.jpg");
	Mat image2 = imread("../c3.jpg");
	imshow("image1",image1);
	imshow("image2", image2);
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
	std::nth_element(matches.begin()+100, matches.begin() + PointNum, matches.end());
	matches.erase(matches.begin() + PointNum+1, matches.end());
	// 画出匹配图
	Mat imageMatches;
	drawMatches(image1, keypoints1, image2, keypoints2, matches,
		imageMatches, Scalar(255, 0, 0));

	namedWindow("image");
	imshow("image", imageMatches);
	waitKey();

	return 0;
}