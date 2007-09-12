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
#include <Exception.h++>
#include <to_string.h++>

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
			template <class T> class DigitOverflow : public Overflow {
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
					DigitOverflow& Digit (T const digit) { _digit = digit; return *this; };
					T              Digit (             ) { return _digit;                };
				private:
					T _digit;
			};
			
			// Negative numbers are not supported, keep track of your own signs.
			class Negative : public Base { public: virtual char const* what () const throw() { return "Negative numbers are not supported."; }; };
			
		}
	}
	
	/***************************************************************************/
	// Functions.
	
	// Convert a native type to another base.
	template <class T, class U> U& baseConvert (T const from, U& to, typename U::value_type const tobase);
	template <class T, class U> U& baseConvert (T const from, U& to                                     );
	
	// Convert a based number to another base.
	template <class T, class U> U& baseConvert (T const& from, typename T::value_type const frombase, U& to, typename U::value_type const tobase);
	template <class T, class U> U& baseConvert (T const& from, typename T::value_type const frombase                                            );
	
	// Convert a based number to native type.
	template <class T, class U> U& baseConvert (T const& from, typename T::value_type const frombase);
	
	/***************************************************************************
	 * Inline and template definitions.
	 ***************************************************************************/
	
	/*
	 * Convert a native type to another base.
	 */
	template <class T, class U> U& baseConvert (T const from, U& to, typename U::value_type const tobase) {
		
		// Check for overflow.
		if (tobase < 2) {
			throw BaseConvert::Errors::MinBaseTo();
		}
		if (is_negative(from)) {
			throw BaseConvert::Errors::Negative();
		}
		
		// Work area.
		U retval       ;
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
	template <class T, class U> U& baseConvert (T const from, U& to) {
		
		// Check for overflow.
		if (is_negative(from)) {
			throw BaseConvert::Errors::Negative();
		}
		
		// Work area.
		U retval       ;
		T work   = from;
		
		// Convert base by shifting numbers into place.
		while (work > 0) {
			retval.push_back(work & std::numeric_limits<typename U::value_type>::max());
			work >>= std::numeric_limits<typename U::value_type>::digits;
		}
		
		to.swap(retval);
		return to;
		
	}
	
	/*
	 * Convert a based number to another base.
	 */
	template <class T, class U> U& baseConvert (T const& from, typename T::value_type const frombase, U& to, typename U::value_type const tobase) {
		
		// Check for overflow.
		if (frombase >= 1 << (std::numeric_limits<typename T::value_type>::digits >> 1)) {
			throw BaseConvert::Errors::MaxBaseFrom();
		}
		if (frombase < 2) {
			throw BaseConvert::Errors::MinBaseFrom();
		}
		if (tobase >= 1 << (std::numeric_limits<typename U::value_type>::digits >> 1)) {
			throw BaseConvert::Errors::MaxBaseTo();
		}
		if (tobase < 2) {
			throw BaseConvert::Errors::MaxBaseFrom();
		}
		
		// Work area.
		U retval      ;
		T work  (from);
		
		// Convert base by storing the remainder of repeated division by the base
		// that we will be converting to. Least significant digits come out first.
		try {
			while (work.size() > 1 && work[0] != 0) {
				retval.push_back(londiv(work, work, tobase, frombase));
			}
		} catch (LongDiv::Errors::DigitOverflow<& e) {
			throw BaseConvert::Errors::DigitOverflow().Digit(e.Digit());
		}
		
		// Done.
		to.swap(retval);
		return retval;
		
	}
	
	/*
	 * Convert a based number to another base, maximum size.
	 */
	template <class T, class U> U& baseConvert (T const& from, typename T::value_type const frombase, U& to) {
		
		if (frombase >= 1 << (std::numeric_limits<typename T::value_type>::digits >> 1)) {
			throw BaseConvert::Errors::MaxBaseFrom();
		}
		
	}
	
}

// End include guard.
#endif

