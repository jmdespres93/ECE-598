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


void kernel_main(uint32_t r0, uint32_t r1, uint32_t *atags,
		uint32_t memory_kernel) {

	int ch;
	struct atag_info_t atag_info;
	unsigned int memory_total;
	int framebuffer_width=1024,framebuffer_height=768;

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

	/* Enable Framebuffer */
	framebuffer_init(framebuffer_width,framebuffer_height,24);
	framebuffer_console_init();

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

	/* Setup hardware for performance */
	enable_l1_icache();
	enable_branch_predictor();
	enable_mmu(0,memory_total);
	enable_l1_dcache();

	asm volatile(
		// 		; Raspberry Pi 'Bare Metal' Sound 8Bit Mono 44100Hz CPU Demo by krom (Peter Lemon):
		// ; 1. Set 3.5" Phone Jack To PWM 
		// ; 2. Setup PWM Sound Buffer
		// ; 3. Play Sound Sample Using CPU & FIFO

		// format binary as 'img'
		"include 'sound/LIB/FASMARM.INC'\n"
		"include 'sound/LIB/R_PI.INC'\n"

		"org $0000\n"

		// ; Set GPIO 40 & 45 (Phone Jack) To Alternate PWM Function 0
		"imm32 r0,PERIPHERAL_BASE + GPIO_BASE\n"
		"imm32 r1,GPIO_FSEL0_ALT0 + GPIO_FSEL5_ALT0\n"
		"str r1,[r0,GPIO_GPFSEL4]\n"

		// ; Set Clock
		"imm32 r0,PERIPHERAL_BASE + CM_BASE\n"
		"imm32 r1,CM_PASSWORD + $2000 ; Bits 0..11 Fractional Part Of Divisor = 0, Bits 12..23 Integer Part Of Divisor = 2\n"
		"str r1,[r0,CM_PWMDIV]\n"

		"imm32 r1,CM_PASSWORD + CM_ENAB + CM_SRC_OSCILLATOR ; Use Default 100MHz Clock\n"
		"str r1,[r0,CM_PWMCTL]\n"

		// ; Set PWM
		"imm32 r0,PERIPHERAL_BASE + PWM_BASE\n"
		"imm32 r1,$1B4 ; Range = 8bit 44100Hz Mono\n"
		"str r1,[r0,PWM_RNG1]\n"
		"str r1,[r0,PWM_RNG2]\n"

		"imm32 r1,PWM_USEF2 + PWM_PWEN2 + PWM_USEF1 + PWM_PWEN1 + PWM_CLRF1\n"
		"str r1,[r0,PWM_CTL]\n"

		"Loop:\n"
		  "imm32 r1,SND_Sample ; R1 = Sound Sample\n"
		  "imm32 r2,SND_SampleEOF ; R2 = End Of Sound Sample\n"
		  "FIFO_Write:\n"
		    "ldrb r3,[r1],1\n" //; Write 1 Byte To FIFO
		    "str r3,[r0,PWM_FIF1]\n" //; FIFO Address
		    "FIFO_Wait:\n"
		      "ldr r3,[r0,PWM_STA]\n"
		      "tst r3,PWM_FULL1 \n"//; Test Bit 1 FIFO Full
		      "bne FIFO_Wait\n"
		    "cmp r1,r2\n" //; Check End Of Sound Sample
		    "bne FIFO_Write\n"

		  "b Loop\n" //; Play Sample Again

		"SND_Sample:\n" //; 8bit 44100Hz Unsigned Mono Sound Sample
		  "file 'sound/Sample.bin'\n"
		  "SND_SampleEOF:\n"
		);
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
