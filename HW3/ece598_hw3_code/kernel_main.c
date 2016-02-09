#include <stddef.h>
#include <stdint.h>
#include "serial.h"
#include "printk.h"
#include "delay.h"


void kernel_main(uint32_t r0, uint32_t r1, uint32_t *atags) {

	char input_string[256];
	int input_pointer;
	int ch;

	(void) r0;	/* Ignore boot method */

	/* Initialize Hardware */
	uart_init();

	/* Pause so we don't scroll away boot messages before */
	/* The terminal emulator is ready */
	printk("Press any key to continue...\r\n");
	uart_getc();

	/* Print boot message */
// YOUR CODE HERE

	/* Print hardware version */
// YOUR CODE HERE

	/* Loop forever, echoing what is typed */
	while (1) {
		uart_putc(uart_getc());
	}
}
