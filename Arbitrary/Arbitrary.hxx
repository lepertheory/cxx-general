/*****************************************************************************
 * Arbitrary.hxx
 *****************************************************************************
 * Interface for class Arbitrary.
 *****************************************************************************/

// Include guard.
#if !defined(ARBITRARY_beckbqcuedirob537)
  #define ARBITRARY_beckbqcuedirob537
  
  // STL includes.
  #include <string>
  #include <iostream>
  #include <vector>
  
  // Internal includes.
  #include "Exception.hxx"
  #include "ReferencePointer.hxx"
  #include "toString.hxx"
  #include "rppower.hxx"
  #include "SafeInteger/SafeInteger.hxx"
  
  // Forward declarations.
  namespace DAC {
    class Arbitrary;
  };
  
  // Stream I/O operators.
  std::istream& operator >> (std::istream& left, DAC::Arbitrary const& right);
  std::ostream& operator << (std::ostream& left, DAC::Arbitrary const& right);
  
  // Arithmetic operators.
  DAC::Arbitrary operator * (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  DAC::Arbitrary operator / (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  //DAC::Arbitrary operator % (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  DAC::Arbitrary operator + (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  DAC::Arbitrary operator - (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  
  // Bit shift operators.
  //DAC::Arbitrary operator << (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  DAC::Arbitrary operator >> (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  
  // Comparison operators.
  bool operator >  (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  bool operator >= (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  bool operator <  (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  bool operator <= (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  bool operator == (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  bool operator != (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  
  // Bitwise operators.
  DAC::Arbitrary operator & (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  //DAC::Arbitrary operator | (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  //DAC::Arbitrary operator ^ (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  
  // Logical operators.
  bool operator && (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  bool operator || (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  
  // Namespace wrapping.
  namespace DAC {
    
    // Errors that Arbitrary may throw.
    namespace ArbitraryErrors {
      
      // Errors.
      class Base               : public Exception      { public: virtual char const* what () const throw(); };
      class BadFormat          : public Base           {
        public:
          virtual char const* what () const throw();
          virtual void Problem  (char const*                   const problem)  throw();
          virtual void Position (std::string::size_type        const position) throw();
          virtual void Number   (ConstReferencePointer<std::string>& number)   throw();
        protected:
          char const*                        _problem;
          std::string::size_type             _position;
          ConstReferencePointer<std::string> _number;
      };
      class Overflow           : public Base           { public: virtual char const* what () const throw(); };
      class ScalarOverflow     : public Overflow       { public: virtual char const* what () const throw(); };
      class ShiftOverflow      : public ScalarOverflow { public: virtual char const* what () const throw(); };
      class RadixConflict      : public Base           { public: virtual char const* what () const throw(); };
      class FractionalConflict : public RadixConflict  { public: virtual char const* what () const throw(); };
      class PrecisionLoss      : public Base           { public: virtual char const* what () const throw(); };
      class ClassInitFailed    : public Base           { public: virtual char const* what () const throw(); };
      class Overrun            : public Base           { public: virtual char const* what () const throw(); };
      class DivideByZero       : public Base           { public: virtual char const* what () const throw(); };
      
    }
    
    /*************************************************************************
     * Arbitrary
     *************************************************************************
     * This is an arbitrary precision number class.
     *************************************************************************/
    class Arbitrary {
      
      /***********************************************************************
       * Public members.
       ***********************************************************************/
      public:
        
        // Typedefs.
        typedef signed int ExponentT;
        
        // Default constructor.
        Arbitrary ();
        
        // Copy constructor.
        Arbitrary (Arbitrary const& number);
        
        // String conversion constructor. This is a special case.
        Arbitrary (std::string const& number, ExponentT const decimal = 0, bool const fixeddecimal = false);
        
        // Conversion constructor. Valid for any type that DAC::toString() can
        // create a valid number string for. Also creates from a SafeInteger,
        // since it's convenient and used internally.
        template <class T> Arbitrary (T const& number, ExponentT const decimal = 0, bool const fixeddecimal = false);
        
        // Increment / decrement operators.
        Arbitrary& operator ++ ();
        Arbitrary  operator ++ (int);
        Arbitrary& operator -- ();
        Arbitrary  operator -- (int);
        
        // Sign operators.
        Arbitrary operator + () const;
        Arbitrary operator - () const;
        
        // Assignment operator.
        Arbitrary& operator = (Arbitrary const& right);
        
        // Arithmetic assignment operators.
        Arbitrary& operator *= (Arbitrary const& right);
        Arbitrary& operator /= (Arbitrary const& right);
        //Arbitrary& operator %= (Arbitrary const& right);
        Arbitrary& operator += (Arbitrary const& right);
        Arbitrary& operator -= (Arbitrary const& right);
        
        // Bit shift assignment operators.
        //Arbitrary& operator <<= (Arbitrary const& right);
        Arbitrary& operator >>= (Arbitrary const& right);
        
        // Bitwise assignment operators.
        Arbitrary& operator &= (Arbitrary const& right);
        //Arbitrary& operator |= (Arbitrary const& right);
        //Arbitrary& operator ^= (Arbitrary const& right);
        
        // Type conversion operators.
        //operator bool () const;
        template <class T> operator T () const;
        
        // Arithmetic operator backends.
        Arbitrary& op_add (Arbitrary const& right);
        Arbitrary& op_sub (Arbitrary const& right);
        Arbitrary& op_mul (Arbitrary const& right);
        Arbitrary& op_div (Arbitrary const& right);
        //Arbitrary& op_mod (Arbitrary const& right);
        
        // Bit shift operator backends.
        Arbitrary& op_shr (Arbitrary const& right);
        //Arbitrary& op_shl (Arbitrary const& right);
        
        // Bitwise operator backends.
        Arbitrary& op_bit_and (Arbitrary const& right);
        //Arbitrary& op_bit_or  (Arbitrary const& right);
        //Arbitrary& op_bit_xor (Arbitrary const& right);
        
        // Comparison operator backends.
        bool op_eq (Arbitrary const& right) const;
        bool op_ne (Arbitrary const& right) const;
        bool op_gt (Arbitrary const& right) const;
        bool op_ge (Arbitrary const& right) const;
        bool op_lt (Arbitrary const& right) const;
        bool op_le (Arbitrary const& right) const;
        
        // Type conversion operator backends.
        bool op_bool () const;
        
        // Raise to a power.
        Arbitrary& pow (Arbitrary const& power);
        
        // Convert to a string.
        std::string toString () const;
        
        // Set the number.
        //                   Arbitrary& set (Arbitrary      const& number);
                           Arbitrary& set (std::string    const& number, ExponentT const decimal = 0, bool const fixeddecimal = false);
        template <class T> Arbitrary& set (SafeInteger<T> const& number, ExponentT const decimal = 0, bool const fixeddecimal = false);
        template <class T> Arbitrary& set (T              const& number, ExponentT const decimal = 0, bool const fixeddecimal = false);
        
        // Copy a given number.
        Arbitrary& copy (Arbitrary const& number);
        Arbitrary& copy (Arbitrary&       number);
        
        // Reset to just-constructed by default constructor state.
        Arbitrary& clear ();
        
      /***********************************************************************
       * Private members.
       ***********************************************************************/
      private:
        
        // Forward declarations.
        class _Data;
        
        // Typedefs.
        typedef SafeInteger<char>          _StrChrT; // String character type.
        typedef SafeInteger<unsigned char> _NumChrT; // Numeric character type.
        typedef SafeInteger<ExponentT>     _ExpT;    // Exponent type.
        typedef _NumChrT                   _DigChrT; // Digit character type.
        typedef SafeInteger<unsigned int>  _DigT;    // Individual digits.
        typedef SafeInteger<unsigned int>  _BaseT;   // Base type.
        
        typedef std::vector<_DigChrT> _DigStrT; // Vector of character digits.
        typedef std::vector<_DigT>    _DigsT;   // Vector of digits.
        
        typedef ReferencePointer<_Data> _DataPT; // Pointer to number data.
        
        // Enumerators.
        enum _BoE { _BEGIN = 0x1, _END = 0x2, _BEGINEND = 0x3 }; // Beginning or end.
        
        // Data types.
        class _Data {
          public:
            bool   positive;
            _DigsT digits;
            _ExpT  exponent;
            _ExpT  fixexponent;
            bool   fix;
            _BaseT originalbase;
            _Data ();
            _Data (_Data const& data);
            _Data& operator = (_Data const& right);
            _Data& copy (_Data const& data);
            _Data& clear ();
        };
        
        // Class members.
        static bool s_initialized;
        
        static SafeInteger<int> s_digitbits;
        static _BaseT           s_digitbase;
        
        static _NumChrT              s_numdigits;
        static _StrChrT              s_odigits[];
        static std::vector<_NumChrT> s_idigits;
        
        // Data members.
        _DataPT _data;
        
        // Initialize the class.
        static bool s_classInit () throw();
        
        // Trim leading and trailing zeros from a given container.
        template <class T, class ST, class IT> static ST s_trimZeros (T& c, _BoE const returnzeros);
        
        // Do any carry needed.
        template <class T, class TS> static void s_carry  (T& digits, TS const start);
        template <class T, class TS> static void s_borrow (T& digits, TS const start);
        
        // Multiplication of two containers.
        template <class T, class ST> static ReferencePointer<T> s_mul (T const& left, T const& right);
        
        // Long division.
        template <class DivndT, class DivndST, class DivndDT, class DivndIT, class DivorT> static DivorT s_longdiv (DivndT& dividend, DivorT const divisor, _BaseT const base);
        
        // Reverse a container.
        template <class T, class RIT> static ReferencePointer<T> s_reverse (T& c);
        
        // Convert the base of a container.
        template <class ToT, class ToDT, class FromT, class FromST, class FromDT, class FromIT> static ReferencePointer<ToT> s_baseconv (FromT const& from, _BaseT const frombase, _BaseT const tobase);
        
        // Normalize this number to another number.
        Arbitrary& _normalizeRadix    (Arbitrary& number);
        Arbitrary& _normalizeExponent (Arbitrary& number);
        
        // Convert this number to a whole number.
        Arbitrary& _toWhole ();
        
        // Set the exponent of this number.
        Arbitrary& _setExponent (_ExpT const exponent, bool const force = false);
        
        // Clean the internal data structure.
        Arbitrary& _cleanup ();
        
    /*************************************************************************
     * End of class declaration.
     *************************************************************************/
    };
    
    /*************************************************************************
     * Inline and template definitions.
     *************************************************************************/
    
    // Error functions.
    namespace ArbitraryErrors {
      
      inline char const* Base::what               () const throw() { return "Undefined error in Arbitrary.";                                                                         }
      inline char const* BadFormat::what          () const throw() { return (std::string(_problem) + " at position " + DAC::toString((SafeInteger<std::string::size_type>(_position) + 1).Value()) + " in number \"" + *(_number.get()) + "\".").c_str(); }
      inline void        BadFormat::Problem       (char const*                   const problem)  throw() { _problem  = problem;  }
      inline void        BadFormat::Position      (std::string::size_type        const position) throw() { _position = position; }
      inline void        BadFormat::Number        (ConstReferencePointer<std::string>& number)   throw() { _number   = number;   }
      inline char const* Overflow::what           () const throw() { return "Overflow error.";                                                                                       }
      inline char const* ScalarOverflow::what     () const throw() { return "Overflow when converting to scalar type.";                                                              }
      inline char const* ShiftOverflow::what      () const throw() { return "Shift amount greater than digit array's maximum size requested.";                                       }
      inline char const* RadixConflict::what      () const throw() { return "Cannot perform operation on numbers of differing bases.";                                               }
      inline char const* FractionalConflict::what () const throw() { return "Cannot perform operation on fractional numbers of differing bases.";                                    }
      inline char const* PrecisionLoss::what      () const throw() { return "Operation will cause precision to be lost.";                                                            }
      inline char const* ClassInitFailed::what    () const throw() { return "Class initialization failed; new objects cannot be constructed.";                                       }
      inline char const* Overrun::what            () const throw() { return "Overrun?";                                                                                              }
      inline char const* DivideByZero::what       () const throw() { return "Divide by zero.";                                                                                       }
      
    };
    
    // Assignment operator. Makes a perfect copy of a given number.
    inline Arbitrary& Arbitrary::operator = (Arbitrary const& right) { copy(right); return *this; }
    
    // Conversion constructor. Requires that DAC::toString will work for the
    // given type.
    template <class T> Arbitrary::Arbitrary (T const& number, ExponentT const decimal, bool const fixeddecimal) {
      
      // Construct object fully.
      clear();
      
      // Set the number.
      set(number, decimal, fixeddecimal);
      
    }
    
    // Increment / decrement operators.
    inline Arbitrary& Arbitrary::operator ++ ()    {                                     return op_add(1); }
    inline Arbitrary  Arbitrary::operator ++ (int) { Arbitrary retval(*this); op_add(1); return retval;    }
    inline Arbitrary& Arbitrary::operator -- ()    {                                     return op_sub(1); }
    inline Arbitrary  Arbitrary::operator -- (int) { Arbitrary retval(*this); op_sub(1); return retval;    }
    
    // Sign operators.
    inline Arbitrary Arbitrary::operator + () const { Arbitrary retval(*this);                                                     return retval; }
    inline Arbitrary Arbitrary::operator - () const { Arbitrary retval(*this); retval._data->positive = !(retval._data->positive); return retval; }
    
    // Arithmetic assignment operators.
    inline Arbitrary& Arbitrary::operator *= (Arbitrary const& right) { return op_mul(right); }
    inline Arbitrary& Arbitrary::operator /= (Arbitrary const& right) { return op_div(right); }
    //inline Arbitrary& Arbitrary::operator %= (Arbitrary const& right) { return op_mod(right); }
    inline Arbitrary& Arbitrary::operator += (Arbitrary const& right) { return op_add(right); }
    inline Arbitrary& Arbitrary::operator -= (Arbitrary const& right) { return op_sub(right); }
    
    // Bit shift assignment operators.
    //inline Arbitrary& Arbitrary::operator <<= (Arbitrary const& right { return op_shl(right); }
    inline Arbitrary& Arbitrary::operator >>= (Arbitrary const& right) { return op_shr(right); }
    
    // Bitwise assignment operators.
    inline Arbitrary& Arbitrary::operator &= (Arbitrary const& right) { return op_bit_and(right); }
    //inline Arbitrary& Arbitrary::operator |= (Arbitrary const& right) { return op_bit_or(right);  }
    //inline Arbitrary& Arbitrary::operator ^= (Arbitrary const& right) { return op_bit_xor(right); }
    
    // Type conversion operators.
    template <> inline Arbitrary::operator bool () const { return op_bool(); }
    template <class T> inline Arbitrary::operator T () const {
      SafeInteger<T> retval = 0;
      for (_DigsT::iterator i = _data->digits.begin(); i != _data->digits.end(); ++i) {
        try {
          retval += *i * rppower(s_digitbase, (i - _data->digits.begin()));
        } catch (SafeIntegerErrors::Overflow& e) {
          throw ArbitraryErrors::ScalarOverflow();
        }
      }
      return retval.Value();
    }
    
    // Comparison operator backends.
    inline bool Arbitrary::op_eq (Arbitrary const& right) const { return (!op_gt(right) && !right.op_gt(*this)); }
    inline bool Arbitrary::op_ne (Arbitrary const& right) const { return !op_eq(right);                          }
    inline bool Arbitrary::op_ge (Arbitrary const& right) const { return (op_gt(right) || op_eq(right));         }
    inline bool Arbitrary::op_lt (Arbitrary const& right) const { return (!op_gt(right) && op_ne(right));        }
    inline bool Arbitrary::op_le (Arbitrary const& right) const { return (op_lt(right) || op_eq(right));         }
    
    // Set with an arbitrary type.
    template <class T> Arbitrary& Arbitrary::set (SafeInteger<T> const& number, ExponentT const decimal, bool const fixeddecimal) { return set(DAC::toString(number.Value()), decimal, fixeddecimal); }
    template <class T> Arbitrary& Arbitrary::set (T              const& number, ExponentT const decimal, bool const fixeddecimal) { return set(DAC::toString(number),         decimal, fixeddecimal); }
    
    // Trim leading and trailing zeros from a given container. Container must
    // support random access iterators.
    template <class T, class ST, class IT> ST Arbitrary::s_trimZeros (T& c, _BoE const trimzeros) {
      
      // Nothing to do if empty.
      if (c.empty()) {
        return 0;
      }
      
      // Data.
      IT pos;
      SafeInteger<ST> retval(0);
      
      // Trim leading zeros.
      if (trimzeros & _BEGIN) {
        for (pos = c.begin(); (pos != c.end()) && (*pos == 0); ++pos) {}
        if (pos >= c.begin()) {
          retval = pos - c.begin();
          c.erase(c.begin(), pos);
        }
      }
      
      // Trim trailing zeros.
      if (trimzeros & _END) {
        for (pos = (c.end() - 1); (pos != (c.begin() - 1)) && (*pos == 0); --pos) {}
        if (pos++ < (c.end() - 1)) {
          retval = c.end() - pos;
          c.erase(pos, c.end());
        }
      }
      
      return retval.Value();
      
    }
    
    // Do any carry needed, starting at given digit.
    template <class T, class TS> void Arbitrary::s_carry (T& digits, TS const start) {
      
      // Make sure we're not stepping out of the container.
      if (start >= digits.size()) {
        throw ArbitraryErrors::Overrun();
      }
      
      // Step through every digit.
      for (TS i = start; i != digits.size(); ++i) {
        
        // If carry isn't needed now, it won't be anywhere further up.
        if (digits[i] < s_digitbase) {
          break;
        }
        
        // Overflow safe addition is expensive, cache it.
        TS j = i + 1;
        
        // How many of the next digit are in this digit?
        _DigT next = digits[i] / s_digitbase;
        
        // Remove the next digit from this digit.
        digits[i] = digits[i] - (s_digitbase * next);
        
        // Add a new digit if needed.
        if (j == digits.size()) {
          digits.push_back(0);
        }
        
        // Add the carry to the next digit.
        digits[j] += next;
        
      }
      
    }
    
    // Borrow, starting at given digit.
    template <class T, class TS> void Arbitrary::s_borrow (T& digits, TS const start) {
      
      // Make sure we're still within the container.
      if (start >= digits.size()) {
        throw ArbitraryErrors::Overrun();
      }
      
      // Cache adding 1.
      TS next = start + 1;
      
      // Make sure that there is something to borrow from.
      if (next >= digits.size()) {
        throw ArbitraryErrors::Overrun();
      }
      
      // Make sure that the digit we are borrowing from has something to
      // borrow. If not, borrow it.
      if (digits[next] <= 0) {
        s_borrow<T, TS>(digits, next);
      }
      
      // Now do the borrow.
      digits[next]  -= 1;
      digits[start] += s_digitbase;
      
    }
    
    // Multiplication of two containers.
    template <class T, class ST> ReferencePointer<T> Arbitrary::s_mul (T const& left, T const& right) {
      
      ReferencePointer<T> retval(new T);
      
      // Multiply like 3rd grade. Outer loop is multiplicand.
      for (ST i = 0; i != left.size(); ++i) {
        
        // Temporary product for this digit.
        T digproduct;
        
        // Inner loop is multiplicator.
        for (ST j = 0; j != right.size(); ++j) {
          
          // Add a new digit to the temporary product if needed.
          if (digproduct.size() == j) {
            digproduct.push_back(0);
          }
          
          // Add the product of these two digits to the appropriate digit of
          // the temporary product.
          digproduct[j] += left[i] * right[j];
          
          // Do any carry needed.
          s_carry<T, ST>(digproduct, j);
          
        }
        
        // Add the single digit product to the final product, offset by the
        // multiplicand digit we are on now.
        for (ST j = 0; j != digproduct.size(); ++j) {
          
          // Digit to add to.
          ST tdig = j + i;
          
          // Add a new digit if needed.
          if (retval->size() == tdig) {
            retval->push_back(0);
          }
          
          // Add this digit.
          (*retval)[tdig] += digproduct[j];
          
          // Do any carry needed.
          s_carry<T, ST>(*retval, tdig);
          
        }
        
      }
      
      // Return the result.
      return retval;
      
    }
    
    // Do long division on an given number. The number must be in little-
    // endian format. This function call is ridiculous, but it's private to
    // the class and this is the only way it can be even sort of efficient.
    template <class DivndT, class DivndST, class DivndDT, class DivndIT, class DivorT> DivorT Arbitrary::s_longdiv (DivndT& dividend, DivorT const divisor, _BaseT const base) {
      
      // Group of digits to divide.
      DivorT dgroup = 0;
      DivorT dquot  = 0;
      
      // Quotient.
      DivndT quotient;
      
      // Long division steps through each digit of the dividend.
      for (DivndIT i = dividend.begin(); i != dividend.end(); ++i) {
        
        // Add this digit to the group.
        dgroup += *i;
        
        // Divide the group. Make sure the quotient can hold the result before
        // adding it to the digit array.
        dquot = dgroup / divisor;
        quotient.push_back(dquot);
        
        // Take out what we've divided. No need to check for overflow,
        // dquot * divisor must be smaller than or equal to the previous
        // value of dgroup since division gets the floor.
        dgroup -= dquot * divisor;
        
        // Move the remainder up to the next order of magnitude.
        dgroup *= base;
        
      }
      
      // Trim zeros from the quotient.
      s_trimZeros<DivndT, DivndST, DivndIT>(quotient, _BEGIN);
      
      // Set the result in place.
      dividend.swap(quotient);
      
      // Undo the last base shift, this is the remainder. Return it.
      return (dgroup / base);
      
    }
    
    // Reverse a given container.
    template <class T, class RIT> ReferencePointer<T> Arbitrary::s_reverse (T& c) {
      
      // Hold the new container.
      ReferencePointer<T> retval(new T);
      
      // Reverse by adding elements one-by-one.
      for (RIT i = c.rbegin(); i != c.rend(); ++i) {
        retval->push_back(*i);
      }
      
      // Move the temp into the return value and we're done.
      return retval;
      
    }
    
    // Convert the base of a given container, return a new container.
    template <class ToT, class ToDT, class FromT, class FromST, class FromDT, class FromIT> ReferencePointer<ToT> Arbitrary::s_baseconv (FromT const& from, _BaseT const frombase, _BaseT const tobase) {
      
      // Return this container. Use a safe pointer to get around ridiculous
      // reference scope problems. Seriously, how hard is it to have a
      // reference type that doesn't stupidly fall out of scope when it's used
      // as a return type? Bleh.
      ReferencePointer<ToT> retval(new ToT);
      
      // Temporary, since we will be doing work in place.
      FromT tempfrom(from);
      
      // Trim insignificant zeros result.
      s_trimZeros<FromT, FromST, FromIT>(tempfrom, _BEGIN);
      
      // Convert base by storing the remainder of repeated division.
      while (tempfrom.size() > 0) {
        retval->push_back(s_longdiv<FromT, FromST, FromDT, FromIT, _BaseT>(tempfrom, tobase, frombase));
      }
      
      // Return
      return retval;
      
    }
    
    // Clean the internal data structure.
    inline Arbitrary& Arbitrary::_cleanup () {
      return set(toString());
    }
    
    inline Arbitrary::_Data& Arbitrary::_Data::operator = (Arbitrary::_Data const& right) { return copy(right); }
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  // Stream I/O operators.
  inline std::istream& operator >> (std::istream& left, DAC::Arbitrary&       right) { std::string input; std::cin >> input; right.set(input); return left; }
  inline std::ostream& operator << (std::ostream& left, DAC::Arbitrary const& right) { left << right.toString();                               return left; }
  
  // Arithmetic operators.
  inline DAC::Arbitrary operator * (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_mul(right); }
  inline DAC::Arbitrary operator / (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_div(right); }
  //inline DAC::Arbitrary operator % (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_div(right); }
  inline DAC::Arbitrary operator + (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_add(right); }
  inline DAC::Arbitrary operator - (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_sub(right); }
  
  // Bitshift operators.
  //inline DAC::Arbitrary operator << (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_shl(right); }
  inline DAC::Arbitrary operator >> (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_shr(right); }
  
  // Comparison operators.
  inline bool operator >  (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { return left.op_gt(right); }
  inline bool operator >= (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { return left.op_ge(right); }
  inline bool operator <  (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { return left.op_lt(right); }
  inline bool operator <= (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { return left.op_le(right); }
  inline bool operator == (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { return left.op_eq(right); }
  inline bool operator != (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { return left.op_ne(right); }

  // Bitwise operators.
  inline DAC::Arbitrary operator & (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_bit_and(right); }
  //DAC::Arbitrary operator | (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_bit_or(right);  }
  //DAC::Arbitrary operator ^ (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_bit_xor(right); }
  
#endif
