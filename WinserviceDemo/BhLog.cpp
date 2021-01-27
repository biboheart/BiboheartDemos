#include "BhLog.h"

BhLog::BhLog()
{
	this->getPath();
	this->getDate();
	this->hLogDll = LoadLibrary(L"bin\\BhClog.dll");								//动态加载DLL模块句柄
	if (NULL == this->hLogDll) {
		std::cout << "获取hLogDll失败\n";
	}
	if (NULL != this->hLogDll) {
		this->fnInsertLog = (lpInsertLogFileFun)GetProcAddress(hLogDll, "insertLogFile");	//得到dll模块的函数地址
	}
	if (NULL == this->fnInsertLog) {
		std::cout << "从动态库中未取到insertLogFile函数\n";
		if (NULL != this->hLogDll) {
			FreeLibrary(this->hLogDll);
			this->hLogDll = NULL;
		}
	}
}

BhLog::~BhLog()
{
	if (NULL != this->pDate) {
		delete[] this->pDate;
	}
	if (NULL != this->pPath) {
		delete[] this->pPath;
	}
	if (NULL != this->hLogDll) {
		FreeLibrary(this->hLogDll);
		this->hLogDll = NULL;
	}
}

void BhLog::insert(const char* text)
{
	this->getDate();
	char* pLogFileName = new char[1024];
	memset(pLogFileName, 0, sizeof(pLogFileName));
	const char* pFileName = "service-";
	if (NULL != this->pPath)
	{
		strcat_s(pLogFileName, strlen(pLogFileName) + strlen(this->pPath) + 1, this->pPath);
	}
	strcat_s(pLogFileName, strlen(pLogFileName) + strlen(pFileName) + 1, pFileName);
	strcat_s(pLogFileName, strlen(pLogFileName) + strlen(this->pDate) + 1, this->pDate);
	strcat_s(pLogFileName, strlen(pLogFileName) + 5, ".log");
	this->fnInsertLog(pLogFileName, text);
	delete[] pLogFileName;
}

void BhLog::getPath()
{
	if (NULL == this->pPath) {
		this->pPath = new char[1024];
		memset(this->pPath, 0, sizeof(this->pPath));
	}
	wchar_t _lpPath[1024] = { 0 }, _lpFullPath[1024] = { 0 }, _lpDrive[8] = { 0 }, _lpDir[512] = { 0 }, _lpFileName[256] = { 0 }, _lpExt[128] = { 0 };
	HMODULE hMobaile = GetModuleHandle(NULL);
	GetModuleFileNameW(NULL, _lpFullPath, 1024);
	_wsplitpath_s(_lpFullPath, _lpDrive, 8, _lpDir, 512, _lpFileName, 256, _lpExt, 128);
	wcscat_s(_lpPath, wcslen(_lpDrive) + 1, _lpDrive);
	wcscat_s(_lpPath, wcslen(_lpPath) + wcslen(_lpDir) + 1, _lpDir);
	int len = WideCharToMultiByte(CP_ACP, 0, _lpPath, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, _lpPath, -1, this->pPath, len, NULL, NULL);
}

void BhLog::getDate()
{
	if (NULL == this->pDate) {
		this->pDate = new char[20];
		memset(this->pDate, 0, sizeof(this->pDate));
	}
	time_t now;
	struct tm newtime;
	time(&now);
	localtime_s(&newtime, &now);
	strftime(this->pDate, 20, "%Y-%m-%d", &newtime);
}
