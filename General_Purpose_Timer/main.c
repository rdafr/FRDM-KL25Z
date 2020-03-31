//TIMER/PWM MODULE: TPM
//TPMx: TPM0 (24), TPM1 (25), TPM2 (26)
//TIMER SYSTEM CLOCK GATE: SCGC6
//TIMER CLOCK SELECT: SOPT2
//16-bit COUNTER: TPMx_CNT
//MODULO VALUE: TPMx_MOD
//TIMER OVERFLOW FLAG: TOF

#include "MKL25Z4.h"                    // Device header

//DEFINE LED PINS
#define LED_RED 18
#define LED_GREEN 19
#define LED_BLUE 1

int main()
{
	//ENABLING GPIO CLOCK SYSTEM
	SIM->SCGC5 |= (1 << 10)|(1 << 12);
	
	//ENABLING TIMER CLOCK SYSTEM IN TMP0
	SIM->SCGC6 |= (1 << 24);
	SIM->SOPT2 |= (1 << 24); //SELECT MCGFLLCLK AS TIMER COUNTER CLK
	
	//CONFIG. LED PORTS AS GPIOs
	PORTB->PCR[LED_RED] = 0x100;
	PORTB->PCR[LED_GREEN] = 0x100;
	PORTD->PCR[LED_BLUE] = 0X100;
	
	//CONFIG. LED PORTS AS OUTPUTS
	GPIOB->PDDR |= (1 << LED_RED)|(1 << LED_GREEN);
	GPIOD->PDDR |= (1 << LED_BLUE);
	
	GPIOB->PSOR |= (1 << LED_RED)|(1 << LED_GREEN);
	GPIOD->PSOR |= (1 << LED_BLUE);
	
	//CONFIGURING TPM
	TPM0->SC = 0;
	TPM0->SC |= (1 << 7)|(1 << 3)|(1 << 2)|(1 << 1)|(1 << 0); //CLEARING TOF, ENABLING TIMER ON CMOD AND DIVIDE CLK BY 8 (PREESCALER)
	TPM0->MOD = 0xffff; //TIMER OVERFLOW AT FREQUENCY CLOCK/65536 = 640 Hz 
	
	while(1)
	{
		//for(int i = 0; i < 300; i++)
		//{
			while((TPM0->SC & (1 << 7)) == 0){}
			TPM0->SC |= (1 << 7); //CLEARING TOF
		  TPM0->SC |= (1 << 1)|(1 << 0);
		//}
		GPIOB->PTOR |= (1 << LED_RED);//|(1 << LED_GREEN);	//TOGGLE BOTH LEDS
	}		
}