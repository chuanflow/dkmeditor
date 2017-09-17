#ifndef __DKM_H__
#define __DKM_H__
#include "row.h"
#include "interface.h"
#include <cstdio>
class DkmEditor {
private:
	RowCoder *rows; 
    Interface *interface;
	int curx,cury; //即将操作的位置
	char *filename; //文件名
	char statusline[100]; 
private:
	int real_size; // 已经使用行数
	int capacity; //最大行数
	int start_blank, end_block;
	int mode;
public:
	//init
	DkmEditor();
	void Start(char* filename);
	void End();
public:
	//io,action
	int ReadFile(char* filename);
	int MemAlloc(int add_size);
	int GetPressKey(FILE* fd);
	int PreChange();//操作内容之前做的工作
	int AddLine();
	int AddManyLine(int lines);
	int AdjustBlankPos(int pos); //将rows的start_blank置为pos,调整空白区域位置;
	int Save();
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
public:
	//render
	RowCoder* ReDraw(); //only return rows
	int DelLine(int lines);
};
#endif
