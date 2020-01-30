#include "BasicOperation.h"

// ������<Windows.h>�����в��ܹ�������"BasicOperation.h"���棬����
#ifndef AFXWIN_H_
#define AFXWIN_H_
#include <afxwin.h>
#endif

using namespace std;

/*
Default constructor

References:
��C++ Primer Plus��6�����İ桷Page 356-359
΢��VS2019�ٷ��ĵ���https://visualstudio.microsoft.com/vs/getting-started/
https://docs.microsoft.com/en-us/visualstudio/get-started/visual-studio-ide?view=vs-2019
https://docs.microsoft.com/en-us/cpp/mfc/reference/cwnd-class?view=vs-2019
*/
BasicOperation::BasicOperation()
{
	rootPath = this->GetCwd();
	this->InitializeAttributes(rootPath);
}

/*
A second constructor
*/
BasicOperation::BasicOperation(const std::string& rootPathRef)
{

	this->InitializeAttributes(rootPathRef);
}

/*
Deconstructor
*/
BasicOperation::~BasicOperation()
{
	//do nothing here
}

/*
References:
C++��ȡ��Ļ�ֱ��ʣ���Ļ���ڴ�С������Ļ��ʾ������DPI�����ַ�����https://blog.csdn.net/yp18792574062/article/details/88351342
*/
void BasicOperation::InitializeAttributes(const std::string& rootPathRef)
{
	/* Initialize screen size */
	HWND hd = GetDesktopWindow();
	RECT rect;
	GetClientRect(hd, &rect);
	if (0 < (rect.right - rect.left) && 0 < (rect.bottom - rect.top))
	{
		clientWidth = (rect.right - rect.left);
		clientHeight = (rect.bottom - rect.top); // 1920,1080
	}

	GetWindowRect(hd, &rect);
	if (0 < (rect.right - rect.left) && 0 < (rect.bottom - rect.top))
	{
		windowWidth = (rect.right - rect.left);
		windowHeight = (rect.bottom - rect.top); // 1920,1080
	}

	/* Initialize rootPath */
	rootPath = rootPathRef;
}

/*
References:
CString��string�໥ת����https://blog.csdn.net/u014801811/article/details/90439888
mfc�У���ν�CString ת���� string��https://zhidao.baidu.com/question/79250977.html

#include <afxwin.h>
*/
std::string BasicOperation::GetCwd(void)
{
	CString path;
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	path.ReleaseBuffer();
	int pos = path.ReverseFind('\\');
	path = path.Left(pos);
	std::string pathString(CW2A(path.GetString()));
	return pathString;

	/*TCHAR szDrive[_MAX_DRIVE] = { 0 };
	TCHAR szDir[_MAX_DIR] = { 0 };
	TCHAR szFname[_MAX_FNAME] = { 0 };
	TCHAR szExt[_MAX_EXT] = { 0 };
	_wsplitpath_s(path.GetString(), szDrive, szDir, szFname, szExt);*/
}

