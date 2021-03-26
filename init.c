#include "init.h"
#include "stm32f446xx.h"
#include "define.h"

// CLOCK
#define APB1_CLK CPU_CLK/4
#define APB2_CLK CPU_CLK/2
#define APB2_CLK_TIM 2*APB2_CLK // таймери APB2


// Master Functions
static void Gpio(void);
static void Systick(void);
static void Dma(void);
static void Uart(void);
static void FlashMemory(void);
static void Rcc(void);

// Master struct
const struct initFunctions Init = {
	Gpio,
	Systick,
	Dma,
	Uart,
	FlashMemory,
	Rcc
};

// Describe master functions
static void Gpio(void) {

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Port B
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Port C
	
	// USART1 - Debug
	// PB6 - TX	
	GPIOB->MODER |= 0x2 << GPIO_MODER_MODE6_Pos; // Alternate function mode
	GPIOB->OTYPER |= 0x0 << GPIO_OTYPER_OT6_Pos; // Push-pul
	GPIOB->OSPEEDR |= 0x3 << GPIO_OSPEEDR_OSPEED6_Pos; // Very high speed
	GPIOB->PUPDR |= 0x2 << GPIO_PUPDR_PUPD6_Pos; // Pull-down
	GPIOB->AFR[0] |= 0x7 << GPIO_AFRL_AFSEL6_Pos; // AF7
	// PB7 - RX	
	GPIOB->MODER |= 0x2 << GPIO_MODER_MODE7_Pos; // Alternate function mode
	GPIOB->OTYPER |= 0x0 << GPIO_OTYPER_OT7_Pos; // Push-pul
	GPIOB->OSPEEDR |= 0x3 << GPIO_OSPEEDR_OSPEED7_Pos; // Very high speed
	GPIOB->PUPDR |= 0x2 << GPIO_PUPDR_PUPD7_Pos; // Pull-down
	GPIOB->AFR[0]  |= 0x7 << GPIO_AFRL_AFSEL7_Pos; // AF7
	
	// LED PB10
	GPIOB->MODER |= 0x1U << GPIO_MODER_MODE10_Pos; // Output mode
	GPIOB->OTYPER |= 0x0U << GPIO_OTYPER_OT10_Pos; // Push-pul
	GPIOB->OSPEEDR |= 0x3U << GPIO_OSPEEDR_OSPEED10_Pos; // Very high speed
	GPIOB->PUPDR |= 0x2U << GPIO_PUPDR_PUPD10_Pos; // Pull-down
	
	// USART3 - Bluetooth
	// PC10 - TX	
	GPIOC->MODER |= 0x2 << GPIO_MODER_MODE10_Pos; // Alternate function mode
	GPIOC->OTYPER |= 0x0 << GPIO_OTYPER_OT10_Pos; // Push-pul
	GPIOC->OSPEEDR |= 0x3 << GPIO_OSPEEDR_OSPEED10_Pos; // Very high speed
	GPIOC->PUPDR |= 0x2 << GPIO_PUPDR_PUPD10_Pos; // Pull-down
	GPIOC->AFR[1] |= 0x7 << GPIO_AFRH_AFSEL10_Pos; // AF7
	// PC11 - RX	
	GPIOC->MODER |= 0x2 << GPIO_MODER_MODE11_Pos; // Alternate function mode
	GPIOC->OTYPER |= 0x0 << GPIO_OTYPER_OT11_Pos; // Push-pul
	GPIOC->OSPEEDR |= 0x3 << GPIO_OSPEEDR_OSPEED11_Pos; // Very high speed
	GPIOC->PUPDR |= 0x2 << GPIO_PUPDR_PUPD11_Pos; // Pull-down
	GPIOC->AFR[1]  |= 0x7 << GPIO_AFRH_AFSEL11_Pos; // AF7	
	
	
	
}

static void Systick(void){
	SysTick->LOAD = 0xFFFFFFFFU;
	SysTick->VAL = 0xFFFFFFFFU;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;  
}

static void Uart(){
	
	// USART 1 - Debug
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // Clock	
	USART1->BRR = (uint32_t) (APB2_CLK/BR_UART1); // Baud rate
	USART1->CR1 |= USART_CR1_TE; // Transmitter enable
	USART1->CR3 |= USART_CR3_DMAT; // DMA enable transmitter
	USART1->CR1 |= USART_CR1_UE; // USART enable 	

	// USART 3 - Bluetooth
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // Clock
	USART3->BRR = (uint32_t) (APB1_CLK/BR_UART3); // Baud rate
	USART3->CR1 |= USART_CR1_TE; // Transmitter enable
	USART3->CR1 |= USART_CR1_RE; // Receiver enable
	USART3->CR1 |= USART_CR1_RXNEIE; // RXNE interrupt enable
	USART3->CR3 |= USART_CR3_DMAT; // DMA enable transmitter
	USART3->CR1 |= USART_CR1_UE; // USART enable
	
}

static void FlashMemory(void){
	FLASH->ACR |= FLASH_ACR_LATENCY_5WS;
	FLASH->ACR |= FLASH_ACR_PRFTEN;	// Prefetch enable
	FLASH->ACR |= FLASH_ACR_ICEN; // Instruction cache enable
	FLASH->ACR |= FLASH_ACR_DCEN;	// Data cache enable
}

static void Rcc(void){
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC; // Main PLL(PLL) and audio PLL (PLLI2S) entry clock source
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM; // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
	RCC->PLLCFGR |= 10 << RCC_PLLCFGR_PLLM_Pos;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN; // Main PLL (PLL) multiplication factor for VCO
	RCC->PLLCFGR |= 256 << RCC_PLLCFGR_PLLN_Pos;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP; // Main PLL (PLL) division factor for main system clock 
	RCC->PLLCFGR |= 0x0 << RCC_PLLCFGR_PLLP_Pos;	
	RCC->CR |= RCC_CR_PLLON; // Main PLL (PLL) enable
	while(!(RCC->CR & RCC_CR_PLLRDY)); // Main PLL (PLL) clock ready flag
	RCC->CFGR &= ~RCC_CFGR_PPRE1; // APB Low speed prescaler (APB1)
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
	RCC->CFGR &= ~RCC_CFGR_PPRE2; // APB high-speed prescaler (APB2)
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
	RCC->CFGR &= ~RCC_CFGR_SW; // System clock switch
	RCC->CFGR |= 0x2 << RCC_CFGR_SW_Pos;
}


static void Dma(void){

	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;	

	// Bluetooth
	DMA1_Stream3->PAR = (int)&(USART3->DR);
	DMA1_Stream3->CR |= 1 << DMA_SxCR_MINC_Pos;
	DMA1_Stream3->CR |= 1 << DMA_SxCR_DIR_Pos;
	DMA1_Stream3->CR |= 4 << DMA_SxCR_CHSEL_Pos;
	
	// Debug
	DMA2_Stream7->PAR = (int)&(USART1->DR);
	DMA2_Stream7->CR |=  1 << DMA_SxCR_MINC_Pos;
	DMA2_Stream7->CR |=  1 << DMA_SxCR_DIR_Pos ;
	DMA2_Stream7->CR |=  4 << DMA_SxCR_CHSEL_Pos;
	
}

