#include <stdint.h>
#include "bcm2835_periph.h"
#include "mmio.h"
#include "led.h"

/* Interrupt handler */

void __attribute__((interrupt("IRQ"))) interrupt_handler(void) {


	/* Check that it was in fact a timer interrupt */

// YOUR CODE HERE

	/* Clear the ARM Timer interrupt		*/

// YOUR CODE HERE


	/* Toggle the LED state */
	/* You can use the provided led_on() and led_off() functions */

// YOUR CODE HERE

}

