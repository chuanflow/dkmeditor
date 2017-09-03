#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "ascii.h"
#include "row.h"
RowCoder::RowCoder() {
	row = nullptr;
	real_size = 0;
	capacity = 0;
	start_blank = 0;	
	end_block = 0;
}
int RowCoder::MemAlloc(size_t add_size) {
	if(row == nullptr)
		row = (char*)malloc(add_size);
	else {
		row = (char*)realloc(row, add_size);
	}
	if(row != nullptr){
		memmove(row+(end_block+add_size), row+end_block, real_size-end_block);
		end_block += add_size;
		capacity += add_size;
		return 0;
	} else {
		return 1;
	}
}
int RowCoder::InsertChar(char ch) {
	if(start_blank < end_block) {
		row[start_blank ++] = ch;
		real_size ++;
	} else {
		size_t add_size =  50;
		MemAlloc(add_size);
		InsertChar(ch);
	}	
	return 0;
}
int RowCoder::InsertString(char *chs, int len) {
	printf("%s\n", chs);
		
	return 0;
}
int RowCoder::ReDraw() {
	return 0;
}
int RowCoder::DeleteChar() {
	if(start_blank > 0)
		--start_blank;
	return 0;
}
int RowCoder::DeleteString(int len) {
	start_blank -= len;
	return 0;
}
int RowCoder::Save() {
	return 0;
}
RowCoder::~RowCoder() {
	free(row);
	row = nullptr;
}
