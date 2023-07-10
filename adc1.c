/*
 * adc1.c
 * author: Nick Fan
 * date: 7/2023
 * 
 */

#include "adc1.h"

void Joystick_ADC_Init(uint16_t *rxdata) {

    /* Setup clock */

    // Enable ADC Clock
    RCC->APBENR2 |= RCC_APBENR2_ADCEN;
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // Enable GPIO port clock
    RCC->IOPENR |= (RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN);

    /* Pin setup: ADC1 IN2, IN3, IN8, IN9 - PA2, PA3, PB0, PB1 */

    // Analog mode
    GPIOA->MODER |= (GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
    GPIOB->MODER |= (GPIO_MODER_MODE0 | GPIO_MODER_MODE1);

    // Pull up/pull down resistors disabled
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD2_Msk | GPIO_PUPDR_PUPD3_Msk);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD0_Msk | GPIO_PUPDR_PUPD1_Msk);

    /* Analog Configurations */

    // Config reg 1: Continuous mode enabled, DMA circular mode, CHSEL mode
    ADC1->CFGR1 &= REG_RESET;

    ADC1->CFGR1 |= (ADC_CFGR1_CONT | ADC_CFGR1_DMACFG | ADC_CFGR1_CHSELRMOD);

    while (!(ADC1->ISR & ADC_ISR_CCRDY));
    ADC1->ISR |= ADC_ISR_CCRDY;

    // CHSELR 2, 3, 8, 9 when mode = 1
    ADC1->CHSELR |= (ADC_CHSELR_CH_2 << ADC_CHSELR_SQ1_Pos) 
                    | (ADC_CHSELR_CH_3 << ADC_CHSELR_SQ2_Pos)
                    | (ADC_CHSELR_CH_8 << ADC_CHSELR_SQ3_Pos)
                    | (ADC_CHSELR_CH_9 << ADC_CHSELR_SQ4_Pos)
                    | (ADC_CHSELR_END << ADC_CHSELR_SQ5_Pos);
    
    while (!(ADC1->ISR & ADC_ISR_CCRDY));
    ADC1->ISR |= ADC_ISR_CCRDY;

    // Set sampling rate
    ADC1->SMPR &= ~(ADC_SMPR_SMPSEL2 | ADC_SMPR_SMPSEL3 
                | ADC_SMPR_SMPSEL8 | ADC_SMPR_SMPSEL9);

    ADC1->SMPR |= ADC_SMPR_MAX << ADC_SMPR_SMP1_Pos;
    ADC1->SMPR |= ADC_SMPR_MAX << ADC_SMPR_SMP2_Pos;

    ADC1->CFGR2 &= REG_RESET;
    ADC1->CFGR2 |= ADC_CFGR2_CKMODE_0; // Synchronous pclk/2

    /* DMA configuration */

    // DMA
    DMA1_Channel1->CCR &= REG_RESET;

    // Set memory size, peripheral size, memory increment enable, circular mode enable
    DMA1_Channel1->CCR |= (DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0
                        | DMA_CCR_MINC | DMA_CCR_CIRC); // 16 bit memory size, 12 bit adc value

    // Set transfer size 
    DMA1_Channel1->CNDTR |= JOYSTICK_TRANSFER_SIZE;
    DMA1_Channel1->CPAR = (uint32_t) (&(ADC1->DR));
    DMA1_Channel1->CMAR = (uint32_t) (rxdata);

    // DMAMUX
    DMAMUX1_Channel0->CCR |= (
        DMAMUX_DMAREQ_ID_ADC << DMAMUX_CxCR_DMAREQ_ID_Pos
    );

    // Set ADVREGEN 
    ADC1->CR &= REG_RESET;
    ADC1->CR |= (ADC_CR_ADVREGEN);

    // Wait 20 us minimum for vreg enable
    for (uint16_t i = 0; i < 400; i++);

    // Calibrate ADC
    ADC1->CR |= ADC_CR_ADCAL;

    // Wait for ADC calibration to complete
    while ((ADC1->CR & ADC_CR_ADCAL) || (!(ADC1->ISR & ADC_ISR_EOCAL)));

    // DMA Enable
    ADC1->CFGR1 |= ADC_CFGR1_DMAEN;
    DMA1_Channel1->CCR |= DMA_CCR_EN;

    // Enable and start ADC
    ADC1->ISR |= ADC_ISR_ADRDY;
    ADC1->CR |= ADC_CR_ADEN;

    // Wait for ADC enable to complete
    while (!(ADC1->ISR & ADC_ISR_ADRDY));

    ADC1->CR |= ADC_CR_ADSTART;

}

void Wait_Transfer(void) {
    while(!(DMA1->ISR & DMA_ISR_TCIF1));
    DMA1->IFCR |= DMA_ISR_TCIF1;
}


