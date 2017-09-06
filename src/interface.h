#ifndef __INTERFACE_H__
#define __INTERFACE_H__
//cursor action,interface
#include "row.h"
class Interface {
private:
    int width,length; 
public:
    Interface();
    virtual int OpenEchoBack()=0;
    virtual int CloseEchoBack()=0;
	virtual void printStatus(char *String)=0;
	virtual void clearStatus()=0;
	virtual void reDraw(RowCoder* rows)=0;
	virtual void goToXy(int x, int y)=0; 
	virtual void upDownRightLeft(int action)=0; 
	virtual void clearScreen()=0;
    virtual ~Interface();
};
#endif
