/*
 * adc1.h
 * for stm32F7
 * author: Nick Fan
 * date: 7/2023
 */


#include "adc1_f7.h"


void ADC1_Init(uint16_t *rxdata) {

	// Init ADC clock
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	// Init GPIOA (Pins 0, 1, 2, 3)

	// Analog mode, pull up/pull down disabled
	GPIOA->MODER |= (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER0_1
					| GPIO_MODER_MODER1_0 | GPIO_MODER_MODER1_1
					| GPIO_MODER_MODER2_0 | GPIO_MODER_MODER2_1
					| GPIO_MODER_MODER3_0 | GPIO_MODER_MODER3_1);

	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0_0 | GPIO_PUPDR_PUPDR0_1
					| GPIO_PUPDR_PUPDR1_0 | GPIO_PUPDR_PUPDR1_1
					| GPIO_PUPDR_PUPDR2_0 | GPIO_PUPDR_PUPDR2_1
					| GPIO_PUPDR_PUPDR3_0 | GPIO_PUPDR_PUPDR3_1);

	// Analog configurations

	// Set scan mode, set 15-bit resolution
	ADC1->CR1 |= ADC_CR1_SCAN;
	ADC1->CR1 &= ~ADC_CR1_RES;

	// Enabled DMA, set continuous mode, continuous DMA requests
	ADC1->CR2 |= ADC_CR2_DMA | ADC_CR2_CONT | ADC_CR2_DDS;

	// Set sample rate
	ADC1->SMPR2 |= (ADC_SMPR2_SMP0_0 | ADC_SMPR2_SMP1_0
					| ADC_SMPR2_SMP2_0 | ADC_SMPR2_SMP3_0);

	// Set sample sequence
	ADC1->SQR1 |= (4U << ADC_SQR1_L_Pos);
	ADC1->SQR3 |= ((0 << ADC_SQR3_SQ1_Pos) | (2 << ADC_SQR3_SQ2_Pos)
				| (1 << ADC_SQR3_SQ2_Pos) | (3 << ADC_SQR3_SQ2_Pos));

	// DMA setup, Channel 0, Periph/mem data size 16 bits, Circular mode enabled
	DMA2_Stream4->CR &= RESET_REG;
	DMA2_Stream4->CR |= (DMA_SxCR_MSIZE_0 | DMA_SxCR_PSIZE_0
						| DMA_SxCR_MINC | DMA_SxCR_CIRC);

	// Transfer size is 4 (4 channels)
	DMA2_Stream4->NDTR &= RESET_REG;
	DMA2_Stream4->NDTR |= 4U;

	// Peripheral address
	DMA2_Stream4->PAR |= (uint32_t) (&ADC1->DR);
	DMA2_Stream4-> M0AR |= (uint32_t) (rxdata);

	// Enable DMA
	DMA2_Stream4->CR |= DMA_SxCR_EN;

	// Enable ADC
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_SWSTART;


}

