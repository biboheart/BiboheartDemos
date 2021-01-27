#pragma once

#include "BhLog.h"
class CServiceMain
{
public:
	CServiceMain(void);
	~CServiceMain(void);
public:
	void start();
	void stop();
	BhLog* getBhLog();
private:
	BhLog* bhLog;
};

