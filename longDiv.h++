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

// Internal includes.
#include <Exception.h++>

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
      
      // Overflow somewhere.
      class Overflow : public Base { public: virtual char const* what () const throw() { return "Overflow."; }; };
      
      // Digit overflows given base.
      template <class T> class DigitOverflow : public Base {
        public:
          ~DigitOverflow () throw() {};
          virtual char const* what () const throw() {
            try {
              std::string tmpmsg("Digit " + to_string(_digit) + " overflows given base.");
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
    
    T                      newquotient;
    typename T::value_type candidate  ;
    typename T::value_type cquotient  ;
    
    if (divisor == 0) {
      throw LongDiv::Errors::DivideByZero();
    }
    if (base < 2) {
      throw LongDiv::Errors::MinBase();
    }
    
    for (typename T::const_reverse_iterator i = dividend.rbegin(); i != dividend.rend(); ++i) {
      
      if (*i >= base) {
        throw LongDiv::Errors::DigitOverflow<typename T::value_type>().Digit(i - dividend.rbegin() + 1);
      }
      
      candidate += *i;
      
      cquotient = candidate / divisor;
      newquotient.insert(newquotient.begin(), cquotient);
      
      candidate -= cquotient * divisor;
      candidate *= base;
      
    }
    
    {
      typename T::size_type              noinsignificantzeros = newquotient.size()  ;
      typename T::const_reverse_iterator i                    = newquotient.rbegin();
      while (*(i++) == 0) {
        --noinsignificantzeros;
      }
      newquotient.resize(noinsignificantzeros);
    }
    
    quotient.swap(newquotient);
    return candidate / base;
    
  }
  
}

// End include guard.
#endif

