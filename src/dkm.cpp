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
	mode = COMMAND_MODE;
	filename = nullptr;
	interface = nullptr;
}
//int DkmEditor::ReadFile(char* filename) {
//	//std::ifstream readfile;
//	//readfile.open(filename, std::ios::in);
//	//char buf[10000];
//	//MemAlloc(100);
//	//int culnum = 0;
//	//while(! readfile.eof() ) {
//	//	readfile.getline(buf, 1000, '\n');	
//	//	rows[culnum++].InsertString(buf, strlen(buf));
//	//}
//	//readfile.close();
//	return 0;
//}
void DkmEditor::Start(char* filename) {
	//读取文件
	//ReadFile(filename);
	//连接界面
    interface = new Terminal();
	//interface->reDraw(rows);
    interface->OpenEchoBack();
	interface->ClearScreen();
	fprintf(stderr,"mode = %d\n", mode);
	//进入编辑器模式
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
//先以界面的坐标为基准,转换为标准的dkmeditor坐标,(从0开始)
	curx = interface->GetCurx();
	cury = interface->GetCury();
	col.nowPos = curx;
	col.content[col.nowPos].nowPos = cury;
	return 0;
}
int DkmEditor::GetPressKey(FILE* fd) {
	char ch;
    while((ch = getc(fd)) == -1);
	if( mode == COMMAND_MODE ) {
		while(true) {
			switch (ch) 
			{
				case 'h': return ARROW_LEFT;	
				case 'j': return ARROW_UP;
				case 'k': return ARROW_DOWN;
				case 'l': return ARROW_RIGHT;
				case 'i': return INSERT_MODE;
				default:
					return ch;
			}
		}
	}
	else{
		fprintf(stderr,"%c\n",ch);
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
	fprintf(stderr,"%d\n",action);
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
		   interface->UpDownRightLeft(action);
		   break;
		case Ctrl_C:
		   mode = ENDALL_MODE;
		   break;
		default:
		   break;
	}
	fprintf(stderr,"CommandMode Ended\n");
	return 0;
}
int DkmEditor::InsertMode() {
	PreChange();
	int action;
    action=GetPressKey(stdin);
	switch (action)
	{
		case ESC:
			mode = COMMAND_MODE;
			break;
		case '\n':
			//col.InsertContent(*new Row);
			break;
		default:
			//在curx行插入一个字符
			if(curx < col.capacity){
				col.content[curx].InsertContent(action);
				ReDraw(0, col.size);
			}
			break;
	}
	fprintf(stderr,"InsertMode Ended\n");
	return 0;
}
int DkmEditor::VisualMode() {
	return 0;
}
int DkmEditor::BlockMode() {
	return 0;
}
//提取[s,e)区间行的字符到二维字符数组,'\0'结束
int DkmEditor::ToArray(int s, int e, char* array[]){
	int p=0,q=0;
	for(int i=s; i<e && i<col.capacity; ++i){
		if(i == col.startBlank){
			i = col.endBlock;
			if(i==e || i == col.capacity)
				continue;
		}
		Row &row = col.content[i];
		array[p] = new char[row.size];
		for(int j=0; j<row.capacity; ++j){
			if(j == row.startBlank){
				j=row.endBlock;
				if(j == row.capacity)
					continue;
			}
			array[p][q++]=row.content[j];	
		}
		array[p++][q]='\0';
		q=0;
	}	
	return 0;
}
//重新渲染第s行到第e行的字符,[s,e)
int DkmEditor::ReDraw(int s,int e){
	char **array = new char*[col.size];
	ToArray(s, e, array);
	for(int i=0; i<e-s; ++i){
		fprintf(stderr,"%s",array[i]);
	}
	//显示到屏幕
	interface->ReDraw(s, e, array);
	//回收内存
	for(int i=0; i<col.size; ++i){
		delete[] array[i];
		array[i] = nullptr;
	}	
	delete[] array;
	return 0;
}
//int DkmEditor::Save() {
//	std::ofstream writefile;
//	writefile.open(filename, std::ios::out);
//	for(int rownum=0; rownum < capacity; ++rownum) {
//		//if(rownum == start_blank)
//		//	rownum = end_block;
//		//int colnum_size = rows[rownum].capacity;
//		////char *buf = rows[rownum].MemAlloc(rows[rownum].real_size);
//		//writefile.write(buf, real_size);
//	}
//	writefile.close();
//	return 0;
//}

