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
  #include <algorithm>
  
  // Internal includes.
  #include "SafeInteger.hxx"
  #include "ReferencePointer.hxx"
  #include "toString.hxx"
  #include "rppower.hxx"
  #include "logBase.hxx"
  
  // Namespace wrapping.
  namespace DAC {
    
    /*************************************************************************
      * ArbInt
      ************************************************************************
      * This is an integer of an arbitrary number of digits.
      ************************************************************************/
    template <class T> class ArbInt {
      
      /*
        * Public members.
        */
      public:
        
        /*********************************************************************/
        // Typedefs.
        
        typedef T value_type;
        
        /*********************************************************************/
        // Function members.
        
        // Default constructor.
        ArbInt ();
        
        // Copy constructor.
        ArbInt (ArbInt const& number, bool const copynow = false);
        
        // Conversion constructor.
                            ArbInt (std::string const& number);
        template <class FT> ArbInt (FT          const  number);
        
        // Boolean conversion operator.
        operator bool () const;
        
        // Unary sign operators.
        int operator + () const;
        int operator - () const;
        
        // Increment / decrement operators.
        ArbInt& operator ++ ();
        ArbInt  operator ++ (int);
        ArbInt& operator -- ();
        ArbInt  operator -- (int);
        
        // Not operator.
        bool operator ! () const;
        
        // Bitwise compliment.
        ArbInt operator ~ () const;
        
        // Assignment operator.
                            ArbInt& operator = (std::string const& number);
                            ArbInt& operator = (ArbInt      const& number);
        template <class FT> ArbInt& operator = (FT          const  number);
        
        // Arithmetic assignment operators.
        ArbInt& operator *= (ArbInt const& number);
        ArbInt& operator /= (ArbInt const& number);
        ArbInt& operator %= (ArbInt const& number);
        ArbInt& operator += (ArbInt const& number);
        ArbInt& operator -= (ArbInt const& number);
        
        // Bit shift assignment operators.
        ArbInt& operator <<= (ArbInt const& number);
        ArbInt& operator >>= (ArbInt const& number);
        
        // Bitwise assignment operators.
        ArbInt& operator &= (ArbInt const& number);
        ArbInt& operator |= (ArbInt const& number);
        ArbInt& operator ^= (ArbInt const& number);
        
        // Reset to just-constructed state.
        ArbInt& clear ();
        
        // Copy another number.
        ArbInt& copy (ArbInt const& number, bool const copynow = false);
        
        // Accessors.
                            T       Base  ()                const;
                            ArbInt& Base  (T const base);
        template <class TT> TT      Value ()                const;
        template <class TT> ArbInt& Value (TT const number);
        
        // Set this number with a string.
                            ArbInt<T>& set (std::string const& number);
                            ArbInt<T>& set (ArbInt      const& number);
        template <class FT> ArbInt<T>& set (FT          const  number);
        
        // Return a string of this number.
        std::string toString () const;
        
        // Tests if this number is equal to zero.
        bool isZero () const;
        
        // Arithmetic operator backends.
        ArbInt& op_mul (ArbInt const& number);
        ArbInt& op_div (ArbInt const& number, ArbInt* remainder = 0);
        ArbInt& op_mod (ArbInt const& number);
        ArbInt& op_add (ArbInt const& number);
        ArbInt& op_sub (ArbInt const& number);
        
        // Bit shift operator backends.
        ArbInt& op_shl (ArbInt const& number);
        ArbInt& op_shr (ArbInt const& number);
        
        // Comparison operator backends.
        bool op_gt (ArbInt const& number) const;
        bool op_ge (ArbInt const& number) const;
        bool op_lt (ArbInt const& number) const;
        bool op_le (ArbInt const& number) const;
        bool op_eq (ArbInt const& number) const;
        bool op_ne (ArbInt const& number) const;
        
        // Bitwise operator backends.
        ArbInt& op_bit_and (ArbInt const& number);
        ArbInt& op_bit_ior (ArbInt const& number);
        ArbInt& op_bit_xor (ArbInt const& number);
        ArbInt& op_bit_cpm ();
        
        // Logical operator backends.
        bool op_log_and (ArbInt const& number) const;
        bool op_log_ior (ArbInt const& number) const;
        
        // Raise this number to a power.
        ArbInt& pow (ArbInt const& exp);
        
        /*********************************************************************/
        // Static function members.
        
        // Return the maximum string input base.
        static T max_input_base ();
      
      /*
        * Private members.
        */
      private:
        
        /*********************************************************************/
        // Typedefs.
        
        typedef char          _StrChrT; // String character.
        typedef unsigned char _NumChrT; // Numeric character.
        typedef T             _DigT;    // Native digit.
        
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
        T _base;
        
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
        ArbInt& _carry  (typename _DigsT::size_type start);
        ArbInt& _borrow (typename _DigsT::size_type start);
        
        // Trim insignificant zeros.
        ArbInt& _trimZeros ();
        
        // Bit shift this number.
        ArbInt& _shift (ArbInt<T> const& bits, _Dir const direction);
        
        /*********************************************************************/
        // Static function members.
        
        // Class initialization.
        static void s_classInit ();
        
        // Trim zeros from a given container.
        template <class CT> static void s_trimZerosB (CT& c);
        template <class CT> static void s_trimZerosE (CT& c);
        template <class CT> static void s_trimZeros  (CT& c);
        
        // Do long division on a given container. Returns the remainder.
        template <class DivndT, class DivorT> static DivorT s_longDiv (DivndT& divnd, DivorT const divor, T const base);
        
        // Convert a container from one base to another.
        template <class FT, class TT> static void s_baseConv (FT const& from, T const frombase, TT& to, T const tobase);
        
        // Validate a base.
        static void s_validateBase (T const base);
      
    };
    
  };
  
  /***************************************************************************
   * Global operators.
   ***************************************************************************/
  
  // Stream I/O operators.
  template <class T> std::ostream& operator << (std::ostream& l, DAC::ArbInt<T> const& r);
  template <class T> std::istream& operator >> (std::istream& l, DAC::ArbInt<T>&       r);
  
  // Arithmetic operators.
  template <class T> DAC::ArbInt<T> operator * (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  template <class T> DAC::ArbInt<T> operator / (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  template <class T> DAC::ArbInt<T> operator % (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  template <class T> DAC::ArbInt<T> operator + (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  template <class T> DAC::ArbInt<T> operator - (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  
  // Bit shift operators.
  template <class T> DAC::ArbInt<T> operator << (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  template <class T> DAC::ArbInt<T> operator >> (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  
  // Comparsion operators.
  template <class T> bool operator >  (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  template <class T> bool operator >= (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  template <class T> bool operator <  (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  template <class T> bool operator <= (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  template <class T> bool operator == (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  template <class T> bool operator != (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  
  // Bitwise operators.
  template <class T> DAC::ArbInt<T> operator & (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  template <class T> DAC::ArbInt<T> operator | (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  template <class T> DAC::ArbInt<T> operator ^ (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  
  // Logical operators.
  template <class T> bool operator && (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  template <class T> bool operator || (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  
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
    };
    
    /*************************************************************************/
    // Static member initialization.
    
    template <class T> int                       ArbInt<T>::s_digitbits = 0;
    template <class T> typename ArbInt<T>::_DigT ArbInt<T>::s_digitbase = 0;
    template <class T> typename ArbInt<T>::_DigT ArbInt<T>::s_bitmask   = 0;
    
    template <class T> typename ArbInt<T>::_NumChrT ArbInt<T>::s_numodigits = 36;
    template <class T> typename ArbInt<T>::_StrChrT ArbInt<T>::s_odigits[]  = {
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
      'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
      'U', 'V', 'W', 'X', 'Y', 'Z'
    };
    template <class T> typename ArbInt<T>::_NumChrT              ArbInt<T>::s_numidigits = 0;
    template <class T> std::vector<typename ArbInt<T>::_NumChrT> ArbInt<T>::s_idigits;
    
    template <class T> bool ArbInt<T>::s_initialized = false;
    
    /*************************************************************************/
    // Function members.
    
    // Default constructor.
    template <class T> ArbInt<T>::ArbInt () {
      
      // Call common init.
      _init();
      
    }
    
    // Copy constructor.
    template <class T> ArbInt<T>::ArbInt (ArbInt<T> const& number, bool const copynow) {
      
      // Call common init.
      _init();
      
      // Set the number.
      copy(number, copynow);
      
    }
    
    // Conversion constructor.
    template <class T> ArbInt<T>::ArbInt (std::string const& number) {
      
      // Call common init.
      _init();
      
      // Set the number.
      set(number);
      
    }
    template <class T> template <class FT> ArbInt<T>::ArbInt (FT const number) {
      
      // Call common init.
      _init();
      
      // Set the number.
      set<FT>(number);
      
    }
    
    // Boolean conversion operator.
    template <class T> inline ArbInt<T>::operator bool () const { return !isZero(); }
    
    // Unary sign operators.
    template <class T> inline int ArbInt<T>::operator + () const { return Value<int>();    }
    template <class T> inline int ArbInt<T>::operator - () const { return -(Value<int>()); }
    
    // Increment / decrement operators.
    template <class T> inline ArbInt<T>& ArbInt<T>::operator ++ ()    { return op_add(ArbInt<T>(1));                                  }
    template <class T> inline ArbInt<T>  ArbInt<T>::operator ++ (int) { ArbInt<T> retval(*this); op_add(ArbInt<T>(1)); return retval; }
    template <class T> inline ArbInt<T>& ArbInt<T>::operator -- ()    { return op_sub(ArbInt<T>(1));                                  }
    template <class T> inline ArbInt<T>  ArbInt<T>::operator -- (int) { ArbInt<T> retval(*this); op_sub(ArbInt<T>(1)); return retval; }
    
    // Negation operator.
    template <class T> inline bool ArbInt<T>::operator ! () const { return isZero(); }
    
    // Bitwise compliment.
    template <class T> inline ArbInt<T> ArbInt<T>::operator ~ () const { return ArbInt<T>(*this).op_bit_cpm(); }
    
    // Assignment operator.
    template <class T>                     inline ArbInt<T>& ArbInt<T>::operator = (std::string const& number) { return set(number);     }
    template <class T>                     inline ArbInt<T>& ArbInt<T>::operator = (ArbInt<T>   const& number) { return copy(number);    }
    template <class T> template <class FT> inline ArbInt<T>& ArbInt<T>::operator = (FT          const  number) { return set<FT>(number); }
    
    // Arithmetic assignment operators.
    template <class T> inline ArbInt<T>& ArbInt<T>::operator *= (ArbInt<T> const& number) { return op_mul(number); }
    template <class T> inline ArbInt<T>& ArbInt<T>::operator /= (ArbInt<T> const& number) { return op_div(number); }
    template <class T> inline ArbInt<T>& ArbInt<T>::operator %= (ArbInt<T> const& number) { return op_mod(number); }
    template <class T> inline ArbInt<T>& ArbInt<T>::operator += (ArbInt<T> const& number) { return op_add(number); }
    template <class T> inline ArbInt<T>& ArbInt<T>::operator -= (ArbInt<T> const& number) { return op_sub(number); }
    
    // Bit shift assignment operators.
    template <class T> inline ArbInt<T>& ArbInt<T>::operator <<= (ArbInt<T> const& number) { return op_shl(number); }
    template <class T> inline ArbInt<T>& ArbInt<T>::operator >>= (ArbInt<T> const& number) { return op_shr(number); }
    
    // Bitwise assignment operators.
    template <class T> inline ArbInt<T>& ArbInt<T>::operator &= (ArbInt<T> const& number) { return op_bit_and(number); }
    template <class T> inline ArbInt<T>& ArbInt<T>::operator |= (ArbInt<T> const& number) { return op_bit_ior(number); }
    template <class T> inline ArbInt<T>& ArbInt<T>::operator ^= (ArbInt<T> const& number) { return op_bit_xor(number); }
    
    // Reset to just-constructed state.
    template <class T> ArbInt<T>& ArbInt<T>::clear () {
      
      // Create a new vector, this preserves COW behavior.
      _digits = new _DigsT;
      
      // Reset the default base.
      _base = 10;
      
      // Return self.
      return *this;
      
    }
    
    // Make a copy of another number.
    template <class T> ArbInt<T>& ArbInt<T>::copy (ArbInt<T> const& number, bool const copynow) {
      
      // Set the new digits. Make a copy if instructed to do so, otherwise
      // just make a reference and use COW.
      _DataPT new_digits;
      if (copynow) {
        new_digits = new _DigsT(*(number._digits));
      } else {
        new_digits = number._digits;
      }
      
      // Set the default base of number.
      _base = number._base;
      
      // Swap in the new digits.
      _digits = new_digits;
      
      // Return self.
      return *this;
      
    }
    
    // Returns or sets the default base of this number.
    template <class T> inline T          ArbInt<T>::Base ()             const { return _base;               }
    template <class T> inline ArbInt<T>& ArbInt<T>::Base (T const base)       {
      if ((base < 2) || (base > s_digitbase)) {
        throw ArbIntErrors::BaseOutOfRange();
      }
      _base = base;
      return *this;
    }
    
    // Returns or sets the value of this number.
    template <class T> template <class TT> TT ArbInt<T>::Value () const {
      SafeInteger<TT> retval;
      for (typename _DigsT::iterator i = _digits->begin(); i != _digits->end(); ++i) {
        try {
          retval += SafeInteger<_DigT>(*i) * rppower(SafeInteger<_DigT>(s_digitbase), (i - _digits->begin()));
        } catch (SafeIntegerErrors::Overflow) {
          throw ArbIntErrors::ScalarOverflow();
        }
      }
      return retval.Value();
    }
    template <class T> template <class TT> ArbInt<T>& ArbInt<T>::Value (TT const number) { return set(number); }
    
    // Set this number with a string.
    template <class T> ArbInt<T>& ArbInt<T>::set (std::string const& number) {
      
      // Load the number into this for exception safety and COW.
      _DataPT new_digits(new _DigsT);
      
      // Hold the number in case an error needs to be thrown.
      ConstReferencePointer<std::string> tmp_number(new std::string(number));
      
      // Parser will load data into here.
      _DigStrT num;
      
      // Parse the number.
      for (typename std::string::size_type i = 0; i != number.length(); ++i) {
        
        // Get the value of this digit.
        _NumChrT digval = s_idigits[number[i]];
        
        // Make sure this digit is within the number base.
        if ((digval >= SafeInteger<T>(_base).Value() || (digval == std::numeric_limits<_NumChrT>::max()))) {
          throw ArbIntErrors::BadFormat().Problem("Unrecognized character").Position(i).Number(tmp_number);
        }
        
        // Add the digit to the digit string.
        num.insert(num.begin(), digval);
        
      }
      
      // Trim insignificant zeros. This number is little-endian at this point.
      s_trimZerosE(num);
      
      // Convert to the native number base.
      s_baseConv(num, _base, *new_digits, s_digitbase);
      
      // The new number has been loaded successfully. Swap it in.
      _digits = new_digits;
      
      // We done, return the new number.
      return *this;
      
    }
    
    // Set this number from another ArbInt.
    template <class T> ArbInt<T>& ArbInt<T>::set (ArbInt<T> const& number) {
      
      // Make another reference to the number. ArbInt is COW, so the actual
      // copy will wait until a change is made.
      _digits = number._digits;
      
      // That's it.
      return *this;
      
    }
    
    // Set this number from another number.
    template <class T> template <class FT> ArbInt<T>& ArbInt<T>::set (FT const number) {
      
      // Work area.
      _DataPT new_digits(new _DigsT);
      
      // Only something to set if the number is not zero.
      if (number != 0) {
        
        // If number is less than zero, throw an error.
        if (number < 0) {
          throw ArbIntErrors::Negative();
        }
        
        // Convert with repeated division.
        SafeInteger<FT> tmp_number(number);
        while (tmp_number > 0) {
          new_digits->push_back((tmp_number % s_digitbase).Value());
          tmp_number /= s_digitbase;
        }
        
      }
      
      // Swap in the new digits and return.
      _digits = new_digits;
      return *this;
      
    }
    
    // Return a string of this number.
    template <class T> std::string ArbInt<T>::toString () const {
      
      // This is the string we will return.
      std::string retval;
      
      // Easy case of 0.
      if (_digits->size() == 0) {
        
        retval = "0";
        
      // Otherwise we have work to do.
      } else {
        
        // Convert to the output base.
        _DigsT num;
        s_baseConv(*_digits, s_digitbase, num, _base);
        
        // Load this into the string. If the base is greater than the number
        // of digits defined, output the raw numbers of each digit.
        if (SafeInteger<T>(_base) > s_numodigits) {
          for (typename _DigsT::reverse_iterator i = num.rbegin(); i != num.rend(); ++i) {
            retval += "'" + DAC::toString(*i) + "'";
            if (i != (num.rend() - 1)) {
              retval += ",";
            }
          }
        } else {
          for (typename _DigsT::reverse_iterator i = num.rbegin(); i != num.rend(); ++i) {
            retval += s_odigits[*i];
          }
        }
        
      }
      
      // String constructed, return it.
      return retval;
      
    }
    
    // Tests if this number is equal to zero.
    template <class T> inline bool ArbInt<T>::isZero () const { return (_digits->size() == 0); }
    
    // Multiply.
    template <class T> ArbInt<T>& ArbInt<T>::op_mul (ArbInt<T> const& number) {
      
      // Work area.
      ArbInt<T> retval;
      
      // If either number is zero, no multiplying to be done.
      if ((_digits->size() != 0) && (number._digits->size() != 0)) {
        
        // Create a digit product and make a reference its digits to reduce
        // typing.
        ArbInt<T> digproduct;
        
        // Multiply like 3rd grade.
        for (typename _DigsT::size_type i = 0; i != number._digits->size(); ++i) {
          
          // Get the product for a single digit.
          for (typename _DigsT::size_type j = 0; j != _digits->size(); ++j) {
            
            // Create a new digit in the digit product if necessary.
            if (digproduct._digits->size() == j) {
              digproduct._digits->push_back(0);
            }
            
            // Multiply into the digit product and carry.
            (*(digproduct._digits))[j] = (SafeInteger<_DigT>((*(digproduct._digits))[j]) + (SafeInteger<_DigT>((*(number._digits))[i]) * SafeInteger<_DigT>((*_digits)[j]))).Value();
            digproduct._carry(j);
            
          }
          
          // Offset this digit product and add it to the final product.
          digproduct._digits->insert(digproduct._digits->begin(), i, 0);
          retval.op_add(digproduct);
          digproduct.clear();
          
        }
        
      }
      
      // Swap in the result and return.
      _digits = retval._digits;
      return *this;
      
    }
    
    // Divide.
    template <class T> ArbInt<T>& ArbInt<T>::op_div (ArbInt<T> const& number, ArbInt<T>* remainder) {
      
      // If number is zero, throw error.
      if (number._digits->size() == 0) {
        throw ArbIntErrors::DivByZero();
      }
      
      // Work area.
      ArbInt<T> retval;
      
      // If number > this, result is zero, remainder is this.
      if (number > *this) {
        
        // Set the remainder.
        if (remainder != 0) {
          *remainder = *this;
        }
        
      // Otherwise we have work to do.
      } else {
        
        // Cache a copy of the high-order digit of the dividend.
        SafeInteger<_DigT> roughdivnd = number._digits->back();
        
        // Seed the group of digits we will be dividing, then iterate through
        // the divisor. diggroup is set to *this's last number._digits's size
        // digits. Since *this is at least as big as number, this will always
        // succeed. The loop iterates backwards from the least significant
        // digit added to diggroup to the end of _digits, although the
        // iterator is only ever referenced to add one to it.
        ArbInt<T> diggroup;
        *(diggroup._digits) = _DigsT(_digits->end() - number._digits->size(), _digits->end());
        for (typename _DigsT::reverse_iterator i = _digits->rbegin() + (number._digits->size() - 1); i != _digits->rend(); ++i) {
          
          // Guess and test. Guess is a single native digit so we can use the
          // computer to do some of our division for us. These need to be set
          // to zero at each iteration, possible cause of bug if they are not.
          SafeInteger<_DigT> guess = 0;
          ArbInt<T>          test  = 0;
          
          // Make sure that the digit group is greater than or equal to the
          // dividend. If not, the quotient for this digit is zero, move on.
          if (diggroup >= number) {
            
            // Make a guess at the quotient of this digit by dividing the
            // high-order digits. Initially the rough divisor is the high-
            // order digit of the digit group, but our guess needs to be based
            // on the number of digits the digit group has over our dividend.
            // The high-order digits of diggroup are added one at a time, and
            // moved up 1/2 of the native type's precision. Since the rough
            // dividend will never be over 1/2 of the native type's precision,
            // roughdivor can never overflow, since it will always spend at
            // least one iteration greater than roughdivnd but cannot be
            // greater than the type maximum.
            SafeInteger<_DigT> roughdivor = diggroup._digits->back();
            typename _DigsT::size_type j;
            for (j = 0; j != (diggroup._digits->size() - number._digits->size()); ++j) {
              roughdivor *= s_digitbase;
              roughdivor += (*(diggroup._digits))[diggroup._digits->size() - 1 - (j + 1)];
            }
            guess = roughdivor / roughdivnd;
              
            // Correct the guess. First create a test. Also create a floor
            // and ceiling for future guesses. Error in guesses is based on
            // the fact that we are only dividing the high order digits of
            // each number, so the worst-case scenario is that the low-order
            // digits are all at maximum or minimum. For the floor divisor,
            // assume that all untested digits are at the minimum by lowering
            // the divisor by s_digitbase^addl digits (necessary because carry
            // would cause high-order digit to change), and raise the dividend
            // by 1. For the ceiling divisor, raise it by s_digitbase^addl
            // and lower the dividend by 1.
            // *** CORRECTION ***
            // Now starting floor at 1 (guess must be at least 1 or we
            // wouldn't be here) and ceil at max(). The performance penalty
            // will be negligable since the first time we go over or under the
            // floor and ceil are corrected, those two possible missed guesses
            // should not take significantly more time than it would take to
            // ensure that floor and ceil do not overflow, and even with those
            // guarantees the better guesses may not even help significantly
            // since they will still almost always be wrong.
            test = number * ArbInt<T>(guess.Value());
            //SafeInteger<_DigT> guessfloor = (roughdivor - rppower(SafeInteger<_DigT>(s_digitbase), j)) / (roughdivnd + 1);
            //SafeInteger<_DigT> guessceil  = (roughdivor + rppower(SafeInteger<_DigT>(s_digitbase), j)) / ((roughdivnd == 1) ? 1 : (roughdivnd - 1));
            SafeInteger<_DigT> guessfloor = 1;
            SafeInteger<_DigT> guessceil  = std::numeric_limits<_DigT>::max();
            
            // Loop until the test is within th ecorrect range.
            while ((test > diggroup) || ((test + number) <= diggroup)) {
              
              // If the test is greater than the digit group, we need to lower
              // our guess. Lower it by moving halfway toward the floor. Also,
              // update the ceiling. We know that we are already over it, so
              // make the ceiling one less than the current guess.
              if (test > diggroup) {
                guessceil = guess - 1;
                guess     = guess - (((guess - guessfloor) >> 1) + 1);
                test      = number * ArbInt<T>(guess.Value());
              
              // If there is room for more dividends, we need to raise our
              // guess. Raise it halfway toward the ceiling. Update the floor,
              // it is at least one higher than our current guess.
              } else if ((test + number) <= diggroup) {
                guessfloor = guess + 1;
                guess      = guess + (((guessceil - guess) >> 1) + 1);
                test       = number * ArbInt<T>(guess.Value());
              }
              
            }
            
          }
          
          // The guess must be correct by this point. Add it to the quotient
          // and prepare for the next iteration by removing the current digit
          // from the digit group, shift the digit group up one digit, and add
          // the next digit of the divisor.
          retval._digits->insert(retval._digits->begin(), guess.Value());
          diggroup -= test;
          if (i != (_digits->rend() - 1)) {
            diggroup._digits->insert(diggroup._digits->begin(), *(i + 1));
          }
          
        }
        
        // Set the remainder if a container was given.
        if (remainder != 0) {
          *remainder = diggroup;
        }
        
      }
      
      // Clean up, move the result into place and return.
      retval._trimZeros();
      _digits = retval._digits;
      return *this;
      
    }
    
    // Modulo division.
    template <class T> ArbInt<T>& ArbInt<T>::op_mod (ArbInt<T> const& number) {
      
      // Work area.
      ArbInt<T> retval;
      ArbInt<T> quotient(*this);
      
      // Do division and return the remainder.
      quotient.op_div(number, &retval);
      _digits = retval._digits;
      return *this;
      
    }
    
    // Add.
    template <class T> ArbInt<T>& ArbInt<T>::op_add (ArbInt<T> const& number) {
      
      // Work area.
      ArbInt<T> retval(*this, true);
      
      // Add like kindergarten.
      for (typename _DigsT::size_type i = 0; i != number._digits->size(); ++i) {
        
        // Create a new digit if necessary.
        if (i == retval._digits->size()) {
          retval._digits->push_back(0);
        }
        
        // Add this digit and carry.
        (*(retval._digits))[i] = (SafeInteger<_DigT>((*(retval._digits))[i]) + SafeInteger<_DigT>((*(number._digits))[i])).Value();
        retval._carry(i);
        
      }
      
      // Move the result into place and return.
      _digits = retval._digits;
      return *this;
      
    }
    
    // Subtract.
    template <class T> ArbInt<T>& ArbInt<T>::op_sub (ArbInt<T> const& number) {
      
      // Work area.
      ArbInt<T> retval(*this, true);
      
      // These are unsigned numbers, so if number > this, throw an error.
      if (number > retval) {
        throw ArbIntErrors::Negative();
      }
      
      // Subtract like kindergarten.
      for (typename _DigsT::size_type i = 0; i != number._digits->size(); ++i) {
        
        // Borrow if necessary and subtract.
        if ((*(retval._digits))[i] < (*(number._digits))[i]) {
          retval._borrow(i);
        }
        (*(retval._digits))[i] = (SafeInteger<_DigT>((*(retval._digits))[i]) - SafeInteger<_DigT>((*(number._digits))[i])).Value();
        
      }
      
      // Trim zeros.
      retval._trimZeros();
      
      // Move the result into place and return.
      _digits = retval._digits;
      return *this;
      
    }
    
    // Shift left, shift right.
    template <class T> inline ArbInt<T>& ArbInt<T>::op_shl (ArbInt<T> const& number) { ArbInt<T> retval(*this, true); retval._shift(number, _DIR_L); return copy(retval); }
    template <class T> inline ArbInt<T>& ArbInt<T>::op_shr (ArbInt<T> const& number) { ArbInt<T> retval(*this, true); retval._shift(number, _DIR_R); return copy(retval); }
    
    // Greater than.
    template <class T> bool ArbInt<T>::op_gt (ArbInt<T> const& number) const {
      
      // Check sizes of containers first.
      if (_digits->size() > number._digits->size()) {
        return true;
      }
      if (_digits->size() < number._digits->size()) {
        return false;
      }
      if ((_digits->size() == 0) && (number._digits->size() == 0)) {
        return false;
      }
      
      // Step through each element looking for a difference, start at high-
      // order and work down.
      for (typename _DigsT::size_type i = 0; i != _digits->size(); ++i) {
        typename _DigsT::size_type j = (_digits->size() - 1) - i;
        if ((*(_digits))[j] > (*(number._digits))[j]) {
          return true;
        }
        if ((*(_digits))[j] < (*(number._digits))[j]) {
          return false;
        }
      }
      
      // No difference was found, is not greater.
      return false;
      
    }
    template <class T> inline bool ArbInt<T>::op_ge (ArbInt<T> const& number) const { return (op_gt(number) || op_eq(number));   }
    template <class T> inline bool ArbInt<T>::op_lt (ArbInt<T> const& number) const { return number.op_gt(*this);                }
    template <class T> inline bool ArbInt<T>::op_le (ArbInt<T> const& number) const { return (op_lt(number) || op_eq(number));   }
    template <class T> inline bool ArbInt<T>::op_eq (ArbInt<T> const& number) const { return (!op_gt(number) && !op_lt(number)); }
    template <class T> inline bool ArbInt<T>::op_ne (ArbInt<T> const& number) const { return (op_gt(number) || op_lt(number));   }
    
    // Bitwise AND.
    template <class T> ArbInt<T>& ArbInt<T>::op_bit_and (ArbInt<T> const& number) {
      
      // Work area.
      ArbInt<T> retval(*this, true);
      
      // Cut off digits longer than number, they will be 0 anyway.
      if (retval._digits->size() > number._digits->size()) {
        retval._digits->resize(number._digits->size());
      }
      
      // AND each digit.
      for (typename _DigsT::size_type i = 0; i != retval._digits->size(); ++i) {
        (*(retval._digits))[i] &= (*(number._digits))[i];
      }
      
      // Clean up.
      retval._trimZeros();
      
      // Move the result into place and return.
      _digits = retval._digits;
      return *this;
      
    }
    
    // Bitwise inclusive OR.
    template <class T> ArbInt<T>& ArbInt<T>::op_bit_ior (ArbInt<T> const& number) {
      
      // Work area.
      ArbInt<T> retval(*this, true);
      
      // Get the original size of retval, no processing of any of the added
      // digits will be necessary b/c of how | works.
      typename _DigsT::size_type origsize = min(retval._digits->size(), number._digits->size());
      
      // Directly copy digits longer than *this.
      if (retval._digits->size() < number._digits->size()) {
        retval._digits->insert(retval._digits->end(), number._digits->begin() + retval._digits->size(), number._digits->end());
      }
      
      // Inclusive OR each digit.
      for (typename _DigsT::size_type i = 0; i != origsize; ++i) {
        (*(retval._digits))[i] |= (*(number._digits))[i];
      }
      
      // No need to trim zeros with inclusive OR. Move the result into place
      // and return.
      _digits = retval._digits;
      return *this;
      
    }
    
    // Bitwise exclusive OR.
    template <class T> ArbInt<T>& ArbInt<T>::op_bit_xor (ArbInt<T> const& number) {
      
      // Work area.
      ArbInt<T> retval(*this, true);
      
      // Get the original size of retval, no processing of any of the added
      // digits will be necessary b/c of how | works.
      typename _DigsT::size_type origsize = min(retval._digits->size(), number._digits->size());
      
      // Directly copy digits longer than *this.
      if (retval._digits->size() < number._digits->size()) {
        retval._digits->insert(retval._digits->end(), number._digits->begin() + retval._digits->size(), number._digits->end());
      }
      
      // Exclusive OR each digit.
      for (typename _DigsT::size_type i = 0; i != origsize; ++i) {
        (*(retval._digits))[i] ^= (*(number._digits))[i];
      }
      
      // Clean up.
      retval._trimZeros();
      
      // Move the result into place and return.
      _digits = retval._digits;
      return *this;
      
    }
    
    // Bitwise compliment.
    template <class T> ArbInt<T>& ArbInt<T>::op_bit_cpm () {
      
      // Work area.
      ArbInt<T> retval(*this, true);
      
      // Apply compliment to each digit.
      for (typename _DigsT::size_type i = 0; i != retval._digits->size(); ++i) {
        (*(retval._digits))[i] = (~((*(retval._digits))[i])) & s_bitmask;
      }
      
      // Clean up.
      retval._trimZeros();
      
      // Move the result into place and return.
      _digits = retval._digits;
      return *this;
      
    }
    
    // Logical operators.
    template <class T> inline bool ArbInt<T>::op_log_and (ArbInt<T> const& number) const { return (!isZero() && !number.isZero()); }
    template <class T> inline bool ArbInt<T>::op_log_ior (ArbInt<T> const& number) const { return (!isZero() || !number.isZero()); }
    
    // Raise this number to a power.
    template <class T> ArbInt<T>& ArbInt<T>::pow (ArbInt<T> const& exp) {
      
      // Work area.
      ArbInt<T> tmp_base(*this);
      ArbInt<T> tmp_expn(exp);
      ArbInt<T> retval(1);
      
      // Russian peasant power.
      while (tmp_expn._digits->size() > 0) {
        if (tmp_expn._digits->front() & 1) {
          retval *= tmp_base;
        }
        tmp_base  *= tmp_base;
        tmp_expn >>= ArbInt<T>(1);
      }
      
      // Set the result and return.
      _digits = retval._digits;
      return *this;
      
    }
    
    // Return the maximum string input base.
    template <class T> inline T ArbInt<T>::max_input_base () { return SafeInteger<T>(s_numidigits).Value(); }
    
    // Common initialization tasks.
    template <class T> void ArbInt<T>::_init () {
      
      // Check that the class constructor was successfully called.
      if (!s_initialized) {
        s_classInit();
      }
      
      // Clear.
      clear();
      
    }
    
    // Perform carry. Only call this on temporary objects that are not copied.
    // Copy on write is not preserved for this function.
    template <class T> ArbInt<T>& ArbInt<T>::_carry (typename _DigsT::size_type start) {
      
      // Ensure that the start is not beyond the end of the container.
      if (start >= _digits->size()) {
        throw ArbIntErrors::Overrun();
      }
      
      // Loop through the container looking for base overflow.
      for (typename _DigsT::size_type i = start; i != _digits->size(); ++i) {
        
        // If we find overflow, push it to the next digit.
        if ((*_digits)[i] >= s_digitbase) {
          
          // Create the next digit if it does not already exist.
          if ((_digits->size() - 1) <= i) {
            _digits->push_back(0);
          }
          
          // Add any overflow to the next digit and remove it from the current
          // digit. No need to check i + 1 for overflow; i is the size_type of
          // _DigsT, and we just added an element, we would have recieved an
          // error when we did that.
          SafeInteger<typename _DigsT::value_type> overflow = SafeInteger<typename _DigsT::value_type>((*_digits)[i]) / s_digitbase;
          (*_digits)[i + 1]                                 = (SafeInteger<typename _DigsT::value_type>((*_digits)[i + 1]) + overflow).Value();
          (*_digits)[i]                                     = (SafeInteger<typename _DigsT::value_type>((*_digits)[i]) - (overflow * s_digitbase)).Value();
          
        // If there is no overflow now, there will be no more overflow.
        } else {
          
          // Simply leave the loop.
          break;
          
        }
        
      }
      
    }
    
    // Perform borrow. Only call this on temporary objects that are not
    // copied. Copy on write is not preserved for this function.
    template <class T> ArbInt<T>& ArbInt<T>::_borrow (typename _DigsT::size_type start) {
      
      // Ensure that the start is not beyond the end of the container.
      if (start >= _digits->size()) {
        throw ArbIntErrors::Overrun();
      }
      
      // Loop through the container borrowing until we've met our borrow.
      for (typename _DigsT::size_type i = start; i != (_digits->size() - 1); ++i) {
        
        // Add base to this digit.
        (*_digits)[i] = (SafeInteger<_DigT>((*_digits)[i]) + s_digitbase).Value();
        
        // If this is not the first digit, we are only here because we
        // borrowed for the previous digit. Subtract one for the borrow.
        if (i != start) {
          --(*_digits)[i];
        }
        
        // If the next digit is > 0, subtract 1 and we're done.
        if ((*_digits)[i + 1] > 0) {
          --(*_digits)[i + 1];
          _trimZeros();
          return *this;
        }
        
      }
      
      // If we get here, we have not paid for our borrow.
      throw ArbIntErrors::Overrun();
      
    }
    
    // Trim insignificant zeros.
    template <class T> inline ArbInt<T>& ArbInt<T>::_trimZeros () {
      
      // Trim zeros and return.
      s_trimZerosE(*_digits);
      return *this;
      
    }
    
    // Bitwise shift.
    template <class T> ArbInt<T>& ArbInt<T>::_shift (ArbInt<T> const& number, _Dir const dir) {
      
      // Only shift if it is needed.
      if (!this->isZero() && !number.isZero()) {
        
        // Cache the number of bits in a digit as an ArbInt, this will be used
        // many times.
        ArbInt<T> arbdigbits(s_digitbits);
        
        // If shift amount will leave no bits in the number, just zero it out.
        if ((dir == _DIR_R) && (number >= (ArbInt<T>(_digits->size()) * arbdigbits))) {
          
          _digits->clear();
          
        } else {
          
          // Check if shift will overrun _DigsT::size_type. This line is:
          // if we are shifting left (up), check if number (number of bits to
          // shift) is greater than the maximum number of bits - the number of
          // bits in the current number. Unless you have over 4GB of VM,
          // you'll run out of RAM first, but this probably indicates a bug in
          // whatever is asking for a shift that big.
          if ((dir == _DIR_L) && (number > ((ArbInt<T>(std::numeric_limits<typename _DigsT::size_type>::max()) * arbdigbits) - ((ArbInt<T>(_digits->size()) * arbdigbits) - ArbInt<T>(logBase(_digits->back(), 2)))))) {
            throw ArbIntErrors::Overrun();
          }
          
          // Shift with whole digits as much as possible. size_type should be
          // fine, since by now we've eliminated any shifts of more bits.
          // Shift by inserting or erasing whole digits.
          typename _DigsT::size_type digshift = (number / arbdigbits).Value<typename _DigsT::size_type>();
          if (digshift > 0) {
            if (dir == _DIR_L) {
              _digits->insert(_digits->begin(), digshift, 0);
            } else {
              _digits->erase(_digits->begin(), _digits->begin() + digshift);
            }
          }
          
          // Now do fine-grained shifting.
          int bitshift = (number - (ArbInt<T>(digshift) * arbdigbits)).Value<int>();
          if (bitshift > 0) {
            _DigT carry    = 0;
            _DigT oldcarry = 0;
            if (dir == _DIR_L) {
              _DigT bitmask  = ((rppower(SafeInteger<_DigT>(2), bitshift) - 1) << (s_digitbits - bitshift)).Value();
              for (typename _DigsT::iterator i = _digits->begin(); i != _digits->end(); ++i) {
                carry      = *i & bitmask;
                *i       <<= bitshift;
                *i        |= oldcarry;
                oldcarry   = carry >> (s_digitbits - bitshift);
              }
            } else {
              _DigT bitmask  = (rppower(SafeInteger<_DigT>(2), bitshift) - 1).Value();
              for (typename _DigsT::reverse_iterator i = _digits->rbegin(); i != _digits->rend(); ++i) {
                carry      = *i & bitmask;
                *i       >>= bitshift;
                *i        |= oldcarry;
                oldcarry   = carry << (s_digitbits - bitshift);
              }
            }
          }
          
          // Clean up.
          _trimZeros();
          
        }
        
      }
      
      // We done.
      return *this;
      
    }
    
    // Class constructor.
    template <class T> void ArbInt<T>::s_classInit () {
      
      // Get the maximum number that can be held in a single digit.
      s_digitbits = std::numeric_limits<_DigT>::digits >> 1;
      s_digitbase = rppower(SafeInteger<_DigT>(2), s_digitbits).Value();
      s_bitmask   = (SafeInteger<_DigT>(s_digitbase) - 1).Value();
      
      // Get the input digits.
      SafeInteger<_NumChrT> j;
      for (_NumChrT i = 0; i != std::numeric_limits<_NumChrT>::max(); ++i) {
        j = i;
        SafeInteger<_NumChrT> digit;
        if      ((j >= '0') && (j <= '9')) { digit = j - '0';                         }
        else if ((j >= 'A') && (j <= 'Z')) { digit = j - 'A' + 10;                    }
        else if ((j >= 'a') && (j <= 'z')) { digit = j - 'a' + 10;                    }
        else                               { digit = std::numeric_limits<_NumChrT>::max(); }
        s_idigits.push_back(digit.Value());
      }
      s_numidigits = 36;
      
      // Class has successfully been initialized.
      s_initialized = true;
      
    }
    
    // Trim leading zeros from a given container.
    template <class T> template <class CT> void ArbInt<T>::s_trimZerosB (CT& c) {
      
      // Nothing to do if empty.
      if (!c.empty()) {
        
        // Work data.
        typename CT::iterator pos;
        
        // Trim leading zeros.
        for (pos = c.begin(); (pos != c.end()) && (*pos == 0); ++pos) {}
        if (pos >= c.begin()) {
          c.erase(c.begin(), pos);
        }
        
      }
      
    }
    
    // Trim trailing zeros from a given container.
    template <class T> template <class CT> void ArbInt<T>::s_trimZerosE (CT& c) {
      
      // Nothing to do if empty.
      if (!c.empty()) {
        
        // Work data.
        typename CT::iterator pos;
        
        // Trim trailing zeros.
        for (pos = (c.end() - 1); (pos != (c.begin() - 1)) && (*pos == 0); --pos) {}
        if (pos++ < (c.end() - 1)) {
          c.erase(pos, c.end());
        }
        
      }
      
    }
    
    // Trim leading and trailing zeros from a given container.
    template <class T> template <class CT> void ArbInt<T>::s_trimZeros (CT& c) { s_trimZerosB(c); s_trimZerosE(c); }
    
    // Do long division on a given container in the specified base. Divisor
    // type must be 2x base. This is not checked, so be careful!
    template <class T> template <class DivndT, class DivorT> DivorT ArbInt<T>::s_longDiv (DivndT& divnd, DivorT const divor, T const base) {
      
      // Group of digits to divide.
      SafeInteger<DivorT> dgroup;
      SafeInteger<DivorT> dquot;
      
      // Quotient.
      DivndT quotient;
      
      // Long division steps through each digit of the dividend.
      for (typename DivndT::reverse_iterator i = divnd.rbegin(); i != divnd.rend(); ++i) {
        
        // Add this digit to the group.
        dgroup += *i;
        
        // Divide the group and add the result to the quotient.
        dquot = dgroup / divor;
        quotient.insert(quotient.begin(), SafeInteger<typename DivndT::value_type>(dquot.Value()).Value());
        
        // Take out what we've divided.
        dgroup -= dquot * divor;
        
        // Move the remainder up to the next order of magnitude.
        dgroup *= base;
        
      }
      
      // Trim insignificant zeros from the quotient.
      s_trimZerosE(quotient);
      
      // Set the result in place.
      divnd.swap(quotient);
      
      // Undo the last base shift, this is the remainder. Return it.
      return SafeInteger<DivorT>((dgroup / base).Value()).Value();
      
    }
    
    // Convert a container from one base to another. Frombase must be
    // 2^(bits/2) of from or smaller, tobase must be 2^(bits/2) of to or
    // smaller.
    template <class T> template <class FT, class TT> void ArbInt<T>::s_baseConv (FT const& from, T const frombase, TT& to, T const tobase) {
      
      // Verify that bases are valid. Base must be at least two, and at most
      // half of the significant bits of the target container's number type.
      // Maximum base is static, and so should only be calculated once per
      // type combination.
      static SafeInteger<typename TT::value_type> maxbase = rppower(SafeInteger<typename TT::value_type>(2), std::numeric_limits<typename TT::value_type>::digits >> 1);
      if ((SafeInteger<T>(frombase) < 2) || (SafeInteger<T>(tobase) < 2) || (SafeInteger<T>(frombase) > maxbase) || (SafeInteger<T>(tobase) > maxbase)) {
        throw ArbIntErrors::BaseOutOfRange();
      }
      
      // Empty out target container.
      to.clear();
      
      // Temporary, since work will be done in place.
      FT tmp_from(from);
      
      // Convert base by storing the remainder of repeated division by the
      // base that we will be converting to, in the base that we are
      // converting from. Least significant digits come out first.
      while (tmp_from.size() > 0) {
        to.push_back(SafeInteger<typename TT::value_type>(s_longDiv(tmp_from, tobase, frombase)).Value());
      }
      
    }
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  // Stream I/O operators.
  template <class T> inline std::ostream& operator << (std::ostream& l, DAC::ArbInt<T> const& r) { l << r.toString();                                  return l; }
  template <class T> inline std::istream& operator >> (std::istream& l, DAC::ArbInt<T>&       r) { std::string input; std::cin >> input; r.set(input); return l; }
  
  // Arithmetic operators.
  template <class T> inline DAC::ArbInt<T> operator * (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return DAC::ArbInt<T>(l).op_mul(r); }
  template <class T> inline DAC::ArbInt<T> operator / (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return DAC::ArbInt<T>(l).op_div(r); }
  template <class T> inline DAC::ArbInt<T> operator % (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return DAC::ArbInt<T>(l).op_mod(r); }
  template <class T> inline DAC::ArbInt<T> operator + (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return DAC::ArbInt<T>(l).op_add(r); }
  template <class T> inline DAC::ArbInt<T> operator - (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return DAC::ArbInt<T>(l).op_sub(r); }
  
  // Bit shift operators.
  template <class T> inline DAC::ArbInt<T> operator << (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return DAC::ArbInt<T>(l).op_shl(r); }
  template <class T> inline DAC::ArbInt<T> operator >> (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return DAC::ArbInt<T>(l).op_shr(r); }
  
  // Comparsion operators.
  template <class T> inline bool operator >  (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return l.op_gt(r); }
  template <class T> inline bool operator >= (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return l.op_ge(r); }
  template <class T> inline bool operator <  (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return l.op_lt(r); }
  template <class T> inline bool operator <= (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return l.op_le(r); }
  template <class T> inline bool operator == (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return l.op_eq(r); }
  template <class T> inline bool operator != (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return l.op_ne(r); }
  
  // Bitwise operators.
  template <class T> inline DAC::ArbInt<T> operator & (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return DAC::ArbInt<T>(l).op_bit_and(r); }
  template <class T> inline DAC::ArbInt<T> operator | (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return DAC::ArbInt<T>(l).op_bit_ior(r); }
  template <class T> inline DAC::ArbInt<T> operator ^ (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return DAC::ArbInt<T>(l).op_bit_xor(r); }
  
  // Logical operators.
  template <class T> inline bool operator && (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return l.op_log_and(r); }
  template <class T> inline bool operator || (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return l.op_log_ior(r); }
  
#endif
