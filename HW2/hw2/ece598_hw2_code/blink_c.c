#include <stdint.h>

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

/* Reference to the GPIO space */
/* An array of 32-bit integers */
volatile uint32_t *gpio;

/* If we make this static, the C compiler will optimize it away */
void __attribute__ ((noinline)) delay(int length) {

	volatile int i;

//continue looping until i==0xFFFF - DELAY
	for(i=0;i<length;i++);

}

/* main function */
int main(int argc, char **argv) {

	/* Point to the physical GPIO region */
	gpio = (uint32_t *)GPIO_BASE;

	/* Enable GPIO for the ACT LED by setting proper GPIO_GPFSEL */
	/* On the Model B this is 16, on the B+ this is 47 */

// Write 1 to GPIO pin 47. make it an output
	gpio[GPIO_GPFSEL4] = 1<<21;

	while(1) {

			/* Write 1 to clear the GPIO */

	// Write 1 to GPIO pin 47 to clear it. Turns on LED
		gpio[GPIO_GPCLR1] = 1<<15;

			/* delay */

	// delay as long as possible (maybe) sometimes ints are more than 32 but we can roll with it.
		delay(0xFFFFFF);

			/* Write 1 to set the GPIO */


	// Write 1 to GPIO 47 to set it. Turns off LED
		gpio[GPIO_GPSET1] = 1<<15;

			/* delay */

	// delay as long as possible
		delay(0xFFFFFF);

	}


	/* Hang forever; there is no OS to return to! */
	while(1) {
	}

	return 0;
}
