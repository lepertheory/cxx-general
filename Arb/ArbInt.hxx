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
  #include "SafeInteger/SafeInteger.hxx"
  #include "ReferencePointer.hxx"
  #include "toString.hxx"
  #include "rppower.hxx"
  
  // Namespace wrapping.
  namespace DAC {
    
    /*********************************************************************
      * ArbInt
      *********************************************************************
      * This is an integer of an arbitrary number of digits.
      *********************************************************************/
    template <class T> class ArbInt {
      
      /*
        * Public members.
        */
      public:
        
        /*****************************************************************/
        // Typedefs.
        
        typedef T value_type;
        
        /*****************************************************************/
        // Function members.
        
        // Default constructor.
        ArbInt ();
        
        // Copy constructor.
        ArbInt (ArbInt const& number, bool const copynow = false);
        
        // Conversion constructor.
                            ArbInt (std::string const& number);
        template <class FT> ArbInt (FT          const  number);
        
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
        
        // Force a copy of another number, no COW.
        ArbInt& copy (ArbInt const& number, bool const copynow = false);
        
        // Accessors.
                            T       Base  ()                const;
                            ArbInt& Base  (T const base);
        template <class TT> TT      Value ()                const;
        template <class TT> ArbInt& Value (TT const number);
        
        // Set this number with a string.
                            ArbInt<T>& set (std::string const& number, T const base = 0);
                            ArbInt<T>& set (ArbInt      const& number);
        template <class FT> ArbInt<T>& set (FT          const  number);
        
        // Return a string of this number.
        std::string toString (T const base = 0) const;
        
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
        
        // Logical operator backends.
        bool log_op_and (ArbInt const& number) const;
        bool log_op_ior (ArbInt const& number) const;
        
        // Raise this number to a power.
        ArbInt& pow (ArbInt const& exp);
      
      /*
        * Private members.
        */
      private:
        
        /*****************************************************************/
        // Typedefs.
        
        typedef char          _StrChrT; // String character.
        typedef unsigned char _NumChrT; // Numeric character.
        typedef T             _DigT;    // Native digit.
        
        typedef std::vector<_NumChrT> _DigStrT; // String of character
                                                // digits.
        typedef std::vector<_DigT>    _DigsT;   // String of native
                                                // digits.
        
        typedef ReferencePointer<_DigsT> _DataPT; // Pointer to data.
        
        /*****************************************************************/
        // Data members.
        
        // The number.
        _DataPT _digits;
        
        // The default base of this number.
        T _base;
        
        /*****************************************************************/
        // Static data members.
        
        static bool s_initialized;
        
        static int   s_digitbits; // Number of bits in a digit.
        static _DigT s_digitbase; // Base of native digits.
        
        static _NumChrT s_numdigits; // Number of output digits.
        static _StrChrT s_odigits[]; // Output digits.
        
        static std::vector<_NumChrT> s_idigits; // Input digits.
        
        /*****************************************************************/
        // Function members.
        
        // Common initialization tasks.
        void _init ();
        
        // Perform carry or borrow.
        ArbInt& _carry  (typename _DigsT::size_type start);
        ArbInt& _borrow (typename _DigsT::size_type start);
        
        // Trim insignificant zeros.
        ArbInt& _trimZeros ();
        
        /*****************************************************************/
        // Static function members.
        
        // Class initialization.
        static bool s_classInit () throw();
        
        // Trim zeros from a given container.
        template <class CT> static void s_trimZerosB (CT& c);
        template <class CT> static void s_trimZerosE (CT& c);
        template <class CT> static void s_trimZeros  (CT& c);
        
        // Do long division on a given container. Returns the remainder.
        template <class DivndT, class DivorT> static DivorT s_longDiv (DivndT& divnd, DivorT const divor, T const base);
        
        // Convert a container from one base to another.
        template <class FT, class TT> static void s_baseConv (FT const& from, T const frombase, TT& to, T const tobase);
      
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
      class Base            : public Exception { public: virtual char const* what () const throw(); };
      class BadFormat       : public Base      {
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
      class ClassInitFailed : public Base      { public: virtual char const* what () const throw(); };
      class Negative        : public Base      { public: virtual char const* what () const throw(); };
      class Overrun         : public Base      { public: virtual char const* what () const throw(); };
      class DivByZero       : public Base      { public: virtual char const* what () const throw(); };
    };
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  namespace DAC {
    
    // Errors.
    namespace ArbIntErrors {
      inline char const* Base::what            () const throw() { return "Undefined error in ArbInt.";                                                                                                                                         }
      inline char const* BadFormat::what       () const throw() { return (std::string(_problem) + " at position " + DAC::toString((SafeInteger<std::string::size_type>(_position) + 1).Value()) + " in number \"" + *_number + "\".").c_str(); }
      inline BadFormat&  BadFormat::Problem    (char const*                   const problem)  throw() { _problem  = problem;  return *this; }
      inline BadFormat&  BadFormat::Position   (std::string::size_type        const position) throw() { _position = position; return *this; }
      inline BadFormat&  BadFormat::Number     (ConstReferencePointer<std::string>& number)   throw() { _number   = number;   return *this; }
      inline char const* ClassInitFailed::what () const throw() { return "Class initialization of ArbInt failed.";                                                                                                                             }
      inline char const* Negative::what        () const throw() { return "Attempted to set ArbInt to a negative number.";                                                                                                                      }
      inline char const* Overrun::what         () const throw() { return "Instruction overruns end of container.";                                                                                                                             }
      inline char const* DivByZero::what       () const throw() { return "Attempt to divide by zero.";                                                                                                                                         }
    };
    
    /*************************************************************************/
    // Static member initialization.
    
    template <class T> int                       ArbInt<T>::s_digitbits = 0;
    template <class T> typename ArbInt<T>::_DigT ArbInt<T>::s_digitbase = 0;
    
    template <class T> typename ArbInt<T>::_NumChrT ArbInt<T>::s_numdigits = 36;
    template <class T> typename ArbInt<T>::_StrChrT ArbInt<T>::s_odigits[] = {
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
      'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
      'U', 'V', 'W', 'X', 'Y', 'Z'
    };
    template <class T> std::vector<typename ArbInt<T>::_NumChrT> ArbInt<T>::s_idigits;
    
    // Call class constructor.
    template <class T> bool ArbInt<T>::s_initialized = s_classInit();
    
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
      set(number, 0);
      
    }
    template <class T> template <class FT> ArbInt<T>::ArbInt (FT const number) {
      
      // Call common init.
      _init();
      
      // Set the number.
      set<FT>(number);
      
    }
    
    // Assignment operator.
    template <class T>                     inline ArbInt<T>& ArbInt<T>::operator = (std::string const& number) { return set(number, 0);  }
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
    
    // Force a copy of another number, no COW.
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
    template <class T> inline ArbInt<T>& ArbInt<T>::Base (T const base)       { _base = base; return *this; }
    
    // Set this number with a string.
    template <class T> ArbInt<T>& ArbInt<T>::set (std::string const& number, T const base) {
      
      // Load the number into this for exception safety and COW.
      _DataPT new_digits(new _DigsT);
      
      // Hold the number in case an error needs to be thrown.
      ConstReferencePointer<std::string> tmp_number(new std::string(number));
      
      // Get the base of this number. Use the default base if none was supplied.
      T numbase = base;
      if (numbase == 0) {
        numbase = _base;
      }
      
      // Parser will load data into here.
      _DigStrT num;
      
      // Parse the number.
      for (typename std::string::size_type i = 0; i != number.length(); ++i) {
        
        // Get the value of this digit.
        _NumChrT digval = s_idigits[number[i]];
        
        // Make sure this digit is within the number base.
        if ((digval >= SafeInteger<T>(numbase).Value() || (digval == std::numeric_limits<_NumChrT>::max()))) {
          throw ArbIntErrors::BadFormat().Problem("Unrecognized character").Position(i).Number(tmp_number);
        }
        
        // Add the digit to the digit string.
        num.insert(num.begin(), digval);
        
      }
      
      // Trim insignificant zeros. This number is little-endian at this point.
      s_trimZerosE(num);
      
      // Convert to the native number base.
      s_baseConv(num, numbase, *new_digits, s_digitbase);
      
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
    template <class T> std::string ArbInt<T>::toString (T const base) const {
      
      // This is the string we will return.
      std::string retval;
      
      // Easy case of 0.
      if (_digits->size() == 0) {
        
        retval = "0";
        
      // Otherwise we have work to do.
      } else {
        
        // Get the base of this number. Use the default base if none was supplied.
        T numbase = base;
        if (numbase == 0) {
          numbase = _base;
        }
        
        // Convert to the output base.
        _DigsT num;
        s_baseConv(*_digits, s_digitbase, num, numbase);
        
        // Load this into the string. If the base is greater than th enumber
        // of digits defined, output the raw numbers of each digit.
        if (SafeInteger<T>(base) > s_numdigits) {
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
        // the divisor.
        ArbInt<T> diggroup;
        ArbInt<T> test;
        *(diggroup._digits) = _DigsT(_digits->end() - number._digits->size(), _digits->end());
        for (typename _DigsT::reverse_iterator i = _digits->rbegin() + (number._digits->size() - 1); i != _digits->rend(); ++i) {
          
          SafeInteger<_DigT> guess = 0;
          test = 0;
          
          // Make sure that the digit group is greater than or equal to the
          // dividend. If not, this digit is zero, move on.
          if (diggroup >= number) {
            
            // Make a guess at the quotient of this digit by dividing the high-
            // order digits.
            SafeInteger<_DigT> roughdivor = diggroup._digits->back();
            typename _DigsT::size_type j;
            for (j = 0; j != (diggroup._digits->size() - number._digits->size()); ++j) {
              roughdivor *= s_digitbase;
              roughdivor += (*(diggroup._digits))[diggroup._digits->size() - 1 - j];
            }
            guess = roughdivor / roughdivnd;
              
            // Correct the guess.
            test = number * ArbInt<T>(guess.Value());
            SafeInteger<_DigT> guessfloor = (roughdivor - rppower(SafeInteger<_DigT>(s_digitbase), j)) / (roughdivnd + 1);
            SafeInteger<_DigT> guessciel  = (roughdivor + rppower(SafeInteger<_DigT>(s_digitbase), j)) / ((roughdivnd == 1) ? 1 : (roughdivnd - 1));
            while ((test > diggroup) || ((test + number) <= diggroup)) {
              if (test > diggroup) {
                guessciel = guess - 1;
                guess     = guess - (((guess - guessfloor) >> 1) + 1);
                test      = number * ArbInt<T>(guess.Value());
              } else if ((test + number) <= diggroup) {
                guessfloor = guess + 1;
                guess      = guess + (((guessciel - guess) >> 1) + 1);
                test       = number * ArbInt<T>(guess.Value());
              }
            }
            
          }
          
          // The guess must be correct by this point. Add it to the quotient
          // and prepare for the next iteration.
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
        tmp_base *= tmp_base;
        tmp_expn /= ArbInt<T>(2);
      }
      
      // Set the result and return.
      _digits = retval._digits;
      return *this;
      
    }
    
    // Common initialization tasks.
    template <class T> void ArbInt<T>::_init () {
      
      // Check that the class constructor was successfully called.
      if (!s_initialized) {
        throw ArbIntErrors::ClassInitFailed();
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
    
    // Class constructor.
    template <class T> bool ArbInt<T>::s_classInit () throw() {
      
      // This cannot throw.
      try {
        
        // Get the maximum number that can be held in a single digit.
        s_digitbits = std::numeric_limits<_DigT>::digits >> 1;
        s_digitbase = rppower(SafeInteger<_DigT>(2), s_digitbits).Value();
        
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
        
      } catch (...) {
        
        // If any exception was caught, we failed. Have fun debugging.
        return false;
        
      }
      
      // Happy joy.
      return true;
      
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
    
    // Do long division on a given container in the specified base.
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
        quotient.insert(quotient.begin(), dquot.Value());
        
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
    
    // Convert a container from one base to another.
    template <class T> template <class FT, class TT> void ArbInt<T>::s_baseConv (FT const& from, T const frombase, TT& to, T const tobase) {
      
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
  template <class T> inline std::ostream& operator << (std::ostream& l, DAC::ArbInt<T> const& r) { l << r.toString(0);                                 return l; }
  template <class T> inline std::istream& operator >> (std::istream& l, DAC::ArbInt<T>&       r) { std::string input; std::cin >> input; r.set(input); return l; }
  
  // Arithmetic operators.
  template <class T> inline DAC::ArbInt<T> operator * (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { DAC::ArbInt<T> retval(l); return retval.op_mul(r); }
  template <class T> inline DAC::ArbInt<T> operator / (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { DAC::ArbInt<T> retval(l); return retval.op_div(r); }
  template <class T> inline DAC::ArbInt<T> operator % (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { DAC::ArbInt<T> retval(l); return retval.op_mod(r); }
  template <class T> inline DAC::ArbInt<T> operator + (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { DAC::ArbInt<T> retval(l); return retval.op_add(r); }
  template <class T> inline DAC::ArbInt<T> operator - (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { DAC::ArbInt<T> retval(l); return retval.op_sub(r); }
  
  // Bit shift operators.
  template <class T> inline DAC::ArbInt<T> operator << (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  template <class T> inline DAC::ArbInt<T> operator >> (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r);
  
  // Comparsion operators.
  template <class T> inline bool operator >  (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return l.op_gt(r); }
  template <class T> inline bool operator >= (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return l.op_ge(r); }
  template <class T> inline bool operator <  (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return l.op_lt(r); }
  template <class T> inline bool operator <= (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return l.op_le(r); }
  template <class T> inline bool operator == (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return l.op_eq(r); }
  template <class T> inline bool operator != (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return l.op_ne(r); }
  
  // Bitwise operators.
  template <class T> inline DAC::ArbInt<T> operator & (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { DAC::ArbInt<T> retval(l); return retval.op_bit_and(r); }
  template <class T> inline DAC::ArbInt<T> operator | (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { DAC::ArbInt<T> retval(l); return retval.op_bit_ior(r); }
  template <class T> inline DAC::ArbInt<T> operator ^ (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { DAC::ArbInt<T> retval(l); return retval.op_bit_xor(r); }
  
  // Logical operators.
  template <class T> inline bool operator && (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return l.op_log_and(r); }
  template <class T> inline bool operator || (DAC::ArbInt<T> const& l, DAC::ArbInt<T> const& r) { return l.op_log_ior(r); }
  
#endif
