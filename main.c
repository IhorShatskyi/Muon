#include "init.h"
#include "delay.h"
#include "led.h"
#include "print.h"
#include "dmaTransfer.h"
#include "welcome.h"
#include "bluetooth.h"

int main(void){
	
	// Welcome
	//Print(Welcome);
	
	Init.Gpio();
	Init.Systick();
	Init.Uart();
	Init.Dma();
	Init.FlashMemory();
	Init.Rcc();
	
	while(1){
	
		Print(Welcome);
		
		Bluetooth.Send.Srting("Test: Slava BOBRAM!");
		Bluetooth.Send.Srting("\r\n");
		
		// Status LED
		LED_TOOGLE;
		
		// Debug out
		DmaTransfer.Uart1(&listPrint);
		
		// Bluetooth out
		DmaTransfer.Uart3(&listBluetooth);
		
		
		Delay.MilliSecond(500.f);
		
		
	}
	
}

