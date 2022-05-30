#include"mine.h"
#include"text.cpp"
/*定义本局变量：[int]buttx,butty:	鼠标点在那个格子上
				[int]wei:			用于显示时间，后面的pow(计算乘方），get(获取x的w位数)都是在显示数字
				[int]pressed,pres:	pressed😀，pres重开	  是否被点击，鼠标摁下去到松开的过程显示被摁的图片
				[int]fail:			这个表示是否失败，当双击失败后，值为双击是的秒数[int]t，三秒后停止显示
				[int]px,py:			记录鼠标坐标
				[int]Connect:		彩蛋作弊码，距离生效差几个，每点对一个字母加一，点错归零
				[bool]cheat:		作弊模式是否开启，点进来非常好玩~
				[HINSTANCE]hRslt    打开应用程序的句柄*/
int buttx, butty, wei, pressed = 0, px, py, fail = 0, pres = 0, Connect = 0; bool cheat = 0; HINSTANCE hRslt;
int pow(int x, int y)
{
	int sum = 1;
	for (int i = 0; i < y; i++)
	{
		sum *= x;
	}
	return sum;
}
int get(int x, int w)
{
	x /= pow(10, w - 1);
	return x % 10;
}
void mine::left_button()
{
	if (open) return;	//被打开就直接返回
	if (flag)
	{
		flag = 0;
		extra++;
		return;
	}					//插了旗直接把旗砍了，避免右键削旗得削两次
	if (question)
	{
		question = 0;
		return;
	}					//打了问号就把问号削了
	open = 1;			//剩下没有任何记号就把它打开
}

void mine::right_button()
{
	if (open) return;	//被打下就直接返回
	if (flag)
	{
		extra++;
		flag = 0;
		question = 1;
		return;
	}					//插了旗把它削成问号，注意将剩余数量补上
	if (question)
	{
		question = 0;
		return;
	}					//打了问号就把问号削了
	extra--;
	flag = 1;			//剩下的就插上旗，把剩余数量减少
}

