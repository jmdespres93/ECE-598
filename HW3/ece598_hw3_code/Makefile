CROSS = arm-none-eabi-
CC = gcc
AS = as
ASFLAGS = -mcpu=arm1176jzf-s
CFLAGS = -O2 -mcpu=arm1176jzf-s -nostartfiles -ffreestanding -nostdlib 


LINKER_SCRIPT = kernel.ld

all:	kernel.img kernel.dis

kernel.img:	kernel.elf
	$(CROSS)objcopy kernel.elf -O binary kernel.img



kernel.elf:	kernel_main.o \
		boot.o console_io.o printk.o serial.o
	$(CROSS)ld --no-undefined \
		kernel_main.o boot.o console_io.o printk.o serial.o \
		-Map serial.map -o kernel.elf -T $(LINKER_SCRIPT)


kernel_main.o:	kernel_main.c
	$(CROSS)$(CC) $(CFLAGS) -o kernel_main.o -c kernel_main.c

console_io.o:	console_io.c
	$(CROSS)$(CC) $(CFLAGS) -o console_io.o -c console_io.c

printk.o:	printk.c
	$(CROSS)$(CC) $(CFLAGS) -o printk.o -c printk.c

serial.o:	serial.c bcm2835_periph.h
	$(CROSS)$(CC) $(CFLAGS) -o serial.o -c serial.c

boot.o:	boot.s
	$(CROSS)as $(ASFLAGS) -o boot.o boot.s




kernel.dis:	kernel.elf
	$(CROSS)objdump --disassemble-all kernel.elf > kernel.dis

clean:
	rm -f *~ *.o *.map *.elf kernel.img *.dis


submit:
	tar -czvf hw3_submit.tar.gz *.c *.h *.s *.ld Makefile README

