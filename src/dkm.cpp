#include "define.h"
#include "terminal.h"
#include "ascii.h"
#include "dkm.h"
#include <fstream>
#include <cstring>
#include <ctime>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
DkmEditor::DkmEditor() {
	curx = 0;
	cury = 0;
	real_size = 0;
	capacity = 0;
	start_blank = 0;
	end_block = 0;
	mode = COMMAND_MODE;
	filename = nullptr;
	interface = nullptr;
}
int DkmEditor::ReadFile(char* filename) {
	//std::ifstream readfile;
	//readfile.open(filename, std::ios::in);
	//char buf[10000];
	//MemAlloc(100);
	//int culnum = 0;
	//while(! readfile.eof() ) {
	//	readfile.getline(buf, 1000, '\n');	
	//	rows[culnum++].InsertString(buf, strlen(buf));
	//}
	//readfile.close();
	return 0;
}
void DkmEditor::Start(char* filename) {
	//读取文件
	ReadFile(filename);
	//连接界面
    interface = new Terminal();
	//interface->reDraw(rows);
    interface->OpenEchoBack();
	//进入编辑器模式
	while( true ){
		switch ( mode ) 
		{
			case COMMAND_MODE:
				CommandMode();
				break;
			//case INSERT_MODE:
			//	PreChange();
			//	InsertMode();
			//	break;
			//case VISUAL_MODE:
			//	VisualMode();
			//	break;
			//case BLOCK_MODE:
			//	BlockMode();
			//	break;
			case ENDALL_MODE:
				goto endTheEditor;
		//	default:
        //        break;
		}
	}
endTheEditor:
	End();
}
void DkmEditor::End() {
    interface->CloseEchoBack();
	fprintf(stderr,"close the editor\n");
	delete interface;
}
int DkmEditor::PreChange() {
	//以界面的坐标为基准,转换为标准的dkmeditor坐标,(从0开始)
	curx = interface->getCurx();
	cury = interface->getCury();
	rows[curx].curx = curx;
	rows[curx].cury = cury;
	return 0;
}
int DkmEditor::AdjustBlankPos(int pos) {
	while(pos != start_blank){
		if(pos > start_blank){
			rows[start_blank++] = rows[end_block++];
		}
		else{
			rows[--end_block] = rows[--start_blank];
		}
	}
	if(end_block > real_size) 
		end_block = real_size;
	return 0;
}
int DkmEditor::GetPressKey(FILE* fd) {
	char ch;
    ch = getc(fd);
	//int nread;
	//while((nread = read(STDIN_FILENO, &ch, 1)) == 0);
	if( mode == COMMAND_MODE ) {
		while(true) {
			switch (ch) 
			{
				case 'h': return ARROW_LEFT;	
				case 'j': return ARROW_UP;
				case 'k': return ARROW_DOWN;
				case 'l': return ARROW_RIGHT;
				default:
					return ch;
			}
		}
	}
	else{
		switch (ch) 
		{
			default:
				return ch;
		}
	}
	return 0;
}
int DkmEditor::CommandMode() {
	int action;
    action=GetPressKey(stdin);
	switch (action) 
	{
		case INSERT_MODE:
			mode = INSERT_MODE;
			break;
		case VISUAL_MODE:
			mode = VISUAL_MODE;
			break;
		case Ctrl_V:
		   mode = BLOCK_MODE;	
		   break;
		case ARROW_UP:
		case ARROW_DOWN:
		case ARROW_LEFT:
		case ARROW_RIGHT:
		   interface->upDownRightLeft(action);
		   break;
		case Ctrl_C:
		   mode = ENDALL_MODE;
		   break;
		default:
		   break;
	}
	return 0;
}
int DkmEditor::InsertMode() {
	int action;
    action=GetPressKey(stdin);
	switch (action)
	{
		case ESC:
			mode = COMMAND_MODE;
			break;
		case '\n':
			AddLine();
			break;
		default:
			rows[curx].InsertChar(action);
			break;
	}
	return 0;
}
int DkmEditor::VisualMode() {
	return 0;
}
int DkmEditor::BlockMode() {
	return 0;
}
//分配行
int DkmEditor::MemAlloc(int add_size) {
	RowCoder *tmp = new RowCoder[real_size + add_size];
	if(rows == nullptr){
		rows = tmp;
	}
	else {
		for(int i=0; i < start_blank; ++i) {
			tmp[i] = rows[i];
		}
		for(int i=capacity-1; i >= end_block; --i){
			tmp[i+add_size] = rows[i];
		}
	}
	end_block += add_size;
	capacity += add_size;
	rows = tmp;
	return rows == nullptr;
}

int DkmEditor::AddLine() {
	AdjustBlankPos(curx);
	++start_blank;
	return 0;
}
int DkmEditor::DelLine(int lines) {
	if(end_block + lines < real_size) {
		end_block += lines;
	} else {
		end_block = real_size;	
	}
	real_size -= lines;
	return 0;
}
RowCoder*  DkmEditor::ReDraw() {
	return rows;
}
int DkmEditor::Save() {
	std::ofstream writefile;
	writefile.open(filename, std::ios::out);
	for(int rownum=0; rownum < capacity; ++rownum) {
		//if(rownum == start_blank)
		//	rownum = end_block;
		//int colnum_size = rows[rownum].capacity;
		////char *buf = rows[rownum].MemAlloc(rows[rownum].real_size);
		//writefile.write(buf, real_size);
	}
	writefile.close();
	return 0;
}