/*
References:
C++ʵ����Ļ��ͼ��ȫ����ͼ����https://blog.csdn.net/sunflover454/article/details/48717731
��VC����fatal error C1189: #error : WINDOWS.H already included. MFC apps must not #include��https://blog.csdn.net/ypist/article/details/8505666
���Ľ��ۣ���#include "windows.h"����һ�з�������#includeͷ�ļ�֮��
CWND��HWND֮��Ĺ�ϵ��ת�� �ͻ�ȡ������https://blog.csdn.net/Alexander_Frank/article/details/52093955
C++string�ೣ�ú��� c++�е�string���ú����÷��ܽ᣺https://blog.csdn.net/pengnanzheng/article/details/80549445

#include <afxwin.h>
����������õ�OpenCV�������ˡ�
*/
/*
int BasicOperation::ScreenSnapshot(const std::string& folderNameRef, const std::string& filePrefix)
{
	CWnd* pDesktop;
	HWND Handle = GetDesktopWindow();
	pDesktop = CWnd::FromHandle(Handle);
	CDC* pdeskdc = pDesktop->GetDC();
	CRect re;
	////��ȡ���ڵĴ�С
	pDesktop->GetClientRect(&re);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pdeskdc, re.Width(), re.Height());
	//����һ�����ݵ��ڴ滭��
	CDC memorydc;
	memorydc.CreateCompatibleDC(pdeskdc);
	//ѡ�л���
	CBitmap* pold = memorydc.SelectObject(&bmp);
	//����ͼ��
	memorydc.BitBlt(0, 0, re.Width(), re.Height(), pdeskdc, 0, 0, SRCCOPY);

	//ѡ��ԭ���Ļ���
	memorydc.SelectObject(pold);
	BITMAP bit;
	bmp.GetBitmap(&bit);
	//���� ͼ���С����λ��byte��
	DWORD size = bit.bmWidthBytes * bit.bmHeight;
	LPSTR lpdata = (LPSTR)GlobalAlloc(GPTR, size);

	//�����Ǵ���һ��bmp�ļ��ı����ļ�ͷ
	BITMAPINFOHEADER pbitinfo;
	pbitinfo.biBitCount = 24;
	pbitinfo.biClrImportant = 0;
	pbitinfo.biCompression = BI_RGB;
	pbitinfo.biHeight = bit.bmHeight;
	pbitinfo.biPlanes = 1;
	pbitinfo.biSize = sizeof(BITMAPINFOHEADER);
	pbitinfo.biSizeImage = size;
	pbitinfo.biWidth = bit.bmWidth;
	pbitinfo.biXPelsPerMeter = 0;
	pbitinfo.biYPelsPerMeter = 0;
	GetDIBits(pdeskdc->m_hDC, bmp, 0, pbitinfo.biHeight, lpdata, (BITMAPINFO*)& pbitinfo, DIB_RGB_COLORS);
	BITMAPFILEHEADER bfh;
	bfh.bfReserved1 = bfh.bfReserved2 = 0;
	bfh.bfType = ((WORD)('M' << 8) | 'B');
	bfh.bfSize = size + 54;
	bfh.bfOffBits = 54;
	//д���ļ�
	CFile file;
	string folder_path(rootPath + "\\");
	if (0 < folderNameRef.length())
	{
		folder_path = rootPath + "\\" + folderNameRef + "\\";
	}
	CString strFileName(folder_path.c_str());
	CreateDirectory((LPCTSTR)strFileName, NULL);
	CTime t = CTime::GetCurrentTime();
	CString tt = t.Format("%Y%m%d_%H%M%S");
	if (0 < filePrefix.length())
	{
		CString prefix(filePrefix.c_str());
		tt = prefix + tt;
	}
	strFileName += tt;
	strFileName += _T(".bmp");
	if (file.Open((LPCTSTR)strFileName, CFile::modeCreate | CFile::modeWrite))
	{
		file.Write(&bfh, sizeof(BITMAPFILEHEADER));
		file.Write(&pbitinfo, sizeof(BITMAPINFOHEADER));
		file.Write(lpdata, size);
		file.Close();
	}
	GlobalFree(lpdata);
	return 0;
}
*/

/*
C / C++����Ŀ¼�µ������ļ���Windowsƪ������ϸ��https://www.cnblogs.com/collectionne/p/6792301.html
����FindFirstFile(),FindNextFile()��������ָ��Ŀ¼�������ļ���https://blog.csdn.net/u012005313/article/details/46490437

#include <Windows.h>

���÷�����
BasicOperation bo;
string folderPath = bo.rootPath + "\\screen_snapshot\\*.*";
int fileNumber = bo.GetFileNumberInDirectory(folderPath);
*/
int BasicOperation::GetFileNumberInDirectory(const std::string& folderPath)
{
	int counter = 0;
	int dirs = 0;
	using namespace std;
	HANDLE hFind;
	WIN32_FIND_DATA findData;
	//LARGE_INTEGER size;

	CString dirName(folderPath.c_str());
	hFind = FindFirstFile((LPCTSTR)dirName, &findData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	do
	{
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//cout << findData.cFileName << "\t<dir>\n";    // �Ƿ���Ŀ¼ 
			dirs++;  // ����"."��".."������� 
		}
		else
		{
			counter++;
			/*size.LowPart = findData.nFileSizeLow;
			size.HighPart = findData.nFileSizeHigh;
			cout << findData.cFileName << "\t" << size.QuadPart << " bytes\n";*/
		}
	} while (FindNextFile(hFind, &findData));
	return counter;
}

