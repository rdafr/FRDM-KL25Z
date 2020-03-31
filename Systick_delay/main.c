#include "MKL25Z4.h"         	 //Header file 

#define LED_RED 18
#define LED_GREEN 19
#define LED_BLUE 1

#define DEFAULT_DELAY 300

void delaySystick_ms(uint32_t delay);

int main()
{
	//CONFIGURING CLOCK ON GPIO
	SIM_SCGC5 |= (1 << 10)|(1 << 12);
	
	//CONFIGURING PORT AS GPIO
	PORTB_PCR18 = 0x100;
	PORTB_PCR19 = 0x100;
	PORTD_PCR1 = 0x100;
	
	//DATA DIRECTION CONFIGURATION
	GPIOB_PDDR |= (1 << LED_RED)|(1 << LED_GREEN);
	GPIOD_PDDR |= (1 << LED_BLUE);
	
	//INITIALIZE OUTPUT AS HIGH LEVEL, WITCH TURNS THE LED OFF
	GPIOB_PDOR |= (1 << LED_RED)|(1 << LED_GREEN);
	GPIOD_PDOR |= (1 << LED_BLUE);
	
	while(1)
	{
	  //if((SysTick->CTRL)&(1 << SysTick_CTRL_COUNTFLAG_Pos)) GPIOB->PDOR ^= (1 << LED_RED);
		delaySystick_ms(DEFAULT_DELAY);
		GPIOB->PDOR ^= (1 << LED_GREEN); //TOGGLE GREEN LED
		delaySystick_ms(DEFAULT_DELAY);
		GPIOB->PDOR ^= (1 << LED_RED); //TOGGLE RED LED
		delaySystick_ms(DEFAULT_DELAY);
		GPIOD->PDOR ^= (1 << LED_BLUE); //TOGGLE BLUE LED
		delaySystick_ms(DEFAULT_DELAY);
		GPIOB->PDOR ^= (1 << LED_RED); //TOGGLE RED LED
		delaySystick_ms(DEFAULT_DELAY);
		GPIOB->PDOR ^= (1 << LED_GREEN); //TOGGLE GREEN LED		
	}	
}
//
//FUNCTION O CONFIGURE SYSTICK
void delaySystick_ms(uint32_t delay)
{
	SysTick->LOAD = (DEFAULT_SYSTEM_CLOCK/1000) - 1; //TAKES THE DEFAULT CLOCK AND DIVIDE TO GET THE NUMBER OF CYCLES PER MILISECOND
	SysTick->VAL = 0; //MAKING SYSTICK INITIAL VALUE AS 0
	SysTick->CTRL |= (1 << SysTick_CTRL_ENABLE_Pos)|(1 << SysTick_CTRL_CLKSOURCE_Pos); //ENABLING SYSTEM CLOCK, CLKSOURCE AND SYSTICK ENABLE
	for(int i = 0; i < delay; i++)
	{
		while((SysTick->CTRL & (1 << SysTick_CTRL_COUNTFLAG_Pos)) == 0);
	}	
	SysTick->CTRL = 0; //DISABE SYSTICK
}	