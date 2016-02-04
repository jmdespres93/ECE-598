/*
 * @author: Jasmine Despres
 * @name: blink_asm
 * @date: Feb 3, 2016
 * @description: This program blinks the status? LED on a raspberry pi B+
 *******ECE 598 Homework 2*******
 */



/* Barebones code to Blink LED on Raspberry Pi board */

/* Defines from the BCM2835 ARM Peripherals Document */
.equ GPIO_BASE,		0x20200000

.equ GPIO_GPFSEL0,	0x00
.equ GPIO_GPFSEL1,	0x04
.equ GPIO_GPFSEL2,	0x08
.equ GPIO_GPFSEL3,	0x0c
.equ GPIO_GPFSEL4,	0x10

.equ GPIO_GPSET0,	0x1c
.equ GPIO_GPSET1,	0x20

.equ GPIO_GPCLR0,	0x28
.equ GPIO_GPCLR1,	0x2C

.equ DELAY,         0xFF

.globl _start
_start:

blinky:
	/* Load the GPIO region into r3 */
	ldr	r3,=GPIO_BASE

	/* The ACT LED is hooked up to GPIO16 on a B, GPIO47 on B+/A+ */

	/* Write to a GPFSEL resgister to enable GPIO16 or GPIO47 */
	/* Write 1 to the three-bit field to enable as an output  */

/* shift 3*7 bits over to access the 7th element in the GPFSEL4 register */
/* (each zero represents 3 bits) */
/* GPFSEL4 = 0001000000 */
    mov r4, #1
    lsl r4, #21
    str r4, [r3, #GPIO_GPFSEL4]

	/* Write 1 to the proper bit of a GPIO_GPCLR register */
	/* which will turn off that GPIO pin.                 */

/* clear the 47th bit in GPCLR to turn the LED on*/
/* GPCLR0 = 00000000000000000000000000000000 */
/* GPCLR1 = 00000000000000000100000000000000 */


    mov r4, #1
    lsl r4, #15
    str r4, [r3, #GPIO_GPSET1]

	/* delay */

/* decrement r5 until zero flag is set in order to delay */
    mov r5, #DELAY
delay_on1:
    subs r5, r5, #1
    bne delay_on1

	/* Write 1 to the proper bit of a GPIO_GPSET register	*/
	/* which will turn on that GPIO pin.                    */
    
/* set the 47th bit in GPSET to turn the LED off*/
/* GPSET0 = 00000000000000000000000000000000 */
/* GPSET1 = 00000000000000000100000000000000 */
    str r4, [r3, #GPIO_GPCLR1]

/* decrement r5 until zero flag is set in order to delay */
    mov r5, #DELAY
delay_off1:
    subs r5, r5, #1
    bne delay_on1

/*blink foevah */    
    b blinky

finished:
	b	finished

