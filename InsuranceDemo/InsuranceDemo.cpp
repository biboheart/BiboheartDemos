// InsuranceDemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include <windows.h>

//typedef void(__stdcall* lpInsertLogFileFun)(const char*, const char*);
//typedef wchar_t*(__stdcall* lpWebApplyFn)(const wchar_t* InstCode, const wchar_t* TransCode, const wchar_t* Data1, const wchar_t* Data2);
typedef char*(__stdcall* lpWebApplyFn)(const char* InstCode, const char* TransCode, const char* Data1, const char* Data2);
lpWebApplyFn fn = NULL;

int main()
{
    std::cout << "Hello World!\n";
    HINSTANCE hDll = LoadLibrary(L"IWebApply.dll");
    if (NULL == hDll)
    {
        std::cout << "获取IWebApply.dll失败\n";
    }
    if (NULL != hDll)
    {
        fn = (lpWebApplyFn)GetProcAddress(hDll, "WebApply");
    }
    if (NULL == fn)
    {
        std::cout << "从动态库中未取到WebApply函数\n";
    }
    else
    {
        char* result = fn("8331", "YB16", "{\"START\":\"10\"}", NULL);
        printf("%s\n", result);
        //std::cout << &result << "\n";
    }
    return getchar();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
