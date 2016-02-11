#include <stddef.h>
#include <stdint.h>
#include "serial.h"
#include "printk.h"
#include "delay.h"

/* Values from the BCM2835-ARM-Peripherals.pdf manual */
#define GPIO_BASE	0x20200000

#define GPIO_GPFSEL0	0
#define GPIO_GPFSEL1	1
#define GPIO_GPFSEL2	2
#define GPIO_GPFSEL3	3
#define GPIO_GPFSEL4	4


#define GPIO_GPSET0	7
#define GPIO_GPSET1	8

#define GPIO_GPCLR0	10
#define GPIO_GPCLR1	11

#define KMAG  "\x1B[35m"

/* Reference to the GPIO space */
/* An array of 32-bit integers */
volatile uint32_t *gpio;



void kernel_main(uint32_t r0, uint32_t r1, uint32_t *atags) {
	/* Point to the physical GPIO region */
	gpio = (uint32_t *)GPIO_BASE;

	/* Enable GPIO for the ACT LED by setting proper GPIO_GPFSEL */
	/* On the Model B this is 16, on the B+ this is 47 */

// Write 1 to GPIO pin 47. make it an output
	gpio[GPIO_GPFSEL4] = 1<<21;

			/* Write 1 to clear the GPIO */

	// Write 1 to GPIO pin 47 to clear it. Turns on LED
		gpio[GPIO_GPSET1] = 1<<15;

			/* delay */

	// delay awhile
		delay(0xFFFFFF);

			/* Write 1 to set the GPIO */


	// Write 1 to GPIO 47 to set it. Turns off LED
		gpio[GPIO_GPCLR1] = 1<<15;

	char input_string[256];
	int input_pointer;
	int ch;

	(void) r0;	/* Ignore boot method */

	/* Initialize Hardware */
	uart_init();
	// 	// delay awhile
	// 	delay(0xFFFFFF);
	// // Write 1 to GPIO pin 47 to clear it. Turns on LED
	// 	gpio[GPIO_GPSET1] = 1<<15;

	// 		/* delay */

	// // delay awhile
	// 	delay(0xFFFFFF);

	// 		/* Write 1 to set the GPIO */


	// // Write 1 to GPIO 47 to set it. Turns off LED
	// 	gpio[GPIO_GPCLR1] = 1<<15;
	/* Pause so we don't scroll away boot messages before */
	/* The terminal emulator is ready */
	printk("Press any key to continue...\r\n");
	// gpio[GPIO_GPSET1] = 1<<15;	
	// delay(0xFFFFFF);
	uart_getc();
// gpio[GPIO_GPCLR1] = 1<<15;

	/* Print boot message */
	printk("+      o     +              o   \r\n    +             o     +       +\r\no          +\r\n    o  +           +        +\r\n+        o     o       +        o\r\n-_-_-_-_-_-_-_,------,      o \r\n_-_-_-_-_-_-_-|   /\\_/\\  \r\n-_-_-_-_-_-_-~|__( ^ .^)  +     +  \r\n_-_-_-_-_-_-_-''''  ''''      \r\n+      o         o   +       o\r\n    +         +\r\no        o         o      o     +\r\n    o           +\r\n+      +     o        o      +    \r\n\r\n", KMAG);

	/* Print hardware version */
// YOUR CODE HERE

	/* Loop forever, echoing what is typed */
	while (1) {
	// gpio[GPIO_GPSET1] = 1<<15;	
	// delay(0xFFFFFF);
		uart_putc(uart_getc());
		gpio[GPIO_GPCLR1] = 1<<15;
			// delay(0xFFFFFF);
	}
}
