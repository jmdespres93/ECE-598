#include <stddef.h>
#include <stdint.h>
#include "serial.h"
#include "printk.h"
#include "atags.h"
#include "led.h"
#include "delay.h"
#include "timer.h"
#include "interrupts.h"
#include "bcm2835_periph.h"
#include "mmio.h"
#include "hardware.h"
#include "shell.h"


/* default, this is over-ridden later */
int hardware_type=RPI_MODEL_B;

void kernel_main(uint32_t r0, uint32_t r1, uint32_t *atags) {

	int ch;
	struct atag_info_t atag_info;

	(void) r0;	/* Ignore boot method */

	/* Detect Hardware */
	atags_detect(atags,&atag_info);
	hardware_type=atag_info.hardware_type;

	/* Initialize Hardware */
	uart_init();
	led_init();
	timer_init();

	/* Enable Interrupts */
//	enable_interrupts();

	/* Delay to allow time for serial port to settle */
	/* So we can actually see the output on the terminal */

	printk("\nWaiting for serial port to be ready (press any key)\n");
	uart_getc();

// Your boot message here


	/* Print hardware version */
	printk("Hardware version: %x ",r1);
	if (r1==0xc42) printk("(Raspberry Pi)");
	else printk("(Unknown Hardware)");
	printk("\r\n");

	printk("Detected Model ");
	switch(hardware_type) {
		case RPI_MODEL_A:	printk("A"); break;
		case RPI_MODEL_APLUS:	printk("A+"); break;
		case RPI_MODEL_B:	printk("B"); break;
		case RPI_MODEL_BPLUS:	printk("B+"); break;
		case RPI_MODEL_B2:	printk("B2"); break;
		case RPI_COMPUTE_NODE:	printk("Compute Node"); break;
		default:		printk("Unknown %x",hardware_type); break;
	}
	printk("\r\n");

	/* Print ATAGS */
	atags_dump(atags);

	/* Enter our "shell" */
	shell();

}
