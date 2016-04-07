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

/* default, this is over-ridden later */
int hardware_type=RPI_MODEL_B;


#define BENCH_SIZE (1024*1024)
#define BENCH_ITERATIONS 16
uint8_t benchmark[BENCH_SIZE];

void run_memory_benchmark(void) {

	int i;
	uint32_t before,after;

	before=ticks_since_boot();

	for(i=0;i<BENCH_ITERATIONS;i++) {
		memset(benchmark,0,BENCH_SIZE);
	}

	after=ticks_since_boot();

	printk("\tMEMSPEED: %d MB took %d ticks\n",
		BENCH_SIZE*BENCH_ITERATIONS,
		(after-before));
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t *atags,
		uint32_t memory_kernel) {

	int ch;
	struct atag_info_t atag_info;
	unsigned int memory_total;

	(void) r0;	/* Ignore boot method */

	/* Detect Hardware */
	atags_detect(atags,&atag_info);
	hardware_type=atag_info.hardware_type;

	/* Initialize Hardware */
	uart_init();
	led_init();
	timer_init();

	/* Enable Interrupts */
	enable_interrupts();

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

	/* Run some memory benchmarks */
	printk("\nRunning Memory benchmarks %x\n",benchmark);

	printk("Default memory:\n");
	run_memory_benchmark();

/* YOUR CODE HERE */

	//Enable L1 instruction cache
	enable_l1_icache();
	printk("\nL1 cache enabled\n");
	run_memory_benchmark();

	//Enable branch predictor
	enable_branch_predictor();
	printk("\nBranch predictor enabled\n");
	run_memory_benchmark();

	//Enable Virtual Memory and D1 cache
	enable_mmu(0, memory_total);
	enable_l1_dcache();
	printk("\nVirtual memory and dcache enabled\n");
	run_memory_benchmark();

/* End your code here */


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
