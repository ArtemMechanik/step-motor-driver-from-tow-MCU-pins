/*
микроконтроллер ATtiny13A
тактовая частота 9,6 МГц
 */ 
#define F_CPU 9600000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//íàçíà÷åíèå âûâîäîâ
#define BUTTON1_PIN PB2
#define BUTTON2_PIN PB1
#define LED_PIN		PB0
#define KEY1_PIN	PB3
#define KEY2_PIN	PB4

#define LED_ON		PORTB |= (1<<LED_PIN)
#define LED_OFF		PORTB &=(~(1<<LED_PIN))

#define HZ		0
#define HIGH	1
#define LOW		2

#define FORWARD	0
#define BACK	1

#define DEADTIME	1000 //ìêÑ



void key1State (uint8_t state) {
	DDRB &=(~(1<<KEY1_PIN));
	PORTB &=(~(1<<KEY1_PIN));
	if(state == 1) {
		DDRB	|=(1<<KEY1_PIN);
		PORTB	|=(1<<KEY1_PIN);
	}
	else if(state == 2) {
		DDRB	|=(1<<KEY1_PIN);
		PORTB	&=(~(1<<KEY1_PIN));
	}
}

void key2State (uint8_t state) {
	DDRB &=(~(1<<KEY2_PIN));
	PORTB &=(~(1<<KEY2_PIN));
	if(state == 1) {
		DDRB	|=(1<<KEY2_PIN);
		PORTB	|=(1<<KEY2_PIN);
	}
	else if(state == 2) {
		DDRB	|=(1<<KEY2_PIN);
		PORTB	&=(~(1<<KEY2_PIN));
	}
}

void delayMs (unsigned int delay) {
	for(unsigned int i=0; i<delay; i++) {
		_delay_ms(1);
	}
}

void pinSetup() {
	DDRB = (1<<LED_PIN)|(0<<BUTTON1_PIN)|(0<<BUTTON2_PIN);
	PORTB |=(1<<BUTTON1_PIN)|(1<<BUTTON2_PIN);
	
	LED_OFF;
	key1State(HZ);
	key2State(HZ);
}

void motorStep (uint8_t direction, uint8_t step, uint16_t delay) {
	if(direction == FORWARD) {
		while(step > 0) {
			// обмотка A				// обмотка В
			key1State(HIGH);
			delayMs(delay/2);
								key2State(HZ);
								_delay_us(DEADTIME);
							
								key2State(HIGH);
								delayMs(delay/2);
		
			key1State(HZ);
			_delay_us(DEADTIME);
		
			key1State(LOW);
			delayMs(delay/2);
		
								key2State(HZ);
								_delay_us(DEADTIME);
							
								key2State(LOW);
								delayMs(delay/2);
		
			key1State(HZ);
			_delay_us(DEADTIME);
		
							
							
			
			step--;
		}
	}
	else if(direction == BACK) {
		while(step > 0) {
			// обмотка А				// обмотка В
			key2State(HIGH);
			delayMs(delay/2);
								key1State(HZ);
								_delay_us(DEADTIME);
			
								key1State(HIGH);
								delayMs(delay/2);
			
			key2State(HZ);
			_delay_us(DEADTIME);
			
			key2State(LOW);
			delayMs(delay/2);
			
								key1State(HZ);
								_delay_us(DEADTIME);
			
								key1State(LOW);
								delayMs(delay/2);
			
			key2State(HZ);
			_delay_us(DEADTIME);
			
			
			
			
			step--;
		}
	}
	_delay_ms(200);
}

int main(void) {
    pinSetup();
	
    while (1) {
		if(!(PINB&(1<<BUTTON1_PIN))) {
			LED_ON;
			//key1State(HIGH);
			motorStep(BACK,4,10);
		}
		else if(!(PINB&(1<<BUTTON2_PIN))) {
			LED_ON;
			//key1State(LOW);
			motorStep(FORWARD,4,10);
		}
		else {
			LED_OFF;
			key1State(HZ);
			key2State(HZ);
		}
    }
}