/*
���÷�����
BasicOperation bo;
int result = bo.GetRandomTime(5000, 3000);
*/
int BasicOperation::GetRandomTime(int midtime, int range)
{
	unsigned int timeSeed = time(0) % 100;
	srand(timeSeed);
	if (1 > midtime)
	{
		midtime = RANDOM_MIDTIME;
	}
	if (1 > range)
	{
		range = RANDOM_TIME_RANGE;
	}
	if (range > midtime)
	{
		midtime = range;
	}
	return midtime - range / 2 + rand() % range;
	// rand() returns an int from 0 to RAND_MAX(2147483647)
}

/*
��minimal_int��maximal_int֮�������ֵ
���÷�����
BasicOperation bo;
int result = bo.GetRandomInt(500, 800);
*/
int BasicOperation::GetRandomInt(int minimal_int, int maximal_int)
{
	unsigned int timeSeed = time(0) % 100;
	srand(timeSeed);
	if (maximal_int < minimal_int) {
		return maximal_int + rand() % (minimal_int - maximal_int);
	}
	else if (minimal_int == maximal_int) {
		return maximal_int;
	}
	return minimal_int + rand() % (maximal_int - minimal_int);
	// rand() returns an int from 0 to RAND_MAX(2147483647)
}

/*
#include <sys/timeb.h>
References:
c++ ʱ��������� time_t��https://www.runoob.com/w3cnote/cpp-time_t.html
VC/c++�汾��ȡ����ʱ�����ȷ�����룺https://www.cnblogs.com/guolongzheng/p/9399414.html
*/
long long BasicOperation::GetCurrentTs(void)
{
	timeb t;
	ftime(&t);
	return t.time * 1000 + t.millitm;
}

/*
References:
C++��ô��дwindows�����������?����˵�Զ���һ���ַ������ƣ�https://zhidao.baidu.com/question/647698977954822045.html
C++��strcpy()������strcpy_s()������ʹ�ü�ע�����https://blog.csdn.net/leowinbow/article/details/82380252
*/
bool BasicOperation::CopyToClipboard(std::string dataToClipboard)
{
	if (OpenClipboard(NULL))
	{
		EmptyClipboard();
		HGLOBAL clipbuffer;
		char* buffer;
		const char* pszData; // ���ܹ�ֱ�ӳ�ʼ����ֻ�ܹ�ʹ��������һ�д�������ֵ
		pszData = dataToClipboard.c_str();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, dataToClipboard.length() + 1);
		if (0 == clipbuffer) {
			return false;
		}
		buffer = (char*)GlobalLock(clipbuffer);
		if (0 == buffer) {
			return false;
		}
		strcpy_s(buffer, strlen(pszData) + 1, pszData); // strcpy(buffer, pszData);
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT, clipbuffer);
		CloseClipboard();
		return true;
	}
	return false;
}

/*
���÷�����
CtrlPaste(300, 500);
#include <Windows.h>�������ӵ�#include <Windows.h>�Ѿ���������#include <afxwin.h>�����ˡ�
*/
void BasicOperation::CtrlPaste(int x, int y)
{
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, x * 65535 / clientWidth, y * 65535 / clientHeight, 0, 0);
	Sleep(15);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(5);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(10);

	keybd_event(0x11, 0, 0, 0); // CTRL key
	keybd_event(0x41, 0, 0, 0); // A key
	keybd_event(0x41, 0, 2, 0);
	keybd_event(0x11, 0, 2, 0);
	Sleep(10);

	keybd_event(0x2E, 0, 0, 0); // DEL key
	keybd_event(0x2E, 0, 2, 0);
	Sleep(20);

	keybd_event(0x11, 0, 0, 0); // CTRL key
	keybd_event(0x56, 0, 0, 0); // V key
	keybd_event(0x56, 0, 2, 0);
	keybd_event(0x11, 0, 2, 0);
	Sleep(20);

	keybd_event(0x23, 0, 0, 0); // END key
	keybd_event(0x23, 0, 2, 0);
	Sleep(20);

	keybd_event(0x0D, 0, 0, 0); // Enter Key
	keybd_event(0x0D, 0, 2, 0);
}

