#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<Windows.h>
#include<time.h>
#include<atlimage.h>
#include<assert.h>
#include<fstream>
void init(int x, int y);			//��ʼ�����֣�ÿ�ο�ʼʱ���ò���
void open_null(int x, int y);		//�򿪿ո���ʱ���ݹ齫��Χ�ĸ���ȫ����
void open_surround(int x, int y);	//˫����Ч��������Χ��������ĸ���ȫ����
LRESULT CALLBACK WindowProc(HWND window, UINT ums, WPARAM wp, LPARAM lp); //���ڽ��̺������ƹ���Ϸ�Ľ���
class mine
{
public:
	int cnt;				// Ϊ 0 - 9 ��Ϊ ��Χ��������Ϊ - 1 ��Ϊ��
	bool flag;				// ����Ƿ񱻲���
	bool open;				// ����Ƿ񱻴�
	bool question;			// ����Ƿ�����ʺ�
	void left_button();		// ������������
	void right_button();	// ����Ҽ�������
	int display()			// ���߻�ͼ����Ӧ�÷�ӳ����ͼƬ
	{
		if (question) return -4;  // �����ʺ�
		if (flag) return -2;      // ������
		if (!open) return -3;     // δ���򿪣���ס
		return cnt;				  // �ѱ��򿪣���ʾ
	}
};
