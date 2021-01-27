#pragma once

#include "stdafx.h"
typedef void(__stdcall* lpInsertLogFileFun)(const char*, const char*);

class BhLog
{
public:
	BhLog();
	~BhLog();
public:
	void insert(const char* text);
private:
	void getDate();
	void getPath();
private:
	char* pDate;
	char* pPath;
	HINSTANCE hLogDll;															//���
	lpInsertLogFileFun fnInsertLog;												//����ָ��
};

