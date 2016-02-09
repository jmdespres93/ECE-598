#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "serial.h"

int console_write(const void *buf, size_t count) {

	/* We only support UART for now */
	uart_write(buf, count);

}
