#include <stdint.h>
#include "bcm2835_periph.h"
#include "mmio.h"
#include "led.h"

/* Interrupt handler */

void __attribute__((interrupt("IRQ"))) interrupt_handler(void) {
 	static int lit = 0;

	/* Check that it was in fact a timer interrupt */
	if (mmio_read(IRQ_BASIC_PENDING)&&0x1) {
		
		/* Clear the ARM Timer interrupt		*/
		mmio_write(TIMER_IRQ_CLEAR, 0x1);

		/* Toggle the LED state */
		if(lit) {
			led_off();
			lit = 0;
		} else {
			led_on();
			lit = 1;
		}
	} else printk("Other Interrupt \r\n");
}

