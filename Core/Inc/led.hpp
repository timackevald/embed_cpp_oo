/*
 * led.h
 *
 *  Created on: 16 May 2026
 *      Author: tim
 */

#ifndef 	LED_HPP_
#define		LED_HPP_

#include	"main.h"
#include	<stdint.h>

#define 	LED_PORT	GPIOA
/* Hardware definitions */
static constexpr uint8_t	MAX_MEMBANK		 =  4U;
static constexpr uint32_t	LED_PORT_CLK	 =  (1U << 0); // Not real
static constexpr uint32_t	LED_RED_PIN		 =  (1U << 4); // Not real
static constexpr uint32_t	LED_GRN_PIN		 =  (1U << 5); // Only real LED on board
static constexpr uint32_t	LED_YLW_PIN		 =  (1U << 6); // Not real
static constexpr uint32_t	LED_BLU_PIN		 =  (1U << 7); // Not real
static constexpr uint32_t	CLR_GPIOA_MODER5 = ~(0x3 << 10);
static constexpr uint32_t	SET_GPIOA_MODER5 =  (0x1 << 10);

/* Scopes enums, LeadColor::Red for access */
enum class LedColor : uint8_t
{
	Red 	= 0,
	Green 	= 1,
	Yellow 	= 2,
	Blue   	= 3,
	Unknown	= 4
};

enum class LedState : uint8_t
{
	Off		= 0,
	On		= 1,
	Unknown = 2
};

/* LED class */
class Led
{
	private:
		LedColor color_;
		LedState state_;
		// Private helper
		uint32_t pin() const;

	public:
		/* Default constructor, sentinel state
		 * Sets color_ and state_ to Unknown
		 * Only used by LedMemBank to init it's array
		 */
		Led();

		/* Parameterised constructor
		 * Sets color_ and state_ via initialiser list.
		 * Gets called by LedMemBank::add()
		 */
		Led(LedColor color, LedState state);

		/* API */
		void set_state(LedState state);
		LedState get_state() const;
};

/* LED Memory bank class */
class LedMembank
{
	private:
		Led 		membank_[MAX_MEMBANK];
		uint8_t 	slots_used_;

	public:
		/* Default constructor
		 * Initialises slots_used_ to zero
		 * membank_[] is default-constructed via Led
		 */
		LedMembank();

		/* API */
		int add(LedColor color, LedState state);

		/* Returns address to obj or nullptr */
		Led *get(uint8_t idx);

		void set_all(LedState state);

		/* No 'this' pointer, can't access members */
		static void hw_init();
};

#endif 		/* LED_HPP_ */
