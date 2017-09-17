#ifndef __ROW_H__
#define __ROW_H__
class RowCoder {
public:
	int capacity; //行的容量
	int real_size; //行的实际长度
	//end_block不可能超过real_size
	//当end_block == capacity时,表示只有左实体段
	int start_blank, end_block; //空白区域，左闭右开,指向即将编辑的位置 and 空白区域的下一个位置
	char *row; //原始字符
	int curx,cury; //
public:
	//init
	RowCoder();
	//memory
	int MemAlloc(int size);
	//action
	int InsertChar(char ch);
	int InsertString(char *chs, int len);
	int DeleteChar();
	int DeleteString(int len);
	int AdjustBlankPos(int pos); //将row的start_blank置为pos,调整空白区域位置;
	//render
	int ReDraw(); //渲染单行
	//io
	int Save();
	~RowCoder();
};
#endif
