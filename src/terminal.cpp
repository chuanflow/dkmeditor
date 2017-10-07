#include <cstdio>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <termios.h>
#include "terminal.h"
#include "define.h"
#include "ascii.h"
struct termios Terminal::orig_termios;
Terminal::Terminal(){
    echo_back = 0;
	curx = 0;
	cury = 0;
	ws_cols = 80;
	ws_rows = 80;
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
//从屏幕上的[s,e)输出rows的内容
void Terminal::ReDraw(int s, int e, char *rows){
	ClearScreen();
	printf("%s",rows);
}
void Terminal::GoToXy(int x, int y) {
	curx = x;
	cury = y;
	printf("\033[%d;%dH",x+1,y+1);
}
int Terminal::GetCurx(){
	GetCursorPosition(STDIN_FILENO,STDOUT_FILENO,&curx,&cury);
	return curx-1;
}
int Terminal::GetCury(){
	GetCursorPosition(STDIN_FILENO,STDOUT_FILENO,&curx,&cury);
	return cury-1;
}
int Terminal::SetCurx(int x) {
	curx = x;
	return 0;
}
int Terminal::SetCury(int y){
	cury = y;
	return 0;
}
void Terminal::UpDownRightLeft(int action) {
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
void Terminal::ClearScreen() {
	printf("\033[2J\033[0;0H");
}
void Terminal::PrintStatus(char *msg) {
}
void Terminal::ClearStatus(){
}
int Terminal::GetCursorPosition(int ifd, int ofd, int *rows, int *cols) {
	char buf[32];
	
	unsigned int i = 0;
    /* Report cursor location */
    if (write(ofd, "\x1b[6n", 4) != 4) return -1;

    /* Read the response: ESC [ rows ; cols R */
    while (i < sizeof(buf)-1) {
	if (read(ifd,buf+i,1) != 1) break;
	if (buf[i] == 'R') break;
	i++;
	}
    buf[i] = '\0';

	/* Parse it. */
	if (buf[0] != ESC || buf[1] != '[') return -1;
	if (sscanf(buf+2,"%d;%d",rows,cols) != 2) return -1;
	return 0;
}

/* Try to get the number of columns in the current terminal. If the ioctl()
 *  * call fails the function will try to query the terminal itself.
 *   * Returns 0 on success, -1 on error. */
int Terminal::GetWindowSize(int ifd, int ofd, int *rows, int *cols) {
	struct winsize ws;
	if (ioctl(1, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
		/* ioctl() failed. Try to query the terminal itself. */
		int orig_row, orig_col, retval;
		/* Get the initial position so we can restore it later. */
		retval = GetCursorPosition(ifd,ofd,&orig_row,&orig_col);
		if (retval == -1) goto failed;
		
		/* Go to right/bottom margin and get position. */
		if (write(ofd,"\x1b[999C\x1b[999B",12) != 12) goto failed;
		retval = GetCursorPosition(ifd,ofd,rows,cols);
		if (retval == -1) goto failed;
		/* Restore position. */
		char seq[32];
		snprintf(seq,32,"\x1b[%d;%dH",orig_row,orig_col);
		if (write(ofd,seq,strlen(seq)) == -1) {
		/* Can't recover... */
	}
	return 0;
	} else {
		*cols = ws.ws_col;
		*rows = ws.ws_row;
		return 0;
	}
	failed:
		return -1;
}
Terminal::~Terminal(){
}

