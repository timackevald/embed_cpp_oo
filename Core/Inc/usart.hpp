/*
 * usart.hpp
 *
 *  Created on: 18 May 2026
 *      Author: tim
 */

#ifndef USART2_HPP_
#define USART2_HPP_

#include "main.h"

static constexpr uint32_t USART2_CLK_ENABLE 		= 	(1U << 17);
static constexpr uint32_t GPIOA_CLK_ENABLE			=	(1U << 0);
static constexpr uint32_t CLR_PA2_PA3_TX_RX_MODE	=	~(0x7 << 4);
static constexpr uint32_t SET_PA2_PA3_TX_RX_MODE	=	(0xA << 4);
static constexpr uint32_t CLR_PA2_PA3_AFR_MODE		=	~(0xFF << 8);
static constexpr uint32_t SET_PA2_PA3_AFR_MODE		=	(0x77 << 8);
static constexpr uint32_t SET_CR1_TE_RE_UE			=	((1U << 13) | (1U << 3) | (1U << 2));
static constexpr uint32_t BAUD_RATE					=	115200U;

/* USART2 class */
class Usart2
{
	public:
		/* Constructor is empty
		 * call once before read or write
		 */
		Usart2() = default;

		static void Usart2_hw_init(void);

		static int Usart2_write(int ch);
		static int Usart2_read(void);
};

#endif /* USART2_HPP_ */
