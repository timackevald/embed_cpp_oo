/*
 * usart.cpp
 *
 *  Created on: 18 May 2026
 *      Author: tim
 */

#include "usart.hpp"

void Usart2::Usart2_hw_init()
{
	RCC->APB1ENR 	|= USART2_CLK_ENABLE;
	RCC->APB1ENR 	|= GPIOA_CLK_ENABLE;
	GPIOA->MODER 	&= CLR_PA2_PA3_TX_RX_MODE;
	GPIOA->MODER 	|= SET_PA2_PA3_TX_RX_MODE;
	GPIOA->AFR[0]	&= CLR_PA2_PA3_AFR_MODE;
	GPIOA->AFR[0]	|= SET_PA2_PA3_AFR_MODE;
	/*
	 * Adding half the divisor before dividing
	 * to get round-to-nearest instead of round-down.
	 */
	uint32_t pclk1 	= HAL_RCC_GetPCLK1Freq();
	USART2->BRR		= (pclk1 + (BAUD_RATE / 2)) / BAUD_RATE;
	USART2->CR1		= SET_CR1_TE_RE_UE;
}

int Usart2::Usart2_write(int ch)
{
    while (!(USART2->SR & (1U << 7))) { }
    USART2->DR = (ch & 0xFF);
    return ch;
}

int Usart2::Usart2_read()
{
    while (!(USART2->SR & (1U << 5))) { }
    return USART2->DR;
}

// Diverts IO to Usart read and write
// hooked in newlib and cannot be part of class
extern "C" int __io_putchar(int ch)
{
    return Usart2::Usart2_write(ch);
}

extern "C" int __io_getchar(void)
{
    return Usart2::Usart2_read();
}
