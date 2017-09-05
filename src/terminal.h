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
	void printStatus(char *String);
	void clearStatus();
	//开启关闭回显
	int OpenEchoBack();
	int CloseEchoBack();
	//重新渲染到屏幕
	void reDraw();
	//光标移动	
	void goToXy(int x, int y); //\033[%d;%dH
	void upDownRightLeft(char action); //\033[1(A|B|C|D)
    ~Terminal();
};
#endif
