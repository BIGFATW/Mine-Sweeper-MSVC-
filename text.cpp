#pragma warning(disable:4996)
#include"resource.h"
#include"mine.h"
#define WID 30
#define TALL 16
#define MINE 99
/*定义全局变量：[mine]layout:				所有格子的信息布局，
				[define]WID,TALL,MINE:		分别代表长，宽，及雷数，自定义下改为自行输入
				[int]extra:					剩余雷数，显示在屏幕上，需要用到窗口过程函数，故作全局变量
				[bool]click:				在此作布尔类型，与整型写在一起方便编译，记录第一下点了没有
				[int]t:						记录时间，显示在屏幕上，需要用到窗口过程函数，故作全局变量
				[int]face:					游戏上面的那个😀，0表示游戏进行中，1胜利墨镜标志，2失败
				[bool]game					判断是否游戏中*/
mine layout[WID][TALL];
extern int extra, click, t, face, game;
using namespace std;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
	fstream file("data.txt", ios::in);
	if (!file)
	{
		file.close();
		{
			fstream create("data.txt", ios::out);
			create.close();
		}
		{
			fstream create("data.txt", ios::out);
			create << 3;
		}
	}
	// 1 refers to easy mode, 2 refers to medium mode, 3 refers to hard mode, 0 refers to custom mode;
	int mode;
	file >> mode;
	file.close();
	switch (mode)
	{
		HINSTANCE hs;
	case 0:
		hs = ShellExecute(NULL, L"open", L"Mine Sweeper Custom.exe", NULL, NULL, SW_SHOWNORMAL);
		assert(hs > (HINSTANCE)HINSTANCE_ERROR);
		exit(0);
	case 1:
		hs = ShellExecute(NULL, L"open", L"Mine Sweeper Easy.exe", NULL, NULL, SW_SHOWNORMAL);
		assert(hs > (HINSTANCE)HINSTANCE_ERROR);
		exit(0);
	case 2:
		hs = ShellExecute(NULL, L"open", L"Mine Sweeper Medium.exe", NULL, NULL, SW_SHOWNORMAL);
		assert(hs > (HINSTANCE)HINSTANCE_ERROR);
		exit(0);
	}
	TCHAR appClassName[] = TEXT("Mine Sweeper");//窗口在任务管理器中的名称
	WNDCLASS wc;								//窗口类WNDCLASS(EX)
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInstance;
	wc.lpszClassName = appClassName;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	RegisterClass(&wc);							//注册窗口类
	HWND window = CreateWindow(
		appClassName,
		TEXT("Mine Sweeper"),
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		100, 100,
		200 + WID * 30, 210 + TALL * 30,
		NULL,
		NULL,
		hInstance,
		NULL
	);											//创建一个叫window的窗口
	ShowWindow(window, SW_SHOW);				//显示窗口
	UpdateWindow(window);						//进入窗口进程函数，并发起绘图消息
	MSG msg;
	extra = MINE, click = 0, t = 0, face = 0, game = 1;
	bool end = 0, failed = 0;						//随时判断游戏是否胜利或失败，将结果保存在[bool]end,failed中
	int cnt = 0;								//计算帧数，睡太久程序卡成PPT
	InvalidateRect(window, 0, true);			//这里InvalidateRect让绘制窗口变为无效图形，说白了就是重画，没它窗口不会更新面貌
	while (1)
	{
		end = 1, failed = 0;
		GetMessage(&msg, NULL, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		//这三个数是让窗口进入过程函数的命令，不加程序会一直未响应
		for (int i = 0; i < WID; i++)
		{
			for (int j = 0; j < TALL; j++)
			{
				if (layout[i][j].cnt != -1 && layout[i][j].open == 0) end = 0;
				if (layout[i][j].cnt == -1 && layout[i][j].open == 1) failed = 1;
			}
		}
		//只要有一个不是雷的格子没打开胜利条件就未达成，只要有一个有雷的格子被打开就算失败，第59行将胜利期望值归为是
		if (end && game)
		{
			for (int i = 0; i < WID; i++)
			{
				for (int j = 0; j < TALL; j++)
				{
					layout[i][j].question = 0;
					if (layout[i][j].cnt == -1)layout[i][j].flag = 1;
					else layout[i][j].open = 1;
				}
			}
			face = 1, end = 0, game = 0;
			SendMessage(window, 0x2021, 0, 0);
			InvalidateRect(window, 0, true);
		}
		if (failed && game)
		{
			for (int i = 0; i < WID; i++)
			{
				for (int j = 0; j < TALL; j++)
				{
					layout[i][j].question = 0;
					layout[i][j].flag = 0;
					layout[i][j].open = 1;
				}
			}
			face = 2, failed = 0, game = 0;
			SendMessage(window, 0x2021, 0, 0);
			InvalidateRect(window, 0, true);
		}
	}
	//胜利就把所有雷的格子标上旗，失败就把所有格子打开
	return 0;
}
