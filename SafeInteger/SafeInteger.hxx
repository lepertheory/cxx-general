/*****************************************************************************
 * SafeInteger.hxx
 *****************************************************************************
 * Overflow safe integer class.
 *****************************************************************************/

// Include guard.
#if !defined(SAFEINTEGER_0234ykb209384jbyp08u)
  #define SAFEINTEGER_0234ykb209384jbyp08u
  
  // STL includes.
  #include <limits>
  
  // Internal includes.
  #include "Exception.hxx"
  #include "toString.hxx"
  
  // Forward declarations.
  namespace DAC {
    template <class T> class SafeInteger;
  };
  
  /*************************************************************************
   * Specialization of numeric_limits.
   *************************************************************************/
  namespace std {
    template <> template <class T> struct numeric_limits< DAC::SafeInteger<T> > {
      
      static const bool is_specialized = numeric_limits<T>::is_specialized;
      
      static int min() throw() { return numeric_limits<T>::min(); }
      static int max() throw() { return numeric_limits<T>::max(); }
      
      static const int  digits     = numeric_limits<T>::digits;
      static const int  digits10   = numeric_limits<T>::digits10;
      static const bool is_signed  = numeric_limits<T>::is_signed;
      static const bool is_integer = numeric_limits<T>::is_integer;
      static const bool is_exact   = numeric_limits<T>::is_exact;
      static const int  radix      = numeric_limits<T>::radix;
      
      static int epsilon()     throw() { return numeric_limits<T>::epsilon();     }
      static int round_error() throw() { return numeric_limits<T>::round_error(); }
      
      static const int min_exponent   = numeric_limits<T>::min_exponent;
      static const int min_exponent10 = numeric_limits<T>::min_exponent10;
      static const int max_exponent   = numeric_limits<T>::max_exponent;
      static const int max_exponent10 = numeric_limits<T>::max_exponent10;
      
      static const bool               has_infinity      = numeric_limits<T>::has_infinity;
      static const bool               has_quiet_NaN     = numeric_limits<T>::has_quiet_NaN;
      static const bool               has_signaling_NaN = numeric_limits<T>::has_signaling_NaN;
      static const float_denorm_style has_denorm        = numeric_limits<T>::has_denorm;
      static const bool               has_denorm_loss   = numeric_limits<T>::has_denorm_loss;
      
      static int infinity()      throw() { return numeric_limits<T>::infinity();      }
      static int quiet_NaN()     throw() { return numeric_limits<T>::quiet_NaN();     }
      static int signaling_NaN() throw() { return numeric_limits<T>::signaling_NaN(); }
      static int denorm_min()    throw() { return numeric_limits<T>::denorm_min();    }
      
      static const bool is_iec559  = numeric_limits<T>::is_iec559;
      static const bool is_bounded = numeric_limits<T>::is_bounded;
      static const bool is_modulo  = numeric_limits<T>::is_modulo;
      
      static const bool              traps           = numeric_limits<T>::traps;
      static const bool              tinyness_before = numeric_limits<T>::tinyness_before;
      static const float_round_style round_style     = numeric_limits<T>::round_style;
      
    };
  };
  
  // Namespace wrapper.
  namespace DAC {
    
    // Errors that SafeInteger may throw.
    namespace SafeIntegerErrors {
      
      // Errors.
      class Base     : public Exception {};
      class Overflow : public Base      {};
      
      // Error factories.
      void throwBase     (std::string const& text) throw(Base);
      void throwOverflow (std::string const& text) throw(Overflow);
      
    };
    
    /*************************************************************************
     * Class SafeInteger
     *************************************************************************
     * This is an overflow safe replacement for any integer type. Should
     * preserve all semantics of using a normal native integer type with a
     * minimum of syntatic changes needed.
     *************************************************************************/
    template <class T> class SafeInteger {
      
      // Public members.
      public:
        
        // Default constructor.
        SafeInteger (T const number = 0);
        
        // Arithmetic assignment operators.
                           SafeInteger& operator += (SafeInteger<T> const& number);
        template <class U> SafeInteger& operator += (U              const  number);
        
        // Increment / decrement operators.
        SafeInteger& operator ++ ();
        
        // Return the value of this number.
        T Value ();
        
        // Arithmetic operator backends.
                           SafeInteger& op_add (SafeInteger<T> const& number);
        template <class U> SafeInteger& op_add (U              const  number);
      
      // Private members.
      private:
        
        // This is the number itself.
        T _number;
      
    };
    
    
    /*************************************************************************
     * Inline and template definitions.
     *************************************************************************/
    
    // Error functions.
    namespace SafeIntegerErrors {
      
      // Throw errors.
      inline void throwBase     (std::string const& text) throw(Base)     { Base     error; try { error.Text(text); } catch (...) {} throw error; }
      inline void throwOverflow (std::string const& text) throw(Overflow) { Overflow error; try { error.Text(text); } catch (...) {} throw error; }
      
    };
    
    // Default constructor.
    template <class T> SafeInteger<T>::SafeInteger (T const number) { _number = number; }
    
    // Arithmetic assignment operators.
    template <class T>                    SafeInteger<T>& SafeInteger<T>::operator += (SafeInteger<T> const& number) { return op_add(number); }
    template <class T> template <class U> SafeInteger<T>& SafeInteger<T>::operator += (U              const  number) { return op_add(number); }
    
    // Increment / decrement operators.
    template <class T> SafeInteger<T>& SafeInteger<T>::operator ++ () { return op_add(1); }
    
    // Return the value of this number.
    template <class T> T SafeInteger<T>::Value () { return _number; }
    
    // Add another SafeInteger of the same type.
    template <class T> SafeInteger<T>& SafeInteger<T>::op_add (SafeInteger<T> const& number) {
      
      // Check for overflow.
      if (std::numeric_limits<T>::is_signed) {
        if ((_number > 0) && (number._number > 0)) {
          if (number._number > (std::numeric_limits<T>::max() - _number)) {
            SafeIntegerErrors::throwOverflow(toString(_number) + " + " + toString(number._number) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
          }
        } else if ((_number < 0) && (number._number < 0)) {
          if (number._number < (std::numeric_limits<T>::min() - _number)) {
            SafeIntegerErrors::throwOverflow(toString(_number) + " + " + toString(number._number) + " is less than type minimum of " + toString(std::numeric_limits<T>::min()) + ".");
          }
        }
      } else {
        if (number._number > (std::numeric_limits<T>::max() - _number)) {
          SafeIntegerErrors::throwOverflow(toString(_number) + " + " + toString(number._number) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
        }
      }
      
      // Do the addition and return result.
      _number += number._number;
      return *this;
      
    }
    
    // Add a number of a different type.
    template <class T> template <class U> SafeInteger<T>& SafeInteger<T>::op_add (U const number) {
      
      // Work with the larger of the two types.
      if (std::numeric_limits<T>::digits >= std::numeric_limits<U>::digits) {
        
        // Both numbers are signed.
        if (std::numeric_limits<T>::is_signed && std::numeric_limits<U>::is_signed) {
          
          if ((_number > 0) && (number > 0)) {
            if (number > (std::numeric_limits<T>::max() - _number)) {
              SafeIntegerErrors::throwOverflow(toString(_number) + " + " + toString(number) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
            }
          }
          
        }
        
      } else {
        
      }
      
      return *this;
      
    }
    
  };
  
#endif
