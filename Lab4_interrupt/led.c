#include "led.h"

static uint8_t BIT_0 = (uint8_t)BIT0;
static uint8_t BIT_1 = (uint8_t)BIT1;
static uint8_t BIT_2 = (uint8_t)BIT2;
static uint8_t BIT_4 = (uint8_t)BIT4;
static uint8_t interrupt = 0x00;			 
//FALSE = RED LED IS ON... TRUE = RGB LED IS ON.
static _Bool lastPressed = false;

static uint8_t nextColourState = 1;
static uint8_t currentColourState;
	

void PORT1_IRQHandler(void){
	if((P1->IFG & BIT_1) != 0){
		interrupt = BIT_1;
	}
	else if((P1->IFG & BIT_4) != 0)
	{
		interrupt = BIT_4;
	}
	
	P1->IFG &= ~BIT_1;
	P1->IFG &= ~BIT_4;
	
}



void ledInitialize(void){
	//SET RED & RGB LED PINS AS GPIO
	P1->SEL0 &= ~BIT_0;
	P1->SEL1 &= ~BIT_0;
	
  P2->SEL0 &= ~(BIT_0 | BIT_1 | BIT_2);
	P2->SEL1 &= ~(BIT_0 | BIT_1 | BIT_2);
	
	//SET PINS AS OUTPUT
	P1->DIR |= BIT_0;
	P2->DIR |= BIT_0 | BIT_1 | BIT_2;

	//SET PINS AS LOW DRIVE STRENGTH
	P1->DS &= ~BIT_0;
	P2->DS &= ~(BIT_0 | BIT_1 | BIT_2);
	
	//CLEAR PINS FROM ANY PREVIOUS VALUES
	P1->OUT &= ~BIT_0;
	P2->OUT &= ~(BIT_0 | BIT_1 | BIT_2);
	
	//SET RED LED ON BY DEFAULT
	P1->OUT ^= BIT_0;
	currentColourState = nextColourState;
}

void switchInitialize(void){
	//SET SWITCH1 AND SWITCH2 PINS AS GPIO
	P1->SEL0 &= ~(BIT_1 | BIT_4);
	P1->SEL1 &= ~(BIT_1 | BIT_4);
	
	//SET PINS AS INPUT
	P1->DIR &= ~(BIT_1 | BIT_4);
	
	//ENABLE SWITCH INTERNAL RESISTORS
	P1->REN |= BIT_1 | BIT_4;
	//SET RESISTORS AS PULL UP RESISTORS
	P1->OUT |= BIT_1 | BIT_4;
}

//INTERRUPTS ONLY NEEDED FOR BUTTONS.
void p1InterruptInitialize(void){
	   //PORT CONFIGURATION //(uint_8)0x02  BIT_1 | BIT_4
	//SET INTERRUPT TO OCCUR ON FALLING EDGE/ACTIVE LOW FOR SWITCHES
	P1->IES |= BIT_1 ;
	P1->IES |= BIT_4 ;
	//CLEAR PENDING INTERRUPT FLAGS FOR PINS
	P1->IFG &= ~BIT_1;
	P1->IFG &= ~BIT_4;
	//ENABLE INTERRUPTS FOR PINS
	P1->IE |= BIT_1;
	P1->IE |= BIT_4;
	
	//NVIC CONFIGURATION
	
	//SET PRIORITY OF INTERRUPT AT 2
	NVIC_SetPriority(PORT1_IRQn, 2);
	//CLEAR ANY PENDING INTERRUPTS
	NVIC_ClearPendingIRQ(PORT1_IRQn);
	//ENABLE INTERRUPTS IN INTERRUPT CONTROLLER
	NVIC_EnableIRQ(PORT1_IRQn);
	
		//ENABLE INTERRUPTS AT PROCESSOR LEVEL
	__enable_irq();
}

void b1ActionPerformed(void){
	//CHECK IF BUTTON 1 IS PRESSED --> SWITCH BETWEEN LEDS
				
				if(lastPressed){
					P2->OUT &= ~(BIT_0 | BIT_1 | BIT_2);
					P1->OUT ^= BIT_0;
					lastPressed = false;
				}
				else{
					P1->OUT ^= BIT_0;
					P2->OUT &= ~(BIT_0 | BIT_1 | BIT_2);
					P2->OUT |= currentColourState;
					lastPressed = true;
			}
	
	
}
//BUTTON 2 CODE
void b2ActionPerformed(void){
			 if(!lastPressed){
				 P1->OUT ^= BIT_0;
				 lastPressed = false;
			 }
			 else{			 
					 P2->OUT &= ~(BIT_0 | BIT_1 | BIT_2);
					 P2->OUT |= nextColourState;
					 currentColourState = nextColourState;
					 nextColourState = (nextColourState + 1) % 8;
					 lastPressed = true;
				 
			 }
}
//CODE TO RUN
void doStuff(void){

		//IF BUTTON ONE CAUSED INTERRUPT
				if((interrupt & BIT_1) != 0){
				b1ActionPerformed();
			  interrupt = 0x00;
	    }
				
			//IF BUTTON TWO CAUSED INTERRUPT
				if((interrupt & BIT_4) != 0)
		 {
   		 b2ActionPerformed();
			 interrupt = 0x00;
		 }	
		
	
}







