#ifndef __TERMIANL_H__
#define __TERMIANL_H__
namespace TERMIANL{
	//状态栏
	void printStatus(char *String);
	void clearStatus();
	//开启关闭回显
	int openEchoBack(int mode);
	int closeEchoBack(int mode);
	void exitEchoBack(int mode);
	//重新渲染到屏幕
	void reDraw();
	//光标移动	
	void goToXy(int x, int y); //\033[%d;%dH
	void upDownRightLeft(char action); //\033[1(A|B|C|D)
}
#endif
