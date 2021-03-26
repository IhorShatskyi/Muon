#ifndef LED_H
#define LED_H

#include "stm32f446xx.h"

#define LED_ON GPIOB->BSRR = GPIO_BSRR_BS_10
#define LED_OFF GPIOB->BSRR = GPIO_BSRR_BR_10
#define LED_TOOGLE GPIOB->ODR ^= GPIO_ODR_ODR_10

#endif
