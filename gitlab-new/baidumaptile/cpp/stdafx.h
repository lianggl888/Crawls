/*
_WIN32_WINNT�汾�б�https://blog.csdn.net/cai6811376/article/details/52637728
�궨�� _WIN32_WINNT=0x0400�������ļӣ�https://bbs.csdn.net/topics/360159997
*/
//#ifndef _WIN32_WINNT
//#define _WIN32_WINNT _WIN32_WINNT_MAXVER // 0x0600 means Windows Server 2008
//#endif
//�������һֱ��û�е�ͨ�������б������ǲ�Ӱ��exe�ļ�ִ�С�

/*
#define _AFXDLL
Managed C++ dll: #define _AFXDLL or do not use /MD[ d]?https://www.cnblogs.com/time-is-life/p/8543585.html
https://social.msdn.microsoft.com/Forums/vstudio/en-US/553fa850-5e6e-4ecb-a4eb-bbd81f8ff6b6/managed-c-dll-define-afxdll-or-do-not-use-md-d?forum=vcgeneral
���İ�Visual Studio2019����Ŀ => ����...���� => �������� => �߼� => MFC��ʹ�ã������Ŀ�������ó�Ϊ���ڹ���DLL��ʹ��MFC
*/

#ifndef STDAFX_H_
#define STDAFX_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include "BasicOperation.h"
#include "OpenCV.h"

#endif
