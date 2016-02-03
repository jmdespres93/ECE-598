CROSS = arm-none-eabi-
CC = gcc
AS = as
LD = ld
ASFLAGS = -mcpu=arm1176jzf-s
CFLAGS = -O2 -mcpu=arm1176jzf-s -nostartfiles


LINKER_SCRIPT = kernel.ld

all:	blink_c.img blink_c.dis blink_asm.img blink_asm.dis


# Assembly files

blink_asm.img:	blink_asm.elf
	$(CROSS)objcopy blink_asm.elf -O binary blink_asm.img

blink_asm.elf:	blink_asm.o
	$(CROSS)ld --no-undefined blink_asm.o -Map blink_asm.map -o blink_asm.elf -T $(LINKER_SCRIPT)

blink_asm.o:	blink_asm.s
	$(CROSS)as $(ASFLAGS) -o blink_asm.o blink_asm.s

blink_asm.dis:	blink_asm.elf
	$(CROSS)objdump --disassemble-all blink_asm.elf > blink_asm.dis


# C

blink_c.img:	blink_c.elf
	$(CROSS)objcopy blink_c.elf -O binary blink_c.img

blink_c.elf:	blink_c.o boot.o
	$(CROSS)ld --no-undefined blink_c.o boot.o -Map blink_c.map -o blink_c.elf -T $(LINKER_SCRIPT)

blink_c.o:	blink_c.c
	$(CROSS)$(CC) $(CFLAGS) -o blink_c.o blink_c.c

boot.o:	boot.s
	$(CROSS)as $(ASFLAGS) -o boot.o boot.s

blink_c.dis:	blink_c.elf
	$(CROSS)objdump --disassemble-all blink_c.elf > blink_c.dis

clean:
	rm -f *~ *.o *.map *.elf *.img *.dis

submit:
	tar -czvf hw2_submit.tar.gz *.c *.s *.ld Makefile README



