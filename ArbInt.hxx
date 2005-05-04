/*****************************************************************************
 * ArbInt.hxx
 *****************************************************************************
 * Arbitrary integer number class.
 *****************************************************************************/

// Include guard.
#if !defined(ARBINT_7b2y3kb789f2u3du)
  #define ARBINT_7b2y3kb789f2u3du
  
  // STL includes.
  #include <iostream>
  #include <string>
  #include <vector>
  
  // Internal includes.
  #include "SafeInteger.hxx"
  #include "ReferencePointer.hxx"
  #include "toString.hxx"
  #include "rppower.hxx"
  
  // Namespace wrapping.
  namespace DAC {
    
    /*************************************************************************
     * ArbInt
     *************************************************************************
     * This is an integer of an arbitrary number of digits.
     *************************************************************************/
    class ArbInt {
      
      /*
       * Private members.
       */
      private:
        
        /*********************************************************************/
        // Typedefs.
        
        // Native digit type. Do not change this unless you have a very good
        // reason. This must be an unsigned integer type.
        typedef unsigned int _DigT;
      
      /*
        * Public members.
        */
      public:
        
        /*********************************************************************/
        // Typedefs.
        
        typedef _DigT value_type;
        
        /*********************************************************************/
        // Function members.
        
        // Default constructor.
        ArbInt ();
        
        // Copy constructor.
        ArbInt (ArbInt const& number, bool const copynow = false);
        
        // Conversion constructor.
                           explicit ArbInt (std::string const& number);
        template <class T> explicit ArbInt (T           const  number);
        
        // Unary sign operators.
        int operator + () const;
        int operator - () const;
        
        // Increment / decrement operators.
        ArbInt& operator ++ ();
        ArbInt  operator ++ (int);
        ArbInt& operator -- ();
        ArbInt  operator -- (int);
        
        // Bitwise compliment.
        ArbInt operator ~ () const;
        
        // Assignment operator.
                           ArbInt& operator = (ArbInt      const& number);
                           ArbInt& operator = (std::string const& number);
        template <class T> ArbInt& operator = (T           const  number);
        
        // Arithmetic assignment operators.
                           ArbInt& operator *= (ArbInt const& number);
        template <class T> ArbInt& operator *= (T      const  number);
                           ArbInt& operator /= (ArbInt const& number);
        template <class T> ArbInt& operator /= (T      const  number);
                           ArbInt& operator %= (ArbInt const& number);
        template <class T> ArbInt& operator %= (T      const  number);
                           ArbInt& operator += (ArbInt const& number);
        template <class T> ArbInt& operator += (T      const  number);
                           ArbInt& operator -= (ArbInt const& number);
        template <class T> ArbInt& operator -= (T      const  number);
        
        // Bit shift assignment operators.
                           ArbInt& operator <<= (ArbInt const& number);
        template <class T> ArbInt& operator <<= (T      const  number);
                           ArbInt& operator >>= (ArbInt const& number);
        template <class T> ArbInt& operator >>= (T      const  number);
        
        // Bitwise assignment operators.
                           ArbInt& operator &= (ArbInt const& number);
        template <class T> ArbInt& operator &= (T      const  number);
                           ArbInt& operator |= (ArbInt const& number);
        template <class T> ArbInt& operator |= (T      const  number);
                           ArbInt& operator ^= (ArbInt const& number);
        template <class T> ArbInt& operator ^= (T      const  number);
        
        // Reset to just-constructed state.
        ArbInt& clear ();
        
        // Copy another number.
        ArbInt& copy (ArbInt const& number, bool const copynow = false);
        
        // Accessors.
                           value_type Base  () const;
        template <class T> T          Value () const;
                           ArbInt& Base  (value_type const base);
        template <class T> ArbInt& Value (T          const number);
        
        // Set this number.
                           ArbInt& set (ArbInt      const& number);
                           ArbInt& set (std::string const& number);
        template <class T> ArbInt& set (T           const  number);
        
        // Push digits into this number.
                           ArbInt& push_back (ArbInt      const& number);
                           ArbInt& push_back (std::string const& number);
        template <class T> ArbInt& push_back (T           const  number);
        
        // Return a string of this number.
        std::string toString () const;
        
        // Arithmetic operator backends.
                           ArbInt& op_mul (ArbInt const& number);
        template <class T> ArbInt& op_mul (T      const  number);
                           ArbInt& op_div (ArbInt const& number, ArbInt* remainder = 0);
        template <class T> ArbInt& op_div (T      const  number, ArbInt* remainder = 0);
                           ArbInt& op_mod (ArbInt const& number);
        template <class T> ArbInt& op_mod (T      const  number);
                           ArbInt& op_add (ArbInt const& number);
        template <class T> ArbInt& op_add (T      const  number);
                           ArbInt& op_sub (ArbInt const& number);
        template <class T> ArbInt& op_sub (T      const  number);
        
        // Bit shift operator backends.
                           ArbInt& op_shl (ArbInt const& number);
        template <class T> ArbInt& op_shl (T      const  number);
                           ArbInt& op_shr (ArbInt const& number);
        template <class T> ArbInt& op_shr (T      const  number);
        
        // Comparison operator backends.
                           bool op_gt (ArbInt const& number) const;
        template <class T> bool op_gt (T      const  number) const;
                           bool op_ge (ArbInt const& number) const;
        template <class T> bool op_ge (T      const  number) const;
                           bool op_lt (ArbInt const& number) const;
        template <class T> bool op_lt (T      const  number) const;
                           bool op_le (ArbInt const& number) const;
        template <class T> bool op_le (T      const  number) const;
                           bool op_eq (ArbInt const& number) const;
        template <class T> bool op_eq (T      const  number) const;
                           bool op_ne (ArbInt const& number) const;
        template <class T> bool op_ne (T      const  number) const;
        
        // Bitwise operator backends.
                           ArbInt& op_bit_and (ArbInt const& number);
        template <class T> ArbInt& op_bit_and (T      const  number);
                           ArbInt& op_bit_ior (ArbInt const& number);
        template <class T> ArbInt& op_bit_ior (T      const  number);
                           ArbInt& op_bit_xor (ArbInt const& number);
        template <class T> ArbInt& op_bit_xor (T      const  number);
                           ArbInt& op_bit_cpm ();
        
        // Logical operator backends.
                           bool op_log_and (ArbInt const& number) const;
        template <class T> bool op_log_and (T      const  number) const;
                           bool op_log_ior (ArbInt const& number) const;
        template <class T> bool op_log_ior (T      const  number) const;
        
        // Return information about this number.
        bool isEven () const;
        bool isZero () const;
        bool isOdd  () const;
        
        // Raise this number to a power.
                           ArbInt pow (ArbInt const& exp);
        template <class T> ArbInt pow (T      const  exp);
        
        // Find a root of this number.
        ArbInt root (ArbInt const& root, ArbInt& divisor, ArbInt& remainder);
        
        /*********************************************************************/
        // Static function members.
        
        // Return the maximum string input base.
        static value_type max_input_base ();
      
      /*
        * Private members.
        */
      private:
        
        /*********************************************************************/
        // Typedefs.
        
        typedef char          _StrChrT; // String character.
        typedef unsigned char _NumChrT; // Numeric character.
        
        typedef std::vector<_NumChrT> _DigStrT; // String of character
                                                // digits.
        typedef std::vector<_DigT>    _DigsT;   // String of native
                                                // digits.
        
        typedef ReferencePointer<_DigsT> _DataPT; // Pointer to data.
        
        /*********************************************************************/
        // Data types.
        
        // Directions.
        enum _Dir { _DIR_L, _DIR_R };
        
        /*********************************************************************/
        // Data members.
        
        // The number.
        _DataPT _digits;
        
        // The default base of this number.
        _DigT _base;
        
        /*********************************************************************/
        // Static data members.
        
        static bool s_initialized;
        
        static int   s_digitbits; // Number of bits in a digit.
        static _DigT s_digitbase; // Base of native digits.
        static _DigT s_bitmask;   // Bitmask of digits.
        
        static _NumChrT s_numodigits; // Number of output digits.
        static _StrChrT s_odigits[];  // Output digits.
        
        static _NumChrT              s_numidigits; // Number of input digits.
        static std::vector<_NumChrT> s_idigits;    // Input digits.
        
        /*********************************************************************/
        // Function members.
        
        // Common initialization tasks.
        void _init ();
        
        // Perform carry or borrow.
        ArbInt& _carry  (_DigsT::size_type start);
        ArbInt& _borrow (_DigsT::size_type start);
        
        // Trim insignificant zeros.
        ArbInt& _trimZeros ();
        
        // Bit shift this number.
        ArbInt& _shift (ArbInt const& bits, _Dir const direction);
        
        /*********************************************************************/
        // Static function members.
        
        // Class initialization.
        static void s_classInit ();
        
        // Trim zeros from a given container.
        template <class T> static void s_trimZerosB (T& c);
        template <class T> static void s_trimZerosE (T& c);
        template <class T> static void s_trimZeros  (T& c);
        
        // Do long division on a given container. Returns the remainder.
        template <class DivndT, class DivorT> static DivorT s_longDiv (DivndT& divnd, DivorT const divor, value_type const base);
        
        // Convert a container from one base to another.
        template <class FT, class TT> static void s_baseConv (FT const& from, value_type const frombase, TT& to, value_type const tobase);
        
        // Validate a base.
        static void s_validateBase (value_type const base);
      
    };
    
  };
  
  /***************************************************************************
   * Global operators.
   ***************************************************************************/
  
  // Stream I/O operators.
  std::ostream& operator << (std::ostream& l, DAC::ArbInt const& r);
  std::istream& operator >> (std::istream& l, DAC::ArbInt&       r);
  
  // Arithmetic operators.
                     DAC::ArbInt operator * (DAC::ArbInt const& l, DAC::ArbInt const& r);
  template <class T> DAC::ArbInt operator * (DAC::ArbInt const& l, T           const  r);
  template <class T> DAC::ArbInt operator * (T           const  l, DAC::ArbInt const& r);
                     DAC::ArbInt operator / (DAC::ArbInt const& l, DAC::ArbInt const& r);
  template <class T> DAC::ArbInt operator / (DAC::ArbInt const& l, T           const  r);
  template <class T> DAC::ArbInt operator / (T           const  l, DAC::ArbInt const& r);
                     DAC::ArbInt operator % (DAC::ArbInt const& l, DAC::ArbInt const& r);
  template <class T> DAC::ArbInt operator % (DAC::ArbInt const& l, T           const  r);
  template <class T> DAC::ArbInt operator % (T           const  l, DAC::ArbInt const& r);
                     DAC::ArbInt operator + (DAC::ArbInt const& l, DAC::ArbInt const& r);
  template <class T> DAC::ArbInt operator + (DAC::ArbInt const& l, T           const  r);
  template <class T> DAC::ArbInt operator + (T           const  l, DAC::ArbInt const& r);
                     DAC::ArbInt operator - (DAC::ArbInt const& l, DAC::ArbInt const& r);
  template <class T> DAC::ArbInt operator - (DAC::ArbInt const& l, T           const  r);
  template <class T> DAC::ArbInt operator - (T           const  l, DAC::ArbInt const& r);
  
  // Bit shift operators.
                     DAC::ArbInt operator << (DAC::ArbInt const& l, DAC::ArbInt const& r);
  template <class T> DAC::ArbInt operator << (DAC::ArbInt const& l, T           const  r);
  template <class T> DAC::ArbInt operator << (T           const  l, DAC::ArbInt const& r);
                     DAC::ArbInt operator >> (DAC::ArbInt const& l, DAC::ArbInt const& r);
  template <class T> DAC::ArbInt operator >> (DAC::ArbInt const& l, T           const  r);
  template <class T> DAC::ArbInt operator >> (T           const  l, DAC::ArbInt const& r);
  
  // Comparsion operators.
                     bool operator >  (DAC::ArbInt const& l, DAC::ArbInt const& r);
  template <class T> bool operator >  (DAC::ArbInt const& l, T           const  r);
  template <class T> bool operator >  (T           const  l, DAC::ArbInt const& r);
                     bool operator >= (DAC::ArbInt const& l, DAC::ArbInt const& r);
  template <class T> bool operator >= (DAC::ArbInt const& l, T           const  r);
  template <class T> bool operator >= (T           const  l, DAC::ArbInt const& r);
                     bool operator <  (DAC::ArbInt const& l, DAC::ArbInt const& r);
  template <class T> bool operator <  (DAC::ArbInt const& l, T           const  r);
  template <class T> bool operator <  (T           const  l, DAC::ArbInt const& r);
                     bool operator <= (DAC::ArbInt const& l, DAC::ArbInt const& r);
  template <class T> bool operator <= (DAC::ArbInt const& l, T           const  r);
  template <class T> bool operator <= (T           const  l, DAC::ArbInt const& r);
                     bool operator == (DAC::ArbInt const& l, DAC::ArbInt const& r);
  template <class T> bool operator == (DAC::ArbInt const& l, T           const  r);
  template <class T> bool operator == (T           const  l, DAC::ArbInt const& r);
                     bool operator != (DAC::ArbInt const& l, DAC::ArbInt const& r);
  template <class T> bool operator != (DAC::ArbInt const& l, T           const  r);
  template <class T> bool operator != (T           const  l, DAC::ArbInt const& r);
  
  // Bitwise operators.
                     DAC::ArbInt operator & (DAC::ArbInt const& l, DAC::ArbInt const& r);
  template <class T> DAC::ArbInt operator & (DAC::ArbInt const& l, T           const  r);
  template <class T> DAC::ArbInt operator & (T           const  l, DAC::ArbInt const& r);
                     DAC::ArbInt operator | (DAC::ArbInt const& l, DAC::ArbInt const& r);
  template <class T> DAC::ArbInt operator | (DAC::ArbInt const& l, T           const  r);
  template <class T> DAC::ArbInt operator | (T           const  l, DAC::ArbInt const& r);
                     DAC::ArbInt operator ^ (DAC::ArbInt const& l, DAC::ArbInt const& r);
  template <class T> DAC::ArbInt operator ^ (DAC::ArbInt const& l, T           const  r);
  template <class T> DAC::ArbInt operator ^ (T           const  l, DAC::ArbInt const& r);
  
  // Logical operators.
                     bool operator && (DAC::ArbInt const& l, DAC::ArbInt const& r);
  template <class T> bool operator && (DAC::ArbInt const& l, T           const  r);
  template <class T> bool operator && (T           const  l, DAC::ArbInt const& r);
                     bool operator || (DAC::ArbInt const& l, DAC::ArbInt const& r);
  template <class T> bool operator || (DAC::ArbInt const& l, T           const  r);
  template <class T> bool operator || (T           const  l, DAC::ArbInt const& r);
  
  /***************************************************************************
   * Error declarations.
   ***************************************************************************/
  namespace DAC {
    
    // Errors.
    namespace ArbIntErrors {
      class Base           : public Exception { public: virtual char const* what () const throw(); };
      class BadFormat      : public Base      {
        public:
          virtual char const* what () const throw();
          virtual BadFormat& Problem  (char const*                   const problem)  throw();
          virtual BadFormat& Position (std::string::size_type        const position) throw();
          virtual BadFormat& Number   (ConstReferencePointer<std::string>& number)   throw();
        protected:
          char const* _problem;
          std::string::size_type _position;
          ConstReferencePointer<std::string> _number;
      };
      class Negative       : public Base      { public: virtual char const* what () const throw(); };
      class Overrun        : public Base      { public: virtual char const* what () const throw(); };
      class DivByZero      : public Base      { public: virtual char const* what () const throw(); };
      class ScalarOverflow : public Base      { public: virtual char const* what () const throw(); };
      class BaseOutOfRange : public Base      { public: virtual char const* what () const throw(); };
      class RootTooLarge   : public Base      { public: virtual char const* what () const throw(); };
    };
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  namespace DAC {
    
    // Errors.
    namespace ArbIntErrors {
      inline char const* Base::what           () const throw() { return "Undefined error in ArbInt.";                                                                                                                                         }
      inline char const* BadFormat::what      () const throw() { return (std::string(_problem) + " at position " + DAC::toString((SafeInteger<std::string::size_type>(_position) + 1).Value()) + " in number \"" + *_number + "\".").c_str(); }
      inline BadFormat&  BadFormat::Problem   (char const*                   const problem)  throw() { _problem  = problem;  return *this; }
      inline BadFormat&  BadFormat::Position  (std::string::size_type        const position) throw() { _position = position; return *this; }
      inline BadFormat&  BadFormat::Number    (ConstReferencePointer<std::string>& number)   throw() { _number   = number;   return *this; }
      inline char const* Negative::what       () const throw() { return "Attempted to set ArbInt to a negative number.";                                                                                                                      }
      inline char const* Overrun::what        () const throw() { return "Instruction overruns end of container.";                                                                                                                             }
      inline char const* DivByZero::what      () const throw() { return "Attempt to divide by zero.";                                                                                                                                         }
      inline char const* ScalarOverflow::what () const throw() { return "ArbInt overflows scalar type.";                                                                                                                                      }
      inline char const* BaseOutOfRange::what () const throw() { return "Requested or specified base is out of range. Base minimum is base 2, base maximum is 2(bits in target container value type/2)-1.";                                   }
      inline char const* RootTooLarge::what   () const throw() { return "Cannot find a root that large.";                                                                                                                                     }
    };
    
    /*************************************************************************/
    // Function members.
    
    // Conversion constructor.
    template <class T> ArbInt::ArbInt (T const number) {
      
      // Call common init.
      _init();
      
      // Set the number.
      set(number);
      
    }
    
    // Unary sign operators.
    inline int ArbInt::operator + () const { return Value<int>();    }
    inline int ArbInt::operator - () const { return -(Value<int>()); }
    
    // Increment / decrement operators.
    inline ArbInt& ArbInt::operator ++ ()    { return op_add(ArbInt(1));                               }
    inline ArbInt  ArbInt::operator ++ (int) { ArbInt retval(*this); op_add(ArbInt(1)); return retval; }
    inline ArbInt& ArbInt::operator -- ()    { return op_sub(ArbInt(1));                               }
    inline ArbInt  ArbInt::operator -- (int) { ArbInt retval(*this); op_sub(ArbInt(1)); return retval; }
    
    // Bitwise compliment.
    inline ArbInt ArbInt::operator ~ () const { return ArbInt(*this).op_bit_cpm(); }
    
    // Assignment operator.
                       inline ArbInt& ArbInt::operator = (ArbInt      const& number) { return set(number); }
                       inline ArbInt& ArbInt::operator = (std::string const& number) { return set(number); }
    template <class T> inline ArbInt& ArbInt::operator = (T           const  number) { return set(number); }
    
    // Arithmetic assignment operators.
                       inline ArbInt& ArbInt::operator *= (ArbInt const& number) { return op_mul(number); }
    template <class T> inline ArbInt& ArbInt::operator *= (T      const  number) { return op_mul(number); }
                       inline ArbInt& ArbInt::operator /= (ArbInt const& number) { return op_div(number); }
    template <class T> inline ArbInt& ArbInt::operator /= (T      const  number) { return op_div(number); }
                       inline ArbInt& ArbInt::operator %= (ArbInt const& number) { return op_mod(number); }
    template <class T> inline ArbInt& ArbInt::operator %= (T      const  number) { return op_mod(number); }
                       inline ArbInt& ArbInt::operator += (ArbInt const& number) { return op_add(number); }
    template <class T> inline ArbInt& ArbInt::operator += (T      const  number) { return op_add(number); }
                       inline ArbInt& ArbInt::operator -= (ArbInt const& number) { return op_sub(number); }
    template <class T> inline ArbInt& ArbInt::operator -= (T      const  number) { return op_sub(number); }
    
    // Bit shift assignment operators.
                       inline ArbInt& ArbInt::operator <<= (ArbInt const& number) { return op_shl(number); }
    template <class T> inline ArbInt& ArbInt::operator <<= (T      const  number) { return op_shl(number); }
                       inline ArbInt& ArbInt::operator >>= (ArbInt const& number) { return op_shr(number); }
    template <class T> inline ArbInt& ArbInt::operator >>= (T      const  number) { return op_shr(number); }
    
    // Bitwise assignment operators.
                       inline ArbInt& ArbInt::operator &= (ArbInt const& number) { return op_bit_and(number); }
    template <class T> inline ArbInt& ArbInt::operator &= (T      const  number) { return op_bit_and(number); }
                       inline ArbInt& ArbInt::operator |= (ArbInt const& number) { return op_bit_ior(number); }
    template <class T> inline ArbInt& ArbInt::operator |= (T      const  number) { return op_bit_ior(number); }
                       inline ArbInt& ArbInt::operator ^= (ArbInt const& number) { return op_bit_xor(number); }
    template <class T> inline ArbInt& ArbInt::operator ^= (T      const  number) { return op_bit_xor(number); }
    
    // Get the base of this number.
    inline ArbInt::value_type ArbInt::Base () const { return _base; }
    
    // Get the value of this number.
    template <class T> T ArbInt::Value () const {
      SafeInteger<T> retval;
      for (typename _DigsT::iterator i = _digits->begin(); i != _digits->end(); ++i) {
        try {
          retval += SafeInteger<_DigT>(*i) * rppower(SafeInteger<_DigT>(s_digitbase), (i - _digits->begin()));
        } catch (SafeIntegerErrors::Overflow) {
          throw ArbIntErrors::ScalarOverflow();
        }
      }
      return retval.Value();
    }
    
    // Set the base of this number.
    inline ArbInt& ArbInt::Base (value_type const base) {
      if ((base < 2) || (base > s_digitbase)) {
        throw ArbIntErrors::BaseOutOfRange();
      }
      _base = base;
      return *this;
    }
    
    // Set the value of this number.
    template <class T> inline ArbInt& ArbInt::Value (T const number) { return set(number); }
    
    // Set this number from another ArbInt.
    inline ArbInt& ArbInt::set (ArbInt const& number) { _digits = number._digits; return *this; }
    
    // Set this number from another number.
    template <class T> ArbInt& ArbInt::set (T const number) {
      
      // Work area.
      _DataPT new_digits(new _DigsT);
      
      // Only something to set if the number is not zero.
      if (number != 0) {
        
        // If number is less than zero, throw an error.
        if (number < 0) {
          throw ArbIntErrors::Negative();
        }
        
        // If the number is less than base, simply set it.
        SafeInteger<T> tmp_number(number);
        if (tmp_number < s_digitbase) {
          
          // Easy :)
          new_digits->push_back(SafeInteger<_DigT>(tmp_number.Value()).Value());
          
        // Otherwise, full conversion.
        } else {
        
          // Convert with repeated division.
          while (tmp_number > 0) {
            new_digits->push_back(SafeInteger<_DigT>((tmp_number % s_digitbase).Value()).Value());
            tmp_number /= s_digitbase;
          }
          
        }
        
      }
      
      // Swap in the new digits and return.
      _digits = new_digits;
      return *this;
      
    }
    
    // Push a string onto the back of this number.
                       inline ArbInt& ArbInt::push_back (ArbInt const& number) { return push_back(ArbInt(number).Base(_base).toString()); }
    template <class T> inline ArbInt& ArbInt::push_back (T      const  number) { return push_back(DAC::toString(number));                 }
    
    // Shift left, shift right.
    inline ArbInt& ArbInt::op_shl (ArbInt const& number) { ArbInt retval(*this, true); retval._shift(number, _DIR_L); return copy(retval); }
    inline ArbInt& ArbInt::op_shr (ArbInt const& number) { ArbInt retval(*this, true); retval._shift(number, _DIR_R); return copy(retval); }
    
    // Comparison operators.
    inline bool ArbInt::op_ge (ArbInt const& number) const { return (op_gt(number) || op_eq(number));   }
    inline bool ArbInt::op_lt (ArbInt const& number) const { return number.op_gt(*this);                }
    inline bool ArbInt::op_le (ArbInt const& number) const { return (op_lt(number) || op_eq(number));   }
    inline bool ArbInt::op_eq (ArbInt const& number) const { return (!op_gt(number) && !op_lt(number)); }
    inline bool ArbInt::op_ne (ArbInt const& number) const { return (op_gt(number) || op_lt(number));   }
    
    // Logical operators.
    inline bool ArbInt::op_log_and (ArbInt const& number) const { return (!isZero() && !number.isZero()); }
    inline bool ArbInt::op_log_ior (ArbInt const& number) const { return (!isZero() || !number.isZero()); }
    
    // Tests if this number is equal to zero.
    inline bool ArbInt::isZero () const { return (_digits->size() == 0); }
    
    // Tests if this number is odd or even.
    inline bool ArbInt::isOdd  () const { return (!isZero() && (_digits->front() & 1)); }
    inline bool ArbInt::isEven () const { return !isOdd();                              }
    
    // Placeholder for automatic pow conversion.
    template <class T> inline ArbInt pow (T const exp) { return pow(ArbInt(exp)); }
    
    // Return the maximum string input base.
    inline ArbInt::value_type ArbInt::max_input_base () { return SafeInteger<value_type>(s_numidigits).Value(); }
    
    // Trim insignificant zeros.
    inline ArbInt& ArbInt::_trimZeros () { s_trimZerosE(*_digits); return *this; }
    
    // Trim leading and trailing zeros from a given container.
    template <class T> void ArbInt::s_trimZeros (T& c) { s_trimZerosB(c); s_trimZerosE(c); }
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  // Stream I/O operators.
  inline std::ostream& operator << (std::ostream& l, DAC::ArbInt const& r) { l << r.toString();                                  return l; }
  inline std::istream& operator >> (std::istream& l, DAC::ArbInt&       r) { std::string input; std::cin >> input; r.set(input); return l; }
  
  // Arithmetic operators.
  inline DAC::ArbInt operator * (DAC::ArbInt const& l, DAC::ArbInt const& r) { return DAC::ArbInt(l).op_mul(r); }
  inline DAC::ArbInt operator / (DAC::ArbInt const& l, DAC::ArbInt const& r) { return DAC::ArbInt(l).op_div(r); }
  inline DAC::ArbInt operator % (DAC::ArbInt const& l, DAC::ArbInt const& r) { return DAC::ArbInt(l).op_mod(r); }
  inline DAC::ArbInt operator + (DAC::ArbInt const& l, DAC::ArbInt const& r) { return DAC::ArbInt(l).op_add(r); }
  inline DAC::ArbInt operator - (DAC::ArbInt const& l, DAC::ArbInt const& r) { return DAC::ArbInt(l).op_sub(r); }
  
  // Bit shift operators.
  inline DAC::ArbInt operator << (DAC::ArbInt const& l, DAC::ArbInt const& r) { return DAC::ArbInt(l).op_shl(r); }
  inline DAC::ArbInt operator >> (DAC::ArbInt const& l, DAC::ArbInt const& r) { return DAC::ArbInt(l).op_shr(r); }
  
  // Comparsion operators.
  inline bool operator >  (DAC::ArbInt const& l, DAC::ArbInt const& r) { return l.op_gt(r); }
  inline bool operator >= (DAC::ArbInt const& l, DAC::ArbInt const& r) { return l.op_ge(r); }
  inline bool operator <  (DAC::ArbInt const& l, DAC::ArbInt const& r) { return l.op_lt(r); }
  inline bool operator <= (DAC::ArbInt const& l, DAC::ArbInt const& r) { return l.op_le(r); }
  inline bool operator == (DAC::ArbInt const& l, DAC::ArbInt const& r) { return l.op_eq(r); }
  inline bool operator != (DAC::ArbInt const& l, DAC::ArbInt const& r) { return l.op_ne(r); }
  
  // Bitwise operators.
  inline DAC::ArbInt operator & (DAC::ArbInt const& l, DAC::ArbInt const& r) { return DAC::ArbInt(l).op_bit_and(r); }
  inline DAC::ArbInt operator | (DAC::ArbInt const& l, DAC::ArbInt const& r) { return DAC::ArbInt(l).op_bit_ior(r); }
  inline DAC::ArbInt operator ^ (DAC::ArbInt const& l, DAC::ArbInt const& r) { return DAC::ArbInt(l).op_bit_xor(r); }
  
  // Logical operators.
  inline bool operator && (DAC::ArbInt const& l, DAC::ArbInt const& r) { return l.op_log_and(r); }
  inline bool operator || (DAC::ArbInt const& l, DAC::ArbInt const& r) { return l.op_log_ior(r); }
  
#endif
