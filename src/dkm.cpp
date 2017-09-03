#include "dkm.h"
#include "ascii.h"
#include "define.h"
#include "terminal.h"
#include <unistd.h>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <iostream>
DkmEditor::DkmEditor() {
	curx = 0;
	cury = 0;
	real_size = 0;
	capacity = 0;
	start_blank = 0;
	end_block = 0;
	echo_back = 0; // 默认会回显
	mode = COMMAND_MODE;
	filename = nullptr;
	//setvbuf(stdin, onech, _IOFBF, 1); //满缓冲模式
	//echo_back = TERMIANL::openEchoBack(echo_back);
}
int DkmEditor::ReadFile(char* filename) {
	//std::ifstream readfile;
	//readfile.open(filename, std::ios::in);
	//char buf[10000];
	//rows = new RowCoder[1000];
	//int culnum = 0;
	//while(! readfile.eof() ) {
	//	readfile.getline(buf, 1000, '\n');	
	//	rows[culnum++].InsertString(buf, strlen(buf));
	//}
	//readfile.close();
	return 0;
}
int DkmEditor::Start() {
	while( true ){
		switch ( mode ) 
		{
			case COMMAND_MODE:
				CommandMode();
				break;
			case INSERT_MODE:
				InsertMode();
				break;
			case VISUAL_MODE:
				VisualMode();
				break;
			case BLOCK_MODE:
				BlockMode();
				break;
			default:
				return -1;
		}
	}
	return 0;
}
int DkmEditor::GetPressKey(int fd) {
	//int nread;
	char ch;
	std::cin.get(ch);
	//while((nread = read(fd, &ch, 1)) == 0);
	//if(nread == -1)
	//	exit(1);
	printf("asdad");
	sleep(2);
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
    action=GetPressKey(STDIN_FILENO);
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
		   MoveCursor(action);		   
		   break;
		case Ctrl_C:
		   exit(0);
		   break;
		default:
		   return 0;
	}
	return 0;
}
int DkmEditor::InsertMode() {
	return 0;
}
int DkmEditor::VisualMode() {
	return 0;
}
int DkmEditor::BlockMode() {
	return 0;
}
int DkmEditor::GoToXy(int x, int y ) {
	{
		TERMIANL::goToXy(x,y);
	}
	return 0;
}
int DkmEditor::MoveCursor(int action) {
	{
		TERMIANL::upDownRightLeft(action-1000+'A');
	}
	return 0;
}
int DkmEditor::MemAlloc(size_t add_size) {
	if(rows == nullptr)
		rows = new RowCoder[add_size];
	else {
		rows = new(rows) RowCoder[add_size];
	}
	if(rows != nullptr){
		memmove(rows+(end_block+add_size), rows+end_block, real_size-end_block);
		end_block += add_size;
		capacity += add_size;
		return 0;
	}
	else {
		return -1;
	}
}
int DkmEditor::DelLine(size_t lines) {
	if(end_block + lines < real_size) {
		end_block += lines;
	} else {
		end_block = real_size;	
	}
	return 0;
}
RowCoder*  DkmEditor::ReDraw() {
	return rows;
}
int DkmEditor::Save() {
	std::ofstream writefile;
	writefile.open(filename, std::ios::out);
	for(size_t rownum=0; rownum < capacity; ++rownum) {
		//if(rownum == start_blank)
		//	rownum = end_block;
		//size_t colnum_size = rows[rownum].capacity;
		////char *buf = rows[rownum].MemAlloc(rows[rownum].real_size);
		//writefile.write(buf, real_size);
	}
	writefile.close();
	return 0;
}
