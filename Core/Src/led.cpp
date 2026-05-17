/*
 * led.c
 *
 *  Created on: 16 May 2026
 *      Author: tim
 */

//#include "main.h"
#include "led.hpp"

// Helper, returns correct bit shift for object
uint32_t Led::pin() const
{
	switch(color_)
	{
		case LedColor::Red:
		{
			return LED_RED_PIN;
		}
		case LedColor::Green:
		{
			return LED_GRN_PIN;
		}
		case LedColor::Yellow:
		{
			return LED_YLW_PIN;
		}
		case LedColor::Blue:
		{
			return LED_BLU_PIN;
		}
		default:
		{
			return 0U;
		}
	}
}

/* Led default constructor
 * Called by LedMembank to init array
 */
Led::Led()
	: color_(LedColor::Unknown)
	, state_(LedState::Unknown)
{
	// objects in array are now in a Unknown but known state
}

/* Led parameterised constructor
 * Called by LedMembank::add()
 */
Led::Led(LedColor color, LedState state)
	: color_(color)
	, state_(LedState::Unknown) // Until we call set_state()
{
	set_state(state); // Will call pin and set ODR
}

/* Set state will update internal state
 * Get the correct pin and set ODR to ON/OFF
 */
void Led::set_state(LedState state)
{
	state_ = state;

	// Get bit shift for object
	uint32_t led_pin = pin();

	if (led_pin == 0U)
	{
		// Do nothing, we're in a unknown state
		return;
	}

	if (state_ == LedState::On)
	{
		LED_PORT->ODR |= led_pin;
	}
	else
	{
		LED_PORT->ODR &= ~led_pin;
	}
}

/* Get LED state */
LedState Led::get_state() const
{
	return state_;
}

/* LED Membank constructor
 * slots_used_ initialised to 0 via initialiser list.
 * membank_[] array is default-constructed
 * color=Unknown, state=Unknown
 */
LedMembank::LedMembank()
	: slots_used_(0U)
{
	/* C++ will default-construct the array */
}

/* LED Membank add will construct proper LED into a free slot */
int LedMembank::add(LedColor color, LedState state)
{
	if (slots_used_ >= MAX_MEMBANK)
	{
		return -1;
	}

	membank_[slots_used_] = Led(color, state);
	slots_used_++;

	return 0;
}

/* Get address to specific obj at index */
Led *LedMembank::get(uint8_t idx)
{
	if (idx >= slots_used_)
	{
		return nullptr;
	}
	return &membank_[idx];
}

/* Set same state for all LED */
void LedMembank::set_all(LedState state)
{
	for (uint8_t i = 0; i < slots_used_; i++)
	{
		membank_[i].set_state(state);
	}
}

/* Start the hardware, called once from main
 * is static so has no this pointer and can't mess with members
 */
void LedMembank::hw_init()
{
    if (!(RCC->AHB1ENR & LED_PORT_CLK))
    {
        RCC->AHB1ENR |= LED_PORT_CLK;
    }

    /* Set PA5 to general purpose output mode (MODER = 01)
     * Clear first, then set */
    LED_PORT->MODER &= CLR_GPIOA_MODER5;
    LED_PORT->MODER |= SET_GPIOA_MODER5;
}



