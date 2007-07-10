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
    class Errors {
      public:
        
        // All baseConvert() errors are based off if this.
        class Base : public Exception { public: virtual char const* what () const throw() { return "Undefined error in baseConvert()."; }; };
        
        // Divide by zero.
        class DivideByZero : public Base { public: virtual char const* what () const throw() { return "Attempted to divide by zero."; }; };
        
        // Overflow... somewhere.
        class Overflow : public Base { public: virtual char const* what () const throw() { return "Overflow."; }; };
        
        // Requested base must fit in 1/2 the given data type.
        class BaseOverflow     : public Overflow     { public: virtual char const* what () const throw() { return "Requested base is too large for given data type."; }; };
        class BaseOverflowFrom : public BaseOverflow { public: virtual char const* what () const throw() { return "From base is too large for given data type."     ; }; };
        class BaseOverflowTo   : public BaseOverflow { public: virtual char const* what () const throw() { return "To base is too large for given data type."       ; }; };
        
        // Digit overflows given base.
        class DigitOverflow : public Overflow {
          public:
            virtual char const* what () const throw() {
              try {
                std::string tmpmsg("Digit " + to_string(_digit) + " overflows given base.");
                return Exception::buffer_message(tmpmsg);
              } catch (...) {
                return "Digit overflows given base. Error creating message string.";
              }
            };
            DigitOverflow& Digit (size_t const digit) { _digit = digit; return *this; };
            size_t         Digit (                  ) { return _digit;                };
          private:
            size_t _digit;
        };
      
      // Do not allow instantiation.
      private:
        Errors ();
        Errors (Errors const&);
        Errors& operator = (Errors const&);
      
    };
  }
  
  /***************************************************************************
   * Utilities.
   ***************************************************************************/
  namespace BaseConvertUtil {
    
    // Long division of a container.
    template <class T> T longDiv (std::vector<T> const& dividend, T const divisor, T const base);
    
  }
  
  /***************************************************************************/
  // Functions.
  
  // Convert a native type to another base.
  template <class T> std::vector<T>& baseConvert (T const& from, std::vector<T>& to, size_t const tobase);
  
  // Convert a based number to another base.
  template <class T> std::vector<T>& baseConvert (std::vector<T> const& from, size_t const frombase, std::vector<T>& to, size_t const tobase);
  
  // Convert a based number to native type.
  template <class T> T& baseConvert (std::vector<T> const& from, size_t const frombase, T& to, size_t const tobase);
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  namespace BaseConvertUtil {
    /*
    * Long division of a container.
    */
    template <class T> T longDiv (std::vector<T>& dividend, T const divisor, T const base) {
      
      // Work area.
      std::vector<T> quotient ;
      T              candidate;
      T              cquotient;
      
      // Make sure somebody's not trying to divide by zero.
      if (divisor == 0) {
        throw BaseConvert::Errors::DivideByZero();
      }
      
      // Divide like 1st grade.
      for (typename std::vector<T>::const_reverse_iterator i = dividend.rbegin(); i != dividend.rend(); ++i) {
        
        if (*i >= base) {
          throw BaseConvert::Errors::DigitOverflow().Digit(i - dividend.rbegin() + 1);
        }
        
        // Add this digit to the division candidate.
        candidate += *i;
        
        // Divide the group and add the result to the quotient.
        cquotient = candidate / divisor;
        quotient.insert(quotient.begin(), cquotient);
        
        // Take out what we've divided.
        candidate -= cquotient * divisor;
        
        // Move the remainder up to the next digit.
        candidate *= base;
        
      }
      
      // Trim insignificant zeros from the quotient.
      while (quotient.back() == 0) {
        quotient.resize(quotient.size() - 1);
      }
      
      // Put the result in place. There can be no errors after this, or we will
      // leave invalid data.
      dividend.swap(quotient);
      
      // Return the remainder. We need to undo the last move up.
      return candidate / base;
      
    }
  }
  
  /*
   * Convert a native type to another base.
   */
  template <class T> std::vector<T>& baseConvert (T const& from, std::vector<T>& to, int const tobase) {
    
    // Make sure that we will not be overflowing.
    if (tobase >= std::numeric_limits<T>::max() / 2) {
      throw BaseConvert::Errors::BaseOverflowTo();
    }
    
    // Work area.
    std::vector<T> retval       ;
    T              work   = from;
    
    // Convert base by storing the remainder of repeated division by the base
    // that we will be converting to. Least significant digits come out first.
    while (work > 0) {
      retval.push_back(work % tobase);
      work /= tobase;
    }
    
    // Done.
    to.swap(work);
    return to;
    
  }
  
  /*
   * Convert a based number to another base.
   */
  template <class T> std::vector<T>& baseConvert (std::vector<T> const& from, size_t const frombase, std::vector<T>& to, size_t const tobase) {
    
    // Make sure that wewill not be overflowing.
    if (frombase >= std::numeric_limits<T>::max() / 2) {
      throw BaseConvert::Errors::BaseOverflowFrom();
    }
    if (tobase >= std::numeric_limits<T>::max() / 2) {
      throw BaseConvert::Errors::BaseOverflowTo();
    }
    
    // Work area.
    std::vector<T> retval      ;
    std::vector<T> work  (from);
    
    // Convert base by storing the remainder of repeated division by the base
    // that we will be converting to. Least significant digits come out first.
    while (work > 0) {
      retval.push_back(londiv(work, tobase, frombase));
    }
    
    // Done.
    to.swap(retval);
    return retval;
    
  }
  
}

// End include guard.
#endif

