#include "stdafx.h"
#include "CServiceMain.h"

DWORD WINAPI _fnThreadRecv(LPVOID para);
volatile bool _bThreadEnd = true;
volatile bool _running = false;
HANDLE _hThreadRecv;

CServiceMain::CServiceMain(void)
{
	this->bhLog = NULL;
	if (NULL == this->bhLog) {
		this->bhLog = new BhLog();
	}
}

CServiceMain::~CServiceMain(void)
{
	if (_running)
	{
		this->stop();
	}
	if (NULL != this->bhLog)
	{
		delete this->bhLog;
	}
}

void CServiceMain::start()
{
	std::cout << "start service\n";
	if (NULL != this->bhLog)
	{
		this->bhLog->insert("service start");
	}
	_running = true;
	_hThreadRecv = CreateThread(
		NULL,	// Ĭ�ϰ�ȫ����
		NULL,	// Ĭ�϶�ջ��С
		_fnThreadRecv, // �߳���ڵ�ַ
		this,	//���ݸ��̺߳����Ĳ���
		0,	// ָ���߳���������
		NULL	//�߳�ID��
	);
	if (NULL == _hThreadRecv)
	{
		std::cout << "create thread error\n";
	}
	else
	{
		std::cout << "thread running ..." << "\n";
		CloseHandle(_hThreadRecv);
	}
}

void CServiceMain::stop()
{
	_running = false;
	while (!_bThreadEnd)
	{
		std::cout << "wait thread stop\n";
		if (NULL != this->bhLog)
		{
			this->bhLog->insert("wait thread stop");
		}
		Sleep(200);
	}
}

BhLog* CServiceMain::getBhLog()
{
	return this->bhLog;
}

DWORD WINAPI _fnThreadRecv(LPVOID para)
{
	CServiceMain* service = (CServiceMain*)para;
	if (NULL == service)
	{
		return NULL;
	}
	int i = 0;
	BhLog* log = service->getBhLog();
	_bThreadEnd = false;
	while (_running)
	{
		if (NULL == log)
		{
			log = service->getBhLog();
		}
		if (NULL != log)
		{
			log->insert("service running");
		}
		Sleep(1000);
		std::cout << "run in thred" << ++i << "\n";
	}
	_bThreadEnd = true;
	return NULL;
}

