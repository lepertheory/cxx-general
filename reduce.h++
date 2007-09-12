/*****************************************************************************
 * reduce.h++
 *****************************************************************************
 * Reduce a fraction.
 *****************************************************************************/

// System includes.
#include <gcd.h++>

// Include guard.
#if !defined(REDUCE_ykbf73k3ub834b)
	#define REDUCE_ykbf73k3ub834b

// Contain in namepsace.
namespace DAC {
	
	/***************************************************************************/
	// Functions.
	
	// Reduce a fraction.
	template <class T> void reduce (T& p, T& q);
	
	/***************************************************************************
	 * Inline and template definitions.
	 ***************************************************************************/
	
	/***************************************************************************/
	// Functions.
	
	/*
	 * Reduce a given fraction.
	 */
	template <class T> void reduce (T& p, T& q) {
		
		// Get the greatest common divisor.
		T tmp = gcd(abs(p), abs(q));
		
		// Reduce.
		p /= tmp;
		q /= tmp;
		
	}
	
}

#endif
