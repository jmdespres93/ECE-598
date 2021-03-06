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
#include "mmu.h"
#include "nyan.h"

#define KMAG  "\x1B[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* default, this is over-ridden later */
int hardware_type=RPI_MODEL_B;


void kernel_main(uint32_t r0, uint32_t r1, uint32_t *atags,
		uint32_t memory_kernel) {
	printk("enter kernel\n");
	int ch;
	struct atag_info_t atag_info;
	unsigned int memory_total;
	int framebuffer_width=1024,framebuffer_height=768;

	(void) r0;	/* Ignore boot method */

	/* Detect Hardware */
	atags_detect(atags,&atag_info);
	hardware_type=atag_info.hardware_type;
	printk("detect atags\n");
	/* Initialize Hardware */
	uart_init();
	led_init();
	timer_init();
	printk("Initialize uart, led, and timer\n");
	/* Enable Interrupts */
	enable_interrupts();
	printk("enable interrupts\n");
	/* Enable Framebuffer */
	// framebuffer_init(framebuffer_width,framebuffer_height,24);
	// framebuffer_console_init();

	printk("\nWaiting for serial port to be ready (press any key)\n");
	uart_getc();

	/* Print hardware version */
	printk("Hardware version: %x ",r1);
	if (r1==0xc42) printk("(Raspberry Pi)");
	else printk("(Unknown Hardware)");
	printk("\n");

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
	printk("\n");

	/* Print ATAGS */
	atags_dump(atags);

	/* Get amount of RAM from ATAGs */
	memory_total=atag_info.ramsize;

	/* Init memory subsystem */
	memory_init(memory_total,memory_kernel);
	printk("Memory Initialized \n");
	/* Setup hardware for performance */
	enable_l1_icache();
	printk("Cache Initialized \n");
	enable_branch_predictor();
	printk("Branch Predictor Initialized \n");
	enable_mmu(0,memory_total);	
	printk("MMU Initialized \n");
	enable_l1_dcache();
	printk("DCache Initialized \n");


	//sing for me you sweet little poptart kitty
	nyan_init();
	printk("Nyan Initialized \n");
		/* Print boot message */
//Nyan cat in magenta (make sure color is on)
	printk(KMAG "+      o     +              o   \r\n    +             o     +       +\r\no          +\r\n    o  +           +        +\r\n+        o     o       +        o\r\n-_-_-_-_-_-_-_,------,      o \r\n_-_-_-_-_-_-_-|   /\\_/\\  \r\n-_-_-_-_-_-_-~|__( ^ .^)  +     +  \r\n");
	printk("-_-_-_-_-_-_-_\"\" \"\"      \r\n+      o         o   +       o\r\n    +         +\r\no        o         o      o     +\r\n    o           +\r\n+      +     o        o      +    \r\n\r\n" ANSI_COLOR_RESET);
	printk("%x %x %x", 16, 128, 65535);

	nyan_sing();
	printk("Song Completed \n");

	/* switch to userspace and enter our shell */

	printk("\nEntering userspace\n");

	/* Setup user stack */

        asm volatile(
                "msr CPSR_c, #0xDF\n" /* System mode, like user but privldg */
                "mov sp, #0x00002000\n"
                "msr CPSR_c, #0xD3\n" /* Back to Supervisor mode */
                : /* output */
		: /* input */
                : "sp", "memory");      /* clobbers */


	/* Switch to userspace */

asm volatile(
                "mov r0, #0x10\n"
                "msr SPSR, r0\n"
		"mov lr, %[shell]\n"
                "movs pc,lr\n"
                : /* output */
		: [shell] "r"(shell) /* input */
                : "r0", "lr", "memory");        /* clobbers */


}
