#ifndef __STRUCT_H__
#define __STRUCT_H__
#include <cstring>
#include <cstdio>
template<typename Content>
class Struct {
//结构图
/*
   |****_____*******|          |
   c    s    e   size+e-s	  cap
*/
public:
	int capacity; //行的容量
	int size; //行的实际长度
	int nowPos; //当前的位置
	//endBlock不可能超过real_size
	int startBlank, endBlock; //空白区域，左闭右开,指向即将编辑的位置 and 空白区域的下一个位置
	Content *content; //指向存储的内容,默认已经有1个内容
public:
	//init
	Struct(){
		/*
		  初始
		   | * |
		  s,e cap
		 */
		content = nullptr;
		size = 1;
		capacity = 80;
		startBlank = 1;	
		endBlock = 80;
		content = new Content[capacity];
	}
	//调整空白位置到nowPos位置
	int AdjustBlank(){
		/*
		 |*********|      |*********|
		 c    pos  s      e        cap 
		 变换为:
		 |*****|      |*************|
		 c  pos,s     e			   cap 
		 */
		while(nowPos < startBlank){
			content[--endBlock] = content[--startBlank];
		}
		while(startBlank < nowPos){
			content[startBlank++] = content[endBlock++];
		}
	}
	//memory
	int MemAlloc(int reSize){
		fprintf(stderr,"Memallocing\n");
		Content *re = new Content[reSize];
		for(int i=0; i<capacity; ++i){
			re[i] = content[i];
		}
		delete[] content;
		content = re;
		re = nullptr;
		capacity = reSize;
		fprintf(stderr,"Memalloced\n");
	}
	//action
	int InsertContent(Content con){
		if(startBlank < endBlock) {
			fprintf(stderr,"installing\n");
			AdjustBlank();
			content[startBlank] = con;
			startBlank ++;
			size ++;
			fprintf(stderr,"installed\n");
		} else {
			int reSize =  capacity + 1000;
			MemAlloc(reSize);
			/*
			|*********|*******|      |
			c        s,e            cap
			变换为:
			|*********|      |*******|
			c        s       e      cap
			 */
			int endPos1 = capacity;
			int endPos2 = size;
			while(endPos2 != startBlank){
				content[--endPos1] = content[--endPos2];
			}
			startBlank = endPos2;
			endBlock = endPos1;
			InsertContent(con);
		}	
		fprintf(stderr,"install s=%d e=%d size=%d c=%d\n",startBlank,endBlock,size,capacity);
		return 0;
	}
	int DeleteContent(){
		if(startBlank > 0){
			--startBlank;
		}	
	};
	~Struct(){
		fprintf(stderr,"~struct\n");
		//delete[] content;
		content = nullptr;
	};
};
#endif
