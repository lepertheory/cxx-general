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
  #include <limits>

// Internal includes.
  #include "SafeInt.hxx"
  #include "ReferencePointer.hxx"
  #include "toString.hxx"
  #include "rppower.hxx"
  #include "Exception.hxx"

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
      template <class T> explicit ArbInt (SafeInt<T>  const& number);
      template <class T> explicit ArbInt (T           const  number);
      
      // Increment / decrement operators.
      ArbInt& operator ++ ();
      ArbInt  operator ++ (int);
      ArbInt& operator -- ();
      ArbInt  operator -- (int);
      
      // Unary sign operators.
      ArbInt operator + () const;
      ArbInt operator - () const;
      
      // Not operator.
      bool operator ! () const;
      
      // Bitwise compliment.
      ArbInt operator ~ () const;
      
      // Casting operators.
      operator bool                   () const;
      operator signed   char          () const;
      operator unsigned char          () const;
      operator signed   short int     () const;
      operator unsigned short int     () const;
      operator signed   int           () const;
      operator unsigned int           () const;
      operator signed   long int      () const;
      operator unsigned long int      () const;
      operator signed   long long int () const;
      operator unsigned long long int () const;
      
      // Assignment operator.
                         ArbInt& operator = (ArbInt      const& number);
                         ArbInt& operator = (std::string const& number);
      template <class T> ArbInt& operator = (SafeInt<T>  const& number);
      template <class T> ArbInt& operator = (T           const  number);
      
      // Arithmetic assignment operators.
                         ArbInt& operator *= (ArbInt     const& number);
      template <class T> ArbInt& operator *= (SafeInt<T> const& number);
      template <class T> ArbInt& operator *= (T          const  number);
                         ArbInt& operator /= (ArbInt     const& number);
      template <class T> ArbInt& operator /= (SafeInt<T> const& number);
      template <class T> ArbInt& operator /= (T          const  number);
                         ArbInt& operator %= (ArbInt     const& number);
      template <class T> ArbInt& operator %= (SafeInt<T> const& number);
      template <class T> ArbInt& operator %= (T          const  number);
                         ArbInt& operator += (ArbInt     const& number);
      template <class T> ArbInt& operator += (SafeInt<T> const& number);
      template <class T> ArbInt& operator += (T          const  number);
                         ArbInt& operator -= (ArbInt     const& number);
      template <class T> ArbInt& operator -= (SafeInt<T> const& number);
      template <class T> ArbInt& operator -= (T          const  number);
      
      // Bit shift assignment operators.
                         ArbInt& operator <<= (ArbInt     const& number);
      template <class T> ArbInt& operator <<= (SafeInt<T> const& number);
      template <class T> ArbInt& operator <<= (T          const  number);
                         ArbInt& operator >>= (ArbInt     const& number);
      template <class T> ArbInt& operator >>= (SafeInt<T> const& number);
      template <class T> ArbInt& operator >>= (T          const  number);
      
      // Bitwise assignment operators.
                         ArbInt& operator &= (ArbInt     const& number);
      template <class T> ArbInt& operator &= (SafeInt<T> const& number);
      template <class T> ArbInt& operator &= (T          const  number);
                         ArbInt& operator |= (ArbInt     const& number);
      template <class T> ArbInt& operator |= (SafeInt<T> const& number);
      template <class T> ArbInt& operator |= (T          const  number);
                         ArbInt& operator ^= (ArbInt     const& number);
      template <class T> ArbInt& operator ^= (SafeInt<T> const& number);
      template <class T> ArbInt& operator ^= (T          const  number);
      
      // Accessors.
                         value_type Base  () const;
      template <class T> T          Value () const;
                         ArbInt& Base  (value_type const base);
      template <class T> ArbInt& Value (T          const number);
      
      // Reset to just-constructed state.
      ArbInt& clear ();
      
      // Copy another number.
      ArbInt& copy (ArbInt const& number, bool const copynow = false);
      
      // Set this number.
                         ArbInt& set (ArbInt      const& number);
                         ArbInt& set (std::string const& number);
      template <class T> ArbInt& set (SafeInt<T>  const& number);
      template <class T> ArbInt& set (T           const  number);
      
      // Push digits into this number.
                         ArbInt& push_back (ArbInt      const& number);
                         ArbInt& push_back (std::string const& number);
      template <class T> ArbInt& push_back (SafeInt<T>  const& number);
      template <class T> ArbInt& push_back (T           const  number);
      
      // Return a string of this number.
      std::string toString () const;
      
      // Arithmetic operator backends.
                         ArbInt& op_mul (ArbInt     const& number);
      template <class T> ArbInt& op_mul (SafeInt<T> const& number);
      template <class T> ArbInt& op_mul (T          const  number);
                         ArbInt& op_div (ArbInt     const& number, ArbInt* remainder = 0);
      template <class T> ArbInt& op_div (SafeInt<T> const& number, ArbInt* remainder = 0);
      template <class T> ArbInt& op_div (T          const  number, ArbInt* remainder = 0);
                         ArbInt& op_mod (ArbInt     const& number);
      template <class T> ArbInt& op_mod (SafeInt<T> const& number);
      template <class T> ArbInt& op_mod (T          const  number);
                         ArbInt& op_add (ArbInt     const& number);
      template <class T> ArbInt& op_add (SafeInt<T> const& number);
      template <class T> ArbInt& op_add (T          const  number);
                         ArbInt& op_sub (ArbInt     const& number);
      template <class T> ArbInt& op_sub (SafeInt<T> const& number);
      template <class T> ArbInt& op_sub (T          const  number);
      
      // Bit shift operator backends.
                         ArbInt& op_shl (ArbInt     const& number);
      template <class T> ArbInt& op_shl (SafeInt<T> const& number);
      template <class T> ArbInt& op_shl (T          const  number);
                         ArbInt& op_shr (ArbInt     const& number);
      template <class T> ArbInt& op_shr (SafeInt<T> const& number);
      template <class T> ArbInt& op_shr (T          const  number);
      
      // Comparison operator backends.
                         bool op_gt (ArbInt     const& number) const;
      template <class T> bool op_gt (SafeInt<T> const& number) const;
      template <class T> bool op_gt (T          const  number) const;
                         bool op_ge (ArbInt     const& number) const;
      template <class T> bool op_ge (SafeInt<T> const& number) const;
      template <class T> bool op_ge (T          const  number) const;
                         bool op_lt (ArbInt     const& number) const;
      template <class T> bool op_lt (SafeInt<T> const& number) const;
      template <class T> bool op_lt (T          const  number) const;
                         bool op_le (ArbInt     const& number) const;
      template <class T> bool op_le (SafeInt<T> const& number) const;
      template <class T> bool op_le (T          const  number) const;
                         bool op_eq (ArbInt     const& number) const;
      template <class T> bool op_eq (SafeInt<T> const& number) const;
      template <class T> bool op_eq (T          const  number) const;
                         bool op_ne (ArbInt     const& number) const;
      template <class T> bool op_ne (SafeInt<T> const& number) const;
      template <class T> bool op_ne (T          const  number) const;
      
      // Bitwise operator backends.
                         ArbInt& op_bit_and (ArbInt     const& number);
      template <class T> ArbInt& op_bit_and (SafeInt<T> const& number);
      template <class T> ArbInt& op_bit_and (T          const  number);
                         ArbInt& op_bit_ior (ArbInt     const& number);
      template <class T> ArbInt& op_bit_ior (SafeInt<T> const& number);
      template <class T> ArbInt& op_bit_ior (T          const  number);
                         ArbInt& op_bit_xor (ArbInt     const& number);
      template <class T> ArbInt& op_bit_xor (SafeInt<T> const& number);
      template <class T> ArbInt& op_bit_xor (T          const  number);
                         ArbInt& op_bit_cpm ();
      
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
      
      // Set from an integer type.
      template <class T> ArbInt& _set_int_signed (T const number);
      template <class T> ArbInt& _set_int        (T const number);
      
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
  
}

