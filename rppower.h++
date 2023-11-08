/*****************************************************************************
 * rppower.h++
 *****************************************************************************
 * Russian Peasant Algorithm method of finding a power. Why isn't this in the
 * Standard Library? I don't want floating point.
 *****************************************************************************/

// Include guard.
#if !defined(RPPOWER_gu2j9g8uuhu)
	#define RPPOWER_gu2j9g8uuhu

// Standard includes.
	#include <limits>

// Internal includes.
	#include "Exception.h++"

// Contain in namespace.
namespace DAC {
	
	/***************************************************************************/
	// Errors.
	namespace RPPower {
		namespace Errors {
			
			// rppower error base.
			class Base : public Exception { public: virtual char const* what () const throw() { return "Undefined exception in rppower()."; }; };
			
			// Cannot raise zero to the zeroth power.
			class ZeroZeroth : public Base { public: virtual char const* what () const throw() { return "Attempted to raise zero to the zeroth power."; }; };
			
			// Overflow.
			class Overflow : public Base { public: virtual char const* what () const throw() { return "Operation overflows data type."; }; };
			
		}
	}
	
	/***************************************************************************/
	// Functions.
	
	// Raise a number to a power.
	template <class BaseT, class ExpT> BaseT rppower (BaseT const x, ExpT const y);
	
	/***************************************************************************/
	// Utilities.
	namespace RPPowerUtil {
		
		// Raise a number to a power.
		template <class BaseT, class ExpT> BaseT rppower_positive (BaseT const x, ExpT const y);
		template <class BaseT, class ExpT> BaseT rppower_negative (BaseT const x, ExpT const y);
		
		// Choose multiplication or division.
		template <class BaseT, class ExpT, bool exp_is_signed> class RPPower;
		template <class BaseT, class ExpT> class RPPower<BaseT, ExpT, false> { public: static BaseT op (BaseT const x, ExpT const y); };
		template <class BaseT, class ExpT> class RPPower<BaseT, ExpT, true > { public: static BaseT op (BaseT const x, ExpT const y); };
		
		// Call the correct check for multiplication overflow.
		template <class T> bool mul_overflows (T const x, T const y);
		
		// Check a multiplication for overflow.
		template <class T, bool is_signed> class Mul_Overflows;
		template <class T> class Mul_Overflows<T, false> { public: static bool op (T const x, T const y); };
		template <class T> class Mul_Overflows<T, true > { public: static bool op (T const x, T const y); };
		
	}
	
	/***************************************************************************
	 * Inline and template definitions.
	 ***************************************************************************/
	
	/***************************************************************************/
	// Functions.
	
	/*
	 * Raise a number to a power.
	 */
	template <class BaseT, class ExpT> inline BaseT rppower (BaseT const x, ExpT const y) {
		if (x == 0 && y == 0) {
			throw RPPower::Errors::ZeroZeroth();
		}
		if (x == 0) {
			return static_cast<BaseT>(0);
		}
		if (y == 0) {
			return static_cast<BaseT>(1);
		}
		return RPPowerUtil::RPPower<BaseT, ExpT, std::numeric_limits<ExpT>::is_signed>::op(x, y);
	}
	
	/*
	 * Raise a number to a positive power.
	 */
	template <class BaseT, class ExpT> BaseT RPPowerUtil::rppower_positive (BaseT const x, ExpT const y) {
		
		// Work area.
		ExpT  tmp_y  = y;
		BaseT tmp_x  = x;
		BaseT result = static_cast<BaseT>(1);
		
		// Changed so that this isn't basically the only function in the entire
		// library that's vulnerable to overflow. Yes, that means it's way slower.
		for (;;) {
			if (tmp_y & 1) {
				if (RPPowerUtil::mul_overflows(result, tmp_x)) {
					throw DAC::RPPower::Errors::Overflow();
				}
				result *= tmp_x;
			}
			tmp_y >>= 1;
			if (tmp_y == 0) {
				break;
			}
			if (RPPowerUtil::mul_overflows(tmp_x, tmp_x)) {
				throw DAC::RPPower::Errors::Overflow();
			}
			tmp_x *= tmp_x;
		}
		
		return result;
		
	}
	template <class BaseT, class ExpT> BaseT RPPowerUtil::rppower_negative (BaseT const x, ExpT const y) {
		
		ExpT  tmp_y  = y;
		BaseT tmp_x  = x;
		BaseT result = static_cast<BaseT>(1);
		
		for (;;) {
			if (tmp_y & 1) {
				result /= tmp_x; // I did this. I am so smrt. S-M-R-T.
			}
			// Can't bitshift divide with negatives, as -1 >> 1 == -1.
			tmp_y /= 2;
			if (tmp_y == 0) {
				break;
			}
			// Overflow in this case just means that tmp_x is greater than result
			// can possibly be, so the -power (root) is less than 1.
			if (RPPowerUtil::mul_overflows(tmp_x, tmp_x)) {
				return 0;
			}
			tmp_x *= tmp_x;
		}
		
		// Done.
		return result;
		
	}
	
	/*
	 * Choose multiplication or division.
	 */
	template <class BaseT, class ExpT> inline BaseT RPPowerUtil::RPPower<BaseT, ExpT, false>::op (BaseT const x, ExpT const y) {
		return RPPowerUtil::rppower_positive(x, y);
	}
	template <class BaseT, class ExpT> inline BaseT RPPowerUtil::RPPower<BaseT, ExpT, true >::op (BaseT const x, ExpT const y) {
		if (y >= 0) {
			return RPPowerUtil::rppower_positive(x, y);
		} else {
			return RPPowerUtil::rppower_negative(x, y);
		}
	}

	/*
	 * Call the correct check for multiplication overflow.
	 */
	template <class T> inline bool RPPowerUtil::mul_overflows (T const x, T const y) {
		return Mul_Overflows<T, std::numeric_limits<T>::is_signed>::op(x, y);
	}
	
	// Check a multiplication for overflow.
	template <class T> inline bool RPPowerUtil::Mul_Overflows<T, false>::op (T const x, T const y) {
		return std::numeric_limits<T>::max() / x < y;
	}
	template <class T> inline bool RPPowerUtil::Mul_Overflows<T, true >::op (T const x, T const y) {
		return
			(x < 0 == y < 0) ? (
				(x < 0) ? (
					std::numeric_limits<T>::max() / x > y
				) : (
					std::numeric_limits<T>::max() / x < y
				)
			) : (
				(x < 0) ? (
					// This is switched up to keep the numbers negative, which has an
					// absolute range one greater than positive numbers.
					std::numeric_limits<T>::min() / y > x
				) : (
					std::numeric_limits<T>::min() / x > y
				)
			)
		;
	}
	
}

// End include guard.
#endif
