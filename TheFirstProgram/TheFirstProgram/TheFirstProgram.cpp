// TheFirstProgram.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TheFirstProgram.h"

int g_postion = 1;
CRITICAL_SECTION gcc;

void gotoxy(int x, int y);
COORD get_xy();
void ClearScreen(void);
void ClearScreen1(void);
DWORD WINAPI  sendCustmer(LPVOID);
DWORD WINAPI  displayServer(LPVOID);
#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_THEFIRSTPROGRAM, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_THEFIRSTPROGRAM));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_THEFIRSTPROGRAM));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_THEFIRSTPROGRAM);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
			system("cls");
			HANDLE    hThread1 = NULL;
			HANDLE    hThread2 = NULL;
			HANDLE hOut;
			hOut = GetStdHandle(STD_OUTPUT_HANDLE);
			WORD att = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE;
			SetConsoleTextAttribute(hOut, att);
			SMALL_RECT rc = { 0, 0, 80, 25 };
			SetConsoleWindowInfo(hOut, true, &rc);
			SetConsoleTitle(L"The Chat Tools ");
			ClearScreen();
			InitializeCriticalSection(&gcc);
			hThread2 = CreateThread(NULL, NULL, sendCustmer, NULL, NULL, NULL);
			hThread1 = CreateThread(NULL, NULL, displayServer, NULL, NULL, NULL);
			HANDLE    tHandle[] = { hThread1, hThread2 };
			WaitForMultipleObjects(2, tHandle, true, INFINITE);
			DeleteCriticalSection(&gcc);

			WSACleanup();
			CloseHandle(hThread1);
			CloseHandle(hThread2);
            /*PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);*/
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

DWORD WINAPI sendCustmer(LPVOID lpParameter)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//向服务器发起请求
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	sockAddr.sin_family = PF_INET;
	//sockAddr.sin_addr.s_addr = inet_pton("192.168.109.23");
	inet_pton(AF_INET, "192.168.109.23", &sockAddr.sin_addr.s_addr);
	//sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockAddr.sin_port = htons(1234);
	char SendBuffer[MAXBYTE];

	while (1)
	{
		SOCKET sock;
		do
		{
			sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (0 != connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)))
			{
				Sleep(1);
			}
			else
			{
				break;
			}
		} while (true);
		memset(SendBuffer, 0, MAXBYTE);

		gotoxy(0, 21);
		ClearScreen1();
#if 0
		for (int j = 0; j < 100; ++j)
		{
			printf_s(" ");
		}
#endif
		do
		{
			gotoxy(0, 21);
			gets_s(SendBuffer);
		} while (0 == strlen(SendBuffer));
		gotoxy(0, g_postion);
		if (g_postion < 20)
		{
			++g_postion;
		}
		else
		{
			g_postion = 1;
			gotoxy(0, g_postion);
			++g_postion;
			ClearScreen();
			//system("cls");
		}
		printf_s("I'm:  %s\n",SendBuffer);
		send(sock, SendBuffer, strlen(SendBuffer) + sizeof(char), NULL);
		shutdown(sock, SD_SEND);
		closesocket(sock);
	}
}


DWORD WINAPI displayServer(LPVOID lpParameter)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建套接字
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//绑定套接字
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));                  //每个字节都用0填充
	sockAddr.sin_family = PF_INET;                           //使用IPv4地址
															 //sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);            //自动获取IP地址
	inet_pton(AF_INET,"192.168.109.154",&sockAddr.sin_addr.s_addr);
	sockAddr.sin_port = htons(4321);                         //端口
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

	//进入监听状态
	listen(servSock, 20);

	//接收客户端请求
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);
	SOCKET clntSock;
	char RecvBuffer[MAXBYTE];
	COORD old_postion = { 0, 0 };
	while (1)
	{
		clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
		memset(RecvBuffer, 0, MAXBYTE);
		do
		{
			recv(clntSock, RecvBuffer, MAXBYTE, NULL);
		} while (0 == strlen(RecvBuffer));
		old_postion = get_xy();

		if (g_postion < 20)
		{
			gotoxy(0, g_postion++);
		}
		else
		{
			g_postion = 1;
			gotoxy(0, g_postion);
			++g_postion;
			ClearScreen();
			//system("cls");
		}
		printf_s("You: %s\n", RecvBuffer);
		gotoxy(old_postion.X, old_postion.Y);
	}

	//关闭套接字
	closesocket(clntSock);
	closesocket(servSock);

	//终止 DLL 的使用
	WSACleanup();
	system("pause");
	return 0;
}

void gotoxy(int x, int y)
{
	HANDLE hOutput;
	COORD loc;
	loc.X = x;
	loc.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, loc);
}

COORD get_xy()
{
	COORD postion = { 0, 0 };
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdOut, &csbi);
	postion = csbi.dwCursorPosition;
	return postion;
}

void ClearScreen(void)
{
	HANDLE hOut;
	DWORD written;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(hOut, &bInfo);
	COORD home = { 0, 0 };
	WORD att = bInfo.wAttributes;
	unsigned long size = bInfo.dwSize.X * 21;
	FillConsoleOutputAttribute(hOut, att, size, home, &written);
	FillConsoleOutputCharacter(hOut, ' ', size, home, &written);
}
void ClearScreen1(void)
{
	HANDLE hOut;
	DWORD written;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(hOut, &bInfo);
	COORD home = { 0, 21 };
	WORD att = bInfo.wAttributes;
	unsigned long size = bInfo.dwSize.X  * (bInfo.dwSize.Y - 21);
	FillConsoleOutputAttribute(hOut, att, size, home, &written);
	FillConsoleOutputCharacter(hOut, ' ', size, home, &written);
}