/*****************************************************************************
 * is_powerOf2.h++
 *****************************************************************************
 * Quick test if a number is a power of 2.
 *****************************************************************************/

// Include guard.
#if !defined IS_POWEROF2_ONIHCOIONQBUHTN
	#define IS_POWEROF2_ONIHCOIONQBUHTN

// Namespace.
namespace DAC {
	
	/***************************************************************************/
	// Functions.
	
	// Quickly test for a power of 2.
	template <class T> bool is_powerOf2 (T const number);
	
	/***************************************************************************
	 * Inline and template definitions.
	 ***************************************************************************/
	
	/***************************************************************************/
	// Functions.
	
	/*
	 * Quickly test for a power of 2.
	 */
	template <class T> bool is_powerOf2 (T const number) {
		
		return number > 0 && (number & (number - 1)) == 0;
		
	}
	
}

// End include guard.
#endif