void init(int x, int y)
{
	int surround = 1;
	if (x > 0) surround++;
	if (x < WID - 1) surround++;
	if (y > 0) surround++;
	if (y < TALL - 1) surround++;
	if (x > 0 && y > 0) surround++;
	if (x < WID - 1 && y>0) surround++;
	if (x > 0 && y < TALL - 1) surround++;
	if (x < WID - 1 && y < TALL - 1) surround++;
	//这里两个参数x,y表示第一次点的时候的格子坐标，[int]surround计算它旁边的格子数包括自己，他们不能是雷
	vector<int> ini;
	for (int i = 0; i < MINE; i++) ini.push_back(1);
	for (int i = 0; i < WID * TALL - MINE - surround; i++) ini.push_back(0);
	srand((unsigned int)time(NULL));
	random_shuffle(ini.begin(), ini.end());
	//将雷作为1，非雷作为0放入[vector]ini容器里，然后用shuffle函数打乱分到各格子，注意时间种子不能缺
	vector<int>::iterator iter = ini.begin();
	for (int i = 0; i < WID; i++)
	{
		for (int j = 0; j < TALL; j++)
		{
			if (abs(i - x) <= 1 && abs(j - y) <= 1)
			{
				layout[i][j].cnt = 0;
				layout[i][j].flag = 0;
				layout[i][j].open = 0;
				layout[i][j].question = 0;
				continue;
			}
			if (*iter == 1) layout[i][j].cnt = -1;
			else layout[i][j].cnt = 0;
			layout[i][j].flag = 0;
			layout[i][j].open = 0;
			layout[i][j].question = 0;
			iter++;
		}
	}
	//将雷分到各格子，将打开，插旗，问号初始为不是
	for (int i = 0; i < WID; i++)
	{
		for (int j = 0; j < TALL; j++)
		{
			if (layout[i][j].cnt == -1) continue;
			if (i < WID - 1 && layout[i + 1][j].cnt == -1) layout[i][j].cnt++;
			if (i > 0 && layout[i - 1][j].cnt == -1) layout[i][j].cnt++;
			if (j < TALL - 1 && layout[i][j + 1].cnt == -1) layout[i][j].cnt++;
			if (j > 0 && layout[i][j - 1].cnt == -1) layout[i][j].cnt++;
			if (i < WID - 1 && j < TALL - 1 && layout[i + 1][j + 1].cnt == -1) layout[i][j].cnt++;
			if (i < WID - 1 && j>0 && layout[i + 1][j - 1].cnt == -1) layout[i][j].cnt++;
			if (i > 0 && j < TALL - 1 && layout[i - 1][j + 1].cnt == -1) layout[i][j].cnt++;
			if (i > 0 && j > 0 && layout[i - 1][j - 1].cnt == -1) layout[i][j].cnt++;
		}
	}
	//计算周围雷数，注意边边角角的格子不能参与判断
}
LRESULT CALLBACK WindowProc(HWND window, UINT ums, WPARAM wp, LPARAM lp)
{
	CImage _4, _3, _2, _1, $0, $1, $2, $3, $4, $5, $6, $7, $8, button[3][2], rs[2];
	_4.Load(L"resources\\_4.png");
	_3.Load(L"resources\\_3.png");
	_2.Load(L"resources\\_2.png");
	_1.Load(L"resources\\_1.png");
	$0.Load(L"resources\\0.png");
	$1.Load(L"resources\\1.png");
	$2.Load(L"resources\\2.png");
	$3.Load(L"resources\\3.png");
	$4.Load(L"resources\\4.png");
	$5.Load(L"resources\\5.png");
	$6.Load(L"resources\\6.png");
	$7.Load(L"resources\\7.png");
	$8.Load(L"resources\\8.png");
	button[0][0].Load(L"resources\\Normal.png");
	button[0][1].Load(L"resources\\Pressed.png");
	button[1][0].Load(L"resources\\Won.png");
	button[1][1] = button[0][1];
	button[2][0].Load(L"resources\\Lost.png");
	button[2][1] = button[0][1];
	rs[0].Load(L"resources\\restart.png");
	rs[1].Load(L"resources\\prestart.png");
	//上面的系列操作是将图片加载，以便显示图片，步骤：①定义[CImage]类图片；②加载图片；③复制图片到窗口（MFC）
	HDC hdc;
	PAINTSTRUCT ps;
	char str[30];
	char time[80] = "time:";
	//[HDC]hdc:绘图信号，[PAINTSTRUCT]ps:没它画不了图，[char*]str,time:显示文字
	switch (ums)
	{
	case 0x2021:
		KillTimer(window, 1);
		break;
	case WM_COMMAND:
	{
		fstream out("data.txt", ios::out);
		switch (LOWORD(wp))
		{
		case EASY:
			hRslt = ShellExecute(NULL, L"open", L"Mine Sweeper Easy.exe", NULL, NULL, SW_SHOWNORMAL);
			assert(hRslt > (HINSTANCE)HINSTANCE_ERROR);
			out << 1;
			exit(0);
			break;
		case MEDIUM:
			hRslt = ShellExecute(NULL, L"open", L"Mine Sweeper Medium.exe", NULL, NULL, SW_SHOWNORMAL);
			assert(hRslt > (HINSTANCE)HINSTANCE_ERROR);
			out << 2;
			exit(0);
			break;
		case HARD:
			hRslt = ShellExecute(NULL, L"open", L"Mine Sweeper Hard.exe", NULL, NULL, SW_SHOWNORMAL);
			assert(hRslt > (HINSTANCE)HINSTANCE_ERROR);
			out << 3;
			exit(0);
			break;
		case CUSTOM:
			hRslt = ShellExecute(NULL, L"open", L"Mine Sweeper Custom.exe", NULL, NULL, SW_SHOWNORMAL);
			assert(hRslt > (HINSTANCE)HINSTANCE_ERROR);
			out << 0;
			exit(0);
			break;
		}
		break;
	}
	case WM_CREATE:
		SetTimer(window, 1, 1000, 0);
		CreateWindow(TEXT("BUTTON"), TEXT("Easy"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 200, TALL * 30 + 80, 90, 30, window, (HMENU)EASY, NULL, NULL);
		CreateWindow(TEXT("BUTTON"), TEXT("Medium"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 300, TALL * 30 + 80, 90, 30, window, (HMENU)MEDIUM, NULL, NULL);
		CreateWindow(TEXT("BUTTON"), TEXT("Hard"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 200, TALL * 30 + 120, 90, 30, window, (HMENU)HARD, NULL, NULL);
		CreateWindow(TEXT("BUTTON"), TEXT("Custom"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 300, TALL * 30 + 120, 90, 30, window, (HMENU)CUSTOM, NULL, NULL);
		break;
	case WM_MOUSEMOVE:
		if (!cheat) break;
		px = LOWORD(lp);
		py = HIWORD(lp);
		if (px >= 50 && px <= 50 + WID * 30 && py >= 60 && py <= 60 + TALL * 30)
		{
			buttx = (px - 50) / 30;
			butty = (py - 60) / 30;
			if (layout[buttx][butty].open) break;
			if (layout[buttx][butty].cnt == -1) SetCursor(LoadCursorFromFile(L"Lost.cur"));
			else SetCursor(LoadCursorFromFile(L"Won.cur"));
		}
		//作弊模式下，如果鼠标在雷格子上光标改为哭脸，在非雷格子上光标改为笑脸
		break;
	case WM_KEYDOWN:
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			face = 0;
			extra = MINE;
			t = 0;
			click = 0;
			game = 1;
			for (int i = 0; i < WID; i++)
			{
				for (int j = 0; j < TALL; j++)
				{
					layout[i][j].question = 0;
					layout[i][j].flag = 0;
					layout[i][j].open = 0;
				}
			}
			SetTimer(window, 1, 1000, 0);
			InvalidateRect(window, nullptr, true);
		}
		//按到空格键时重开，类似于点到笑脸
		if (Connect == 0 && lp == 3014657) Connect++;
		else if (Connect == 1 && lp == 2293761) Connect++;
		else if (Connect == 2 && lp == 1179649) Connect++;
		else if (Connect == 3 && lp == 1966081) Connect++;
		else if (Connect == 4 && lp == 1310721)
		{
			Connect = 0;
			cheat = !cheat ? 1 : 0;
		}
		else Connect = 0;
		//五次连击后启动作弊模式开关
		break;
	case WM_TIMER:
		t += 1;
		{
			RECT* p = new RECT;
			p->left = 55;
			p->right = 120;
			p->top = 5;
			p->bottom = 40;
			InvalidateRect(window, p, 1);
			delete p;
		}
		break;
	case WM_LBUTTONDOWN:
		px = LOWORD(lp);
		py = HIWORD(lp);
		if (px >= 60 + WID * 15 && px <= 110 + WID * 15 && py >= 5 && py <= 55)
		{
			pressed = 1;
			InvalidateRect(window, nullptr, true);
		}
		if (px >= 50 + WID * 30 && px <= 150 + WID * 30 && py >= 25 + TALL * 30 && py <= 100 + TALL * 30)
		{
			pres = 1;
			InvalidateRect(window, nullptr, true);
		}
		if (px >= 50 && px <= 50 + WID * 30 && py >= 60 && py <= 60 + TALL * 30)
		{
			buttx = (px - 50) / 30;
			butty = (py - 60) / 30;
		}
		//鼠标左键被摁下：记录被摁下的格子[[int]buttx,[int]butty]或😀，重开，放开时在奏效
		break;
	case WM_LBUTTONUP:
		if (pressed)
		{
			pressed = 0;
			face = 0;
			extra = MINE;
			t = 0;
			click = 0;
			game = 1;
			for (int i = 0; i < WID; i++)
			{
				for (int j = 0; j < TALL; j++)
				{
					layout[i][j].question = 0;
					layout[i][j].flag = 0;
					layout[i][j].open = 0;
				}
			}
			SetTimer(window, 1, 1000, 0);
			InvalidateRect(window, nullptr, true);
			break;
		}
		//😀被摁下：重开一局：😀变为游戏时，第一下设置为未点击，剩余雷数重置
		if (pres)
		{
			pres = 0;
			face = 0;
			extra = MINE;
			t = 0;
			game = 1;
			for (int i = 0; i < WID; i++)
			{
				for (int j = 0; j < TALL; j++)
				{
					layout[i][j].question = 0;
					layout[i][j].flag = 0;
					layout[i][j].open = 0;
				}
			}
			SetTimer(window, 1, 1000, 0);
			InvalidateRect(window, nullptr, true);
			MessageBox(window, L"地雷的位置与上一个游戏相同\n小心，第一次点击就有可能踩雷！", L"重玩", MB_ICONINFORMATION);
			break;
		}
		//重开一局：点开，插旗，问号全部重置为不是，😀、时间、剩余雷数重置，并跳出提示框
		if (!click)
		{
			init(buttx, butty);
			click = 1;
		}
		//第一次点击：初始化布局，再执行下面的操作
		if (!layout[buttx][butty].question && !layout[buttx][butty].flag && layout[buttx][butty].cnt == 0)
		{
			open_null(buttx, butty);
			InvalidateRect(window, 0, true);
			break;
		}
		//点到空格子：在此格子没有作任何标记的情况下，应利用open_null函数
		layout[buttx][butty].left_button();
		InvalidateRect(window, 0, true);
		break;
	case WM_RBUTTONDOWN:
		//右键就不去要做任何动画了，直接生效
		buttx = (LOWORD(lp) - 50) / 30;
		butty = (HIWORD(lp) - 60) / 30;
		open_surround(buttx, butty);
		layout[buttx][butty].right_button();
		InvalidateRect(window, 0, true);
		//平板无法双击，所以用右击代替，考虑到这两个函数无法同时满足，不需要加条件判断
		break;
	case WM_PAINT:
		hdc = BeginPaint(window, &ps);
		if (extra == 1)strcpy(str, "1 mine remains");
		else sprintf(str, "%d mines remain", extra);
		//显示剩余雷数
		for (int i = 1; 1; i++)
		{
			if (t < pow(10, i))
			{
				wei = i;
				break;
			}
		}
		for (int i = wei + 5; i < 80; i++) time[i] = '\0';
		for (int i = wei + 4; i >= 5; i--)
		{
			time[i] = get(t, wei + 5 - i) + '0';
		}
		//显示时间
		WCHAR text[20], Time[80];
		MultiByteToWideChar(CP_ACP, 0, str, strlen(str) + 1, text, sizeof(text) / sizeof(text[0]));
		mbstowcs_s(NULL, Time, wei + 6, time, _TRUNCATE);
		TextOut(hdc, 60, 100 + TALL * 30, text, strlen(str));
		TextOut(hdc, 60, 10, Time, wei + 5);
		if (fail != 0) TextOut(hdc, 60, 120 + TALL * 30, L"Failed", sizeof("Failed"));
		if (t - fail >= 3) fail = 0;
		button[face][pressed].BitBlt(hdc, 60 + WID * 15, 5, 50, 50, 0, 0, SRCCOPY);
		rs[pres].BitBlt(hdc, 50 + WID * 30, 25 + TALL * 30, 100, 75, 0, 0, SRCCOPY);
		//显示重开键
		for (int i = 0; i < WID; i++)
		{
			for (int j = 0; j < TALL; j++)
			{
				switch (layout[i][j].display())
				{
				case -4:
					_4.BitBlt(hdc, 50 + i * 30, 60 + j * 30, 30, 30, 0, 0, SRCCOPY);
					break;
				case -3:
					_3.BitBlt(hdc, 50 + i * 30, 60 + j * 30, 30, 30, 0, 0, SRCCOPY);
					break;
				case -2:
					_2.BitBlt(hdc, 50 + i * 30, 60 + j * 30, 30, 30, 0, 0, SRCCOPY);
					break;
				case -1:
					_1.BitBlt(hdc, 50 + i * 30, 60 + j * 30, 30, 30, 0, 0, SRCCOPY);
					break;
				case 0:
					$0.BitBlt(hdc, 50 + i * 30, 60 + j * 30, 30, 30, 0, 0, SRCCOPY);
					break;
				case 1:
					$1.BitBlt(hdc, 50 + i * 30, 60 + j * 30, 30, 30, 0, 0, SRCCOPY);
					break;
				case 2:
					$2.BitBlt(hdc, 50 + i * 30, 60 + j * 30, 30, 30, 0, 0, SRCCOPY);
					break;
				case 3:
					$3.BitBlt(hdc, 50 + i * 30, 60 + j * 30, 30, 30, 0, 0, SRCCOPY);
					break;
				case 4:
					$4.BitBlt(hdc, 50 + i * 30, 60 + j * 30, 30, 30, 0, 0, SRCCOPY);
					break;
				case 5:
					$5.BitBlt(hdc, 50 + i * 30, 60 + j * 30, 30, 30, 0, 0, SRCCOPY);
					break;
				case 6:
					$6.BitBlt(hdc, 50 + i * 30, 60 + j * 30, 30, 30, 0, 0, SRCCOPY);
					break;
				case 7:
					$7.BitBlt(hdc, 50 + i * 30, 60 + j * 30, 30, 30, 0, 0, SRCCOPY);
					break;
				case 8:
					$8.BitBlt(hdc, 50 + i * 30, 60 + j * 30, 30, 30, 0, 0, SRCCOPY);
					break;
				}
			}
		}
		//依次显示所有格子的图片
		EndPaint(window, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(window);
		exit(0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		//窗口退出组合键
	}
	return DefWindowProc(window, ums, wp, lp);
}
void open_null(int x, int y)
{
	if (layout[x][y].question || layout[x][y].cnt == -1 || layout[x][y].flag || layout[x][y].open || x > WID - 1 || y > TALL - 1 || x < 0 || y < 0) return;
	//有雷，有标记，被打开的情况下返回（越界时也返回）
	if (layout[x][y].cnt != 0)
	{
		layout[x][y].open = 1;
		return;
	}
	//不是零的时候说明已到达边界（空格边界即递归边界），打开后返回
	layout[x][y].open = 1;
	open_null(x - 1, y);
	open_null(x + 1, y);
	open_null(x, y - 1);
	open_null(x, y + 1);
	open_null(x - 1, y - 1);
	open_null(x - 1, y + 1);
	open_null(x + 1, y - 1);
	open_null(x + 1, y + 1);
	//递归打开周围所有空格子
}
void open_surround(int x, int y)
{
	if (!layout[x][y].open) return; //未被打开则返回，这正好与左右键相对
	if (x > 0 && layout[x - 1][y].cnt == -1 && !layout[x - 1][y].flag ||
		x < WID - 1 && layout[x + 1][y].cnt == -1 && !layout[x + 1][y].flag ||
		y>0 && layout[x][y - 1].cnt == -1 && !layout[x][y - 1].flag ||
		y < TALL - 1 && layout[x][y + 1].cnt == -1 && !layout[x][y + 1].flag ||
		x>0 && y > 0 && layout[x - 1][y - 1].cnt == -1 && !layout[x - 1][y - 1].flag ||
		x > 0 && y < TALL - 1 && layout[x - 1][y + 1].cnt == -1 && !layout[x - 1][y + 1].flag ||
		x < WID - 1 && y>0 && layout[x + 1][y - 1].cnt == -1 && !layout[x + 1][y - 1].flag ||
		x < WID - 1 && y < TALL - 1 && layout[x + 1][y + 1].cnt == -1 && !layout[x + 1][y + 1].flag)
	{
		fail = t;
		return;
	}
	//旁边有雷没标显示失败，这其实是一个作弊机制
	if (x > 0)
	{
		if (layout[x - 1][y].cnt == 0) open_null(x - 1, y);
		else if (!(layout[x - 1][y].question || layout[x - 1][y].flag))
		{
			layout[x - 1][y].left_button();
		}
	}
	if (x < WID - 1)
	{
		if (layout[x + 1][y].cnt == 0) open_null(x + 1, y);
		else if (!(layout[x + 1][y].question || layout[x + 1][y].flag))
		{
			layout[x + 1][y].left_button();
		}
	}
	if (y > 0)
	{
		if (layout[x][y - 1].cnt == 0) open_null(x, y - 1);
		else if (!(layout[x][y - 1].question || layout[x][y - 1].flag))
		{
			layout[x][y - 1].left_button();
		}
	}
	if (y < TALL - 1)
	{
		if (layout[x][y + 1].cnt == 0) open_null(x, y + 1);
		else if (!(layout[x][y + 1].question || layout[x][y + 1].flag))
		{
			layout[x][y + 1].left_button();
		}
	}
	if (x > 0 && y > 0)
	{
		if (layout[x - 1][y - 1].cnt == 0) open_null(x - 1, y - 1);
		else if (!(layout[x - 1][y - 1].question || layout[x - 1][y - 1].flag))
		{
			layout[x - 1][y - 1].left_button();
		}
	}
	if (x > 0 && y < TALL - 1)
	{
		if (layout[x - 1][y + 1].cnt == 0) open_null(x - 1, y + 1);
		else if (!(layout[x - 1][y + 1].question || layout[x - 1][y + 1].flag))
		{
			layout[x - 1][y + 1].left_button();
		}
	}
	if (x < WID - 1 && y>0)
	{
		if (layout[x + 1][y - 1].cnt == 0) open_null(x + 1, y - 1);
		else if (!(layout[x + 1][y - 1].question || layout[x + 1][y - 1].flag))
		{
			layout[x + 1][y - 1].left_button();
		}
	}
	if (x < WID - 1 && y < TALL - 1)
	{
		if (layout[x + 1][y + 1].cnt == 0) open_null(x + 1, y + 1);
		else if (!(layout[x + 1][y + 1].question || layout[x + 1][y + 1].flag))
		{
			layout[x + 1][y + 1].left_button();
		}
	}
	//依次打开所有周围格子，注意加上判断语句
}