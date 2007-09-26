/*****************************************************************************
 * baseConvert.h++
 *****************************************************************************
 * Convert a number to another base.
 *****************************************************************************/

// Include guard.
#if !defined(BASECONVERT_u9843h)
  #define BASECONVERT_u9843h

// Standard inclueds.
#include <vector>
#include <limits>
#include <string>

// Internal includes.
#include "Exception.h++"
#include "to_string.h++"
#include "longDiv.h++"
#include "rppower.h++"

// Contain in namespace.
namespace DAC {
	
	/***************************************************************************/
	// Errors.
	namespace BaseConvert {
		namespace Errors {
			
			// All baseConvert() errors are based off if this.
			class Base : public Exception { public: virtual char const* what () const throw() { return "Undefined error in baseConvert()."; }; };
			
			// Divide by zero.
			class DivideByZero : public Base { public: virtual char const* what () const throw() { return "Attempted to divide by zero."; }; };
			
			// Minimum base is base 2.
			class MinBase     : public Base    { public: virtual char const* what () const throw() { return "Minimum base is base 2.";      }; };
			class MinBaseTo   : public MinBase { public: virtual char const* what () const throw() { return "Minimum to base is base .";    }; };
			class MinBaseFrom : public MinBase { public: virtual char const* what () const throw() { return "Minimum from base is base 2."; }; };
			
			// Maximum base must leave 1/2 bits available if long division will be necessary.
			class MaxBase     : public Base    { public: virtual char const* what () const throw() { return "Maximum base is 2^(data type bit length / 2).";      }; };
			class MaxBaseTo   : public MaxBase { public: virtual char const* what () const throw() { return "Maximum to base is 2^(data type bit length / 2).";   }; };
			class MaxBaseFrom : public MaxBase { public: virtual char const* what () const throw() { return "Maximum from base is 2^(data type bit length / 2)."; }; };
			
			// Overflow... somewhere.
			class Overflow : public Base { public: virtual char const* what () const throw() { return "Overflow."; }; };
			
			// Digit overflows given base.
			class DigitOverflow : public Overflow {
				public:
					~DigitOverflow () throw() {};
					virtual char const* what () const throw() {
						try {
							std::string tmpmsg("Digit at offset " + to_string(_digit) + " overflows given base.");
							return buffer_message(tmpmsg);
						} catch (...) {
							return "Digit overflows given base. Error creating message string.";
						}
					};
					DigitOverflow& Digit (size_t const digit) { _digit = digit; return *this; };
					size_t         Digit (                  ) { return _digit;                };
				private:
					size_t _digit;
			};
			
			// Based numeber overflows native type.
			class NativeOverflow : public Overflow { public: virtual char const* what () const throw() { return "Based number overflows given type."; }; };
			
			// Negative numbers are not supported, keep track of your own signs.
			class Negative : public Base { public: virtual char const* what () const throw() { return "Negative numbers are not supported."; }; };
			
		}
	}
	
	/***************************************************************************/
	// Functions.
	
	// Convert a native type to another base.
	template <class T> T& baseConvert (T const from, T& to, typename T::value_type const tobase);
	template <class T> T& baseConvert (T const from, T& to                                     );
	
	// Convert a based number to another base.
	template <class T> T& baseConvert (T const& from, typename T::value_type const frombase, T& to, typename T::value_type const tobase);
	
	// Convert a based number to native type.
	template <class T> T& baseConvert (T const& from, typename T::value_type const frombase);
	template <class T> T& baseConvert (T const& from                                       );
	
	/***************************************************************************
	 * Inline and template definitions.
	 ***************************************************************************/
	
	/*
	 * Convert a native type to another base.
	 */
	template <class T> T& baseConvert (T const from, T& to, typename T::value_type const tobase) {
		
		// Shunt off to the proper algorithm for a max base conversion.
		if (tobase == 0) {
			return baseConvert(from, to);
		}
		
		// Check for overflow.
		if (tobase < 2) {
			throw BaseConvert::Errors::MinBaseTo();
		}
		if (is_negative(from)) {
			throw BaseConvert::Errors::Negative();
		}
		
		// Work area.
		T retval       ;
		T work   = from;
		
		// Convert base by storing the remainder of repeated division by the base
		// that we will be converting to. Least significant digits come out first.
		while (work > 0) {
			retval.push_back(work % tobase);
			work /= tobase;
		}
		
		to.swap(retval);
		return to;
		
	}
	
	/*
	 * Convert a native type to another base, maximum size.
	 */
	template <class T> T& baseConvert (T const from, T& to) {
		
		// Check for overflow.
		if (is_negative(from)) {
			throw BaseConvert::Errors::Negative();
		}
		
		// Work area.
		T retval       ;
		T work   = from;
		
		// Nothing to it.
		retval.push_back(from);
		
		to.swap(retval);
		return to;
		
	}
	
	/*
	 * Convert a based number to another base.
	 */
	template <class T> T& baseConvert (T const& from, typename T::value_type const frombase, T& to, typename T::value_type const tobase) {
		
		// Check for overflow.
		if (frombase >= 1 << (std::numeric_limits<typename T::value_type>::digits >> 1)) {
			throw BaseConvert::Errors::MaxBaseFrom();
		}
		if (frombase < 2) {
			throw BaseConvert::Errors::MinBaseFrom();
		}
		if (tobase >= 1 << (std::numeric_limits<typename T::value_type>::digits >> 1)) {
			throw BaseConvert::Errors::MaxBaseTo();
		}
		if (tobase < 2) {
			throw BaseConvert::Errors::MaxBaseFrom();
		}
		
		// Work area.
		T retval      ;
		T work  (from);
		
		// Convert base by storing the remainder of repeated division by the base
		// that we will be converting to. Least significant digits come out first.
		try {
			while (work.size() > 0 && work[0] != 0) {
				retval.push_back(londiv(work, work, tobase, frombase));
			}
		} catch (LongDiv::Errors::DigitOverflow& e) {
			throw BaseConvert::Errors::DigitOverflow().Digit(e.Digit());
		}
		
		// Done.
		to.swap(retval);
		return retval;
		
	}
	
	/*
	 * Convert a based number to a native type.
	 */
	template <class T> T& baseConvert (T const& from, typename T::value_type const frombase) {
		
		// Check for overflow.
		/*
		if (frombase >= 1 << (std::numeric_limits<typename T::value_type>::digits >> 1)) {
			throw BaseConvert::Errors::MaxBaseFrom();
		}
		*/
		if (frombase < 2) {
			throw BaseConvert::Errors::MinBaseFrom();
		}
		
		
		// Work area.
		typename T::value_type retval;
		
		// Convert base by multiplying out.
		typename T::value_type magnitude = 1;
		for (typename T::const_iterator i = from.begin(); i != from.end(); ++i) {
			if (std::numeric_limits<typename T::value_type>::max() / magnitude > *i) {
				throw BaseConvert::Errors::NativeOverflow();
			}
			if (std::numeric_limits<typename T::value_type>::max() - *i * magnitude) {
				throw BaseConvert::Errors::NativeOverflow();
			}
			if (i + 1 != from.end() && std::numeric_limits<typename T::value_type>::max() / frombase > magnitude) {
				throw BaseConvert::Errors::NativeOverflow();
			}
			retval    += *i * magnitude;
			magnitude *= frombase;
		}
		
		return retval;
		
	}
	
	/*
	 * Convert a based number to a native type, maximum base.
	 */
	template <class T> T& baseConvert (T const& from) {
		
		
		
	}
	
}

// End include guard.
#endif

