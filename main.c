#include "led.h"


int main(void)
{

	//DISABLE WATCHDOG TIMER
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

	
	ledInitialize();
	switchInitialize();
	p1InterruptInitialize();

	//PROGRAM LOOP
	while(1){
	
		//PUT PROCESSOR IN LOW POWER MODE WHILE WAITING FOR INTERRUPTS
    __WFI();
	  doStuff();
		
  }
}


