#ifndef INIT_H
#define INIT_H

extern const struct initFunctions {
	void (*const Gpio)(void);
	void (*const Systick)(void);
	void (*const Dma)(void);
	void (*const Uart)(void);
	void (*const FlashMemory)(void);
	void (*const Rcc)(void);
} Init;

#endif
