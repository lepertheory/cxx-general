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
  
  // Stream I/O operators.
  template <class T> std::istream& operator >> (std::istream& left, DAC::SafeInteger<T>&       right);
  template <class T> std::ostream& operator << (std::ostream& left, DAC::SafeInteger<T> const& right);
  
  // Arithmetic operators.
  template <class T>            DAC::SafeInteger<T>  operator + (DAC::SafeInteger<T>  const& left, DAC::SafeInteger<T>  const& right);
  template <class LT, class RT> DAC::SafeInteger<LT> operator + (DAC::SafeInteger<LT> const& left, RT                   const  right);
  template <class LT, class RT> DAC::SafeInteger<RT> operator + (LT                   const  left, DAC::SafeInteger<RT> const& right);
  template <class T>            DAC::SafeInteger<T>  operator - (DAC::SafeInteger<T>  const& left, DAC::SafeInteger<T>  const& right);
  template <class LT, class RT> DAC::SafeInteger<LT> operator - (DAC::SafeInteger<LT> const& left, RT                   const  right);
  template <class LT, class RT> DAC::SafeInteger<RT> operator - (LT                   const  left, DAC::SafeInteger<RT> const& right);
  template <class T>            DAC::SafeInteger<T>  operator * (DAC::SafeInteger<T>  const& left, DAC::SafeInteger<T>  const& right);
  template <class LT, class RT> DAC::SafeInteger<LT> operator * (DAC::SafeInteger<LT> const& left, RT                   const  right);
  template <class LT, class RT> DAC::SafeInteger<RT> operator * (LT                   const  left, DAC::SafeInteger<RT> const& right);
  template <class T>            DAC::SafeInteger<T>  operator / (DAC::SafeInteger<T>  const& left, DAC::SafeInteger<T>  const& right);
  template <class LT, class RT> DAC::SafeInteger<LT> operator / (DAC::SafeInteger<LT> const& left, RT                   const  right);
  template <class LT, class RT> DAC::SafeInteger<RT> operator / (LT                   const  left, DAC::SafeInteger<RT> const& right);
  
  /*************************************************************************
   * Specialization of numeric_limits.
   *************************************************************************/
  namespace std {
    template <> template <class T> struct numeric_limits< DAC::SafeInteger<T> > {
      
      static bool const is_specialized = numeric_limits<T>::is_specialized;
      
      static int min() throw() { return numeric_limits<T>::min(); }
      static int max() throw() { return numeric_limits<T>::max(); }
      
      static int  const digits     = numeric_limits<T>::digits;
      static int  const digits10   = numeric_limits<T>::digits10;
      static bool const is_signed  = numeric_limits<T>::is_signed;
      static bool const is_integer = numeric_limits<T>::is_integer;
      static bool const is_exact   = numeric_limits<T>::is_exact;
      static int  const radix      = numeric_limits<T>::radix;
      
      static int epsilon()     throw() { return numeric_limits<T>::epsilon();     }
      static int round_error() throw() { return numeric_limits<T>::round_error(); }
      
      static int const min_exponent   = numeric_limits<T>::min_exponent;
      static int const min_exponent10 = numeric_limits<T>::min_exponent10;
      static int const max_exponent   = numeric_limits<T>::max_exponent;
      static int const max_exponent10 = numeric_limits<T>::max_exponent10;
      
      static bool               const has_infinity      = numeric_limits<T>::has_infinity;
      static bool               const has_quiet_NaN     = numeric_limits<T>::has_quiet_NaN;
      static bool               const has_signaling_NaN = numeric_limits<T>::has_signaling_NaN;
      static float_denorm_style const has_denorm        = numeric_limits<T>::has_denorm;
      static bool               const has_denorm_loss   = numeric_limits<T>::has_denorm_loss;
      
      static int infinity()      throw() { return numeric_limits<T>::infinity();      }
      static int quiet_NaN()     throw() { return numeric_limits<T>::quiet_NaN();     }
      static int signaling_NaN() throw() { return numeric_limits<T>::signaling_NaN(); }
      static int denorm_min()    throw() { return numeric_limits<T>::denorm_min();    }
      
      static bool const is_iec559  = numeric_limits<T>::is_iec559;
      static bool const is_bounded = numeric_limits<T>::is_bounded;
      static bool const is_modulo  = numeric_limits<T>::is_modulo;
      
      static bool              const traps           = numeric_limits<T>::traps;
      static bool              const tinyness_before = numeric_limits<T>::tinyness_before;
      static float_round_style const round_style     = numeric_limits<T>::round_style;
      
    };
  };
  
  // Namespace wrapper.
  namespace DAC {
    
    // Errors that SafeInteger may throw.
    namespace SafeIntegerErrors {
      
      // Errors.
      class Base      : public Exception {};
      class Overflow  : public Base      {};
      class DivByZero : public Base      {};
      
      // Error factories.
      void throwBase      (std::string const& text) throw(Base);
      void throwOverflow  (std::string const& text) throw(Overflow);
      void throwDivByZero (std::string const& text) throw(DivByZero);
      
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
                            SafeInteger& operator += (SafeInteger<T>  const& right);
        template <class RT> SafeInteger& operator += (RT              const  right);
                            SafeInteger& operator -= (SafeInteger<T>  const& right);
        template <class RT> SafeInteger& operator -= (RT              const  right);
                            SafeInteger& operator *= (SafeInteger<T>  const& right);
        template <class RT> SafeInteger& operator *= (RT              const  right);
                            SafeInteger& operator /= (SafeInteger<T>  const& right);
        template <class RT> SafeInteger& operator /= (RT              const  right);
        
        // Increment / decrement operators.
        SafeInteger& operator ++ ();
        SafeInteger  operator ++ (int);
        SafeInteger& operator -- ();
        SafeInteger  operator -- (int);
        
        // Return the value of this number.
        T Value () const;
        
        // Arithmetic operator backends.
                            SafeInteger& op_add (SafeInteger<T> const& number);
        template <class RT> SafeInteger& op_add (RT             const  number);
                            SafeInteger& op_sub (SafeInteger<T> const& number);
        template <class RT> SafeInteger& op_sub (RT             const  number);
                            SafeInteger& op_mul (SafeInteger<T> const& number);
        template <class RT> SafeInteger& op_mul (RT             const  number);
                            SafeInteger& op_div (SafeInteger<T> const& number);
        template <class RT> SafeInteger& op_div (RT             const  number);
      
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
      inline void throwBase      (std::string const& text) throw(Base)      { Base      error; try { error.Text(text); } catch (...) {} throw error; }
      inline void throwOverflow  (std::string const& text) throw(Overflow)  { Overflow  error; try { error.Text(text); } catch (...) {} throw error; }
      inline void throwDivByZero (std::string const& text) throw(DivByZero) { DivByZero error; try { error.Text(text); } catch (...) {} throw error; }
      
    };
    
    // Default constructor.
    template <class T> SafeInteger<T>::SafeInteger (T const number) { _number = number; }
    
    // Arithmetic assignment operators.
    template <class T>                     SafeInteger<T>& SafeInteger<T>::operator += (SafeInteger<T> const& number) { return op_add(number); }
    template <class T> template <class RT> SafeInteger<T>& SafeInteger<T>::operator += (RT             const  number) { return op_add(number); }
    template <class T>                     SafeInteger<T>& SafeInteger<T>::operator -= (SafeInteger<T> const& number) { return op_sub(number); }
    template <class T> template <class RT> SafeInteger<T>& SafeInteger<T>::operator -= (RT             const  number) { return op_sub(number); }
    template <class T>                     SafeInteger<T>& SafeInteger<T>::operator *= (SafeInteger<T> const& number) { return op_mul(number); }
    template <class T> template <class RT> SafeInteger<T>& SafeInteger<T>::operator *= (RT             const  number) { return op_mul(number); }
    template <class T>                     SafeInteger<T>& SafeInteger<T>::operator /= (SafeInteger<T> const& number) { return op_div(number); }
    template <class T> template <class RT> SafeInteger<T>& SafeInteger<T>::operator /= (RT             const  number) { return op_div(number); }
    
    // Increment / decrement operators.
    template <class T> SafeInteger<T>& SafeInteger<T>::operator ++ ()    { return op_add(1); }
    template <class T> SafeInteger<T>  SafeInteger<T>::operator ++ (int) { SafeInteger<T> retval(*this); op_add(1); return retval; }
    template <class T> SafeInteger<T>& SafeInteger<T>::operator -- ()    { return op_sub(1); }
    template <class T> SafeInteger<T>  SafeInteger<T>::operator -- (int) { SafeInteger<T> retval(*this); op_sub(1); return retval; }
    
    // Return the value of this number.
    template <class T> T SafeInteger<T>::Value () const { return _number; }
    
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
    
    // Subtract another SafeInteger of the same type.
    template <class T> SafeInteger<T>& SafeInteger<T>::op_sub (SafeInteger<T> const& number) {
      
      // Check for overflow.
      if (std::numeric_limits<T>::is_signed) {
        if ((_number > 0) && (number._number < 0)) {
          if (number._number < -(std::numeric_limits<T>::max() - _number)) {
            SafeIntegerErrors::throwOverflow(toString(_number) + " - " + toString(number._number) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
          }
        } else if ((_number < 0) && (number._number > 0)) {
          if (-number._number < (std::numeric_limits<T>::min() - _number)) {
            SafeIntegerErrors::throwOverflow(toString(_number) + " - " + toString(number._number) + " is less than type minimum of " + toString(std::numeric_limits<T>::min()) + ".");
          }
        }
      } else {
        if (number._number > (std::numeric_limits<T>::min() + _number)) {
          SafeIntegerErrors::throwOverflow(toString(_number) + " - " + toString(number._number) + " is less than type minimum of " + toString(std::numeric_limits<T>::min()) + ".");
        }
      }
      
      // Do the subtract and return result.
      _number -= number._number;
      return *this;
      
    }
    
    // Multiply by another SafeInteger of the same type.
    template <class T> SafeInteger<T>& SafeInteger<T>::op_mul (SafeInteger<T> const& number) {
      
      // No overflow is possible when multiplying by 0, also prevents divide
      // by zero errors.
      if ((_number != 0) && (number._number != 0)) {
        
        // Check for overflow.
        if (std::numeric_limits<T>::is_signed) {
          if ((_number > 0) && (number._number > 0)) {
            if (number._number > (std::numeric_limits<T>::max() / _number)) {
              SafeIntegerErrors::throwOverflow(toString(_number) + " * " + toString(number._number) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
            }
          } else if ((_number < 0) && (number._number < 0)) {
            if (number._number < (std::numeric_limits<T>::max() / _number)) {
              SafeIntegerErrors::throwOverflow(toString(_number) + " * " + toString(number._number) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
            }
          } else if ((_number > 0) && (number._number < 0)) {
            if (_number != -1) {
              if (number._number < (std::numeric_limits<T>::min() / _number)) {
                SafeIntegerErrors::throwOverflow(toString(_number) + " * " + toString(number._number) + " is less than type minimum of " + toString(std::numeric_limits<T>::min()) + ".");
              }
            }
          } else if ((_number < 0) && (number._number > 0)) {
            if (_number != 1) {
              if (-number._number < -(std::numeric_limits<T>::min() / _number)) {
                SafeIntegerErrors::throwOverflow(toString(_number) + " * " + toString(number._number) + " is less than type minimum of " + toString(std::numeric_limits<T>::min()) + ".");
              }
            }
          }
        } else {
          if (number._number > (std::numeric_limits<T>::max() / _number)) {
            SafeIntegerErrors::throwOverflow(toString(_number) + " * " + toString(number._number) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
          }
        }
        
      }
        
      // Do the multiply and return result.
      _number *= number._number;
      return *this;
      
    }
    
    // Divide by another SafeInteger of the same type.
    template <class T> SafeInteger<T>& SafeInteger<T>::op_div (SafeInteger<T> const& number) {
      
      // Check for divide by 0.
      if (number._number == 0) {
        SafeIntegerErrors::throwDivByZero("Divide by zero.");
      }
      
      // Check for overflow.
      if (std::numeric_limits<T>::is_signed) {
        if ((_number == std::numeric_limits<T>::min()) && (number._number == -1)) {
          SafeIntegerErrors::throwOverflow(toString(_number) + " / " + toString(number._number) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
        }
      }
      
      // Do the divide and return result.
      _number /= number._number;
      return *this;
      
    }
    
    // Add a number of a different type.
    template <class T> template <class RT> SafeInteger<T>& SafeInteger<T>::op_add (RT const number) {
      
      // Work with the larger of the two types.
      if (std::numeric_limits<T>::digits >= std::numeric_limits<RT>::digits) {
        
        // Only the right number is signed.
        if (!std::numeric_limits<T>::is_signed && std::numeric_limits<RT>::is_signed) {
          
          if (number < 0) {
            if (number < -(std::numeric_limits<T>::min() + _number)) {
              SafeIntegerErrors::throwOverflow(toString(_number) + " + " + toString(number) + " is less than type minimum of " + toString(std::numeric_limits<T>::min()) + ".");
            }
          } else if (number > 0) {
            if (number > (std::numeric_limits<T>::max() - _number)) {
              SafeIntegerErrors::throwOverflow(toString(_number) + " + " + toString(number) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
            }
          }
          
        }
        
      } else {
        
        // Neither number is signed.
        if (!std::numeric_limits<T>::is_signed && !std::numeric_limits<RT>::is_signed) {
          
          if (number > (std::numeric_limits<T>::max() - _number)) {
            SafeIntegerErrors::throwOverflow(toString(_number) + " + " + toString(number) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
          }
        
        // Only the left number is signed.
        } else if (std::numeric_limits<T>::is_signed && !std::numeric_limits<RT>::is_signed) {
          
          if (_number >= 0) {
            if (number > (std::numeric_limits<T>::max() - _number)) {
              SafeIntegerErrors::throwOverflow(toString(_number) + " + " + toString(number) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
            }
          } else {
            T tmpnum;
            if (_number == std::numeric_limits<RT>::min()) {
              tmpnum = std::numeric_limits<RT>::max() + 1;
            } else {
              tmpnum = -_number;
            }
            if (number > tmpnum) {
              if ((number + _number) > std::numeric_limits<T>::max()) {
                SafeIntegerErrors::throwOverflow(toString(_number) + " + " + toString(number) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
              }
            }
          }
          
        }
        
      }
      
      // Do the addition and return result.
      _number += number;
      return *this;
      
    }
    
    // Subtract a number of a different type.
    template <class T> template <class RT> SafeInteger<T>& SafeInteger<T>::op_sub (RT const number) {
      
      // Work with the larger of the two sides.
      if (std::numeric_limits<T>::digits >= std::numeric_limits<RT>::digits) {
        
        // Only the right number is signed.
        if (!std::numeric_limits<T>::is_signed && std::numeric_limits<RT>::is_signed) {
          
          if (number > 0) {
            if (number > (std::numeric_limits<T>::min() + _number)) {
              SafeIntegerErrors::throwOverflow(toString(_number) + " - " + toString(number) + " is less than type minimum of " + toString(std::numeric_limits<T>::min()) + ".");
            }
          } else if (number < 0) {
            T tmpnum;
            if (number == std::numeric_limits<RT>::min()) {
              tmpnum = std::numeric_limits<RT>::max() + 1;
            } else {
              tmpnum = -number;
            }
            if (tmpnum > (std::numeric_limits<T>::max() - _number)) {
              SafeIntegerErrors::throwOverflow(toString(_number) + " - " + toString(number) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
            }
          }
          
        }
        
      } else {
        
        // Neither number is signed.
        if (!std::numeric_limits<T>::is_signed && !std::numeric_limits<RT>::is_signed) {
          
          if (number > (std::numeric_limits<T>::min() + _number)) {
            SafeIntegerErrors::throwOverflow(toString(_number) + " - " + toString(number) + " is less than type minimum of " + toString(std::numeric_limits<T>::min()) + ".");
          }
          
        // Only the left number is signed.
        } else if (std::numeric_limits<T>::is_signed && !std::numeric_limits<RT>::is_signed) {
          
          if (_number < 0) {
            if (number > -(std::numeric_limits<T>::min() - _number)) {
              SafeIntegerErrors::throwOverflow(toString(_number) + " - " + toString(number) + " is less than type minimum of " + toString(std::numeric_limits<T>::min()) + ".");
            }
          } else {
            if (number > _number) {
              if ((number - _number) > (static_cast<T>(-(std::numeric_limits<T>::min() + 1)) + 1)) {
                SafeIntegerErrors::throwOverflow(toString(_number) + " - " + toString(number) + " is less than type minimum of " + toString(std::numeric_limits<T>::min()) + ".");
              }
            }
          }
          
        }
        
      }
      
      // Do the subtraction and return result.
      _number -= number;
      return *this;
      
    }
    
    // Multiply by a number of a different type.
    template <class T> template <class RT> SafeInteger<T>& SafeInteger<T>::op_mul (RT const number) {
      
      // No overflow is possible if either number is 0, also prevents divide
      // by zero errors.
      if ((_number != 0) && (number != 0)) {
        
        // Work with the larger of the two sides.
        if (std::numeric_limits<T>::digits >= std::numeric_limits<RT>::digits) {
          
          // Only the right number is signed.
          if (!std::numeric_limits<T>::is_signed && std::numeric_limits<RT>::is_signed) {
            
            if (number > 0) {
              if (number > (std::numeric_limits<T>::max() / _number)) {
                SafeIntegerErrors::throwOverflow(toString(_number) + " * " + toString(number) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
              }
            } else if (number < 0) {
              if (_number != 0) {
                SafeIntegerErrors::throwOverflow(toString(_number) + " * " + toString(number) + " is less than type minimum of " + toString(std::numeric_limits<T>::min()) + ".");
              }
            }
            
          }
          
        } else {
          
          // Neither number is signed.
          if (!std::numeric_limits<T>::is_signed && !std::numeric_limits<RT>::is_signed) {
            
            if (number > (std::numeric_limits<T>::max() / _number)) {
              SafeIntegerErrors::throwOverflow(toString(_number) + " * " + toString(number) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
            }
            
          // Only the left number is signed.
          } else if (std::numeric_limits<T>::is_signed && !std::numeric_limits<RT>::is_signed) {
            
            if (_number > 0) {
              if (number > (std::numeric_limits<T>::max() / _number)) {
                SafeIntegerErrors::throwOverflow(toString(_number) + " * " + toString(number) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
              }
            } else if (_number < 0) {
              if ((_number == -1) && (number > (-(static_cast<RT>(std::numeric_limits<T>::min() + 1)) + 1))) {
                SafeIntegerErrors::throwOverflow(toString(_number) + " * " + toString(number) + " is less than type minimum of " + toString(std::numeric_limits<T>::min()) + ".");
              } else if (number > (std::numeric_limits<T>::min() / _number)) {
                SafeIntegerErrors::throwOverflow(toString(_number) + " * " + toString(number) + " is less than type minimum of " + toString(std::numeric_limits<T>::min()) + ".");
              }
            }
            
          }
          
        }
        
      }
      
      // Do the multiplication and return the result.
      _number *= number;
      return *this;
      
    }
    
    // Divide by a number of a different type.
    template <class T> template <class RT> SafeInteger<T>& SafeInteger<T>::op_div (RT const number) {
      
      // Check for divide by 0.
      if (number == 0) {
        SafeIntegerErrors::throwDivByZero("Divide by zero.");
      }
      
      // Work with the larger of the two sides.
      if (std::numeric_limits<T>::digits >= std::numeric_limits<RT>::digits) {
        
      } else {
        
      }
      
      // Do the division and return the result.
      _number /= number;
      return *this;
      
    }
    
  };
  
  // Stream I/O operators.
  template <class T> inline std::istream& operator >> (std::istream& left, DAC::SafeInteger<T>&       right) { T input; left >> input; right = input; return left; }
  template <class T> inline std::ostream& operator << (std::ostream& left, DAC::SafeInteger<T> const& right) { left << DAC::toString(right.Value());  return left; }
  
  // Arithmetic operators.
  template <class T>            inline DAC::SafeInteger<T>  operator + (DAC::SafeInteger<T>  const& left, DAC::SafeInteger<T>  const& right) { DAC::SafeInteger<T>  retval(left); return retval.op_add(right); }
  template <class LT, class RT> inline DAC::SafeInteger<LT> operator + (DAC::SafeInteger<LT> const& left, RT                   const  right) { DAC::SafeInteger<LT> retval(left); return retval.op_add(right); }
  template <class LT, class RT> inline DAC::SafeInteger<RT> operator + (LT                   const  left, DAC::SafeInteger<RT> const& right) { DAC::SafeInteger<RT> retval(left); return retval.op_add(right); }
  template <class T>            inline DAC::SafeInteger<T>  operator - (DAC::SafeInteger<T>  const& left, DAC::SafeInteger<T>  const& right) { DAC::SafeInteger<T>  retval(left); return retval.op_sub(right); }
  template <class LT, class RT> inline DAC::SafeInteger<LT> operator - (DAC::SafeInteger<LT> const& left, RT                   const  right) { DAC::SafeInteger<LT> retval(left); return retval.op_sub(right); }
  template <class LT, class RT> inline DAC::SafeInteger<RT> operator - (LT                   const  left, DAC::SafeInteger<RT> const& right) { DAC::SafeInteger<RT> retval(left); return retval.op_sub(right); }
  template <class T>            inline DAC::SafeInteger<T>  operator * (DAC::SafeInteger<T>  const& left, DAC::SafeInteger<T>  const& right) { DAC::SafeInteger<T>  retval(left); return retval.op_mul(right); }
  template <class LT, class RT> inline DAC::SafeInteger<LT> operator * (DAC::SafeInteger<LT> const& left, RT                   const  right) { DAC::SafeInteger<LT> retval(left); return retval.op_mul(right); }
  template <class LT, class RT> inline DAC::SafeInteger<RT> operator * (LT                   const  left, DAC::SafeInteger<RT> const& right) { DAC::SafeInteger<RT> retval(left); return retval.op_mul(right); }
  template <class T>            inline DAC::SafeInteger<T>  operator / (DAC::SafeInteger<T>  const& left, DAC::SafeInteger<T>  const& right) { DAC::SafeInteger<T>  retval(left); return retval.op_div(right); }
  template <class LT, class RT> inline DAC::SafeInteger<LT> operator / (DAC::SafeInteger<LT> const& left, RT                   const  right) { DAC::SafeInteger<LT> retval(left); return retval.op_div(right); }
  template <class LT, class RT> inline DAC::SafeInteger<RT> operator / (LT                   const  left, DAC::SafeInteger<RT> const& right) { DAC::SafeInteger<RT> retval(left); return retval.op_div(right); }
  
#endif
