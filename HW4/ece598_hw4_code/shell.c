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


uint32_t shell(void) {

	uint32_t ch;

	/* Enter our "shell" */

	while (1) {

		while(1) {
			ch=uart_getc();
			uart_putc(ch);
		}
	}
}
