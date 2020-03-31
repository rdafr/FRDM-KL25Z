#include "MKL25Z4.h"                    // Device header

#define LED_RED 18
#define LED_GREEN 19
#define LED_BLUE 1
#define INTERRUPT_1 1
#define INTERRUPT_2 2
#define INTERRUPT_3 4

void PORTA_IRQHandler(void);
void PORTD_IRQHandler(void);
void delaySystick_ms(int delay);
	
int main()
{
	__disable_irq(); //GLOBAL INTERRUPTS DISABLED
	
	
	//ENABLING CLOCK TO PORT(A, B, C AND D)	
	SIM->SCGC5 |= (1 << 12)|(1 << 10)|(1 << 9); 
	
	//ENABLING LED PORTS AS GPIOs
	PORTB->PCR[LED_RED] = 0x100;
	PORTB->PCR[LED_GREEN] = 0x100;
	PORTD->PCR[LED_BLUE] = 0x100;
			
  
	//MAKING LED PINS AS OUTPUTS
	GPIOB->PDDR |= (1 << LED_RED)|(1 << LED_GREEN);
	GPIOD->PDDR |= (1 << LED_BLUE);
	
	
	//MAKING INPUT PINS
	GPIOA->PDDR &= ~(1 << INTERRUPT_1)& ~(1 << INTERRUPT_2);
	GPIOD->PDDR &= ~(1 << INTERRUPT_3);
	

	//TURN LEDs OFF
	GPIOB->PSOR |= (1 << LED_RED)|(1 << LED_GREEN);
	GPIOD->PSOR |= (1 << LED_BLUE);
	
	//ENABLE INTERRUPT GPIO PIN
	PORTA->PCR[INTERRUPT_1] = 0x100; //(1 << 8)
	PORTA->PCR[INTERRUPT_1] |= (1 << PORT_PCR_PS_SHIFT)|(1 << PORT_PCR_PE_SHIFT); //ENABLING PULL-UP
	
	PORTA->PCR[INTERRUPT_2] = 0x100; //(1 << 8)
	PORTA->PCR[INTERRUPT_2] |= (1 << PORT_PCR_PS_SHIFT)|(1 << PORT_PCR_PE_SHIFT); //ENABLING PULL-UP
	
	PORTD->PCR[INTERRUPT_3] = 0x100; //(1 << 8) //ENABLING PULL-UP
	PORTD->PCR[INTERRUPT_3] |= (1 << PORT_PCR_PS_SHIFT)|(1 << PORT_PCR_PE_SHIFT); //ENABLING PULL-UP
	
	
	//INTERRUPT CONFIGURATION ON PCR
	PORTA->PCR[INTERRUPT_1] &= ~(1 << 16)&~(1 << 17)&~(1 << 18)&~(1 << 19); //CLEARING IRQC
	PORTA->PCR[INTERRUPT_1] |= (1 << 17)|(1 << 19); //INTERRUPT ON FALLING EGDE
	
	PORTA->PCR[INTERRUPT_2] &= ~(1 << 16)&~(1 << 17)&~(1 << 18)&~(1 << 19); //CLEARING IRQC
	PORTA->PCR[INTERRUPT_2] |= (1 << 17)|(1 << 19); //INTERRUPT ON FALLING EGDE
  
	PORTD->PCR[INTERRUPT_3] &= ~(1 << 16)&~(1 << 17)&~(1 << 18)&~(1 << 19); //CLEARING IRQC
	PORTD->PCR[INTERRUPT_3] |= (1 << 17)|(1 << 19); //INTERRUPT ON RISING EGDE
	
	
	//ENABLE INTERRUPTION BY PORTA AND PORTD DETECTION
	__NVIC_EnableIRQ(PORTA_IRQn);
	__NVIC_EnableIRQ(PORTD_IRQn);
	
	__enable_irq(); //GLOBAL INTERRUPT ENABLED
	
	while(1)
	{
		GPIOB->PTOR |= (1 << LED_RED); //TOGGLE RED LED
		delaySystick_ms(2000); 
	}	
}	

void PORTA_IRQHandler(void)
{
	if(PORTA->ISFR & (1 << INTERRUPT_1)) //COMPARE WITCH INTERRUPT HAS BEEN USED
	{
		GPIOB->PTOR |= (1 << LED_GREEN); //TOGGLE GREEN LED
	}
	
	else if(PORTA->ISFR & (1 << INTERRUPT_2)) //COMPARE WITCH INTERRUPT HAS BEEN USED
	{
		GPIOB->PTOR |= (1 << LED_RED); //TOGGLE RED LED
	}
	
	PORTA->ISFR |= (1 << INTERRUPT_1)|(1 << INTERRUPT_2); //CLEARING INTERRUPT FLAG REGISTER
}	

void PORTD_IRQHandler(void)
{
  GPIOD->PTOR |= (1 << LED_BLUE); //TOGGLE BLUE LED
	PORTD->ISFR |= (1 << INTERRUPT_3); //CLEARING INTERRUPT FLAG REGISTER
}	

void delaySystick_ms(int delay)
{
	SysTick->LOAD = (DEFAULT_SYSTEM_CLOCK/1000) - 1; //LOAD REFER TO 1mS
	SysTick->VAL = 0; //CLEARING TIMER VALUE
	SysTick->CTRL |= (1 << SysTick_CTRL_ENABLE_Pos)|(1 << SysTick_CTRL_CLKSOURCE_Pos); //ENABLE TIMER AND CLOCK SOURCE
	
	for(int i = 0; i < delay; i++)
	{
		while(((SysTick->CTRL) & (1 << SysTick_CTRL_COUNTFLAG_Pos)) == 0);//WAIT UNTILL THE TIMER END
	}	
	
	SysTick->CTRL = 0;
}	
