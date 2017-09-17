#ifndef __INTERFACE_H__
#define __INTERFACE_H__
//cursor action,interface
#include "row.h"
class Interface {
private:
protected:
    int ws_cols,ws_rows; 
	int curx,cury;
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
	virtual int getCurx()=0;
	virtual int getCury()=0;
	virtual int setCurx(int x)=0;
	virtual int setCury(int y)=0;
    virtual ~Interface();
};
#endif
