#include "dkm.h"
#include <cstdlib>
#include <iostream>
/*
连续存储,动态分配
 */
static DkmEditor E;
int main(int argc, char *argv[])
{
	if(argc != 2) {
		std::cerr<<"use: dkm <filename>"<<std::endl;	
		exit(1);
	}	
	E.ReadFile(argv[1]);
	E.Start();
	return 0;
}
