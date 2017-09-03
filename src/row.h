#ifndef __ROW_H__
#define __ROW_H__
#include <cstddef>
class RowCoder {
public:
	size_t capacity; //行的容量
	size_t start_blank, end_block; //空白区域，左闭右开,指向即将编辑的位置 and 空白区域的下一个位置
	char *row; //原始字符
	size_t real_size; //行的实际长度
public:
	//init
	RowCoder();
	//memory
	int MemAlloc(size_t size);
	//action
	int InsertChar(char ch);
	int InsertString(char *chs, int len);
	int DeleteChar();
	int DeleteString(int len);
	//render
	int ReDraw(); //渲染单行
	//io
	int Save();
	~RowCoder();
};
#endif
