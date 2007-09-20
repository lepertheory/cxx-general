/*****************************************************************************
 * longDiv.h++
 *****************************************************************************
 * Do long division on a sequence.
 *****************************************************************************/

// Include guard.
#if !defined(LONGDIV_8u7b345k02)
  #define LONGDIV_8u7b345k02

// Standard includes.
#include <string>
#include <limits>

// Internal includes.
#include "Exception.h++"
#include "NumInfo.h++"
#include "to_string.h++"

// Contain in namespace.
namespace DAC {
  
  /***************************************************************************/
  // Errors.
  namespace LongDiv {
    namespace Errors {
      
      // All longDiv() errors are based off this.
      class Base : public Exception { public: virtual char const* what () const throw() { return "Undefined error in longDiv()."; }; };
      
      // Divide by zero.
      class DivideByZero : public Base { public: virtual char const* what () const throw() { return "Divide by zero."; }; };
      
      // Base 2 is lowest supported base.
      class MinBase : public Base { public: virtual char const* what () const throw() { return "Minimum base is base 2."; }; };
      
      // Base must fit numbers in 1/2 available bits.
      class MaxBase : public Base { public: virtual char const* what () const throw() { return "Maximum base is 2^data type bit length."; }; };
      
      // Overflow somewhere.
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
      
      // Divisor overflows given base.
      class DivisorOverflow : public Overflow { public: virtual char const* what () const throw() { return "Divisor overflows given base."; }; };
      
    }
  }
  
  /***************************************************************************/
  // Functions.
  
  // Long division of a sequence.
  template <class T> typename T::value_type longDiv (T& quotient, T const& dividend, typename T::value_type const divisor, typename T::value_type const base);
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /*
   * Long division of a sequence.
   */
  template <class T> typename T::value_type longDiv (T& quotient, T const& dividend, typename T::value_type const divisor, typename T::value_type const base) {
    
    T                      newquotient    ;
    typename T::value_type candidate   = 0;
    typename T::value_type cquotient      ;
    
    if (divisor == 0) {
      throw LongDiv::Errors::DivideByZero();
    }
    if (base < 2) {
      throw LongDiv::Errors::MinBase();
    }
    if (base > 1 << (std::numeric_limits<typename T::value_type>::digits >> 1)) {
      throw LongDiv::Errors::MaxBase();
    }
    if (divisor >= base || is_negative(divisor)) {
      throw LongDiv::Errors::DivisorOverflow();
    }
    
    for (typename T::const_reverse_iterator i = dividend.rbegin(); i != dividend.rend(); ++i) {
      
      if (*i >= base || is_negative(*i)) {
        throw LongDiv::Errors::DigitOverflow().Digit(dividend.rend() - 1 - i);
      }
      
      candidate += *i;
      
      cquotient = candidate / divisor;
      newquotient.insert(newquotient.begin(), cquotient);
      
      candidate -= cquotient * divisor;
      candidate *= base;
      
    }
    
    // Trim insignificant zeros.
    {
      typename T::size_type              noinsignificantzeros = newquotient.size  ();
      typename T::const_reverse_iterator i                    = newquotient.rbegin();
      while (i != newquotient.rend() && *i == 0) {
        --noinsignificantzeros;
				++i;
      }
      newquotient.resize(noinsignificantzeros);
    }
    
    quotient.swap(newquotient);
    return candidate / base;
    
  }
  
}

// End include guard.
#endif

