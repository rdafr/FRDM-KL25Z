//PTB18 RED CATHODE
//PTB19 GREEN CATHODE
//PTD1 BLUE CATHODE

//CLOCK GATE CONTROL: SCGC5
//PORT DATA DIRECTION REGISTER: PDDR
//PORT DATA OUTPUT REGISTER: PDOR 
//PIN CONTROL REGISTER: PCR

#include "MKL25Z4.h"  // Device header

#define LED_RED 18
#define LED_GREEN 19
#define LED_BLUE 1

void delay_ms(int delay){  //DELAY FUNCTION JUST TO TEST BLINK IN FRDM KL25Z
	for(int i = 0; i < delay; i++){
		for(int j = 0; j < 4000; j++){
		}	
	}	
}	

int main()
{
	//ENABLE CLOCK TO PORT B AND D
	SIM_SCGC5 |= (1 << 10)|(1 << 12);
	
	//PORT CONFIGURATION AS GPIO
	PORTB_PCR18 = 0x100;
	PORTB_PCR19 = 0x100;
	PORTD_PCR1 = 0x100;
	
	//PORT CONFIGURATION  AS OUTPUT 	
	GPIOB_PDDR = (1 << LED_RED)|(1 << LED_GREEN);
	GPIOD_PDDR = (1 << LED_BLUE);
	
	//TOOGLE PINS USING PSOR AND PCOR REGISTERS
	while(1)
	{
		GPIOB_PCOR |= (1 << LED_RED)|(1 << LED_GREEN);
    GPIOD_PCOR |= (1 << LED_BLUE);
		delay_ms(500);
		GPIOB_PSOR |= (1 << LED_RED)|(1 << LED_GREEN);
    GPIOD_PSOR |= (1 << LED_BLUE);
		delay_ms(500);
	}
}	