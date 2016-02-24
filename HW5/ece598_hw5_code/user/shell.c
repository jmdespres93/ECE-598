#include <stddef.h>
#include <stdint.h>
#include "../syscalls.h"

static int parse_input(char *string) {

	int result=0;

	if (!strncmp(string,"print",sizeof(string))) {
		printk("Hello World!\n");
	}
	else {
		printk("\nUnknown commmand!\n");
	}

	return result;
}


uint32_t shell(void) {

	char input_string[256];
	uint32_t input_pointer,ch;

	while (1) {
		input_pointer=0;
		printk("$ ");

		while(1) {
			ch=uart_getc();
			if ((ch=='\n') || (ch=='\r')) {
				input_string[input_pointer]=0;
				printk("\n");
				parse_input(input_string);
				break;
			}

			input_string[input_pointer]=ch;
			input_pointer++;
			uart_putc(ch);
		}
	}
}
