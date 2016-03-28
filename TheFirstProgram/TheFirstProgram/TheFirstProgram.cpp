// TheFirstProgram.cpp : ����Ӧ�ó������ڵ㡣
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

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
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

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_THEFIRSTPROGRAM, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_THEFIRSTPROGRAM));

    MSG msg;

    // ����Ϣѭ��: 
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �����˵�ѡ��: 
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
            // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
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

// �����ڡ������Ϣ�������
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
	//���������������
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
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

	//�����׽���
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//���׽���
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));                  //ÿ���ֽڶ���0���
	sockAddr.sin_family = PF_INET;                           //ʹ��IPv4��ַ
															 //sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);            //�Զ���ȡIP��ַ
	inet_pton(AF_INET,"192.168.109.154",&sockAddr.sin_addr.s_addr);
	sockAddr.sin_port = htons(4321);                         //�˿�
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

	//�������״̬
	listen(servSock, 20);

	//���տͻ�������
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

	//�ر��׽���
	closesocket(clntSock);
	closesocket(servSock);

	//��ֹ DLL ��ʹ��
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