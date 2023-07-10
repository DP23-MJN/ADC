/*
 * adc1.h
 * author: Nick Fan
 * date: 7/2023
 */

#ifndef ADC1_H
#define ADC1_H

#include "stm32g0xx.h"

#define REG_RESET 0UL
#define ADC_SMPR_MAX 0x3
#define ADC_SMPR_7_5 0x2
#define JOYSTICK_TRANSFER_SIZE 4
#define DMAMUX_DMAREQ_ID_ADC 0x5


#define ADC_CHSELR_CH_2 0x2
#define ADC_CHSELR_CH_3 0x3
#define ADC_CHSELR_CH_8 0x8
#define ADC_CHSELR_CH_9 0x9
#define ADC_CHSELR_END 0xF

void Joystick_ADC_Init(uint16_t *rxdata);
void Wait_Transfer(void);

#endif ADC1_H
