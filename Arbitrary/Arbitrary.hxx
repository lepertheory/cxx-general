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
  #include "overflow.hxx"
  
  // Forward declarations.
  namespace DAC {
    class Arbitrary;
  };
  
  // Stream I/O operators.
  std::istream& operator >> (std::istream& left, DAC::Arbitrary const& right);
  std::ostream& operator << (std::ostream& left, DAC::Arbitrary const& right);
  
  // Arithmetic operators.
  DAC::Arbitrary operator + (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  //DAC::Arbitrary operator - (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  DAC::Arbitrary operator * (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  DAC::Arbitrary operator / (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  //DAC::Arbitrary operator % (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  
  // Bit shift operators.
  DAC::Arbitrary operator >> (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  //DAC::Arbitrary operator << (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  
  // Bitwise operators.
  DAC::Arbitrary operator & (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  //DAC::Arbitrary operator | (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  //DAC::Arbitrary operator ^ (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  
  // Comparison operators.
  bool operator == (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  bool operator != (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  bool operator >  (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  bool operator >= (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  bool operator <  (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  bool operator <= (DAC::Arbitrary const& left, DAC::Arbitrary const& right);
  
  // Namespace wrapping.
  namespace DAC {
    
    // Errors that Arbitrary may throw.
    namespace ArbitraryErrors {
      
      // Errors.
      class Base               : public Exception     {};
      class BadFormat          : public Base          {};
      class Overflow           : public Base          {};
      class RadixConflict      : public Base          {};
      class FractionalConflict : public RadixConflict {};
      class PrecisionLoss      : public Base          {};
      class ClassInitFailed    : public Base          {};
      class Overrun            : public Base          {};
      
      // Error factories.
      void throwBase               (std::string const& text)                                                        throw(Base);
      void throwBadFormat          (std::string const& text, std::string::size_type pos, std::string const& number) throw(BadFormat);
      void throwOverflow           (std::string const& text)                                                        throw(Overflow);
      void throwRadixConflict      (std::string const& text)                                                        throw(RadixConflict);
      void throwFractionalConflict (std::string const& text)                                                        throw(FractionalConflict);
      void throwPrecisionLoss      (std::string const& text)                                                        throw(PrecisionLoss);
      void throwClassInitFailed    (std::string const& text)                                                        throw(ClassInitFailed);
      void throwOverrun            (std::string const& text)                                                        throw(Overrun);
      
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
        
        // Default constructor.
        Arbitrary ();
        
        // Copy constructor.
        Arbitrary (Arbitrary const& number);
        
        // String conversion constructor. This is a special case.
        Arbitrary (std::string const& number);
        
        // Conversion constructor. Valid for any type that DAC::toString() can
        // create a valid number string for.
        template <class T> Arbitrary (T const& number);
        
        // Assignment operator.
        Arbitrary& operator = (Arbitrary const& right);
        
        // Arithmetic assignment operators.
        Arbitrary& operator += (Arbitrary const& right);
        //Arbitrary& operator -= (Arbitrary const& right);
        Arbitrary& operator *= (Arbitrary const& right);
        Arbitrary& operator /= (Arbitrary const& right);
        //Arbitrary& operator %= (Arbitrary const& right);
        
        // Bit shift assignment operators.
        Arbitrary& operator >>= (Arbitrary const& right);
        //Arbitrary& operator <<= (Arbitrary const& right);
        
        // Bitwise assignment operators.
        Arbitrary& operator &= (Arbitrary const& right);
        //Arbitrary& operator |= (Arbitrary const& right);
        //Arbitrary& operator ^= (Arbitrary const& right);
        
        // Increment / decrement operators.
        Arbitrary& operator ++ ();
        Arbitrary  operator ++ (int);
        //Arbitrary& operator -- ();
        //Arbitrary  operator -- (int);
        
        // Negation operator.
        //Arbitrary operator - () const;
        
        // Type conversion operators.
        operator bool () const;
        
        // Arithmetic operator backends.
        Arbitrary& op_add (Arbitrary const& right);
        //Arbitrary& op_sub (Arbitrary const& right);
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
        //                   Arbitrary& set (Arbitrary   const& number);
                           Arbitrary& set (std::string const& number);
        template <class T> Arbitrary& set (T           const& number);
        
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
        typedef char          _StrChrT; // String character type.
        typedef unsigned char _NumChrT; // Numeric character type.
        typedef long double   _LogT;    // Logarithm type.
        typedef signed int    _ExpT;    // Exponent type.
        typedef _NumChrT      _DigChrT; // Digit character type.
        typedef unsigned int  _DigT;    // Individual digits.
        typedef unsigned int  _BaseT;   // Base type.
        
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
            _BaseT originalbase;
            _Data ();
            _Data (_Data const& data);
            _Data& operator = (_Data const& right);
            _Data& copy (_Data const& data);
            _Data& clear ();
        };
        
        // Class members.
        static bool s_initialized;
        
        static _BaseT s_digitbase;
        
        static const _NumChrT        s_numdigits;
        static const _StrChrT        s_odigits[];
        static std::vector<_NumChrT> s_idigits;
        
        // Data members.
        _DataPT _data;
        
        // Initialize the class.
        static bool s_classInit () throw();
        
        // Trim leading and trailing zeros from a given container.
        template <class T, class IT> static void s_trimZeros (T& c, _BoE const returnzeros);
        
        // Do any carry needed.
        template <class T, class TI, class TS> static void s_carry (T& digits, TS const start);
        
        // Long division.
        template <class DivndT, class DivndDT, class DivndIT, class DivorT> static DivorT s_longdiv (DivndT& dividend, DivorT const divisor, _BaseT const base);
        
        // Reverse a container.
        template <class T, class RIT> static ReferencePointer<T> s_reverse (T& c);
        
        // Convert the base of a container.
        template <class ToT, class ToDT, class FromT, class FromDT, class FromIT> static ReferencePointer<ToT> s_baseconv (FromT const& from, _BaseT const frombase, _BaseT const tobase);
        
        // Normalize this number to another number.
        Arbitrary& _normalizeRadix   (Arbitrary& number);
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
    
    /*************************************************************************/
    // Error functions.
    namespace ArbitraryErrors {
      
      /***********************************************************************/
      // Throw normal text errors.
      inline void throwBase               (std::string const& text) throw(Base)               { Base               error; try { error.Text(text); } catch (...) {} throw error; }
      inline void throwOverflow           (std::string const& text) throw(Overflow)           { Overflow           error; try { error.Text(text); } catch (...) {} throw error; }
      inline void throwRadixConflict      (std::string const& text) throw(RadixConflict)      { RadixConflict      error; try { error.Text(text); } catch (...) {} throw error; }
      inline void throwFractionalConflict (std::string const& text) throw(FractionalConflict) { FractionalConflict error; try { error.Text(text); } catch (...) {} throw error; }
      inline void throwPrecisionLoss      (std::string const& text) throw(PrecisionLoss)      { PrecisionLoss      error; try { error.Text(text); } catch (...) {} throw error; }
      inline void throwClassInitFailed    (std::string const& text) throw(ClassInitFailed)    { ClassInitFailed    error; try { error.Text(text); } catch (...) {} throw error; }
      inline void throwOverrun            (std::string const& text) throw(Overrun)            { Overrun            error; try { error.Text(text); } catch (...) {} throw error; }
      /***********************************************************************/
      
      /***********************************************************************/
      // Throw a BadFormat error.
      inline void throwBadFormat (std::string const& text, std::string::size_type pos, std::string const& number) throw(BadFormat) {
        BadFormat error;
        try {
          error.Text(text + " at position " + DAC::toString(pos + 1) + " in '" + number + "'.");
        } catch (...) {}
        throw error;
      }
      /***********************************************************************/
      
    };
    /*************************************************************************/
    
    /*************************************************************************/
    // Assignment operator. Makes a perfect copy of a given number.
    inline Arbitrary& Arbitrary::operator = (Arbitrary const& right) { copy(right); return *this; }
    /*************************************************************************/
    
    /*************************************************************************/
    // Conversion constructor. Requires that DAC::toString will work for the
    // given type.
    template <class T> Arbitrary::Arbitrary (T const& number) {
      
      // Construct object fully.
      clear();
      
      // Set the number.
      set(number);
      
    }
    /*************************************************************************/
    
    /*************************************************************************/
    // Set with an arbitrary type.
    template <class T> Arbitrary& Arbitrary::set (T const& number) { return set(DAC::toString(number)); }
    /*************************************************************************/
    
    /*************************************************************************/
    // Arithmetic assignment operators.
    inline Arbitrary& Arbitrary::operator += (Arbitrary const& right) { return op_add(right); }
    //inline Arbitrary& Arbitrary::operator -= (Arbitrary const& right) { return op_sub(right); }
    inline Arbitrary& Arbitrary::operator *= (Arbitrary const& right) { return op_mul(right); }
    inline Arbitrary& Arbitrary::operator /= (Arbitrary const& right) { return op_div(right); }
    //inline Arbitrary& Arbitrary::operator %= (Arbitrary const& right) { return op_mod(right); }
    /*************************************************************************/
    
    /*************************************************************************/
    inline Arbitrary& Arbitrary::operator >>= (Arbitrary const& right) { return op_shr(right); }
    //inline Arbitrary& Arbitrary::operator <<= (Arbitrary const& right { return op_shl(right); }
    /*************************************************************************/
    
    /*************************************************************************/
    // Bitwise assignment operators.
    inline Arbitrary& Arbitrary::operator &= (Arbitrary const& right) { return op_bit_and(right); }
    //inline Arbitrary& Arbitrary::operator |= (Arbitrary const& right) { return op_bit_or(right);  }
    //inline Arbitrary& Arbitrary::operator ^= (Arbitrary const& right) { return op_bit_xor(right); }
    /*************************************************************************/
    
    /*************************************************************************/
    // Increment / decrement operators.
    inline Arbitrary& Arbitrary::operator ++ ()    {                                     return op_add(1); }
    inline Arbitrary  Arbitrary::operator ++ (int) { Arbitrary retval(*this); op_add(1); return retval;    }
    //inline Arbitrary& Arbitrary::operator -- ()    {                                     return op_sub(1); }
    //inline Arbitrary  Arbitrary::operator -- (int) { Arbitrary retval(*this); op_sub(1); return retval;    }
    /*************************************************************************/
    
    /*************************************************************************/
    // Negation operator.
    //inline Arbitrary Arbitrary::operator - () const { Arbitrary retval(*this); retval._data->positive = !(retval._data->positive); return retval; }
    /*************************************************************************/
    
    /*************************************************************************/
    // Type conversion operators.
    inline Arbitrary::operator bool () const { return op_bool(); }
    /*************************************************************************/
    
    /*************************************************************************/
    // Comparison operator backends.
    inline bool Arbitrary::op_eq (Arbitrary const& right) const { return (!op_gt(right) && !right.op_gt(*this)); }
    inline bool Arbitrary::op_ne (Arbitrary const& right) const { return !op_eq(right);                          }
    inline bool Arbitrary::op_ge (Arbitrary const& right) const { return (op_gt(right) || op_eq(right));         }
    inline bool Arbitrary::op_lt (Arbitrary const& right) const { return (!op_gt(right) && op_ne(right));        }
    inline bool Arbitrary::op_le (Arbitrary const& right) const { return (op_lt(right) || op_eq(right));         }
    /*************************************************************************/
    
    /*************************************************************************/
    // Trim leading and trailing zeros from a given container. Container must
    // support random access iterators.
    template <class T, class IT> void Arbitrary::s_trimZeros (T& c, _BoE const trimzeros) {
      
      // Nothing to do if empty.
      if (c.empty()) {
        return;
      }
      
      // Data.
      IT pos;
      
      // Trim leading zeros.
      if (trimzeros & _BEGIN) {
        for (pos = c.begin(); (pos != c.end()) && (*pos == 0); ++pos) {}
        if (pos >= c.begin()) {
          c.erase(c.begin(), pos);
        }
      }
      
      // Trim trailing zeros.
      if (trimzeros & _END) {
        for (pos = (c.end() - 1); (pos != (c.begin() - 1)) && (*pos == 0); --pos) {}
        if (pos++ < (c.end() - 1)) {
          c.erase(pos, c.end());
        }
      }
      
    }
    /*************************************************************************/
    
    /*************************************************************************/
    // Do any carry needed, starting at given digit.
    template <class T, class TI, class TS> void Arbitrary::s_carry (T& digits, TS const start) {
      
      // Make sure we're not stepping out of the container.
      if (start > digits.size()) {
        ArbitraryErrors::throwOverrun("Container overrun in s_carry(). digits.size(): " + DAC::toString(digits.size()) + "  start: " + DAC::toString(start) + ".");
      }
      
      // Step through every digit.
      for (TS i = start; i != digits.size(); ++i) {
        
        // If carry isn't needed now, it won't be anywhere further up.
        if (digits[i] < s_digitbase) {
          break;
        }
        
        // Remove the next digit from this digit.
        digits[i] = of_sub<_DigT, _DigT, _BaseT>(digits[i], s_digitbase);
        
        // Add a new digit if needed.
        if (of_add<TS, TS, int>(i, 1) == digits.size()) {
          digits.push_back(0);
        }
        
        // Add the carry to the next digit.
        digits[of_add<TS, TS, int>(i, 1)] = of_add<_DigT, _DigT, int>(digits[of_add<TS, TS, int>(i, 1)], 1);
        
      }
      
      /*
       * What the hell? Somehow, at the push_back between output of pos3 and
       * pos4, i goes from digits.begin() to digits.begin() - 8. And STLPort
       * has problems compiling under WinXP, I don't have the energy to find
       * out why. Moving over to a different iteration method, maybe someday
       * this will be fixed.
       * 
      std::cout << "digits: ";
      for (TI i = digits.begin(); i != digits.end(); ++i) {
        std::cout << "'" << *i << "' ";
      }
      std::cout << " start: " << start << std::endl;
      
      std::cout << "digits.size():" << digits.size() << std::endl;
      
      // Go through every digit.
      for (TI i = digits.begin() + start; i != digits.end(); ++i) {
        
        std::cout << "*i: " << *i << "  s_digitbase: " << s_digitbase << std::endl;
        
        // If carry isn't needed now, it won't be anywhere further up.
        if (*i < s_digitbase) {
          break;
        }
        
        // Remove the next digit from this digit.
        *i = of_sub<_DigT, _DigT, _BaseT>(*i, s_digitbase);
        
        // Add a new digit if needed.
        if ((i + 1) == digits.end()) {
          std::cout << "i pos3: " << (i - digits.begin()) << std::endl;
          digits.push_back(0);
          std::cout << "i pos4: " << (i - digits.begin()) << std::endl;
          std::cout << "digits.size() 2: " << digits.size() << std::endl;
        }
        
        // Add the carry to the next digit.
        std::cout << "i pos1: " << (i - digits.begin()) << std::endl;
        *(i + 1) = of_add<_DigT, _DigT, int>(*(i + 1), 1);
        std::cout << "digits.size() 3: " << digits.size() << std::endl;
        std::cout << "i pos2: " << (i - digits.begin()) << std::endl;
        
      }
      */
      
    }
    /*************************************************************************/
    
    /*************************************************************************/
    // Do long division on an given number. The number must be in little-
    // endian format. This function call is ridiculous, but it's private to
    // the class and this is the only way it can be even sort of efficient.
    template <class DivndT, class DivndDT, class DivndIT, class DivorT> DivorT Arbitrary::s_longdiv (DivndT& dividend, DivorT const divisor, _BaseT const base) {
      
      // Group of digits to divide.
      DivorT dgroup = 0;
      DivorT dquot  = 0;
      
      // Quotient.
      DivndT quotient;
      
      // Long division steps through each digit of the dividend.
      for (DivndIT i = dividend.begin(); i != dividend.end(); ++i) {
        
        // Add this digit to the group.
        dgroup = of_add<DivorT, DivorT, DivndDT>(dgroup, *i);
        
        // Divide the group. Make sure the quotient can hold the result before
        // adding it to the digit array.
        dquot = dgroup / divisor;
        quotient.push_back(of_static_cast<DivndDT, DivorT>(dquot));
        
        // Take out what we've divided. No need to check for overflow,
        // dquot * divisor must be smaller than or equal to the previous
        // value of dgroup since division gets the floor.
        dgroup = of_sub<DivorT, DivorT, DivorT>(dgroup, dquot * divisor);
        
        // Move the remainder up to the next order of magnitude.
        dgroup = of_mul<DivorT, DivorT, _BaseT>(dgroup, base);
        
      }
      
      // Trim zeros from the quotient.
      s_trimZeros<DivndT, DivndIT>(quotient, _BEGIN);
      
      // Set the result in place.
      dividend.swap(quotient);
      
      // Undo the last base shift, this is the remainder. Return it.
      return (dgroup / base);
      
    }
    /*************************************************************************/
    
    /*************************************************************************/
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
    /*************************************************************************/
    
    /*************************************************************************/
    // Convert the base of a given container, return a new container.
    template <class ToT, class ToDT, class FromT, class FromDT, class FromIT> ReferencePointer<ToT> Arbitrary::s_baseconv (FromT const& from, _BaseT const frombase, _BaseT const tobase) {
      
      // Return this container. Use a safe pointer to get around ridiculous
      // reference scope problems. Seriously, how hard is it to have a
      // reference type that doesn't stupidly fall out of scope when it's used
      // as a return type? Bleh.
      ReferencePointer<ToT> retval(new ToT);
      
      // Temporary, since we will be doing work in place.
      FromT tempfrom(from);
      
      // Convert base by storing the remainder of repeated division.
      while (tempfrom.size() > 0) {
        retval->push_back(of_static_cast<ToDT, _BaseT>(s_longdiv<FromT, FromDT, FromIT, _BaseT>(tempfrom, tobase, frombase)));
      }
      
      // Return
      return retval;
      
    }
    /*************************************************************************/
    
    /*************************************************************************/
    // Clean the internal data structure.
    inline Arbitrary& Arbitrary::_cleanup () {
      return set(toString());
    }
    /*************************************************************************/
    
    /*************************************************************************/
    inline Arbitrary::_Data& Arbitrary::_Data::operator = (Arbitrary::_Data const& right) { return copy(right); }
    /*************************************************************************/
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************/
  // Stream I/O operators.
  inline std::istream& operator >> (std::istream& left, DAC::Arbitrary&       right) { std::string input; std::cin >> input; right.set(input); return left; }
  inline std::ostream& operator << (std::ostream& left, DAC::Arbitrary const& right) { left << right.toString();                               return left; }
  /***************************************************************************/
  
  /***************************************************************************/
  // Arithmetic operators.
  inline DAC::Arbitrary operator + (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_add(right); }
  //inline DAC::Arbitrary operator - (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_sub(right); }
  inline DAC::Arbitrary operator * (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_mul(right); }
  inline DAC::Arbitrary operator / (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_div(right); }
  //inline DAC::Arbitrary operator % (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_div(right); }
  /***************************************************************************/
  
  /***************************************************************************/
  // Bitshift operators.
  inline DAC::Arbitrary operator >> (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_shr(right); }
  //inline DAC::Arbitrary operator << (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_shl(right); }
  /***************************************************************************/
  
  /***************************************************************************/
  // Comparison operators.
  inline bool operator == (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { return left.op_eq(right); }
  inline bool operator != (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { return left.op_ne(right); }
  inline bool operator >  (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { return left.op_gt(right); }
  inline bool operator >= (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { return left.op_ge(right); }
  inline bool operator <  (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { return left.op_lt(right); }
  inline bool operator <= (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { return left.op_le(right); }
  /***************************************************************************/

  /***************************************************************************/
  // Bitwise operators.
  inline DAC::Arbitrary operator & (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_bit_and(right); }
  //DAC::Arbitrary operator | (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_bit_or(right);  }
  //DAC::Arbitrary operator ^ (DAC::Arbitrary const& left, DAC::Arbitrary const& right) { DAC::Arbitrary retval(left); return retval.op_bit_xor(right); }
  /***************************************************************************/
  
#endif
