// WinserviceDemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "stdafx.h"

//#include <iostream>
//#include <windows.h>

#include "CServiceMain.h"

/**
 * 注释此宏，会以控制台程序运行，如果不注释就是windows服务程序
 */
 //#ifdef _DEBUG
 //#else
 //	#define  SERVICE
 //#endif
//#define  SERVICE

CServiceMain* service = NULL;
BhLog* pLog = NULL;

#ifdef SERVICE
/*********************************************************
 *Functiopn:
 *Description:服务程序
 *******************************************************/
#define SERVICE_NAME "testService"

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hServiceStatusHandle;
//HANDLE serviceThreadHandle;
bool runningService;

void WINAPI ServiceHandler(DWORD fdwControl)
{
    switch (fdwControl)
    {
    case SERVICE_CONTROL_STOP:			//处理停止服务事件
    case SERVICE_CONTROL_SHUTDOWN:
        if (NULL != pLog) pLog->insert("service stop");
        ServiceStatus.dwWin32ExitCode = 0;
        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        ServiceStatus.dwCheckPoint = 0;
        ServiceStatus.dwWaitHint = 0;
        SetServiceStatus(hServiceStatusHandle, &ServiceStatus);
        runningService = FALSE;
        if (NULL != service) {
            service->stop();
            delete service;
        }
        break;
    case SERVICE_CONTROL_PAUSE:			//处理暂停服务事件
        if (NULL != pLog) pLog->insert("service pause");
        ServiceStatus.dwCurrentState = SERVICE_PAUSED;
        SetServiceStatus(hServiceStatusHandle, &ServiceStatus);
        if (NULL != service) {
            service->stop();
        }
        break;
    case SERVICE_CONTROL_CONTINUE:      //处理服务暂停后继续
        if (NULL != pLog) pLog->insert("service continue");
        ServiceStatus.dwCurrentState = SERVICE_RUNNING;
        SetServiceStatus(hServiceStatusHandle, &ServiceStatus);
        if (NULL != service) {
            service->start();
        }
        break;
    case SERVICE_CONTROL_PRESHUTDOWN:
        if (NULL != pLog) pLog->insert("service preshutdown");
        break;
    case SERVICE_CONTROL_INTERROGATE:
        if (NULL != pLog) pLog->insert("service interrogate");
        break;
    default:
        if (fdwControl > 127 && fdwControl < 256)//User defined
        {
            break;
        }
        break;
    };
    if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus))
    {
        DWORD nError = GetLastError();
    }
}

void WINAPI serviceMain(int argc, char** argv)
{
    //提供单个服务还是多个服务
    ServiceStatus.dwServiceType = SERVICE_WIN32;
    //等用户程序完成后在当前运行状态设为SERVICE_RUNNING
    ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    //目前能接受的是停止命令
    ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_CONTROL_PAUSE | SERVICE_CONTROL_CONTINUE;
    //服务的出错代码
    ServiceStatus.dwWin32ExitCode = NO_ERROR;
    //当错误发生时，服务返回的服务相关的错误代码
    ServiceStatus.dwServiceSpecificExitCode = 0;
    //置服务在启动/关闭/运行操作中反映操作进度
    ServiceStatus.dwCheckPoint = 0;
    //置服务在启动/关闭/运行操作时将持续的时间
    ServiceStatus.dwWaitHint = 0;

    //调用RegisterServiceCtrlHandler()获得服务状态句柄并注册生成Handler
    hServiceStatusHandle = RegisterServiceCtrlHandler(TEXT(SERVICE_NAME), ServiceHandler);

    if (!hServiceStatusHandle)
    {
        DWORD nError = GetLastError();
        return;
    }

    SetServiceStatus(hServiceStatusHandle, &ServiceStatus);

    //add your init code here
    if (NULL == service) {
        service = new CServiceMain();
    }
    pLog = service->getBhLog();
    // Initialization complete - report running status 
    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    ServiceStatus.dwCheckPoint = 0;
    ServiceStatus.dwWaitHint = 9000;
    if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus))
    {
        DWORD nError = GetLastError();
    }

    runningService = TRUE;
    //add your service thread here
    if (NULL != service) service->start();
    //if (NULL != pLog) pLog->insert("service started");
}

void charToWchar(const char* lpChar, wchar_t* lpWchar)
{
    int len = MultiByteToWideChar(CP_ACP, 0, lpChar, strlen(lpChar), NULL, 0);
    MultiByteToWideChar(CP_ACP, 0, lpChar, strlen(lpChar), lpWchar, len);
    lpWchar[len] = '\0';
}

int main()
{
    //Sleep(10000);
    wchar_t* lpServiceName = new wchar_t[strlen(SERVICE_NAME) + 1];
    memset(lpServiceName, 0, sizeof(lpServiceName));
    charToWchar(SERVICE_NAME, lpServiceName);
    SERVICE_TABLE_ENTRY ServiceTable[] =
    {
        {lpServiceName,(LPSERVICE_MAIN_FUNCTION)serviceMain},
        {NULL,NULL}
    };
    // 启动服务的控制分派机线程
    StartServiceCtrlDispatcher(ServiceTable);
    delete[] lpServiceName;
    return 0;
}


#else //#ifdef SERVICE
/*********************************************************
 Functiopn:
 Description:控制台程序
 *******************************************************/
int main()
{
    std::cout << "Hello Console!\n";
    if (NULL == service) {
        service = new CServiceMain();
    }
    pLog = service->getBhLog();
    service->start();
    int gc = getchar();
    service->stop();
    delete service;
    return gc;
}

#endif

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
