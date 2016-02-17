CROSS = arm-none-eabi-
CC = gcc
AS = as
ASFLAGS = -mcpu=arm1176jzf-s
CFLAGS = -O2 -mcpu=arm1176jzf-s -marm -nostartfiles -ffreestanding -nostdlib
LFLAGS = --no-undefined -T $(LINKER_SCRIPT)

LINKER_SCRIPT = kernel.ld

all:	kernel.img kernel.dis

kernel.img:	kernel.elf
	$(CROSS)objcopy kernel.elf -O binary kernel.img



kernel.elf:	kernel_main.o \
		atags.o boot.o interrupts.o \
		console_io.o gpio.o led.o printk.o \
		serial.o shell.o string.o timer.o
	$(CROSS)ld $(LFLAGS) \
		kernel_main.o atags.o boot.o \
		interrupts.o console_io.o gpio.o led.o printk.o \
		serial.o shell.o string.o timer.o \
		-Map kernel.map -o kernel.elf


kernel_main.o:	kernel_main.c
	$(CROSS)$(CC) $(CFLAGS) -o kernel_main.o -c kernel_main.c


atags.o:	atags.c atags.h
	$(CROSS)$(CC) $(CFLAGS) -o atags.o -c atags.c

console_io.o:	console_io.c console_io.h
	$(CROSS)$(CC) $(CFLAGS) -o console_io.o -c console_io.c

gpio.o:	gpio.c gpio.h
	$(CROSS)$(CC) $(CFLAGS) -o gpio.o -c gpio.c

interrupts.o:	interrupts.c
	$(CROSS)$(CC) $(CFLAGS) -o interrupts.o -c interrupts.c

led.o:	led.c led.h
	$(CROSS)$(CC) $(CFLAGS) -o led.o -c led.c

printk.o:	printk.c printk.h
	$(CROSS)$(CC) $(CFLAGS) -o printk.o -c printk.c

serial.o:	serial.c bcm2835_periph.h serial.h
	$(CROSS)$(CC) $(CFLAGS) -o serial.o -c serial.c

shell.o:	shell.c bcm2835_periph.h shell.h
	$(CROSS)$(CC) $(CFLAGS) -o shell.o -c shell.c

string.o:	string.c string.h
	$(CROSS)$(CC) $(CFLAGS) -o string.o -c string.c

syscalls.o:	syscalls.c
	$(CROSS)$(CC) $(CFLAGS) -o syscalls.o -c syscalls.c

timer.o:	timer.c timer.h
	$(CROSS)$(CC) $(CFLAGS) -o timer.o -c timer.c

boot.o:	boot.s
	$(CROSS)as $(ASFLAGS) -o boot.o boot.s




kernel.dis:	kernel.elf
	$(CROSS)objdump --disassemble-all kernel.elf > kernel.dis

clean:
	rm -f *~ *.o *.map *.elf kernel.img *.dis


submit:
	tar -czvf hw4_submit.tar.gz *.c *.h *.s *.ld Makefile

