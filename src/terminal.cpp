#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include "terminal.h"
#include "define.h"
struct termios Terminal::orig_termios;
Terminal::Terminal(){
    echo_back = 0;
	curx = 0;
	cury = 0;
	goToXy(curx,cury);
	clearScreen();
}
int Terminal::CloseEchoBack(){
/* Don't even check the return value as it's too late. */
	if (echo_back) {
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
		echo_back = 0;
	}
    return 0;
}
int Terminal::OpenEchoBack(){
	struct termios raw;
	if (!isatty(STDIN_FILENO)) goto fatal;
	if (tcgetattr(STDIN_FILENO,&orig_termios) == -1) goto fatal;
	raw = orig_termios;  /* modify the original echo_back */
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0; 
	raw.c_cc[VTIME] = 1;
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0) goto fatal;
	echo_back = 1;
	return 0;
	fatal:
		return -1;
}
void Terminal::reDraw(RowCoder *rows){
	for(size_t i=0; i<rows->capacity; ++i) {
		if(i == rows->start_blank) 
			i = rows->end_block;
		for(size_t j=0; j<rows[i].capacity; ++j) {
			if(j == rows[i].start_blank)
				j = rows[i].end_block;
			printf("\033[%zu;%zuH%c",i,j,rows[i].row[j]);	
		}
		printf("\n");
	}
}
void Terminal::goToXy(int x, int y) {
	curx = x;
	cury = y;
	printf("\033[%d;%dH",x+1,y+1);
}
void Terminal::upDownRightLeft(int action) {
	switch(action) {
	case ARROW_UP:
		cury--;
		break;
	case ARROW_RIGHT:
		cury++;
		break;
	case ARROW_DOWN:
		curx--;
		break;
	case ARROW_LEFT:
		curx++;
		break;
	}
	printf("\033[1%c",action-1000+'A');
}
void Terminal::clearScreen() {
	printf("\033[2J");
}
void Terminal::printStatus(char *String) {
}
void Terminal::clearStatus(){
}
Terminal::~Terminal(){
}
