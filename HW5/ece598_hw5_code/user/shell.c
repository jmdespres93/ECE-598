#include <stddef.h>
#include <stdint.h>
#include "../syscalls.h"
#include "vlibc.h"

void prtint_err();

uint32_t shell(void) {

	uint32_t ch;
	uint8_t buff[4096] = {0}; // buffer to hold input
	uint32_t i; // placeholder in buffer
	uint8_t *print = "print", *meow = "meow", *ptime = "time";
	uint32_t ticks;

	/* Enter our "shell" */
	while (1) {
		i = 0;
		while (1) {
			//get input and echo it to screen
			ch=getchar();
			putchar(ch);

			//check if return was hit, 
			//if so, terminate string and break out of loop to analyze input
			if (ch == '\r') {
				buff[i] = '\0';
				break;
			}
			//while within the limits of the buffer, keep adding characters
			if (i<4096) {
				buff[i] = ch;
				i++;
			} 
			//if exceeded buffer size, print error and break out of loop
			else {
				printf("chill wif teh input\n");
				break;
			}
		}

		//compare input with list of known commands
		//first check that lengths match
		//then check if content matches
		if (i==5) {
			if(strncmp(print, buff, 5)==0) {
				printf("Hello World\n");
			} else print_err();
		} else if (i == 4) {
			if (strncmp(ptime, buff, 4)==0) {
				syscall1(SYSCALL_TIME,(long)&ticks);
				printf("%d seconds\n", ticks);
			} else if (strncmp(meow, buff, 4)==0) {
				nyan();
			} else print_err();
		} else print_err();

	}
}

print_err() {
	printf("i doan knoe dis command\n");
}