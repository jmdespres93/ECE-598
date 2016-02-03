
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


.globl _start
_start:

	/* Load the GPIO region into r0 */
	ldr	r0,=GPIO_BASE

	/* The ACT LED is hooked up to GPIO16 on a B, GPIO47 on B+/A+ */

	/* Write to a GPFSEL resgister to enable GPIO16 or GPIO47 */
	/* Write 1 to the three-bit field to enable as an output  */

// YOUR CODE HERE

	/* Write 1 to the proper bit of a GPIO_GPCLR register */
	/* which will turn off that GPIO pin.                 */

// YOUR CODE HERE

	/* delay */

// YOUR CODE HERE

	/* Write 1 to the proper bit of a GPIO_GPSET register	*/
	/* which will turn on that GPIO pin.                    */

// YOUR CODE HERE

	/* delay again */

// YOUR CODE HERE

finished:
	wfe			/* wait for event */
				/* suspend CPU, low-power */
	b	finished