/*
���÷�����
BasicOperation bo;
string filePath = bo.rootPath + "//..//inputs//test.txt";
bo.ReadFileToAttribute(filePath);
*/
void BasicOperation::ReadFileToAttribute(const std::string& filePath)
{
	char buffer[CHAR_ARRAY_SIZE];
	cout << sizeof(buffer);
	string temp;
	fstream out;

	out.open(filePath, ios::in | ios::binary);
	while (!out.eof())
	{
		out.getline(buffer, CHAR_ARRAY_SIZE, '\n');
		temp = buffer;
		if (5 < sizeof(buffer) && 5 < temp.length())
		{
			cout << "This line is: " << temp << endl;
			CopyToClipboard(temp);
			CtrlPaste(300, 500);
		}
	}
	out.close();
}

/*
References:
https://docs.microsoft.com/zh-cn/windows/win32/api/winuser/nf-winuser-getcursorpos
[C++] GetCursorPos������ʹ�÷�����Ӧ��ʵ����https://blog.csdn.net/baidu_38494049/article/details/82930099
GetCursorPos() ��GetMessagePos()�������ж��Ƿ���browser�ڣ���https://blog.csdn.net/jiangqin115/article/details/44916351
MFC������ת��GetClientRect/GetWindowRect/ClientToScreen/GetCursorPos/ScreenToClient��https://blog.csdn.net/chunyexiyu/article/details/9020151
C++ ������� vector����Ϊ�����Ĳ�����https://blog.csdn.net/hujunyin/article/details/79489522
c++ ��λ�ȡ����������ϵ����꣺https://jingyan.baidu.com/article/b0b63dbf254a3b4a483070b8.html
C++ vector ����ǳ����https://www.runoob.com/w3cnote/cpp-vector-container-analysis.html

#include <Windows.h>�������ӵ�#include <Windows.h>�Ѿ���������#include <afxwin.h>�����ˡ�
todo: ��ģ�嶨���Ϊvector<POINT> &pt_vector�����Ǳ���Windows.h���ظ�included������Ϊ�˼ӿ쿪���ٶȣ�ֱ��ʹ���Զ���ScreenPointģ�塣
*/
int BasicOperation::PrintXY(vector<ScreenPoint>& pt_vector, int sleepTimeBetweenPoints)
{
	ScreenPoint new_pt;
	POINT point;
	BOOL bReturn;
	int i = 0;

	Sleep(sleepTimeBetweenPoints);
	bReturn = GetCursorPos(&point);
	while (true) {
		if (0 == bReturn)
		{
			cout << "cannot get the cursor point. Please try again for Point # " << i << " : " << endl;
		}
		else {
			cout << "Point # " << i << " : " << point.x << "," << point.y << endl;
			new_pt = {
				point.x,point.y
			};
			pt_vector.push_back(new_pt);
			i++;
		}
		if (ListenEscKey(sleepTimeBetweenPoints)) {
			cout << "you pressed ESC key. Totally " << i << " points have been recorded" << endl;
			break;
		}
		bReturn = GetCursorPos(&point);
	}

	return i;
}

/*
#include <Windows.h>�������ӵ�#include <Windows.h>�Ѿ���������#include <afxwin.h>�����ˡ�
*/
bool BasicOperation::ListenEscKey(int sleepTime)
{
	int newTime = 0;
	long long endTs = 0LL;
	if (1 > sleepTime)
	{
		newTime = GetRandomTime(0, 0); // in case that 0 > sleepTime
	}
	else {
		newTime = GetRandomTime(sleepTime, 0);
	}
	endTs = GetCurrentTs() + newTime;

	while (1) {
		if (GetCurrentTs() > endTs) {
			break;
		}
		if (KEY_DOWN(ESC_KEY)) {
			return true;
		}
		Sleep(20);
	}
	return false;
}

/*
References:
����C��C++��int���ַ��������໥ת����https://www.cnblogs.com/qinsun/p/10091910.html
��C++ Primer Plus��6�����İ桷Page 272
*/
std::string BasicOperation::ConvertIntToString(vector<ScreenPoint>& pt_vector)
{
	string out_string = "";
	string temp = "";
	for (int i = 0; i < pt_vector.size(); i++)
	{
		temp = std::to_string(pt_vector[i].x) + "," + std::to_string(pt_vector[i].y) + "\n";
		out_string += temp;
		i++;
	}
	return out_string;
}

/*
��C++ Primer Plus��6�����İ桷Page 272
*/
bool BasicOperation::WriteFile(const std::string& content, const std::string& filePath)
{
	if (1 > content.length()) {
		cout << "Please pass content for writing " << filePath << endl;
		return false;
	}
	ofstream fileOut;
	fileOut.open(filePath);
	if (!fileOut.is_open())
	{
		cout << "Cannot open " << filePath << endl;
		return false;
	}
	fileOut << content;
	return true;
}

