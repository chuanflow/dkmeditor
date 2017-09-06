#ifndef __DKM_H__
#define __DKM_H__
#include "row.h"
#include "interface.h"
#include <cstdio>
class DkmEditor {
private:
	RowCoder *rows; 
    Interface *interface;
	size_t curx,cury; //光标位置
	char *filename; //文件名
	char statusline[100]; 
private:
	size_t real_size; // 已经使用行数
	size_t capacity; //最大行数
	size_t start_blank, end_block;
	int mode;
public:
	//init
	DkmEditor();
	void Start();
	void End();
public:
	//io
	int ReadFile(char* filename);
	int MemAlloc(size_t add_size);
	int GetPressKey(FILE* fd);
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
	int DelLine(size_t lines);
};
#endif
