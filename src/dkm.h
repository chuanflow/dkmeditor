#ifndef __DKM_H__
#define __DKM_H__
#include "row.h"
#include "col.h"
#include "interface.h"
#include <cstdio>
class DkmEditor {
private:
	Col col;
    Interface *interface;
	int curx,cury; //文本中操作的位置
	char *filename; //文件名
	char statusline[100]; 
private:
	//int real_size; // 已经使用行数
	//int capacity; //最大行数
	//int start_blank, end_block;
	int mode;
public:
	//init
	DkmEditor();
	void Start(char* filename);
	void End();
public:
	//io,action
	//int ReadFile(char* filename);
	int GetPressKey(FILE* fd);
	int PreChange();//操作内容之前做的工作
	//int Save();
	int ToArray(int s, int e, char *array[]);
public:
	//mode
	int CommandMode();
	int InsertMode();
	int VisualMode();
	int BlockMode();
public:
	int GoToXy(int x,int y);
	int MoveCursor(int action);
	void ClearScreen();
	int ReDraw(int s, int e);
public:
	//int DelLine(int lines);
};
#endif
