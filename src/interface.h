#ifndef __INTERFACE_H__
#define __INTERFACE_H__
//cursor action,interface
class Interface {
private:
protected:
    int ws_cols,ws_rows; 
	int curx,cury;//界面光标位置
public:
    Interface();
    virtual int OpenEchoBack()=0;
    virtual int CloseEchoBack()=0;
	virtual void PrintStatus(char *String)=0;
	virtual void ClearStatus()=0;
	virtual void ReDraw(int s, int e, char* rows)=0;//80*80
	virtual void GoToXy(int x, int y)=0; 
	virtual void UpDownRightLeft(int action)=0; 
	virtual void ClearScreen()=0;
	virtual int GetCurx()=0;
	virtual int GetCury()=0;
	virtual int SetCurx(int x)=0;
	virtual int SetCury(int y)=0;
    virtual ~Interface();
};
#endif
