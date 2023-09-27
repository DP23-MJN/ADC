# STM32 DMA ADC

This driver is written to configure the STM32G0 for continuous conversion on multiple channels using the internal ADC. 
To use, copy the header file ``adc1.h`` to your ``inc`` folder and ``adc1.c`` to your ``src`` folder.

Then, use ``#include "adc1.h"`` to use it in your main program.
The function ``Joystick_ADC_Init`` (the function name can be changed) will automatically complete all necessary initializations, so calling it once will suffice.

Originally written for joystick ADC testing for RC controller.
