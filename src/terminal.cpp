#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
namespace TERMIANL{
	static termios orig_termios;
	int closeEchoBack(int mode){
	/* Don't even check the return value as it's too late. */
		if (mode) {
			tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
			mode = 0;
		}
		return mode;
	}
	void exitEchoBack() {
		int mode= 0;
		closeEchoBack(mode);
	}
	int openEchoBack(int mode){
		struct termios raw;
		if (mode) return 0; /* Already enabled. */
		if (!isatty(STDIN_FILENO)) goto fatal;
		typedef void (*exitEchoBackPoint)();
		exitEchoBackPoint = exitEchoBack;
		atexit(exitEchoBack(mode));
		if (tcgetattr(STDIN_FILENO,&orig_termios) == -1) goto fatal;
		raw = orig_termios;  /* modify the original mode */
		/* input modes: no break, no CR to NL, no parity check, no strip char,
		*      * no start/stop output control. */
		raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
		/* output modes - disable post processing */
		raw.c_oflag &= ~(OPOST);
		/* control modes - set 8 bit chars */
		raw.c_cflag |= (CS8);
		/* local modes - choing off, canonical off, no extended functions,
		*      * no signal chars (^Z,^C) */
		raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
		/* control chars - set return condition: min number of bytes and timer. */
		raw.c_cc[VMIN] = 0; /* Return each byte, or zero for timeout. */
		raw.c_cc[VTIME] = 1; /* 100 ms timeout (unit is tens of second). */
		/* put terminal in raw mode after flushing */
		if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0) goto fatal;
		mode = 1;
		return 0;

		fatal:
			return -1;
	}
	void reDraw(){
	}
	void goToXy(int x, int y) {
		printf("\033[%d;%dH",x+1,y+1);
	}
	void upDownRightLeft(char action) {
		printf("%c",action);
		printf("\033[1%c",action);
	}
	void printStatus(char *String) {
	
	}
	void clearStatus(){
	}
}
