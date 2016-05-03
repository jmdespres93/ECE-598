#include <stddef.h>
#include <stdint.h>
#include "bcm2835_periph.h"
#include "nyan.h"
#include "song.h"
/** created from binary via objcopy */
extern uint8_t Sample[]      asm("_binary_Sample_bin_start");
extern uint8_t Sample_size[] asm("_binary_Sample_bin_size");
extern uint8_t Sample_end[]  asm("_binary_Sample_bin_end");

void nyan_init(void) {
	//load GPIO base
	uint32_t * gpio = (uint32_t *)GPIO_BASE;
	//Set GPIO 45 & 40 to ALT0
	gpio[GPIO_GPFSEL4] = (0b100<<(3*5))|(0b100<<3*0);

	//load PWM base
	uint32_t * pwm = (uint32_t *)PWM_BASE;
	//set both channels to to 8bit 44100Hz Mono
	pwm[PWM_RNG1] = 0x1B4;
	pwm[PWM_RNG2] = 0x1B4;
	pwm[PWM_CTL]  = PWM_USEF2 + PWM_PWEN2 + PWM_USEF1 + PWM_PWEN1 + PWM_CLRF1;

}
void nyan_sing(void) {
	//load PWM base
	uint32_t * pwm  = (uint32_t *)PWM_BASE;
	// uint8_t  * psong = song;
	uint8_t * psong = Sample;
	// uint8_t  * psong_end = &song[1539490];
	uint8_t * psong_end = Sample_end;
	//Check if at end of song
	while(psong!=psong_end) {
		//load byte into PWM FIF1
		pwm[PWM_FIF1] = *psong;
		while(pwm[PWM_STA] && PWM_FULL1);
		//move to next byte
		psong++;
	}
}
void nyan_dance(void);
