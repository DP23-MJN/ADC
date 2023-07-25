/*
 * adc1.h
 * for stm32F7
 * author: Nick Fan
 * date: 7/2023
 */

#ifndef ADC1_H
#define ADC1_H

#include "stm32f7xx.h"

#define RESET_REG			0x00UL

void ADC1_Init(uint16_t *rxdata);


#endif // ADC1_H
