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
		
		// Here it is. I didn't write this. It's very sneaky. x's last
		// multiplication gets thrown away. If small exponents are primarily
		// being used, it may be more efficient to put in a test on the odd
		// number to see if y is 1 and return at that point. I like less lines
		// though. Whee. I did write the bit that makes this handle negative
		// powers though. Whee.
		for (;;) {
			if (tmp_y & 1) {
				result *= tmp_x;
			}
			tmp_y >>= 1;
			if (!tmp_y) {
				break;
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
			tmp_y >>= 1;
			if (!tmp_y) {
				break;
			}
			tmp_x *= tmp_x;
		}
		
		// Done.
		return result;
		
	}
	
	// Choose multiplication or division.
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

}

// End include guard.
#endif