/*
https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-mouse_event
#include <Windows.h>�������ӵ�#include <Windows.h>�Ѿ���������#include <afxwin.h>�����ˡ�
*/
void BasicOperation::DragTheMap(const MouseTrace& mt)
{
	int x = GetRandomInt(mt.boxStartX, mt.boxEndX);
	int y = GetRandomInt(mt.boxStartY, mt.boxEndY);
	ScreenPoint pt = { x, y }, new_pt;
	new_pt = GetEndXY(mt, pt);
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, x * 65535 / clientWidth, y * 65535 / clientHeight, 0, 0);
	Sleep(20);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(20);
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, new_pt.x * 65535 / clientWidth, new_pt.y * 65535 / clientHeight, 0, 0);
	Sleep(20);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

/*
��ȡ���յ��x, yֵ�������Զ���ṹ��ScreenPoint��
*/
ScreenPoint BasicOperation::GetEndXY(const MouseTrace& mt, ScreenPoint pt)
{
	ScreenPoint return_pt = pt;
	if (toRight == mt.moveDirection) {
		return_pt.x = pt.x + mt.movePixel;
	}
	else if (toBottom == mt.moveDirection) {
		return_pt.y = pt.y + mt.movePixel;
	}
	else if (toLeft == mt.moveDirection) {
		return_pt.x = pt.x - mt.movePixel;
	}
	else if (toTop == mt.moveDirection) {
		return_pt.y = pt.y - mt.movePixel;
	}
	return return_pt;
}

/*
���mt���MouseTrace�ṹ��ʵ���Ƿ���ȷ
*/
std::string BasicOperation::CheckMouseTraceStructure(const MouseTrace& mt)
{
	string errorMsg = "";
	if (10 > mt.movePixel) {
		errorMsg = std::to_string(mt.movePixel);
		return "mt.movePixel is too small to move: " + errorMsg;
	}
	if (0 > mt.boxEndX || 0 > mt.boxEndY || 0 > mt.boxStartX || 0 > mt.boxStartY) {
		errorMsg = std::to_string(mt.boxEndX) + "," + std::to_string(mt.boxEndY) + "," + std::to_string(mt.boxStartX) + "," + std::to_string(mt.boxStartY);
		return "mt has negative attribute(s): boxEndX, boxEndY, boxStartX, boxStartY == " + errorMsg;
	}
	if (clientWidth < mt.boxEndX || clientHeight < mt.boxEndY || clientWidth < mt.boxStartX || clientHeight < mt.boxStartY) {
		errorMsg = std::to_string(clientWidth) + "," + std::to_string(clientHeight) + "," + std::to_string(mt.boxEndX) + "," + std::to_string(mt.boxEndY) + \
			"," + std::to_string(mt.boxStartX) + "," + std::to_string(mt.boxStartY);
		return "mt has over large attribute(s): boxEndX, boxEndY, boxStartX, boxStartY == " + errorMsg;
	}

	if (toRight == mt.moveDirection) {
		if (clientWidth < mt.movePixel + mt.boxEndX) {
			errorMsg = std::to_string(mt.movePixel) + " + " + std::to_string(mt.boxEndX);
			return "clientWidth is smaller than mt.movePixel + mt.boxEndX: " + errorMsg;
		}
	}
	else if (toBottom == mt.moveDirection) {
		if (clientHeight < mt.movePixel + mt.boxEndY) {
			errorMsg = std::to_string(mt.movePixel) + " + " + std::to_string(mt.boxEndY);
			return "clientHeight is smaller than mt.movePixel + mt.boxEndY: " + errorMsg;
		}
	}
	else if (toLeft == mt.moveDirection) {
		if (mt.boxStartX < mt.movePixel) {
			errorMsg = std::to_string(mt.movePixel) + " > " + std::to_string(mt.boxStartX);
			return "mt.boxStartX < mt.movePixel: " + errorMsg;
		}
	}
	else if (toTop == mt.moveDirection) {
		if (mt.boxStartY < mt.movePixel) {
			errorMsg = std::to_string(mt.movePixel) + " > " + std::to_string(mt.boxStartY);
			return "mt.boxStartY < mt.movePixel: " + errorMsg;
		}
	}
	return "";
}

