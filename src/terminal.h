#ifndef __TERMIANL_H__
#define __TERMIANL_H__
#include "interface.h"
#include <termios.h>
class Terminal:public Interface{
public:
    static struct termios orig_termios;
    int echo_back; //回显
public:
    Terminal();
	//状态栏
	void PrintStatus(char *msg);
	void ClearStatus();
	//开启关闭回显
	int OpenEchoBack();
	int CloseEchoBack();
	//重新渲染到屏幕
	void ReDraw(int s,int e, char* rows[]);
	//光标移动,屏幕清屏
	void GoToXy(int x, int y); //\033[%d;%dH
	void UpDownRightLeft(int action); //\033[1(A|B|C|D)
	void ClearScreen();
	int GetCurx();
	int GetCury();
	int SetCurx(int x);
	int SetCury(int y);
	int GetWindowSize(int ifd, int ofd, int *rows, int *cols);
	int GetCursorPosition(int ifd, int ofd, int *rows, int *cols);
    ~Terminal();
};
#endif
