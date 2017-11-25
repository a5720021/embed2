/*
* LED 
* Embedded Homework 2
*/
#include "msp.h"
#define SYSTICK_STCSR (*((volatile unsigned long *)0xE000E010))
#define SYSTICK_STRVR (*((volatile unsigned long *)0xE000E014))
#define SYSTICK_STCVR (*((volatile unsigned long *)0xE000E018))

void SysTick_Init(void);
void SysTick_wait(uint32_t delay);
void SysTick_wait1ms(uint32_t delay);
void Port_Init(void);
void led_rgb(uint32_t delay);
void led_bgr(uint32_t delay);
void switch_1_4(void);

volatile uint8_t flag = 1;					//State of switch initail at RGB

int main(void) {
	SysTick_Init();
	Port_Init();
	while (1) {
		switch_1_4();										//Switch Condition
		if (flag == 1){									//push switch 1 
			led_rgb(1000);
			//P2OUT = 0x00;
		}else if (flag == 2){
			led_bgr(1000);
			//P2OUT = 0x00;
		}
	}
}

//Set Initial SysTick Config
void SysTick_Init (void) {
	SYSTICK_STCSR = 0;
	SYSTICK_STRVR = 0x00FFFFFF;
	SYSTICK_STCVR = 0;
	SYSTICK_STCSR = 0x00000005;
}

//Function use Systick 1/3M 
void SysTick_wait(uint32_t delay){
	SYSTICK_STRVR = delay - 1;
	SYSTICK_STCVR = 0;
	while ((SYSTICK_STCSR & 0x00010000) == 0){
	}
}

void SysTick_wait1ms(uint32_t delay){
	uint32_t i;
	for (i = 0;i < delay;i++){
		SysTick_wait(3000);
	}
}

void Port_Init(void) {
	P1SEL1 = 0x00; // config P1.0 to P1.7 as simple I/O
	P1SEL0 = 0x00;
	P2SEL1 = 0x00; // config P2.0 to P2.7 as simple I/O
	P2SEL0 = 0x00;
	P1DIR = 0x00; // make port P1 are input
	P1REN = 0x12; // set pull-up resister at P1.4 and P1.1
	P1OUT = 0x12; // set pull-up resister at P1.4 and P1.1
	P2DIR = 0x07; // make port P2 at P2.2, P2.1 and P2.0 are output
	P2OUT = 0x00; // set output logic low at at P2.2, P2.1 and P2.0
}

//Led RGB run with delay 1 second
void led_rgb(uint32_t delay){
	P2OUT = 0x01;
	SysTick_wait1ms(delay);
	P2OUT = 0x02;
	SysTick_wait1ms(delay);
	P2OUT = 0x04;
	SysTick_wait1ms(delay);
}

//Led BGR run with delay 1 second
void led_bgr(uint32_t delay){
	P2OUT = 0x04;
	SysTick_wait1ms(delay);
	P2OUT = 0x02;
	SysTick_wait1ms(delay);
	P2OUT = 0x01;
	SysTick_wait1ms(delay);
}

//Function check switch if push SW1 led rgb() if push SW4 led bgr()
void switch_1_4(void){
	uint8_t in_sw1_1,in_sw1_4;
	in_sw1_1 = P1IN & 0x02;
	in_sw1_4 = P1IN & 0x10;
	if(!in_sw1_1){
		flag = 1;
	}else if(!in_sw1_4){
		flag = 2;
	}
}