#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<Windows.h>
#include<time.h>
#include<atlimage.h>
#include<assert.h>
#include<fstream>
void init(int x, int y);			//初始化布局，每次开始时都得布局
void open_null(int x, int y);		//打开空格子时，递归将周围的格子全部打开
void open_surround(int x, int y);	//双击的效果，将周围除插了旗的格子全都打开
LRESULT CALLBACK WindowProc(HWND window, UINT ums, WPARAM wp, LPARAM lp); //窗口进程函数，掌管游戏的进程
class mine
{
public:
	int cnt;				// 为 0 - 9 则为 周围雷数量，为 - 1 则为雷
	bool flag;				// 检测是否被插旗
	bool open;				// 检测是否被打开
	bool question;			// 检测是否打了问号
	void left_button();		// 鼠标左键被摁下
	void right_button();	// 鼠标右键被摁下
	int display()			// 告诉绘图函数应该放映那张图片
	{
		if (question) return -4;  // 被打问号
		if (flag) return -2;      // 被插旗
		if (!open) return -3;     // 未被打开，掩住
		return cnt;				  // 已被打开，显示
	}
};
