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
	this->filename = filename;
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
		fprintf(stderr,"操作的位置:%d:%d",curx,cury);
	}
endTheEditor:
	End();
}
void DkmEditor::End() {
    interface->CloseEchoBack();
	Save();
	fprintf(stderr,"close the editor\n");
	delete interface;
}
int DkmEditor::PreChange() {
	//传递操作的位置
	col.nowPos = curx;
	col.content[col.nowPos].nowPos = cury;
	return 0;
}
int DkmEditor::GetPressKey(FILE* fd) {
	char ch;
    while((ch = getc(fd)) == -1);
	if( mode == COMMAND_MODE ) {
		switch (ch) 
		{
			case 'h': 
			{
				if(cury > 0) 
				{
					--cury;
					if(cury == col.content[curx].endBlock-1){
						cury=col.content[curx].startBlank-1;
					}
					return ARROW_LEFT;
				}	
				break;
			}
			case 'j':
			{
				if(curx < col.capacity-1)
				{
					++curx;
					if(curx == col.startBlank){
						curx = col.endBlock;
					}
					return ARROW_UP;
				}
				break;
			}
			case 'k': 
			{
				if(curx > 0)
				{
					--curx;
					if(curx == col.endBlock-1){
						curx = col.startBlank-1;
					}
					return ARROW_DOWN;
				}
				break;
			}
			case 'l': 
			{
				if(cury < col.content[curx].capacity) 
				{
					++cury;
					if(cury == col.content[curx].startBlank)
						cury=col.content[curx].endBlock;
					return ARROW_RIGHT;
				}
				break;
			}
			case 'i': return INSERT_MODE;
			default:
				return ch;
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
	return -1;
}
int DkmEditor::CommandMode() {
	int action;
    while((action=GetPressKey(stdin))==-1);
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
		case '\r':
			cury=0;
		case '\n':
			//col.InsertContent(*new Row);
			curx++;
			break;
		case 127://backspace
			if(cury > 0){
				col.content[curx].DeleteContent();
				cury--;
			}
			fprintf(stderr,"backspace\n");
			break;
		default:
			//在curx行插入一个字符
			if(curx < col.capacity){
				col.content[curx].InsertContent(action);
				cury++;
			}
			break;
	}
	ReDraw(0, col.size);
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
	//恢复光标
	interface->GoToXy(curx,cury);
	//回收内存
	for(int i=0; i<col.size; ++i){
		delete[] array[i];
		array[i] = nullptr;
	}	
	delete[] array;
	return 0;
}
int DkmEditor::Save() {
	FILE *fp = NULL;
	if((fp=fopen(filename,"w")) != NULL){
		char **array = new char*[col.size];
		ToArray(0, col.size, array);
		for(int i=0; i<col.size; ++i){
			fprintf(fp,"%s\n",array[i]);
		}
		fclose(fp);
		fprintf(stderr,"save file success\n");	
		for(int i=0; i<col.size; ++i){
			delete[] array[i];
			array[i] = nullptr;
		}	
		delete[] array;
	}
	else{
		fprintf(stderr,"save file failed\n");	
	}
	return 0;
}

