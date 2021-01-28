// RestfulDemo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include "WinSock2.h"

#pragma comment(lib, "WS2_32")

#include <event2/http.h>
#include <event2/event.h>
#include <event2/buffer.h>
// ?
#include <event2/http_struct.h>
#include <event2/http_compat.h>
#include <event2/util.h>

#define MYHTTPD_SIGNATURE   "myhttpd v 0.0.1"

struct event_base* base = NULL;
struct evhttp* httpd = NULL;

//处理模块
void httpd_handler(struct evhttp_request* req, void* arg)
{
    char output[2048] = "\0";
    char tmp[1024];
    //获取客户端请求的URI
    const char* uri = req->uri;
    sprintf_s(tmp, "uri=%s\n", uri);
    strcat_s(output, tmp);
    char* decoded_uri = evhttp_decode_uri(uri);
    sprintf_s(tmp, "decoded_uri=%s\n", decoded_uri);
    strcat_s(output, tmp);
    /* 输出到客户端 */

    //HTTP header
    evhttp_add_header(req->output_headers, "Server", MYHTTPD_SIGNATURE);
    evhttp_add_header(req->output_headers, "Content-Type", "text/plain; charset=UTF-8");
    evhttp_add_header(req->output_headers, "Connection", "close");
    //输出的内容
    struct evbuffer* buf;
    buf = evbuffer_new();
    evbuffer_add_printf(buf, "It works!\n%s\n", output);
    evhttp_send_reply(req, 200, "OK", buf);
    evbuffer_free(buf);
}

static BOOL WINAPI console_ctrl_handler(DWORD dwCtrlType)
{
    switch (dwCtrlType)
    {
        // handle the ctrl-c signal.
    case CTRL_C_EVENT:
        std::cout << "ctrl-c event\n\n";
        event_base_loopbreak(base);
        evhttp_free(httpd);
        WSACleanup();
        break;
        // ctrl-close: confirm that the user wants to exit.
    case CTRL_CLOSE_EVENT:
        std::cout << "ctrl-close event\n\n";
        event_base_loopbreak(base);
        evhttp_free(httpd);
        //WSACleanup();
        break;
        // pass other signals to the next handler.
    case CTRL_BREAK_EVENT:
        std::cout << "ctrl-break event\n\n";
        break;
    case CTRL_LOGOFF_EVENT:
        std::cout << "ctrl-logoff event\n\n";
        break;
    case CTRL_SHUTDOWN_EVENT:
        std::cout << "ctrl-shutdown event\n\n";
        break;
    default:
        break;
    }
    return false;
}

int main()
{
    std::cout << "Hello World!\n";

    WSADATA wsa_data;
    int res = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    std::cout << "WSAStartup result:" << res << "\n";
    //自定义信号处理函数
    SetConsoleCtrlHandler(console_ctrl_handler, true);

    //默认参数
    const char* httpd_option_listen = "0.0.0.0";
    int httpd_option_port = 8081;
    int httpd_option_timeout = 120;

    /* 使用libevent创建HTTP Server */
    //初始化event API
    base = event_base_new();
    if (!base)
    {
        printf("create event_base failed!\n");
        return getchar();
    }

    //创建一个http server
    httpd = evhttp_new(base);
    if (!httpd)
    {
        printf("create evhttp failed!\n");
        return getchar();
    }
    if (evhttp_bind_socket(httpd, httpd_option_listen, httpd_option_port) != 0)
    {
        printf("bind socket failed! port:%d\n", httpd_option_port);
        return getchar();
    }
    evhttp_set_timeout(httpd, httpd_option_timeout);

    //指定generic callback
    evhttp_set_gencb(httpd, httpd_handler, NULL);
    //也可以为特定的URI指定callback
    //evhttp_set_cb(httpd, "/", specific_handler, NULL);

    //循环处理events
    event_base_dispatch(base);

    //evhttp_free(httpd);
    WSACleanup();
    return 0;
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
