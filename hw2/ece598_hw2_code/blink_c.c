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

// YOUR CODE HERE

}

/* main function */
int main(int argc, char **argv) {

	/* Point to the physical GPIO region */
	gpio = (uint32_t *)GPIO_BASE;

	/* Enable GPIO for the ACT LED by setting proper GPIO_GPFSEL */
	/* On the Model B this is 16, on the B+ this is 47 */

// YOUR CODE HERE

	while(1) {

		/* Write 1 to clear the GPIO */

// YOUR CODE HERE

		/* delay */

// YOUR CODE HERE

		/* Write 1 to set the GPIO */

// YOUR CODE HERE

		/* delay */
// YOUR CODE HERE

	}


	/* Hang forever; there is no OS to return to! */
	while(1) {
	}

	return 0;
}
