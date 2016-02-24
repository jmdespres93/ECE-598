#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "serial.h"

int console_write(const void *buf, size_t count) {

	/* We only support UART for now */
	uart_write(buf, count);

}

int console_read(void *buf, size_t count) {

        int i;
        char *buffer=buf;

        /* We only support UART for now */

        for(i=0;i<count;i++) {
                buffer[i]=uart_getc_noblock();
                if (buffer[i]<=0) break;
        }

        return i;

}

