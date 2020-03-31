//TOOGLE LEDS FROM KL25Z IN 200 mS

#include "MKL25Z4.h"                    // Device header

#define LED_RED 18
#define LED_GREEN 19
#define LED_BLUE 1

#define F_CPU 41840000

int cont = 0;

void configSystick_ms(int delay);

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
	
	//CONFIGURING SYSTICK 
	configSystick_ms(500);
	//SysTick->LOAD = 8380000 - 1; //SYSTEM CLOCK IS 41.84 MHZ, COUNTING TIL 200 mS
	//SysTick->CTRL |= (1 << SysTick_CTRL_ENABLE_Pos)|(1 << SysTick_CTRL_CLKSOURCE_Pos); //ENABLING SYSTEM CLOCK, CLKSOURCE AND SYSTICK ENABLE
   
	while(1)
	{
		if((SysTick->CTRL)&(1 << SysTick_CTRL_COUNTFLAG_Pos))
		{ 
			//GPIOB->PTOR = (1 << LED_RED); //TOGGLE RED LED
			
			if (cont > 2) cont = 0;
			else if(cont == 0) GPIOB->PDOR ^= (1 << LED_RED);
			else if (cont == 1) GPIOB->PDOR ^= (1 << LED_GREEN);
			else if (cont == 2) GPIOD->PDOR ^= (1 << LED_BLUE);
			cont++;	
		}
	}	
}	

//FUNCTION O CONFIGURE SYSTICK
void configSystick_ms(int delay)
{
	int tickMS = (F_CPU * delay)/1000;
	SysTick->LOAD = tickMS - 1; //SYSTEM CLOCK IS 41.84 MHZ, COUNTING TIL 200 mS
	SysTick->CTRL |= (1 << SysTick_CTRL_ENABLE_Pos)|(1 << SysTick_CTRL_CLKSOURCE_Pos); //ENABLING SYSTEM CLOCK, CLKSOURCE AND SYSTICK ENABLE
}	