/***************************************************************************
 * Global operators.
 ***************************************************************************/

// Stream I/O operators.
std::ostream& operator << (std::ostream& l, DAC::ArbInt const& r);
std::istream& operator >> (std::istream& l, DAC::ArbInt&       r);

// Arithmetic operators.
                   DAC::ArbInt operator * (DAC::ArbInt     const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator * (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r);
template <class T> DAC::ArbInt operator * (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator * (DAC::ArbInt     const& l, T               const  r);
template <class T> DAC::ArbInt operator * (T               const  l, DAC::ArbInt     const& r);
                   DAC::ArbInt operator / (DAC::ArbInt     const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator / (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r);
template <class T> DAC::ArbInt operator / (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator / (DAC::ArbInt     const& l, T               const  r);
template <class T> DAC::ArbInt operator / (T               const  l, DAC::ArbInt     const& r);
                   DAC::ArbInt operator % (DAC::ArbInt     const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator % (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r);
template <class T> DAC::ArbInt operator % (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator % (DAC::ArbInt     const& l, T               const  r);
template <class T> DAC::ArbInt operator % (T               const  l, DAC::ArbInt     const& r);
                   DAC::ArbInt operator + (DAC::ArbInt     const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator + (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r);
template <class T> DAC::ArbInt operator + (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator + (DAC::ArbInt     const& l, T               const  r);
template <class T> DAC::ArbInt operator + (T               const  l, DAC::ArbInt     const& r);
                   DAC::ArbInt operator - (DAC::ArbInt     const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator - (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r);
template <class T> DAC::ArbInt operator - (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator - (DAC::ArbInt     const& l, T               const  r);
template <class T> DAC::ArbInt operator - (T               const  l, DAC::ArbInt     const& r);

// Bit shift operators.
                   DAC::ArbInt operator << (DAC::ArbInt     const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator << (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r);
template <class T> DAC::ArbInt operator << (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator << (DAC::ArbInt     const& l, T               const  r);
template <class T> DAC::ArbInt operator << (T               const  l, DAC::ArbInt     const& r);
                   DAC::ArbInt operator >> (DAC::ArbInt     const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator >> (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r);
template <class T> DAC::ArbInt operator >> (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator >> (DAC::ArbInt     const& l, T               const  r);
template <class T> DAC::ArbInt operator >> (T               const  l, DAC::ArbInt     const& r);

// Comparsion operators.
                   bool operator >  (DAC::ArbInt     const& l, DAC::ArbInt     const& r);
template <class T> bool operator >  (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r);
template <class T> bool operator >  (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r);
template <class T> bool operator >  (DAC::ArbInt     const& l, T               const  r);
template <class T> bool operator >  (T               const  l, DAC::ArbInt     const& r);
                   bool operator >= (DAC::ArbInt     const& l, DAC::ArbInt     const& r);
template <class T> bool operator >= (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r);
template <class T> bool operator >= (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r);
template <class T> bool operator >= (DAC::ArbInt     const& l, T               const  r);
template <class T> bool operator >= (T               const  l, DAC::ArbInt     const& r);
                   bool operator <  (DAC::ArbInt     const& l, DAC::ArbInt     const& r);
template <class T> bool operator <  (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r);
template <class T> bool operator <  (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r);
template <class T> bool operator <  (DAC::ArbInt     const& l, T               const  r);
template <class T> bool operator <  (T               const  l, DAC::ArbInt     const& r);
                   bool operator <= (DAC::ArbInt     const& l, DAC::ArbInt     const& r);
template <class T> bool operator <= (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r);
template <class T> bool operator <= (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r);
template <class T> bool operator <= (DAC::ArbInt     const& l, T               const  r);
template <class T> bool operator <= (T               const  l, DAC::ArbInt     const& r);
                   bool operator == (DAC::ArbInt     const& l, DAC::ArbInt     const& r);
template <class T> bool operator == (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r);
template <class T> bool operator == (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r);
template <class T> bool operator == (DAC::ArbInt     const& l, T               const  r);
template <class T> bool operator == (T               const  l, DAC::ArbInt     const& r);
                   bool operator != (DAC::ArbInt     const& l, DAC::ArbInt     const& r);
template <class T> bool operator != (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r);
template <class T> bool operator != (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r);
template <class T> bool operator != (DAC::ArbInt     const& l, T               const  r);
template <class T> bool operator != (T               const  l, DAC::ArbInt     const& r);

// Bitwise operators.
                   DAC::ArbInt operator & (DAC::ArbInt     const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator & (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r);
template <class T> DAC::ArbInt operator & (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator & (DAC::ArbInt     const& l, T               const  r);
template <class T> DAC::ArbInt operator & (T               const  l, DAC::ArbInt     const& r);
                   DAC::ArbInt operator | (DAC::ArbInt     const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator | (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r);
template <class T> DAC::ArbInt operator | (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator | (DAC::ArbInt     const& l, T               const  r);
template <class T> DAC::ArbInt operator | (T               const  l, DAC::ArbInt     const& r);
                   DAC::ArbInt operator ^ (DAC::ArbInt     const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator ^ (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r);
template <class T> DAC::ArbInt operator ^ (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r);
template <class T> DAC::ArbInt operator ^ (DAC::ArbInt     const& l, T               const  r);
template <class T> DAC::ArbInt operator ^ (T               const  l, DAC::ArbInt     const& r);

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
  }
  
}

/***************************************************************************
 * Inline and template definitions.
 ***************************************************************************/
namespace DAC {
  
  // Errors.
  namespace ArbIntErrors {
    inline char const* Base::what           () const throw() { return "Undefined error in ArbInt.";                                                                                                                                         }
    inline char const* BadFormat::what      () const throw() { return (std::string(_problem) + " at position " + DAC::toString(static_cast<std::string::size_type>(SafeInt<std::string::size_type>(_position) + 1)) + " in number \"" + *_number + "\".").c_str(); }
    inline BadFormat&  BadFormat::Problem   (char const*                   const problem)  throw() { _problem  = problem;  return *this; }
    inline BadFormat&  BadFormat::Position  (std::string::size_type        const position) throw() { _position = position; return *this; }
    inline BadFormat&  BadFormat::Number    (ConstReferencePointer<std::string>& number)   throw() { _number   = number;   return *this; }
    inline char const* Negative::what       () const throw() { return "Attempted to set ArbInt to a negative number.";                                                                                                                      }
    inline char const* Overrun::what        () const throw() { return "Instruction overruns end of container.";                                                                                                                             }
    inline char const* DivByZero::what      () const throw() { return "Attempt to divide by zero.";                                                                                                                                         }
    inline char const* ScalarOverflow::what () const throw() { return "ArbInt overflows scalar type.";                                                                                                                                      }
    inline char const* BaseOutOfRange::what () const throw() { return "Requested or specified base is out of range. Base minimum is base 2, base maximum is 2(bits in target container value type/2)-1.";                                   }
    inline char const* RootTooLarge::what   () const throw() { return "Cannot find a root that large.";                                                                                                                                     }
  }
  
  /*************************************************************************/
  // Function members.
  
  // Conversion constructor.
  template <class T> ArbInt::ArbInt (SafeInt<T> const& number) {
    
    // Call common init.
    _init();
    
    // Set the number.
    set(number);
    
  }
  template <class T> ArbInt::ArbInt (T const number) {
    
    // Call common init.
    _init();
    
    // Set the number.
    set(number);
    
  }
  
  // Increment / decrement operators.
  inline ArbInt& ArbInt::operator ++ ()    { return op_add(1);                               }
  inline ArbInt  ArbInt::operator ++ (int) { ArbInt retval(*this); op_add(1); return retval; }
  inline ArbInt& ArbInt::operator -- ()    { return op_sub(1);                               }
  inline ArbInt  ArbInt::operator -- (int) { ArbInt retval(*this); op_sub(1); return retval; }
  
  // Unary sign operators.
  inline ArbInt ArbInt::operator + () const { return *this;                   }
  inline ArbInt ArbInt::operator - () const { throw ArbIntErrors::Negative(); }
  
  // Bitwise compliment.
  inline ArbInt ArbInt::operator ~ () const { return ArbInt(*this).op_bit_cpm(); }
  
  // Casting operators.
  inline ArbInt::operator bool                   () const { return !isZero();                       }
  inline ArbInt::operator signed   char          () const { return Value<signed   char         >(); }
  inline ArbInt::operator unsigned char          () const { return Value<unsigned char         >(); }
  inline ArbInt::operator signed   short int     () const { return Value<signed   short int    >(); }
  inline ArbInt::operator unsigned short int     () const { return Value<unsigned short int    >(); }
  inline ArbInt::operator signed   int           () const { return Value<signed   int          >(); }
  inline ArbInt::operator unsigned int           () const { return Value<unsigned int          >(); }
  inline ArbInt::operator signed   long int      () const { return Value<signed   long int     >(); }
  inline ArbInt::operator unsigned long int      () const { return Value<unsigned long int     >(); }
  inline ArbInt::operator signed   long long int () const { return Value<signed   long long int>(); }
  inline ArbInt::operator unsigned long long int () const { return Value<unsigned long long int>(); }
  
  // Assignment operator.
                     inline ArbInt& ArbInt::operator = (ArbInt      const& number) { return set(number); }
                     inline ArbInt& ArbInt::operator = (std::string const& number) { return set(number); }
  template <class T> inline ArbInt& ArbInt::operator = (SafeInt<T>  const& number) { return set(number); }
  template <class T> inline ArbInt& ArbInt::operator = (T           const  number) { return set(number); }
  
  // Arithmetic assignment operators.
                     inline ArbInt& ArbInt::operator *= (ArbInt     const& number) { return op_mul(number); }
  template <class T> inline ArbInt& ArbInt::operator *= (SafeInt<T> const& number) { return op_mul(number); }
  template <class T> inline ArbInt& ArbInt::operator *= (T          const  number) { return op_mul(number); }
                     inline ArbInt& ArbInt::operator /= (ArbInt     const& number) { return op_div(number); }
  template <class T> inline ArbInt& ArbInt::operator /= (SafeInt<T> const& number) { return op_div(number); }
  template <class T> inline ArbInt& ArbInt::operator /= (T          const  number) { return op_div(number); }
                     inline ArbInt& ArbInt::operator %= (ArbInt     const& number) { return op_mod(number); }
  template <class T> inline ArbInt& ArbInt::operator %= (SafeInt<T> const& number) { return op_mod(number); }
  template <class T> inline ArbInt& ArbInt::operator %= (T          const  number) { return op_mod(number); }
                     inline ArbInt& ArbInt::operator += (ArbInt     const& number) { return op_add(number); }
  template <class T> inline ArbInt& ArbInt::operator += (SafeInt<T> const& number) { return op_add(number); }
  template <class T> inline ArbInt& ArbInt::operator += (T          const  number) { return op_add(number); }
                     inline ArbInt& ArbInt::operator -= (ArbInt     const& number) { return op_sub(number); }
  template <class T> inline ArbInt& ArbInt::operator -= (SafeInt<T> const& number) { return op_sub(number); }
  template <class T> inline ArbInt& ArbInt::operator -= (T          const  number) { return op_sub(number); }
  
  // Bit shift assignment operators.
                     inline ArbInt& ArbInt::operator <<= (ArbInt     const& number) { return op_shl(number); }
  template <class T> inline ArbInt& ArbInt::operator <<= (SafeInt<T> const& number) { return op_shl(number); }
  template <class T> inline ArbInt& ArbInt::operator <<= (T          const  number) { return op_shl(number); }
                     inline ArbInt& ArbInt::operator >>= (ArbInt     const& number) { return op_shr(number); }
  template <class T> inline ArbInt& ArbInt::operator >>= (SafeInt<T> const& number) { return op_shr(number); }
  template <class T> inline ArbInt& ArbInt::operator >>= (T          const  number) { return op_shr(number); }
  
  // Bitwise assignment operators.
                     inline ArbInt& ArbInt::operator &= (ArbInt     const& number) { return op_bit_and(number); }
  template <class T> inline ArbInt& ArbInt::operator &= (SafeInt<T> const& number) { return op_bit_and(number); }
  template <class T> inline ArbInt& ArbInt::operator &= (T          const  number) { return op_bit_and(number); }
                     inline ArbInt& ArbInt::operator |= (ArbInt     const& number) { return op_bit_ior(number); }
  template <class T> inline ArbInt& ArbInt::operator |= (SafeInt<T> const& number) { return op_bit_ior(number); }
  template <class T> inline ArbInt& ArbInt::operator |= (T          const  number) { return op_bit_ior(number); }
                     inline ArbInt& ArbInt::operator ^= (ArbInt     const& number) { return op_bit_xor(number); }
  template <class T> inline ArbInt& ArbInt::operator ^= (SafeInt<T> const& number) { return op_bit_xor(number); }
  template <class T> inline ArbInt& ArbInt::operator ^= (T          const  number) { return op_bit_xor(number); }
  
  // Get the base of this number.
  inline ArbInt::value_type ArbInt::Base () const { return _base; }
  
  // Get the value of this number.
  template <class T> T ArbInt::Value () const {
    SafeInt<T> retval;
    for (typename _DigsT::iterator i = _digits->begin(); i != _digits->end(); ++i) {
      try {
        retval += *i * rppower(SafeInt<_DigT>(s_digitbase), (i - _digits->begin()));
      } catch (SafeIntErrors::Overflow) {
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
  template <class T> inline ArbInt& ArbInt::set (SafeInt<T>         const& number) { return set(number.Value());     }
  template <>        inline ArbInt& ArbInt::set (bool               const  number) { return _set_int(number);        }
  template <>        inline ArbInt& ArbInt::set (unsigned char      const  number) { return _set_int(number);        }
  template <>        inline ArbInt& ArbInt::set (signed   char      const  number) { return _set_int_signed(number); }
  template <>        inline ArbInt& ArbInt::set (unsigned short int const  number) { return _set_int(number);        }
  template <>        inline ArbInt& ArbInt::set (signed   short int const  number) { return _set_int_signed(number); }
  template <>        inline ArbInt& ArbInt::set (unsigned int       const  number) { return _set_int(number);        }
  template <>        inline ArbInt& ArbInt::set (signed   int       const  number) { return _set_int_signed(number); }
  template <>        inline ArbInt& ArbInt::set (unsigned long int  const  number) { return _set_int(number);        }
  template <>        inline ArbInt& ArbInt::set (signed   long int  const  number) { return _set_int_signed(number); }
  template <class T>        ArbInt& ArbInt::set (T                  const  number) {
    std::string                     num(DAC::toString(number));
    typename std::string::size_type pos(num.find_first_of('.'));
    if (pos != std::string::npos) {
      num.erase(pos);
    }
    return set(num);
  }
  
  // Push a string onto the back of this number.
                     inline ArbInt& ArbInt::push_back (ArbInt     const& number) { return push_back(ArbInt(number).Base(_base).toString()); }
  template <class T> inline ArbInt& ArbInt::push_back (SafeInt<T> const& number) { return push_back(number.Value());                        }
  template <class T> inline ArbInt& ArbInt::push_back (T          const  number) { return push_back(DAC::toString(number));                 }
  
  // Multiply by an integral type.
  template <class T> ArbInt& ArbInt::op_mul (SafeInt<T> const& number) {
    
    // If the number is too large to do in one step, resort to conversion to
    // ArbInt then multiply.
    if (number >= s_digitbase) {
      
      // DO IT.
      return op_mul(ArbInt(number));
      
    // Otherwise, multiply directly.
    } else {
      
      // If we were sent a negative number, error.
      if (number < 0) {
        throw ArbIntErrors::Negative();
      }
      
      // If we were sent zero, blank the digits.
      if (number == 0) {
        _digits = new _DigsT;
        return *this;
      }
      
      // If we were sent 1, easy.
      if (number == 1) {
        return *this;
      }
      
      // Work area.
      ArbInt retval;
      
      // Multiply like 3rd grade.
      for (_DigsT::size_type i = 0; i != _digits->size(); ++i) {
        
        // Create a new digit if necessary.
        if (retval._digits->size() == i) {
          retval._digits->push_back(0);
        }
        
        // Multiply the digit and carry.
        (*(retval._digits))[i] = (*(retval._digits))[i] + (*(_digits))[i] * number;
        retval._carry(i);
        
      }
      
      // Move the digits into place and return.
      _digits = retval._digits;
      return *this;
      
    }
    
  }
  template <class T> inline ArbInt& ArbInt::op_mul (T const number) { return op_mul(SafeInt<T>(number)); }
  
  // Divide by an integral type.
  template <class T> ArbInt& ArbInt::op_div (SafeInt<T> const& number, ArbInt* remainder) {
    
    // If the number is too large to do in one step, resort to conversion to
    // ArbInt then divide.
    if (number >= s_digitbase) {
      
      // Just do it.
      return op_div(ArbInt(number), remainder);
      
    // Otherwise, divide directly.
    } else {
      
      // Cannot divide by zero.
      if (number == 0) {
        throw ArbIntErrors::DivByZero();
      }
      
      // Cannot return a negative number.
      if (number < 0) {
        throw ArbIntErrors::Negative();
      }
      
      // If dividing by 1, easy, remainder is zero and return.
      if (number == 1) {
        if (remainder) {
          *remainder = 0;
        }
        return *this;
      }
      
      // Work area.
      ArbInt retval(*this, true);
      
      // Already wrote the long division algorithm, reuse it. Different
      // calls for remainder or not.
      if (remainder) {
        *remainder = s_longDiv((*(retval._digits)), number, s_digitbase);
      } else {
        s_longDiv((*(retval._digits)), number, s_digitbase);
      }
      
      // Move the digits into place and return.
      _digits = retval._digits;
      
    }
    
  }
  template <class T> inline ArbInt& ArbInt::op_div (T const number, ArbInt* remainder) { return op_div(SafeInt<T>(number), remainder); }
  
  // Modulo division by an integral type.
  template <class T> ArbInt& ArbInt::op_mod (SafeInt<T> const& number) {
    
    // Work area.
    ArbInt retval;
    ArbInt quotient(*this);
    
    // Do division and return the remainder.
    quotient.op_div(number, &retval);
    _digits = retval._digits;
    return *this;
    
  }
  template <class T> inline ArbInt& ArbInt::op_mod (T const number) { return op_mod(SafeInt<T>(number)); }
  
  // Add an integral type.
  template <class T> ArbInt& ArbInt::op_add (SafeInt<T> const& number) {
    
    // If the number is too large to add directly, resort to conversion to
    // ArbInt then add.
    if (number >= s_digitbase) {
      
      // Do it do it do it.
      return op_add(ArbInt(number));
      
    // Otherwise add directly.
    } else {
      
      // If adding a negative number, subtract the positive.
      if (number < 0) {
        return op_sub(-number);
      }
      
      // Nothing to do if adding 0.
      if (number == 0) {
        return *this;
      }
      
      // Work area.
      ArbInt retval(*this, true);
      
      // Add and carry or create a digit if necessary.
      if (retval._digits->empty()) {
        retval._digits->push_back(number);
      } else {
        (*(retval._digits))[0] = (*(retval._digits))[0] + number;
        retval._carry(0);
      }
      
      // Move digits into place and return.
      _digits = retval._digits;
      return *this;
      
    }
    
  }
  template <class T> inline ArbInt& ArbInt::op_add (T const number) { return op_add(SafeInt<T>(number)); }
  
  // Subtract an integral type.
  template <class T> ArbInt& ArbInt::op_sub (SafeInt<T> const& number) {
    
    // If the number is too large to subtract directly, resort to conversion
    // to ArbInt then subtract.
    if (number >= s_digitbase) {
      
      // Doin it and doin it and doin it.
      return op_sub(ArbInt(number));
      
    // Otherwise subtract directly.
    } else {
      
      // If subtracting a negative number, add the positive.
      if (number < 0) {
        return op_add(-number);
      }
      
      // Nothing to do if subtracting 0.
      if (number == 0) {
        return *this;
      }
      
      // If number > this, throw an error.
      if (number > *this) {
        throw ArbIntErrors::Negative();
      }
      
      // Work area.
      ArbInt retval(*this, true);
      
      // Borrow if necessary and subtract.
      if ((*(retval._digits))[0] < number) {
        retval._borrow(0);
      }
      (*(retval._digits))[0] = (*(retval._digits))[0] - number;
      
      // Trim zeros.
      retval._trimZeros();
      
      // Move the digits into place and return.
      _digits = retval._digits;
      return *this;
      
    }
    
  }
  template <class T> inline ArbInt& ArbInt::op_sub (T const number) { return op_sub(SafeInt<T>(number)); }
  
  // Shift left, shift right.
                     inline ArbInt& ArbInt::op_shl (ArbInt     const& number) { ArbInt retval(*this, true); retval._shift(number, _DIR_L); return copy(retval); }
  template <class T> inline ArbInt& ArbInt::op_shl (SafeInt<T> const& number) { ArbInt retval(*this, true); retval._shift(number, _DIR_L); return copy(retval); }
  template <class T> inline ArbInt& ArbInt::op_shl (T          const  number) { ArbInt retval(*this, true); retval._shift(number, _DIR_L); return copy(retval); }
                     inline ArbInt& ArbInt::op_shr (ArbInt     const& number) { ArbInt retval(*this, true); retval._shift(number, _DIR_R); return copy(retval); }
  template <class T> inline ArbInt& ArbInt::op_shr (SafeInt<T> const& number) { ArbInt retval(*this, true); retval._shift(number, _DIR_L); return copy(retval); }
  template <class T> inline ArbInt& ArbInt::op_shr (T          const  number) { ArbInt retval(*this, true); retval._shift(number, _DIR_R); return copy(retval); }
  
  // Greater than an integral type.
  template <class T> bool ArbInt::op_gt (SafeInt<T> const& number) const {
    
    // Always greater than negative numbers.
    if (number < 0) {
      return true;
    }
    
    // If the number is too large to compare directly, resort to conversion
    // to ArbInt then compare.
    if (number >= s_digitbase) {
      
      // RAWR!
      return op_gt(ArbInt(number));
      
    // Otherwise compare directly.
    } else {
      
      // If the container is empty, it cannot be greater than anything but
      // a negative number.
      if (_digits->empty()) {
        return (number < 0);
      }
      
      // If the container is larger than 1, it must be greater.
      if (_digits->size() > 1) {
        return true;
      }
      
      // Compare.
      return ((*(_digits))[0] > number);
      
    }
    
  }
  template <class T> inline bool ArbInt::op_gt (T const number) const { return op_gt(SafeInt<T>(number)); }
  
  // Less than an integral type.
  template <class T> bool ArbInt::op_lt (SafeInt<T> const& number) const {
    
    // Never less than 0.
    if (number <= 0) {
      return false;
    }
    
    // If the number is too large to compare directly, resort to conversion
    // to ArbInt then compare.
    if (number >= s_digitbase) {
      
      // Pong.
      return op_lt(ArbInt(number));
    
    // Otherwise compare directly.  
    } else {
      
      // If the container is empty, it must be less than anything but 0,
      // which was already checked.
      if (_digits->empty()) {
        return true;
      }
      
      // If the container is larger than 1 element, it cannot be less.
      if (_digits->size() > 1) {
        return false;
      }
      
      // Compare.
      return ((*(_digits))[0] < number);
      
    }
    
  }
  template <class T> inline bool ArbInt::op_lt (T const number) const { return op_lt(SafeInt<T>(number)); }
  
  // Equal to an integral type.
  template <class T> bool ArbInt::op_eq (SafeInt<T> const& number) const {
    
    // Never less than 0.
    if (number < 0) {
      return false;
    }
    
    // If the number is too large to compare directly, resort to conversion
    // to ArbInt then compare.
    if (number >= s_digitbase) {
      
      // Horf.
      return op_eq(ArbInt(number));
      
    // Otherwise compare directly.
    } else {
      
      // If the container is empty, it can only equal 0.
      if (_digits->empty()) {
        return (number == 0);
      }
      
      // If the container is larger than 1 element, it cannot be equal.
      if (_digits->size() > 1) {
        return false;
      }
      
      // Compare.
      return ((*(_digits))[0] == number);
      
    }
    
  }
  template <class T> inline bool ArbInt::op_eq (T const number) const { return op_eq(SafeInt<T>(number)); }
  
  // Comparison operators.
                     inline bool ArbInt::op_ge (ArbInt     const& number) const { return !op_lt(number); }
  template <class T> inline bool ArbInt::op_ge (SafeInt<T> const& number) const { return !op_lt(number); }
  template <class T> inline bool ArbInt::op_ge (T          const  number) const { return !op_lt(number); }
                     inline bool ArbInt::op_le (ArbInt     const& number) const { return !op_gt(number); }
  template <class T> inline bool ArbInt::op_le (SafeInt<T> const& number) const { return !op_gt(number); }
  template <class T> inline bool ArbInt::op_le (T          const  number) const { return !op_gt(number); }
                     inline bool ArbInt::op_ne (ArbInt     const& number) const { return !op_eq(number); }
  template <class T> inline bool ArbInt::op_ne (SafeInt<T> const& number) const { return !op_eq(number); }
  template <class T> inline bool ArbInt::op_ne (T          const  number) const { return !op_eq(number); }
  
  // Bitwise AND with an integral type.
  template <class T> ArbInt& ArbInt::op_bit_and (SafeInt<T> const& number) {
    
    // No negatives.
    if (number < 0) {
      throw ArbIntErrors::Negative();
    }
    
    // If the number is too large to AND directly, resort to conversion to
    // ArbInt then AND.
    if (number >= s_digitbase) {
      
      // Poopyfarts!
      return op_bit_and(ArbInt(number));
      
    // Otherwise AND directly.
    } else {
      
      // If this number is 0, nothing needs to be done.
      if (_digits->empty()) {
        return *this;
      }
      
      // Work area, but don't bother copying anything more than the least
      // significant digit. In fact, do the AND right here, it's easy.
      ArbInt retval((*(_digits))[0] & number);
      
      // Copy into place and return.
      _digits = retval._digits;
      return *this;
      
    }
    
  }
  template <class T> inline ArbInt& ArbInt::op_bit_and (T const number) { return op_bit_and(SafeInt<T>(number)); }
  
  // Bitwise inclusive OR with an integral type.
  template <class T> ArbInt& ArbInt::op_bit_ior (SafeInt<T> const& number) {
    
    // No negatives.
    if (number < 0) {
      throw ArbIntErrors::Negative();
    }
    
    // If the number is too large to OR directly, resort to conversion to
    // ArbInt then OR.
    if (number >= s_digitbase) {
      
      // Stinknugget.
      return op_bit_ior(ArbInt(number));
      
    // Otherwise OR directly.
    } else {
      
      // If this number is 0, simply set with number.
      if (_digits->empty()) {
        return set(number);
      }
      
      // Copy digits and OR.
      ArbInt retval(*this, true);
      (*(retval._digits))[0] = (*(retval._digits))[0] | number;
      
      // Move the result into place and return.
      _digits = retval._digits;
      return *this;
      
    }
    
  }
  template <class T> inline ArbInt& ArbInt::op_bit_ior (T const number) { return op_bit_ior(SafeInt<T>(number)); }
  
  // Bitwise exclusive OR with an integral type.
  template <class T> ArbInt& ArbInt::op_bit_xor (SafeInt<T> const& number) {
    
    // No negatives.
    if (number < 0) {
      throw ArbIntErrors::Negative();
    }
    
    // If the number is too large to OR directly, resort to conversion to
    // ArbInt then OR.
    if (number >= s_digitbase) {
      
      // Blargh.
      return op_bit_xor(ArbInt(number));
      
    // Otherwise OR directly.
    } else {
      
      // If this number is 0, simply set with number.
      if (_digits->empty()) {
        return set(number);
      }
      
      // Copy digits and OR.
      ArbInt retval(*this, true);
      (*(retval._digits))[0] = (*(retval._digits))[0] ^ number;
      
      // Move the result into place and return.
      _digits = retval._digits;
      return *this;
      
    }
    
  }
  template <class T> inline ArbInt& ArbInt::op_bit_xor (T const number) { return op_bit_xor(SafeInt<T>(number)); }
  
  // Tests if this number is equal to zero.
  inline bool ArbInt::isZero () const { return (_digits->empty()); }
  
  // Tests if this number is odd or even.
  inline bool ArbInt::isOdd  () const { return (!isZero() && (_digits->front() & 1)); }
  inline bool ArbInt::isEven () const { return !isOdd();                              }
  
  // Placeholder for automatic pow conversion.
  template <class T> inline ArbInt ArbInt::pow (T const exp) { return pow(ArbInt(exp)); }
  
  // Return the maximum string input base.
  inline ArbInt::value_type ArbInt::max_input_base () { return SafeInt<value_type>(s_numidigits); }
  
  // Set from an integer type.
  template <class T> ArbInt& ArbInt::_set_int_signed (T const number) {
    
    // If number is less than zero, throw an error.
    if (number < 0) {
      throw ArbIntErrors::Negative();
    }
    
    // Otherwise, set like normal.
    return _set_int(number);
    
  }
  template <class T> ArbInt& ArbInt::_set_int (T const number) {
    
    // Work area.
    _DataPT new_digits(new _DigsT);
    
    // Only something to set if the number is not zero.
    if (number != 0) {
      
      // If the number is less than base, simply set it.
      SafeInt<T> tmp_number(number);
      if (tmp_number < s_digitbase) {
        
        // Easy :)
        new_digits->push_back(SafeInt<_DigT>(tmp_number.Value()));
        
      // Otherwise, full conversion.
      } else {
      
        // Convert with repeated division.
        while (tmp_number > 0) {
          new_digits->push_back(SafeInt<_DigT>(tmp_number % s_digitbase));
          tmp_number /= s_digitbase;
        }
        
      }
      
    }
    
    // Swap in the new digits and return.
    _digits = new_digits;
    return *this;
    
  }
  
  // Trim insignificant zeros.
  inline ArbInt& ArbInt::_trimZeros () { s_trimZerosE(*_digits); return *this; }
  
  // Trim leading and trailing zeros from a given container.
  template <class T> void ArbInt::s_trimZeros (T& c) { s_trimZerosB(c); s_trimZerosE(c); }
  
  // Trim leading zeros from a given container.
  template <class T> void ArbInt::s_trimZerosB (T& c) {
    
    // Nothing to do if empty.
    if (!c.empty()) {
      
      // Work data.
      typename T::iterator pos;
      
      // Trim leading zeros.
      for (pos = c.begin(); (pos != c.end()) && (*pos == 0); ++pos) {}
      if (pos >= c.begin()) {
        c.erase(c.begin(), pos);
      }
      
    }
    
  }
  
  // Trim trailing zeros from a given container.
  template <class T> void ArbInt::s_trimZerosE (T& c) {
    
    // Nothing to do if empty.
    if (!c.empty()) {
      
      // Work data.
      typename T::iterator pos;
      
      // Trim trailing zeros.
      for (pos = (c.end() - 1); (pos != (c.begin() - 1)) && (*pos == 0); --pos) {}
      if (pos++ < (c.end() - 1)) {
        c.erase(pos, c.end());
      }
      
    }
    
  }
  
  // Do long division on a given container in the specified base. Divisor
  // type must be 2x base. This is not checked, so be careful!
  template <class DivndT, class DivorT> DivorT ArbInt::s_longDiv (DivndT& divnd, DivorT const divor, value_type const base) {
    
    // Group of digits to divide.
    SafeInt<DivorT> dgroup;
    SafeInt<DivorT> dquot;
    
    // Quotient.
    DivndT quotient;
    
    // Long division steps through each digit of the dividend.
    for (typename DivndT::reverse_iterator i = divnd.rbegin(); i != divnd.rend(); ++i) {
      
      // Add this digit to the group.
      dgroup += *i;
      
      // Divide the group and add the result to the quotient.
      dquot = dgroup / divor;
      quotient.insert(quotient.begin(), SafeInt<typename DivndT::value_type>(dquot));
      
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
    return SafeInt<DivorT>(dgroup / base);
    
  }
  
  // Convert a container from one base to another. Frombase must be
  // 2^(bits/2) of from or smaller, tobase must be 2^(bits/2) of to or
  // smaller.
  template <class FT, class TT> void ArbInt::s_baseConv (FT const& from, value_type const frombase, TT& to, value_type const tobase) {
    
    // Verify that bases are valid. Base must be at least two, and at most
    // half of the significant bits of the target container's number type.
    // Maximum base is static, and so should only be calculated once per
    // type combination.
    static SafeInt<typename TT::value_type> maxbase = rppower(SafeInt<typename TT::value_type>(2), std::numeric_limits<typename TT::value_type>::digits >> 1);
    if ((SafeInt<value_type>(frombase) < 2) || (SafeInt<value_type>(tobase) < 2) || (SafeInt<value_type>(frombase) > maxbase) || (SafeInt<value_type>(tobase) > maxbase)) {
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
      to.push_back(SafeInt<typename TT::value_type>(s_longDiv(tmp_from, tobase, frombase)));
    }
    
  }
  
}

/***************************************************************************
 * Inline and template definitions.
 ***************************************************************************/

// Stream I/O operators.
inline std::ostream& operator << (std::ostream& l, DAC::ArbInt const& r) { l << r.toString();                                  return l; }
inline std::istream& operator >> (std::istream& l, DAC::ArbInt&       r) { std::string input; std::cin >> input; r.set(input); return l; }

// Arithmetic operators.
                   inline DAC::ArbInt operator * (DAC::ArbInt     const& l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_mul(r); }
template <class T> inline DAC::ArbInt operator * (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r) { return DAC::ArbInt(l).op_mul(r); }
template <class T> inline DAC::ArbInt operator * (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r) { return DAC::ArbInt(r).op_mul(l); }
template <class T> inline DAC::ArbInt operator * (DAC::ArbInt     const& l, T               const  r) { return DAC::ArbInt(l).op_mul(r); }
template <class T> inline DAC::ArbInt operator * (T               const  l, DAC::ArbInt     const& r) { return DAC::ArbInt(r).op_mul(l); }
                   inline DAC::ArbInt operator / (DAC::ArbInt     const& l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_div(r); }
template <class T> inline DAC::ArbInt operator / (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r) { return DAC::ArbInt(l).op_div(r); }
template <class T> inline DAC::ArbInt operator / (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_div(r); }
template <class T> inline DAC::ArbInt operator / (DAC::ArbInt     const& l, T               const  r) { return DAC::ArbInt(l).op_div(r); }
template <class T> inline DAC::ArbInt operator / (T               const  l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_div(r); }
                   inline DAC::ArbInt operator % (DAC::ArbInt     const& l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_mod(r); }
template <class T> inline DAC::ArbInt operator % (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r) { return DAC::ArbInt(l).op_mod(r); }
template <class T> inline DAC::ArbInt operator % (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_mod(r); }
template <class T> inline DAC::ArbInt operator % (DAC::ArbInt     const& l, T               const  r) { return DAC::ArbInt(l).op_mod(r); }
template <class T> inline DAC::ArbInt operator % (T               const  l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_mod(r); }
                   inline DAC::ArbInt operator + (DAC::ArbInt     const& l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_add(r); }
template <class T> inline DAC::ArbInt operator + (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r) { return DAC::ArbInt(l).op_add(r); }
template <class T> inline DAC::ArbInt operator + (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r) { return DAC::ArbInt(r).op_add(l); }
template <class T> inline DAC::ArbInt operator + (DAC::ArbInt     const& l, T               const  r) { return DAC::ArbInt(l).op_add(r); }
template <class T> inline DAC::ArbInt operator + (T               const  l, DAC::ArbInt     const& r) { return DAC::ArbInt(r).op_add(l); }
                   inline DAC::ArbInt operator - (DAC::ArbInt     const& l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_sub(r); }
template <class T> inline DAC::ArbInt operator - (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r) { return DAC::ArbInt(l).op_sub(r); }
template <class T> inline DAC::ArbInt operator - (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_sub(r); }
template <class T> inline DAC::ArbInt operator - (DAC::ArbInt     const& l, T               const  r) { return DAC::ArbInt(l).op_sub(r); }
template <class T> inline DAC::ArbInt operator - (T               const  l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_sub(r); }

// Bit shift operators.
                   inline DAC::ArbInt operator << (DAC::ArbInt     const& l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_shl(r); }
template <class T> inline DAC::ArbInt operator << (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r) { return DAC::ArbInt(l).op_shl(r); }
template <class T> inline DAC::ArbInt operator << (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_shl(r); }
template <class T> inline DAC::ArbInt operator << (DAC::ArbInt     const& l, T               const  r) { return DAC::ArbInt(l).op_shl(r); }
template <class T> inline DAC::ArbInt operator << (T               const  l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_shl(r); }
                   inline DAC::ArbInt operator >> (DAC::ArbInt     const& l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_shr(r); }
template <class T> inline DAC::ArbInt operator >> (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r) { return DAC::ArbInt(l).op_shr(r); }
template <class T> inline DAC::ArbInt operator >> (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_shr(r); }
template <class T> inline DAC::ArbInt operator >> (DAC::ArbInt     const& l, T               const  r) { return DAC::ArbInt(l).op_shr(r); }
template <class T> inline DAC::ArbInt operator >> (T               const  l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_shr(r); }

// Comparsion operators.
                   inline bool operator >  (DAC::ArbInt     const& l, DAC::ArbInt     const& r) { return  l.op_gt(r); }
template <class T> inline bool operator >  (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r) { return  l.op_gt(r); }
template <class T> inline bool operator >  (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r) { return !r.op_ge(l); }
template <class T> inline bool operator >  (DAC::ArbInt     const& l, T               const  r) { return  l.op_gt(r); }
template <class T> inline bool operator >  (T               const  l, DAC::ArbInt     const& r) { return !r.op_ge(l); }
                   inline bool operator >= (DAC::ArbInt     const& l, DAC::ArbInt     const& r) { return  l.op_ge(r); }
template <class T> inline bool operator >= (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r) { return  l.op_ge(r); }
template <class T> inline bool operator >= (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r) { return !r.op_gt(l); }
template <class T> inline bool operator >= (DAC::ArbInt     const& l, T               const  r) { return  l.op_ge(r); }
template <class T> inline bool operator >= (T               const  l, DAC::ArbInt     const& r) { return !r.op_gt(l); }
                   inline bool operator <  (DAC::ArbInt     const& l, DAC::ArbInt     const& r) { return  l.op_lt(r); }
template <class T> inline bool operator <  (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r) { return  l.op_lt(r); }
template <class T> inline bool operator <  (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r) { return !r.op_le(l); }
template <class T> inline bool operator <  (DAC::ArbInt     const& l, T               const  r) { return  l.op_lt(r); }
template <class T> inline bool operator <  (T               const  l, DAC::ArbInt     const& r) { return !r.op_le(l); }
                   inline bool operator <= (DAC::ArbInt     const& l, DAC::ArbInt     const& r) { return  l.op_le(r); }
template <class T> inline bool operator <= (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r) { return  l.op_le(r); }
template <class T> inline bool operator <= (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r) { return !r.op_lt(l); }
template <class T> inline bool operator <= (DAC::ArbInt     const& l, T               const  r) { return  l.op_le(r); }
template <class T> inline bool operator <= (T               const  l, DAC::ArbInt     const& r) { return !r.op_lt(l); }
                   inline bool operator == (DAC::ArbInt     const& l, DAC::ArbInt     const& r) { return  l.op_eq(r); }
template <class T> inline bool operator == (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r) { return  l.op_eq(r); }
template <class T> inline bool operator == (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r) { return  r.op_eq(l); }
template <class T> inline bool operator == (DAC::ArbInt     const& l, T               const  r) { return  l.op_eq(r); }
template <class T> inline bool operator == (T               const  l, DAC::ArbInt     const& r) { return  r.op_eq(l); }
                   inline bool operator != (DAC::ArbInt     const& l, DAC::ArbInt     const& r) { return  l.op_ne(r); }
template <class T> inline bool operator != (DAC::ArbInt     const& l, DAC::SafeInt<T> const& r) { return  l.op_ne(r); }
template <class T> inline bool operator != (DAC::SafeInt<T> const& l, DAC::ArbInt     const& r) { return  r.op_ne(l); }
template <class T> inline bool operator != (DAC::ArbInt     const& l, T               const  r) { return  l.op_ne(r); }
template <class T> inline bool operator != (T               const  l, DAC::ArbInt     const& r) { return  r.op_ne(l); }

// Bitwise operators.
                   inline DAC::ArbInt operator & (DAC::ArbInt     const& l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_bit_and(r); }
template <class T> inline DAC::ArbInt operator & (DAC::ArbInt     const& l, DAC::SafeInt<T> const  r) { return DAC::ArbInt(l).op_bit_and(r); }
template <class T> inline DAC::ArbInt operator & (DAC::SafeInt<T> const  l, DAC::ArbInt     const& r) { return DAC::ArbInt(r).op_bit_and(l); }
template <class T> inline DAC::ArbInt operator & (DAC::ArbInt     const& l, T               const  r) { return DAC::ArbInt(l).op_bit_and(r); }
template <class T> inline DAC::ArbInt operator & (T               const  l, DAC::ArbInt     const& r) { return DAC::ArbInt(r).op_bit_and(l); }
                   inline DAC::ArbInt operator | (DAC::ArbInt     const& l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_bit_ior(r); }
template <class T> inline DAC::ArbInt operator | (DAC::ArbInt     const& l, DAC::SafeInt<T> const  r) { return DAC::ArbInt(l).op_bit_ior(r); }
template <class T> inline DAC::ArbInt operator | (DAC::SafeInt<T> const  l, DAC::ArbInt     const& r) { return DAC::ArbInt(r).op_bit_ior(l); }
template <class T> inline DAC::ArbInt operator | (DAC::ArbInt     const& l, T               const  r) { return DAC::ArbInt(l).op_bit_ior(r); }
template <class T> inline DAC::ArbInt operator | (T               const  l, DAC::ArbInt     const& r) { return DAC::ArbInt(r).op_bit_ior(l); }
                   inline DAC::ArbInt operator ^ (DAC::ArbInt     const& l, DAC::ArbInt     const& r) { return DAC::ArbInt(l).op_bit_xor(r); }
template <class T> inline DAC::ArbInt operator ^ (DAC::ArbInt     const& l, DAC::SafeInt<T> const  r) { return DAC::ArbInt(l).op_bit_xor(r); }
template <class T> inline DAC::ArbInt operator ^ (DAC::SafeInt<T> const  l, DAC::ArbInt     const& r) { return DAC::ArbInt(r).op_bit_xor(l); }
template <class T> inline DAC::ArbInt operator ^ (DAC::ArbInt     const& l, T               const  r) { return DAC::ArbInt(l).op_bit_xor(r); }
template <class T> inline DAC::ArbInt operator ^ (T               const  l, DAC::ArbInt     const& r) { return DAC::ArbInt(r).op_bit_xor(l); }

#endif
