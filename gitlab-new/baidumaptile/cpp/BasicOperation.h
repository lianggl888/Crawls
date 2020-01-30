//don't use #pragma once, it cannot prevent damages from dulplicate files with different file names.
#ifndef BASIC_OPERATION_H_
#define BASIC_OPERATION_H_

#include <fstream>
#include <iostream>
#include <string>
#include <sys/timeb.h>
#include <vector>

#define ESC_KEY 0x1B
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

const int CHAR_ARRAY_SIZE = 8192; // 16384�ᱨ��
const int RANDOM_MIDTIME = 1500;
const int RANDOM_TIME_RANGE = 1000;

/*
�����ö�ٺͽṹ���Ǹ�DragTheMapʹ�õ�
�����޷�����Windows.h��POINT��CPoint�ṹ�壬ֻ���Լ��������ScreenPoint
*/
struct ScreenPoint
{
	int x;
	int y;
};
/* ������Ҫ�ر�ע����ǣ�toRight��ָ��������ƶ����ڵ�ͼ�����⽫���µ�ͼ�����ƶ����������ȼ��٣��� */
enum FourDirections { toRight = 1, toBottom = 2, toLeft = 3, toTop = 4 };
struct MouseTrace
{
	int boxStartX;
	int boxStartY;
	int boxEndX;
	int boxEndY;
	int movePixel;
	FourDirections moveDirection;
};

class BasicOperation
{
private:
	std::string name;
public:
	std::string rootPath;
	int clientWidth = 1920;
	int clientHeight = 1080;
	int windowWidth = 1920;
	int windowHeight = 1080;

	BasicOperation();
	BasicOperation(const std::string& rootPathRef);
	~BasicOperation();
	void InitializeAttributes(const std::string& rootPathRef);
	std::string GetCwd(void);
	int GetFileNumberInDirectory(const std::string& folderPath);
	int GetRandomTime(int midtime = 0, int range = 0);
	int GetRandomInt(int minimal_int, int maximal_int);
	long long GetCurrentTs(void);
	bool CopyToClipboard(std::string dataToClipboard);
	void CtrlPaste(int x = 0, int y = 0);
	void ReadFileToAttribute(const std::string& filePath);
	int PrintXY(std::vector<ScreenPoint>& pt_vector, int sleepTimeBetweenPoints = 2000);
	bool ListenEscKey(int sleepTime = 5000);
	bool WriteFile(const std::string& content, const std::string& filePath);
	std::string ConvertIntToString(std::vector<ScreenPoint>& pt_vector);
	void DragTheMap(const MouseTrace& mt);
	ScreenPoint GetEndXY(const MouseTrace& mt, ScreenPoint pt);
	std::string CheckMouseTraceStructure(const MouseTrace& mt);

	// int ScreenSnapshot(const std::string& folderNameRef, const std::string& filePrefix);
	//����������������õ�OpenCV�������ˡ�
};

#endif
