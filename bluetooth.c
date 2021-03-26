#include "bluetooth.h"

// list
static uint8_t bluetooth[SIZE_BLUETOOTH];
list listBluetooth = {"Bluetooth", bluetooth, SIZE_BLUETOOTH, 0, 0};

// master functions
static void bluetoothSendData(uint32_t);
static void bluetoothSendString(char *);

// slave functions
static void bluetoothSend(uint8_t src);
static char *toStringUint(uint32_t src);

// struct functions
const struct bluetoothFunctions Bluetooth = {
	
	{
		bluetoothSendData,
		bluetoothSendString
	}
	
};

// describe master functions
static void bluetoothSendData(uint32_t src){
	bluetoothSendString(toStringUint(src));	
}
static void bluetoothSendString(char *string){
	while(*string)
		bluetoothSend(*string++);
}

// describe slave functions
static void bluetoothSend(uint8_t src){
	List.Write.Ubyte(&listBluetooth, src);
}

static char *toStringUint(uint32_t src){
	static char string[16] = {[15] = 0};
	int32_t i = 15;
	do{
		string[--i] = '0' + src % 10;
		src /= 10;
	}	while(src);
	return &string[i];
	
}
