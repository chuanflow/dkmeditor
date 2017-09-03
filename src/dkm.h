#ifndef __DKM_H__
#define __DKM_H__
#include "row.h"
class DkmEditor {
private:
	RowCoder *rows; 
	size_t curx,cury; //光标位置
	char *filename; //文件名
	char onech[20]; //满缓冲模式缓冲区
	char statusline[100]; 
private:
	size_t real_size; // 已经使用行数
	size_t capacity; //最大行数
	size_t start_blank, end_block;
	int mode;
	int echo_back;
public:
	//init
	DkmEditor();
	int Start();
public:
	//io
	int ReadFile(char* filename);
	int MemAlloc(size_t add_size);
	int GetPressKey(int fd);
	int Save();
public:
	//mode
	int CommandMode();
	int InsertMode();
	int VisualMode();
	int BlockMode();
public:
	//cursor action
	int GoToXy(int x,int y);
	int MoveCursor(int action);
public:
	//render
	RowCoder* ReDraw(); //only return rows
	int DelLine(size_t lines);
};
#endif
