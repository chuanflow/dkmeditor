#ifndef __INTERFACE_H__
#define __INTERFACE_H__
//cursor action,interface
class Interface {
private:
    int width,length; 
public:
    Interface();
    virtual int OpenEchoBack();
    virtual int CloseEchoBack();
	virtual void printStatus(char *String);
	virtual void clearStatus();
	virtual void reDraw();
	virtual void goToXy(int x, int y); 
	virtual void upDownRightLeft(char action); 
    virtual ~Interface()=0;
};
#endif
