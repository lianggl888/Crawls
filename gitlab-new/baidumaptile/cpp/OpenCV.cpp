#include "OpenCV.h"

using namespace cv;
using namespace std;

/*
Default constructor

References:
��C++ Primer Plus��6�����İ桷Page 356-359
΢��VS2019�ٷ��ĵ���https://visualstudio.microsoft.com/vs/getting-started/
https://docs.microsoft.com/en-us/visualstudio/get-started/visual-studio-ide?view=vs-2019
https://docs.microsoft.com/en-us/cpp/mfc/reference/cwnd-class?view=vs-2019
*/
OpenCV::OpenCV(const InitAttribute& initAttributes)
{
	this->InitializeAttributes(initAttributes);
}

/*
Deconstructor
*/
OpenCV::~OpenCV()
{
	//do nothing here
}

/*
pass all these auguments during class initialization
InitAttribute is defined in OpenCV.h
*/
void OpenCV::InitializeAttributes(const InitAttribute& initAttributes)
{
	clientWidth = initAttributes.clientWidth;
	clientHeight = initAttributes.clientHeight;
	windowWidth = initAttributes.windowWidth;
	windowHeight = initAttributes.windowHeight;
	rootPath = initAttributes.rootPath;
}

/*
��ʾһ��ͼƬ
*/
int OpenCV::ShowImage(std::string& imageFilePath)
{
	Mat image;
	image = imread(imageFilePath, IMREAD_COLOR); // Read the file
	if (image.empty()) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", image); // Show our image inside it.
	waitKey(0); // Wait for a keystroke in the window
	destroyAllWindows();
	return 0;
}

/*
windowsƽ̨����opencvѰ�������ϵ�ͼ��https://blog.csdn.net/qq_18984151/article/details/79689732
*/

/*
ץȡ��ǰ��Ļ����
References:
���ߣ�windows api+opencvʵ�ֶ�̬��������ʾ��https://blog.csdn.net/qq_18984151/article/details/79231953
cv::Mat��ʶ���������ִ���������https://blog.csdn.net/u012058778/article/details/90764430
ʹ��gdi��opencv�����������档https://blog.csdn.net/jia_zhengshen/article/details/9384245
�������һƪ�����Ѿ�������opencv4.1.1�汾�˶�����Ҫ��
#include <opencv2\imgproc\imgproc_c.h>
#include <opencv2\imgcodecs.hpp>

CreateDCA function��https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createdca

*/
void OpenCV::ScreenSnapshot(cv::Mat& src)
{
	HBITMAP	hBmp;
	HBITMAP	hOld;
	Mat v_mat;
	BITMAP bmp;
	int nWidth, nHeight, nChannels;

	//��������
	HDC hScreen = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	HDC	hCompDC = CreateCompatibleDC(hScreen);
	//ȡ��Ļ��Ⱥ͸߶�
	nWidth = GetSystemMetrics(SM_CXSCREEN);
	nHeight = GetSystemMetrics(SM_CYSCREEN);
	/*int nWidth = clientWidth;
	int nHeight = clientHeight;*/
	cout << nWidth << "; nHeight == " << nHeight << endl;
	//����Bitmap����
	hBmp = CreateCompatibleBitmap(hScreen, nWidth, nHeight);
	hOld = (HBITMAP)SelectObject(hCompDC, hBmp);
	BitBlt(hCompDC, 0, 0, nWidth, nHeight, hScreen, 0, 0, SRCCOPY);
	SelectObject(hCompDC, hOld);
	//�ͷŶ���
	DeleteDC(hScreen);
	DeleteDC(hCompDC);

	//BITMAP����
	GetObject(hBmp, sizeof(BITMAP), &bmp);
	nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	//int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U; // ��Ҫ#include <opencv2/core/core_c.h>

	//mat����
	v_mat.create(bmp.bmHeight, bmp.bmWidth, CV_MAKETYPE(CV_8U, nChannels));
	GetBitmapBits(hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, v_mat.data);
	
	src = v_mat;
	DeleteObject(hBmp);
}
