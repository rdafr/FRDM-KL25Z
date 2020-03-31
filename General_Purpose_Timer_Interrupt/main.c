//General Timer Interrupt Example

#include "MKL25Z4.h"                    // Device header

#define LED_RED 18
#define LED_GREEN 19
#define LED_BLUE 1 

volatile int cont = 0, tickCont = 0;

void TPM0_IRQHandler(void);
void SysTick_Handler(void);

int main(void)
{
	__disable_irq(); //DISABLE GLOBAL INTERRUPTS
	
	//ENABLING CLOCK ON PORTB AND PORTD
	SIM->SCGC5 |= (1 << 10)|(1 << 12);
	
	//ENABLE TPM0 CLOCK
	SIM->SCGC6 |= (1 << 24);
	
	//ENABLE FLL CLOCK ON TPM0
	SIM->SOPT2 |= (1 << 24);
	
	//ENABLE LED PINS AS GPIOs
	PORTB->PCR[LED_RED] = 0x100;
	PORTB->PCR[LED_GREEN] = 0x100;
	PORTD->PCR[LED_BLUE] = 0x100;
	
  //MAKING LED PINS DIRECTION AS OUTPUTs
	GPIOB->PDDR |= (1 << LED_RED)|(1 << LED_GREEN);
	GPIOD->PDDR |= (1 << LED_BLUE);
	
	//TURN LEDS OFF
	GPIOB->PSOR |= (1 << LED_RED)|(1 << LED_GREEN);
	GPIOD->PSOR |= (1 << LED_BLUE);
	
	//CONFIGURING TPM0
	TPM0->SC = 0;
	TPM0->SC |= TPM_SC_PS(3); //SET 128 PRESCALE
	TPM0->SC |= TPM_SC_CMOD(1); //INCREMENT ON EVERY LPTPM COUNTER CLK
	TPM0->SC |= (1 << TPM_SC_TOIE_SHIFT); //ACTIVATE OVERFLOW INTERRUPT
	TPM0->MOD = 0xffff; //PUT THE MAXIMUM VALUE ON TIMER MODULO
	TPM0->SC |= (1 << TPM_SC_TOF_SHIFT); //CLEAR TIMER OVERFLOW FLAG
	
	//SYSTICK CONFIGURATION, IF ITS BEEN USED
	SysTick->LOAD = (DEFAULT_SYSTEM_CLOCK/1000) - 1;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL = 0;
	
	//CONFUGURING NVIC INTERRUPT
	__NVIC_EnableIRQ(TPM0_IRQn);

	__enable_irq(); //ENABLE GLOBAL INTERRUPTS
	
	while(1);
}	

void TPM0_IRQHandler(void)
{
	cont++;
	if(cont == 20) GPIOB->PTOR |= (1 << LED_GREEN);
	else if(cont == 40) 
	{
		GPIOD->PTOR |= (1 << LED_BLUE);
		cont = 0;
	}	
	TPM0->SC |= (1 << TPM_SC_TOF_SHIFT); //CLEAR TIMER OVERFLOW FLAG
}

void SysTick_Handler(void)
{
	tickCont++;
	
	if(tickCont == 10000)
	{
		GPIOB->PTOR |= (1 << LED_RED);
		tickCont = 0;
	}	
}
