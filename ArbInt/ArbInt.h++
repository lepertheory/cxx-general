/*****************************************************************************
 * ArbInt.h++
 *****************************************************************************
 * Arbitrary integer number class.
 *****************************************************************************/

// Include guard.
#if !defined(ARBINT_7b2y3kb789f2u3du)
  #define ARBINT_7b2y3kb789f2u3du

// STL includes.
  #include <iostream>
  #include <sstream>
  #include <string>
  #include <vector>
  #include <limits>
  #include <cmath>
  
// System includes.
  #include <SafeInt.h++>
  #include <to_string.h++>
  #include <rppower.h++>
  #include <Exception.h++>

// Namespace wrapping.
namespace DAC {
  
  /***************************************************************************
   * UArbInt
   ***************************************************************************
   * This is an integer of an arbitrary number of digits.
   ***************************************************************************/
  class UArbInt {
    
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Typedefs.
      
      // Native digit type. Do not change this unless you have a very good
      // reason. This must be an unsigned integer type.
      typedef unsigned int _DigT;
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Data types.
      
      // The internal type that this number is stored as.
      typedef _DigT value_type;
      
      /***********************************************************************/
      // Constants.
      
      // Maximum input base.
      static value_type const max_input_base;
      
      /***********************************************************************/
      // Errors
      class Errors {
        
        public:
          
          // All UArbInt errors are based off of this.
          class Base : public Exception { public: virtual char const* what () const throw() { return "Undefined error in UArbInt."; }; };
          
          // Bad format.
          class BadFormat : public Base {
            public:
              virtual ~BadFormat () throw() {};
              virtual char const* what () const throw() {
                try {
                  std::string tmpmsg(_problem + " at position " + DAC::to_string(SafeInt<std::string::size_type>(_position) + 1) + ".");
                  return Exception::buffer_message(tmpmsg);
                } catch (...) {
                  return "Bad format. Error creating message string.";
                }
              };
              BadFormat& Problem  (char const*            const problem ) { _problem  = problem ; return *this; };
              BadFormat& Position (std::string::size_type const position) { _position = position; return *this; };
              char const*            Problem  () const { return _problem.c_str(); };
              std::string::size_type Position () const { return _position       ; };
            private:
              std::string            _problem;
              std::string::size_type _position;
          };
          
          // Attempt to set UArbInt to a negative number.
          class Negative : public Base { public: virtual char const* what () const throw() { return "Attempt to set UArbInt to a negative number."; }; };
          
          // Operation overruns container size. Generic version first for easy
          // catching, then specialized version based off that.
          class Overrun : public Base { public: virtual char const* what () const throw() { return "Instruction overruns end of container."; }; };
          
          // Divide by zero.
          class DivByZero : public Base { public: virtual char const* what () const throw() { return "Divide by zero."; }; };
          
          // Overflow converting to a scalar type.
          class ScalarOverflow : public Base { public: virtual char const* what () const throw() { return "Number overflows requested scalar type."; }; };
          
          // Requested base is out of range.
          class BaseOutOfRange    : public Base           { public: virtual char const* what () const throw() { return "Requested base is out of range.";  }; };
          class BaseOutOfRangeMin : public BaseOutOfRange { public: virtual char const* what () const throw() { return "Requested base is below minimum."; }; };
          class BaseOutOfRangeMax : public BaseOutOfRange { public: virtual char const* what () const throw() { return "Requested base is above maximum."; }; };
        
        // Private constructor, cannot instantiate this class.
        private:
          Errors ();
          Errors (Errors const&);
          Errors& operator = (Errors const&);
        
      };
      
      /***********************************************************************/
      // Function members.
      
      // Default constructor.
      UArbInt ();
      
      // Copy constructor.
      UArbInt (UArbInt const& number);
      
      // Conversion constructor.
                         explicit UArbInt (std::string const& number);
      template <class T> explicit UArbInt (T           const  number);
      
      // Increment / decrement operators.
      UArbInt& operator ++ (   );
      UArbInt  operator ++ (int);
      UArbInt& operator -- (   );
      UArbInt  operator -- (int);
      
      // Unary sign operators.
      UArbInt operator + () const;
      UArbInt operator - () const;
      
      // Bitwise compliment.
      UArbInt operator ~ () const;
      
      // Casting operators.
      operator bool           () const;
      operator char           () const;
      operator signed   char  () const;
      operator unsigned char  () const;
      operator wchar_t        () const;
      operator short          () const;
      operator unsigned short () const;
      operator int            () const;
      operator unsigned       () const;
      operator long           () const;
      operator unsigned long  () const;
      
      // Assignment operator.
                         UArbInt& operator = (UArbInt     const& number);
                         UArbInt& operator = (std::string const& number);
      template <class T> UArbInt& operator = (T           const  number);
      
      // Accessors.
                         value_type Base  () const;
      template <class T> T          Value () const;
                         UArbInt& Base  (value_type const base  );
      template <class T> UArbInt& Value (T          const number);
      
      // Reset to just-constructed state.
      void clear ();
      
      // Copy another number.
      void copy (UArbInt const& number);
      
      // Swap this number with another number.
      void swap (UArbInt& number);
      
      // Set this number.
                         UArbInt& set (UArbInt     const& number                            );
                         UArbInt& set (std::string const& number, bool const autobase = true);
      template <class T> UArbInt& set (SafeInt<T>  const  number                            );
      template <class T> UArbInt& set (T           const  number                            );
      
      // Set this number with an exact bitwise copy.
      template <class T> UArbInt& setBitwise (SafeInt<T> const number);
      template <class T> UArbInt& setBitwise (T          const number);
      
      // Push digits into this number.
                         void push_back (UArbInt     const& number);
                         void push_back (std::string const& number);
      template <class T> void push_back (SafeInt<T>  const  number);
      template <class T> void push_back (T           const  number);
      
      // Convert to string.
      std::string      & to_string (std::string& buffer, value_type const base = 0) const;
      std::string const& to_string (                     value_type const base = 0) const;
      
      // Arithmetic operator backends.
                         UArbInt& op_mul (UArbInt    const& number                              );
      template <class T> UArbInt& op_mul (SafeInt<T> const  number                              );
      template <class T> UArbInt& op_mul (T          const  number                              );
                         UArbInt& op_div (UArbInt    const& number, UArbInt* const remainder = 0);
      template <class T> UArbInt& op_div (SafeInt<T> const  number, UArbInt* const remainder = 0);
      template <class T> UArbInt& op_div (T          const  number, UArbInt* const remainder = 0);
                         UArbInt& op_mod (UArbInt    const& number                              );
      template <class T> UArbInt& op_mod (SafeInt<T> const  number                              );
      template <class T> UArbInt& op_mod (T          const  number                              );
                         UArbInt& op_add (UArbInt    const& number                              );
      template <class T> UArbInt& op_add (SafeInt<T> const  number                              );
      template <class T> UArbInt& op_add (T          const  number                              );
                         UArbInt& op_sub (UArbInt    const& number                              );
      template <class T> UArbInt& op_sub (SafeInt<T> const  number                              );
      template <class T> UArbInt& op_sub (T          const  number                              );
      
      // Bit shift operator backends.
                         UArbInt& op_shl (UArbInt    const& number);
      template <class T> UArbInt& op_shl (SafeInt<T> const  number);
      template <class T> UArbInt& op_shl (T          const  number);
                         UArbInt& op_shr (UArbInt    const& number);
      template <class T> UArbInt& op_shr (SafeInt<T> const  number);
      template <class T> UArbInt& op_shr (T          const  number);
      
      // Master comparison operator.
                         int op_compare (UArbInt    const& number) const;
      template <class T> int op_compare (SafeInt<T> const  number) const;
      template <class T> int op_compare (T          const  number) const;
      
      // Comparison operator backends.
                         bool op_gt (UArbInt    const& number) const;
      template <class T> bool op_gt (SafeInt<T> const  number) const;
      template <class T> bool op_gt (T          const  number) const;
                         bool op_ge (UArbInt    const& number) const;
      template <class T> bool op_ge (SafeInt<T> const  number) const;
      template <class T> bool op_ge (T          const  number) const;
                         bool op_lt (UArbInt    const& number) const;
      template <class T> bool op_lt (SafeInt<T> const  number) const;
      template <class T> bool op_lt (T          const  number) const;
                         bool op_le (UArbInt    const& number) const;
      template <class T> bool op_le (SafeInt<T> const  number) const;
      template <class T> bool op_le (T          const  number) const;
                         bool op_eq (UArbInt    const& number) const;
      template <class T> bool op_eq (SafeInt<T> const  number) const;
      template <class T> bool op_eq (T          const  number) const;
                         bool op_ne (UArbInt    const& number) const;
      template <class T> bool op_ne (SafeInt<T> const  number) const;
      template <class T> bool op_ne (T          const  number) const;
      
      // Bitwise operator backends.
                         UArbInt& op_bit_and (UArbInt    const& number);
      template <class T> UArbInt& op_bit_and (SafeInt<T> const  number);
      template <class T> UArbInt& op_bit_and (T          const  number);
                         UArbInt& op_bit_ior (UArbInt    const& number);
      template <class T> UArbInt& op_bit_ior (SafeInt<T> const  number);
      template <class T> UArbInt& op_bit_ior (T          const  number);
                         UArbInt& op_bit_xor (UArbInt    const& number);
      template <class T> UArbInt& op_bit_xor (SafeInt<T> const  number);
      template <class T> UArbInt& op_bit_xor (T          const  number);
                         UArbInt& op_bit_cpm ()                        ;
      
      // Return information about this number.
      bool    isEven       () const;
      bool    isZero       () const;
      bool    isOdd        () const;
      size_t  numDigits    () const;
      UArbInt bitsInNumber () const;
      
      // Raise this number to a power.
                         UArbInt pow (UArbInt const& exp) const;
      template <class T> UArbInt pow (T       const  exp) const;
      
      // Access a specific digit.
      value_type get_digit (size_t const digit) const;
      
      // Access a specific bit.
      bool get_bit (UArbInt const& bit                  ) const;
      bool get_bit (size_t const digit, size_t const bit) const;
      
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Typedefs.
      
      typedef char          _StrChrT; // String character.
      typedef unsigned char _NumChrT; // Numeric character.
      
      typedef std::vector<_NumChrT> _DigStrT; // String of character
                                              // digits.
      typedef std::vector<_DigT>    _DigsT;   // String of native
                                              // digits.
      
      /***********************************************************************/
      // Data types.
      
      // Directions.
      enum _Dir { _DIR_L, _DIR_R };
      
      // Number types.
      enum _NumType { _NUM_UINT, _NUM_SINT, _NUM_UNKNOWN };
      
      // Determine number type.
      template <class T> class _GetNumType { public: static _NumType const value; };
      
      // Set.
      template <class T, _NumType> class _Set;
      template <class T> class _Set<T, _NUM_UINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      template <class T> class _Set<T, _NUM_SINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      
      // Set a bitwise copy.
      template <class T, _NumType> class _SetBitwise;
      template <class T> class _SetBitwise<T, _NUM_UINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      template <class T> class _SetBitwise<T, _NUM_SINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      
      // Multiply.
      template <class T, _NumType> class _Mul;
      template <class T> class _Mul<T, _NUM_UINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      template <class T> class _Mul<T, _NUM_SINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      
      // Divide.
      template <class T, _NumType> class _Div;
      template <class T> class _Div<T, _NUM_UINT> { public: static void op (UArbInt& l, SafeInt<T> const r, UArbInt* const remainder = 0); static void op (UArbInt& l, T const r, UArbInt* const remainder = 0); };
      template <class T> class _Div<T, _NUM_SINT> { public: static void op (UArbInt& l, SafeInt<T> const r, UArbInt* const remainder = 0); static void op (UArbInt& l, T const r, UArbInt* const remainder = 0); };
      
      // Modulo divide.
      template <class T, _NumType> class _Mod;
      template <class T> class _Mod<T, _NUM_UINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      template <class T> class _Mod<T, _NUM_SINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      
      // Add.
      template <class T, _NumType> class _Add;
      template <class T> class _Add<T, _NUM_UINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      template <class T> class _Add<T, _NUM_SINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      
      // Subtract.
      template <class T, _NumType> class _Sub;
      template <class T> class _Sub<T, _NUM_UINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      template <class T> class _Sub<T, _NUM_SINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      
      // Shift.
      template <class T, _NumType> class _Shift;
      template <class T> class _Shift<T, _NUM_UINT> { public: static void op (UArbInt& l, SafeInt<T> const r, _Dir const dir); static void op (UArbInt& l, T const r, _Dir const dir); };
      template <class T> class _Shift<T, _NUM_SINT> { public: static void op (UArbInt& l, SafeInt<T> const r, _Dir const dir); static void op (UArbInt& l, T const r, _Dir const dir); };
      
      // Shift left.
      template <class T, _NumType> class _ShL;
      template <class T> class _ShL<T, _NUM_UINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      template <class T> class _ShL<T, _NUM_SINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      
      // Shift right.
      template <class T, _NumType> class _ShR;
      template <class T> class _ShR<T, _NUM_UINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      template <class T> class _ShR<T, _NUM_SINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      
      // Compare.
      template <class T, _NumType> class _Compare;
      template <class T> class _Compare<T, _NUM_UINT> { public: static int op (UArbInt const& l, SafeInt<T> const r); static int op (UArbInt const& l, T const r); };
      template <class T> class _Compare<T, _NUM_SINT> { public: static int op (UArbInt const& l, SafeInt<T> const r); static int op (UArbInt const& l, T const r); };
      
      // Greater than.
      template <class T, _NumType> class _GT;
      template <class T> class _GT<T, _NUM_UINT> { public: static bool op (UArbInt const& l, SafeInt<T> const r); static bool op (UArbInt const& l, T const r); };
      template <class T> class _GT<T, _NUM_SINT> { public: static bool op (UArbInt const& l, SafeInt<T> const r); static bool op (UArbInt const& l, T const r); };
      
      // Greater than or equal to.
      template <class T, _NumType> class _GE;
      template <class T> class _GE<T, _NUM_UINT> { public: static bool op (UArbInt const& l, SafeInt<T> const r); static bool op (UArbInt const& l, T const r); };
      template <class T> class _GE<T, _NUM_SINT> { public: static bool op (UArbInt const& l, SafeInt<T> const r); static bool op (UArbInt const& l, T const r); };
      
      // Less than.
      template <class T, _NumType> class _LT;
      template <class T> class _LT<T, _NUM_UINT> { public: static bool op (UArbInt const& l, SafeInt<T> const r); static bool op (UArbInt const& l, T const r); };
      template <class T> class _LT<T, _NUM_SINT> { public: static bool op (UArbInt const& l, SafeInt<T> const r); static bool op (UArbInt const& l, T const r); };
      
      // Less than or equal to.
      template <class T, _NumType> class _LE;
      template <class T> class _LE<T, _NUM_UINT> { public: static bool op (UArbInt const& l, SafeInt<T> const r); static bool op (UArbInt const& l, T const r); };
      template <class T> class _LE<T, _NUM_SINT> { public: static bool op (UArbInt const& l, SafeInt<T> const r); static bool op (UArbInt const& l, T const r); };
      
      // Equal to.
      template <class T, _NumType> class _EQ;
      template <class T> class _EQ<T, _NUM_UINT> { public: static bool op (UArbInt const& l, SafeInt<T> const r); static bool op (UArbInt const& l, T const r); };
      template <class T> class _EQ<T, _NUM_SINT> { public: static bool op (UArbInt const& l, SafeInt<T> const r); static bool op (UArbInt const& l, T const r); };
      
      // Not equal to.
      template <class T, _NumType> class _NE;
      template <class T> class _NE<T, _NUM_UINT> { public: static bool op (UArbInt const& l, SafeInt<T> const r); static bool op (UArbInt const& l, T const r); };
      template <class T> class _NE<T, _NUM_SINT> { public: static bool op (UArbInt const& l, SafeInt<T> const r); static bool op (UArbInt const& l, T const r); };
      
      // Bitwise AND.
      template <class T, _NumType> class _Bit_AND;
      template <class T> class _Bit_AND<T, _NUM_UINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      template <class T> class _Bit_AND<T, _NUM_SINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      
      // Bitwise inclusive OR.
      template <class T, _NumType> class _Bit_IOR;
      template <class T> class _Bit_IOR<T, _NUM_UINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      template <class T> class _Bit_IOR<T, _NUM_SINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      
      // Bitwise exclusive OR.
      template <class T, _NumType> class _Bit_XOR;
      template <class T> class _Bit_XOR<T, _NUM_UINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      template <class T> class _Bit_XOR<T, _NUM_SINT> { public: static void op (UArbInt& l, SafeInt<T> const r); static void op (UArbInt& l, T const r); };
      
      /***********************************************************************/
      // Data members.
      
      // The number.
      _DigsT _digits;
      
      // The default base of this number.
      _DigT _base;
      
      // Temporary buffer for string output.
      mutable std::string _strbuf;
      
      /***********************************************************************/
      // Static data members.
      
      static unsigned int const s_digitbits; // Number of bits in a digit.
      static _DigT        const s_digitbase; // Base of native digits.
      static _DigT        const s_bitmask  ; // Bitmask of digits.
      
      static _NumChrT const s_numodigits; // Number of output digits.
      static _StrChrT const s_odigits[] ; // Output digits.
      
      static _NumChrT const s_idigits[]; // Input digits.
      
      /***********************************************************************/
      // Function members.
      
      // Perform carry or borrow.
      UArbInt& _carry  (_DigsT::size_type start);
      UArbInt& _borrow (_DigsT::size_type start);
      
      // Trim insignificant zeros.
      UArbInt& _trimZeros ();
      
      // Bit shift this number.
      UArbInt& _shift       (UArbInt                    const& bits,                                     _Dir const dir);
      UArbInt& _shift       (SafeInt<_DigsT::size_type> const  digits, SafeInt<unsigned int> const bits, _Dir const dir);
      UArbInt& _shiftDigits (SafeInt<_DigsT::size_type> const  digits,                                   _Dir const dir);
      UArbInt& _shiftBits   (                                          SafeInt<unsigned int> const bits, _Dir const dir);
      
      /***********************************************************************/
      // Static function members.
      
      // Trim zeros from a given container.
      template <class T> static void s_trimZerosB (T& c);
      template <class T> static void s_trimZerosE (T& c);
      template <class T> static void s_trimZeros  (T& c);
      
      // Do long division on a given container. Returns the remainder.
      template <class DivndT, class DivorT> static DivorT s_longDiv (DivndT& divnd, DivorT const divor, value_type const base);
      
      // Convert a container from one base to another.
      template <class FT, class TT> static void s_baseConv (FT const& from, value_type const frombase, TT& to, value_type const tobase);
      
  };
  
  /***************************************************************************
   * ArbInt
   ***************************************************************************
   * Arbitrary length signed integer.
   ***************************************************************************/
  class ArbInt {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Data types.
      
      // Digit type.
      typedef UArbInt::value_type value_type;
      
      /***********************************************************************/
      // Constants.
      
      // Maximum input base.
      static value_type const max_input_base;
      
      /***********************************************************************/
      // Errors.
      class Errors {
        public:
          
          // All errors are base off this.
          class Base : public Exception { public: virtual char const* what () const throw() { return "Undefined error in ArbInt."; }; };
          
          // Bad format.
          class BadFormat : public Base {
            public:
              virtual ~BadFormat () throw() {};
              virtual char const* what () const throw() {
                try {
                  std::string tmpmsg(_problem + " at position " + DAC::to_string(SafeInt<std::string::size_type>(_position) + 1) + ".");
                  return Exception::buffer_message(tmpmsg);
                } catch (...) {
                  return "Bad format. Error creating message string.";
                }
              };
              BadFormat& Problem  (char const*            const problem ) { _problem  = problem ; return *this; };
              BadFormat& Position (std::string::size_type const position) { _position = position; return *this; };
              char const*            Problem  () const { return _problem.c_str(); };
              std::string::size_type Position () const { return _position       ; };
            private:
              std::string            _problem ;
              std::string::size_type _position;
          };
          
          // Divide by zero.
          class DivByZero : public Base { public: virtual char const* what () const throw() { return "Divide by zero."; }; };
          
          // Overflow converting to a scalar type.
          class ScalarOverflow : public Base { public: virtual char const* what () const throw() { return "Number overflows requested scalar type."; }; };
          
          // Requested base is out of range.
          class BaseOutOfRange    : public Base           { public: virtual char const* what () const throw() { return "Requested base is out of range." ; }; };
          class BaseOutOfRangeMin : public BaseOutOfRange { public: virtual char const* what () const throw() { return "Requested base is below minimum."; }; };
          class BaseOutOfRangeMax : public BaseOutOfRange { public: virtual char const* what () const throw() { return "Requested base is above maximum."; }; };
          
        // Instantiation is not allowed.
        private:
          Errors ();
          Errors (Errors const&);
          Errors& operator = (Errors const&);
          
      };
      
      /***********************************************************************/
      // Function members.
      
      // Default constructor.
      ArbInt ();
      
      // Copy constructor.
      ArbInt (ArbInt const& number);
      
      // Conversion constructor.
                         explicit ArbInt (UArbInt     const& number);
                         explicit ArbInt (std::string const& number);
      template <class T> explicit ArbInt (T           const  number);
      
      // Increment / decrement operators.
      ArbInt& operator ++ (   );
      ArbInt  operator ++ (int);
      ArbInt& operator -- (   );
      ArbInt  operator -- (int);
      
      // Unary sign operators.
      ArbInt operator + () const;
      ArbInt operator - () const;
      
      // Bitwise compliment.
      ArbInt operator ~ () const;
      
      // Casting operators.
      operator bool           () const;
      operator char           () const;
      operator signed char    () const;
      operator unsigned char  () const;
      operator wchar_t        () const;
      operator short          () const;
      operator unsigned short () const;
      operator int            () const;
      operator unsigned       () const;
      operator long           () const;
      operator unsigned long  () const;
      
      // Assignment operator.
                         ArbInt& operator = (ArbInt      const& number);
                         ArbInt& operator = (UArbInt     const& number);
                         ArbInt& operator = (std::string const& number);
      template <class T> ArbInt& operator = (T           const  number);
      
      // Accessors.
                         value_type Base  () const;
      template <class T> T          Value () const;
                         ArbInt& Base  (value_type const base  );
      template <class T> ArbInt& Value (T          const number);
      
      // Reset to just-constructed state.
      void clear ();
      
      // Copy another number.
      void copy (ArbInt const& number);
      
      // Swap this number with another number.
      void swap (ArbInt& number);
      
      // Set this number.
                         ArbInt& set (ArbInt      const& number                            );
                         ArbInt& set (UArbInt     const& number                            );
                         ArbInt& set (std::string const& number, bool const autobase = true);
      template <class T> ArbInt& set (SafeInt<T>  const  number                            );
      template <class T> ArbInt& set (T           const  number                            );
      
      // Set this number with a bitwise copy.
      template <class T> ArbInt& setBitwise (SafeInt<T> const number);
      template <class T> ArbInt& setBitwise (T          const number);
      
      // Push digits into this number.
                         void push_back (ArbInt      const& number);
                         void push_back (UArbInt     const& number);
                         void push_back (std::string const& number);
      template <class T> void push_back (SafeInt<T>  const  number);
      template <class T> void push_back (T           const  number);
      
      // Convert to string.
      std::string&       to_string (std::string& buffer, value_type const base = 0) const;
      std::string const& to_string (                     value_type const base = 0) const;
      
      // Arithmetic operator backends.
                         ArbInt& op_mul (ArbInt     const& number                             );
                         ArbInt& op_mul (UArbInt    const& number                             );
      template <class T> ArbInt& op_mul (SafeInt<T> const  number                             );
      template <class T> ArbInt& op_mul (T          const  number                             );
                         ArbInt& op_div (ArbInt     const& number, ArbInt* const remainder = 0);
                         ArbInt& op_div (UArbInt    const& number, ArbInt* const remainder = 0);
      template <class T> ArbInt& op_div (SafeInt<T> const  number, ArbInt* const remainder = 0);
      template <class T> ArbInt& op_div (T          const  number, ArbInt* const remainder = 0);
                         ArbInt& op_mod (ArbInt     const& number                             );
                         ArbInt& op_mod (UArbInt    const& number                             );
      template <class T> ArbInt& op_mod (SafeInt<T> const  number                             );
      template <class T> ArbInt& op_mod (T          const  number                             );
                         ArbInt& op_add (ArbInt     const& number                             );
                         ArbInt& op_add (UArbInt    const& number                             );
      template <class T> ArbInt& op_add (SafeInt<T> const  number                             );
      template <class T> ArbInt& op_add (T          const  number                             );
                         ArbInt& op_sub (ArbInt     const& number                             );
                         ArbInt& op_sub (UArbInt    const& number                             );
      template <class T> ArbInt& op_sub (SafeInt<T> const  number                             );
      template <class T> ArbInt& op_sub (T          const  number                             );
      
      // Bit shift operator backends.
                         ArbInt& op_shl (ArbInt     const& number);
                         ArbInt& op_shl (UArbInt    const& number);
      template <class T> ArbInt& op_shl (SafeInt<T> const  number);
      template <class T> ArbInt& op_shl (T          const  number);
                         ArbInt& op_shr (ArbInt     const& number);
                         ArbInt& op_shr (UArbInt    const& number);
      template <class T> ArbInt& op_shr (SafeInt<T> const  number);
      template <class T> ArbInt& op_shr (T          const  number);
      
      // Master comparison operator.
                         int op_compare (ArbInt     const& number) const;
                         int op_compare (UArbInt    const& number) const;
      template <class T> int op_compare (SafeInt<T> const  number) const;
      template <class T> int op_compare (T          const  number) const;
      
      // Comparison operator backends.
                         bool op_gt (ArbInt     const& number) const;
                         bool op_gt (UArbInt    const& number) const;
      template <class T> bool op_gt (SafeInt<T> const  number) const;
      template <class T> bool op_gt (T          const  number) const;
                         bool op_ge (ArbInt     const& number) const;
                         bool op_ge (UArbInt    const& number) const;
      template <class T> bool op_ge (SafeInt<T> const  number) const;
      template <class T> bool op_ge (T          const  number) const;
                         bool op_lt (ArbInt     const& number) const;
                         bool op_lt (UArbInt    const& number) const;
      template <class T> bool op_lt (SafeInt<T> const  number) const;
      template <class T> bool op_lt (T          const  number) const;
                         bool op_le (ArbInt     const& number) const;
                         bool op_le (UArbInt    const& number) const;
      template <class T> bool op_le (SafeInt<T> const  number) const;
      template <class T> bool op_le (T          const  number) const;
                         bool op_eq (ArbInt     const& number) const;
                         bool op_eq (UArbInt    const& number) const;
      template <class T> bool op_eq (SafeInt<T> const  number) const;
      template <class T> bool op_eq (T          const  number) const;
                         bool op_ne (ArbInt     const& number) const;
                         bool op_ne (UArbInt    const& number) const;
      template <class T> bool op_ne (SafeInt<T> const  number) const;
      template <class T> bool op_ne (T          const  number) const;
      
      // Bitwise operator backends.
                         ArbInt& op_bit_and (ArbInt     const& number);
                         ArbInt& op_bit_and (UArbInt    const& number);
      template <class T> ArbInt& op_bit_and (SafeInt<T> const  number);
      template <class T> ArbInt& op_bit_and (T          const  number);
                         ArbInt& op_bit_ior (ArbInt     const& number);
                         ArbInt& op_bit_ior (UArbInt    const& number);
      template <class T> ArbInt& op_bit_ior (SafeInt<T> const  number);
      template <class T> ArbInt& op_bit_ior (T          const  number);
                         ArbInt& op_bit_xor (ArbInt     const& number);
                         ArbInt& op_bit_xor (UArbInt    const& number);
      template <class T> ArbInt& op_bit_xor (SafeInt<T> const  number);
      template <class T> ArbInt& op_bit_xor (T          const  number);
                         ArbInt& op_bit_cpm (                        );
      
      // Return information about this number.
      bool   isEven       () const;
      bool   isOdd        () const;
      bool   isZero       () const;
      bool   isPositive   () const;
      bool   isNegative   () const;
      size_t numDigits    () const;
      size_t bitsInNumber () const;
      
      // Convert. Drops sign, does not check.
      UArbInt toUArbInt () const;
      
      // Raise this number to a power.
                         ArbInt pow (UArbInt const& exp) const;
      template <class T> ArbInt pow (T       const  exp) const;
      
      // Access a specific digit.
      value_type get_digit (size_t const digit) const;
      
      // Access a specific bit. A bit index that does not exist is always 0.
      bool get_bit(UArbInt const& bit                  ) const;
      bool get_bit(size_t const digit, size_t const bit) const;
      
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Data types.
      
      // Directions.
      enum _Dir { _DIR_L, _DIR_R };
      
      // Number types.
      enum _NumType { _NUM_UINT, _NUM_SINT, _NUM_UNKNOWN };
      
      // Determine number type.
      template <class T> class _GetNumType { public: static _NumType const value; };
      
      // Set.
      template <class T, _NumType> class _Set;
      template <class T> class _Set<T, _NUM_UINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      template <class T> class _Set<T, _NUM_SINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      
      // Set a bitwise copy.
      template <class T, _NumType> class _SetBitwise;
      template <class T> class _SetBitwise<T, _NUM_UINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      template <class T> class _SetBitwise<T, _NUM_SINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      
      // Multiply.
      template <class T, _NumType> class _Mul;
      template <class T> class _Mul<T, _NUM_UINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      template <class T> class _Mul<T, _NUM_SINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      
      // Divide.
      template <class T, _NumType> class _Div;
      template <class T> class _Div<T, _NUM_UINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      template <class T> class _Div<T, _NUM_SINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      
      // Modulo divide.
      template <class T, _NumType> class _Mod;
      template <class T> class _Mod<T, _NUM_UINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      template <class T> class _Mod<T, _NUM_SINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      
      // Add.
      template <class T, _NumType> class _Add;
      template <class T> class _Add<T, _NUM_UINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      template <class T> class _Add<T, _NUM_SINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      
      // Subtract.
      template <class T, _NumType> class _Sub;
      template <class T> class _Sub<T, _NUM_UINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      template <class T> class _Sub<T, _NUM_SINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      
      // Shift left.
      template <class T, _NumType> class _ShL;
      template <class T> class _ShL<T, _NUM_UINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      template <class T> class _ShL<T, _NUM_SINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      
      // Shift right.
      template <class T, _NumType> class _ShR;
      template <class T> class _ShR<T, _NUM_UINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      template <class T> class _ShR<T, _NUM_SINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      
      // Master comparison.
      template <class T, _NumType> class _Compare;
      template <class T> class _Compare<T, _NUM_UINT> { public: static int op (ArbInt const& l, SafeInt<T> const r); static int op (ArbInt const& l, T const r); };
      template <class T> class _Compare<T, _NUM_SINT> { public: static int op (ArbInt const& l, SafeInt<T> const r); static int op (ArbInt const& l, T const r); };
      
      // Greater than.
      template <class T, _NumType> class _GT;
      template <class T> class _GT<T, _NUM_UINT> { public: static bool op (ArbInt const& l, SafeInt<T> const r); static bool op (ArbInt const& l, T const r); };
      template <class T> class _GT<T, _NUM_SINT> { public: static bool op (ArbInt const& l, SafeInt<T> const r); static bool op (ArbInt const& l, T const r); };
      
      // Greater than or equal to.
      template <class T, _NumType> class _GE;
      template <class T> class _GE<T, _NUM_UINT> { public: static bool op (ArbInt const& l, SafeInt<T> const r); static bool op (ArbInt const& l, T const r); };
      template <class T> class _GE<T, _NUM_SINT> { public: static bool op (ArbInt const& l, SafeInt<T> const r); static bool op (ArbInt const& l, T const r); };
      
      // Less than.
      template <class T, _NumType> class _LT;
      template <class T> class _LT<T, _NUM_UINT> { public: static bool op (ArbInt const& l, SafeInt<T> const r); static bool op (ArbInt const& l, T const r); };
      template <class T> class _LT<T, _NUM_SINT> { public: static bool op (ArbInt const& l, SafeInt<T> const r); static bool op (ArbInt const& l, T const r); };
      
      // Less than or equal to.
      template <class T, _NumType> class _LE;
      template <class T> class _LE<T, _NUM_UINT> { public: static bool op (ArbInt const& l, SafeInt<T> const r); static bool op (ArbInt const& l, T const r); };
      template <class T> class _LE<T, _NUM_SINT> { public: static bool op (ArbInt const& l, SafeInt<T> const r); static bool op (ArbInt const& l, T const r); };
      
      // Equal to.
      template <class T, _NumType> class _EQ;
      template <class T> class _EQ<T, _NUM_UINT> { public: static bool op (ArbInt const& l, SafeInt<T> const r); static bool op (ArbInt const& l, T const r); };
      template <class T> class _EQ<T, _NUM_SINT> { public: static bool op (ArbInt const& l, SafeInt<T> const r); static bool op (ArbInt const& l, T const r); };
      
      // Not equal.
      template <class T, _NumType> class _NE;
      template <class T> class _NE<T, _NUM_UINT> { public: static bool op (ArbInt const& l, SafeInt<T> const r); static bool op (ArbInt const& l, T const r); };
      template <class T> class _NE<T, _NUM_SINT> { public: static bool op (ArbInt const& l, SafeInt<T> const r); static bool op (ArbInt const& l, T const r); };
      
      // Bitwise AND.
      template <class T, _NumType> class _Bit_AND;
      template <class T> class _Bit_AND<T, _NUM_UINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      template <class T> class _Bit_AND<T, _NUM_SINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      
      // Bitwise inclusive OR.
      template <class T, _NumType> class _Bit_IOR;
      template <class T> class _Bit_IOR<T, _NUM_UINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      template <class T> class _Bit_IOR<T, _NUM_SINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      
      // Bitwise exclusive OR.
      template <class T, _NumType> class _Bit_XOR;
      template <class T> class _Bit_XOR<T, _NUM_UINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      template <class T> class _Bit_XOR<T, _NUM_SINT> { public: static void op (ArbInt& l, SafeInt<T> const r); static void op (ArbInt& l, T const r); };
      
      /***********************************************************************/
      // Data members.
      
      // Sign. True is negative.
      bool _sign;
      
      // The number.
      UArbInt _digits;
      
      // Temporary buffer for string output.
      mutable std::string _strbuf;
      
      /***********************************************************************/
      // Function members.
      
      // Make sure we do not attempt -0.
      void _check_sign ();
      
  };
  
  /***************************************************************************
   * Operators.
   ***************************************************************************/
  
  // Stream I/O operators.
  std::ostream&       operator << (std::ostream&       l, UArbInt const& r);
  std::ostringstream& operator << (std::ostringstream& l, UArbInt const& r);
  std::istream&       operator >> (std::istream&       l, UArbInt&       r);
  std::ostream&       operator << (std::ostream&       l, ArbInt  const& r);
  std::ostringstream& operator << (std::ostringstream& l, ArbInt  const& r);
  std::istream&       operator >> (std::istream&       l, ArbInt&        r);
  
  // Arithmetic operators.
                     UArbInt operator * (UArbInt    const& l, UArbInt    const& r);
                     ArbInt  operator * (ArbInt     const& l, ArbInt     const& r);
                     ArbInt  operator * (UArbInt    const& l, ArbInt     const& r);
                     ArbInt  operator * (ArbInt     const& l, UArbInt    const& r);
  template <class T> UArbInt operator * (UArbInt    const& l, SafeInt<T> const  r);
  template <class T> ArbInt  operator * (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> UArbInt operator * (SafeInt<T> const  l, UArbInt    const& r);
  template <class T> ArbInt  operator * (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> UArbInt operator * (UArbInt    const& l, T          const  r);
  template <class T> ArbInt  operator * (ArbInt     const& l, T          const  r);
  template <class T> UArbInt operator * (T          const  l, UArbInt    const& r);
  template <class T> ArbInt  operator * (T          const  l, ArbInt     const& r);
                     UArbInt operator / (UArbInt    const& l, UArbInt    const& r);
                     ArbInt  operator / (ArbInt     const& l, ArbInt     const& r);
                     ArbInt  operator / (UArbInt    const& l, ArbInt     const& r);
                     ArbInt  operator / (ArbInt     const& l, UArbInt    const& r);
  template <class T> UArbInt operator / (UArbInt    const& l, SafeInt<T> const  r);
  template <class T> ArbInt  operator / (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> UArbInt operator / (SafeInt<T> const  l, UArbInt    const& r);
  template <class T> ArbInt  operator / (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> UArbInt operator / (UArbInt    const& l, T          const  r);
  template <class T> ArbInt  operator / (ArbInt     const& l, T          const  r);
  template <class T> UArbInt operator / (T          const  l, UArbInt    const& r);
  template <class T> ArbInt  operator / (T          const  l, ArbInt     const& r);
                     UArbInt operator % (UArbInt    const& l, UArbInt    const& r);
                     ArbInt  operator % (ArbInt     const& l, ArbInt     const& r);
                     ArbInt  operator % (UArbInt    const& l, ArbInt     const& r);
                     ArbInt  operator % (ArbInt     const& l, UArbInt    const& r);
  template <class T> UArbInt operator % (UArbInt    const& l, SafeInt<T> const  r);
  template <class T> ArbInt  operator % (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> UArbInt operator % (SafeInt<T> const  l, UArbInt    const& r);
  template <class T> ArbInt  operator % (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> UArbInt operator % (UArbInt    const& l, T          const  r);
  template <class T> ArbInt  operator % (ArbInt     const& l, T          const  r);
  template <class T> UArbInt operator % (T          const  l, UArbInt    const& r);
  template <class T> ArbInt  operator % (T          const  l, ArbInt     const& r);
                     UArbInt operator + (UArbInt    const& l, UArbInt    const& r);
                     ArbInt  operator + (ArbInt     const& l, ArbInt     const& r);
                     ArbInt  operator + (UArbInt    const& l, ArbInt     const& r);
                     ArbInt  operator + (ArbInt     const& l, UArbInt    const& r);
  template <class T> UArbInt operator + (UArbInt    const& l, SafeInt<T> const  r);
  template <class T> ArbInt  operator + (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> UArbInt operator + (SafeInt<T> const  l, UArbInt    const& r);
  template <class T> ArbInt  operator + (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> UArbInt operator + (UArbInt    const& l, T          const  r);
  template <class T> ArbInt  operator + (ArbInt     const& l, T          const  r);
  template <class T> UArbInt operator + (T          const  l, UArbInt    const& r);
  template <class T> ArbInt  operator + (T          const  l, ArbInt     const& r);
                     UArbInt operator - (UArbInt    const& l, UArbInt    const& r);
                     ArbInt  operator - (ArbInt     const& l, ArbInt     const& r);
                     ArbInt  operator - (UArbInt    const& l, ArbInt     const& r);
                     ArbInt  operator - (ArbInt     const& l, UArbInt    const& r);
  template <class T> UArbInt operator - (UArbInt    const& l, SafeInt<T> const  r);
  template <class T> ArbInt  operator - (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> UArbInt operator - (SafeInt<T> const  l, UArbInt    const& r);
  template <class T> ArbInt  operator - (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> UArbInt operator - (UArbInt    const& l, T          const  r);
  template <class T> ArbInt  operator - (ArbInt     const& l, T          const  r);
  template <class T> UArbInt operator - (T          const  l, UArbInt    const& r);
  template <class T> ArbInt  operator - (T          const  l, ArbInt     const& r);
  
  // Bit shift operators.
                     UArbInt operator << (UArbInt    const& l, UArbInt    const& r);
                     ArbInt  operator << (ArbInt     const& l, ArbInt     const& r);
                     ArbInt  operator << (UArbInt    const& l, ArbInt     const& r);
                     ArbInt  operator << (ArbInt     const& l, UArbInt    const& r);
  template <class T> UArbInt operator << (UArbInt    const& l, SafeInt<T> const  r);
  template <class T> ArbInt  operator << (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> UArbInt operator << (SafeInt<T> const  l, UArbInt    const& r);
  template <class T> ArbInt  operator << (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> UArbInt operator << (UArbInt    const& l, T          const  r);
  template <class T> ArbInt  operator << (ArbInt     const& l, T          const  r);
  template <class T> UArbInt operator << (T          const  l, UArbInt    const& r);
  template <class T> ArbInt  operator << (T          const  l, ArbInt     const& r);
                     UArbInt operator >> (UArbInt    const& l, UArbInt    const& r);
                     ArbInt  operator >> (ArbInt     const& l, ArbInt     const& r);
                     ArbInt  operator >> (UArbInt    const& l, ArbInt     const& r);
                     ArbInt  operator >> (ArbInt     const& l, UArbInt    const& r);
  template <class T> UArbInt operator >> (UArbInt    const& l, SafeInt<T> const  r);
  template <class T> ArbInt  operator >> (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> UArbInt operator >> (SafeInt<T> const  l, UArbInt    const& r);
  template <class T> ArbInt  operator >> (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> UArbInt operator >> (UArbInt    const& l, T          const  r);
  template <class T> ArbInt  operator >> (ArbInt     const& l, T          const  r);
  template <class T> UArbInt operator >> (T          const  l, UArbInt    const& r);
  template <class T> ArbInt  operator >> (T          const  l, ArbInt     const& r);
  
  // Comparsion operators.
                     bool operator >  (UArbInt    const& l, UArbInt    const& r);
                     bool operator >  (ArbInt     const& l, ArbInt     const& r);
                     bool operator >  (UArbInt    const& l, ArbInt     const& r);
                     bool operator >  (ArbInt     const& l, UArbInt    const& r);
  template <class T> bool operator >  (UArbInt    const& l, SafeInt<T> const  r);
  template <class T> bool operator >  (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> bool operator >  (SafeInt<T> const  l, UArbInt    const& r);
  template <class T> bool operator >  (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> bool operator >  (UArbInt    const& l, T          const  r);
  template <class T> bool operator >  (ArbInt     const& l, T          const  r);
  template <class T> bool operator >  (T          const  l, UArbInt    const& r);
  template <class T> bool operator >  (T          const  l, ArbInt     const& r);
                     bool operator >= (UArbInt    const& l, UArbInt    const& r);
                     bool operator >= (ArbInt     const& l, ArbInt     const& r);
                     bool operator >= (UArbInt    const& l, ArbInt     const& r);
                     bool operator >= (ArbInt     const& l, UArbInt    const& r);
  template <class T> bool operator >= (UArbInt    const& l, SafeInt<T> const  r);
  template <class T> bool operator >= (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> bool operator >= (SafeInt<T> const  l, UArbInt    const& r);
  template <class T> bool operator >= (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> bool operator >= (UArbInt    const& l, T          const  r);
  template <class T> bool operator >= (ArbInt     const& l, T          const  r);
  template <class T> bool operator >= (T          const  l, UArbInt    const& r);
  template <class T> bool operator >= (T          const  l, ArbInt     const& r);
                     bool operator <  (UArbInt    const& l, UArbInt    const& r);
                     bool operator <  (ArbInt     const& l, ArbInt     const& r);
                     bool operator <  (UArbInt    const& l, ArbInt     const& r);
                     bool operator <  (ArbInt     const& l, UArbInt    const& r);
  template <class T> bool operator <  (UArbInt    const& l, SafeInt<T> const  r);
  template <class T> bool operator <  (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> bool operator <  (SafeInt<T> const  l, UArbInt    const& r);
  template <class T> bool operator <  (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> bool operator <  (UArbInt    const& l, T          const  r);
  template <class T> bool operator <  (ArbInt     const& l, T          const  r);
  template <class T> bool operator <  (T          const  l, UArbInt    const& r);
  template <class T> bool operator <  (T          const  l, ArbInt     const& r);
                     bool operator <= (UArbInt    const& l, UArbInt    const& r);
                     bool operator <= (ArbInt     const& l, ArbInt     const& r);
                     bool operator <= (UArbInt    const& l, ArbInt     const& r);
                     bool operator <= (ArbInt     const& l, UArbInt    const& r);
  template <class T> bool operator <= (UArbInt    const& l, SafeInt<T> const  r);
  template <class T> bool operator <= (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> bool operator <= (SafeInt<T> const  l, UArbInt    const& r);
  template <class T> bool operator <= (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> bool operator <= (UArbInt    const& l, T          const  r);
  template <class T> bool operator <= (ArbInt     const& l, T          const  r);
  template <class T> bool operator <= (T          const  l, UArbInt    const& r);
  template <class T> bool operator <= (T          const  l, ArbInt     const& r);
                     bool operator == (UArbInt    const& l, UArbInt    const& r);
                     bool operator == (ArbInt     const& l, ArbInt     const& r);
                     bool operator == (UArbInt    const& l, ArbInt     const& r);
                     bool operator == (ArbInt     const& l, UArbInt    const& r);
  template <class T> bool operator == (UArbInt    const& l, SafeInt<T> const  r);
  template <class T> bool operator == (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> bool operator == (SafeInt<T> const  l, UArbInt    const& r);
  template <class T> bool operator == (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> bool operator == (UArbInt    const& l, T          const  r);
  template <class T> bool operator == (ArbInt     const& l, T          const  r);
  template <class T> bool operator == (T          const  l, UArbInt    const& r);
  template <class T> bool operator == (T          const  l, ArbInt     const& r);
                     bool operator != (UArbInt    const& l, UArbInt    const& r);
                     bool operator != (ArbInt     const& l, ArbInt     const& r);
                     bool operator != (UArbInt    const& l, ArbInt     const& r);
                     bool operator != (ArbInt     const& l, UArbInt    const& r);
  template <class T> bool operator != (UArbInt    const& l, SafeInt<T> const  r);
  template <class T> bool operator != (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> bool operator != (SafeInt<T> const  l, UArbInt    const& r);
  template <class T> bool operator != (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> bool operator != (UArbInt    const& l, T          const  r);
  template <class T> bool operator != (ArbInt     const& l, T          const  r);
  template <class T> bool operator != (T          const  l, UArbInt    const& r);
  template <class T> bool operator != (T          const  l, ArbInt     const& r);
  
  // Bitwise operators.
                     UArbInt operator & (UArbInt    const& l, UArbInt    const& r);
                     ArbInt  operator & (ArbInt     const& l, ArbInt     const& r);
                     ArbInt  operator & (UArbInt    const& l, ArbInt     const& r);
                     ArbInt  operator & (ArbInt     const& l, UArbInt    const& r);
  template <class T> UArbInt operator & (UArbInt    const& l, SafeInt<T> const  r);
  template <class T> ArbInt  operator & (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> UArbInt operator & (SafeInt<T> const  l, UArbInt    const& r);
  template <class T> ArbInt  operator & (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> UArbInt operator & (UArbInt    const& l, T          const  r);
  template <class T> ArbInt  operator & (ArbInt     const& l, T          const  r);
  template <class T> UArbInt operator & (T          const  l, UArbInt    const& r);
  template <class T> ArbInt  operator & (T          const  l, ArbInt     const& r);
                     UArbInt operator | (UArbInt    const& l, UArbInt    const& r);
                     ArbInt  operator | (ArbInt     const& l, ArbInt     const& r);
                     ArbInt  operator | (UArbInt    const& l, ArbInt     const& r);
                     ArbInt  operator | (ArbInt     const& l, UArbInt    const& r);
  template <class T> UArbInt operator | (UArbInt    const& l, SafeInt<T> const  r);
  template <class T> ArbInt  operator | (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> UArbInt operator | (SafeInt<T> const  l, UArbInt    const& r);
  template <class T> ArbInt  operator | (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> UArbInt operator | (UArbInt    const& l, T          const  r);
  template <class T> ArbInt  operator | (ArbInt     const& l, T          const  r);
  template <class T> UArbInt operator | (T          const  l, UArbInt    const& r);
  template <class T> ArbInt  operator | (T          const  l, ArbInt     const& r);
                     UArbInt operator ^ (UArbInt    const& l, UArbInt    const& r);
                     ArbInt  operator ^ (ArbInt     const& l, ArbInt     const& r);
                     ArbInt  operator ^ (UArbInt    const& l, ArbInt     const& r);
                     ArbInt  operator ^ (ArbInt     const& l, UArbInt    const& r);
  template <class T> UArbInt operator ^ (UArbInt    const& l, SafeInt<T> const  r);
  template <class T> ArbInt  operator ^ (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> UArbInt operator ^ (SafeInt<T> const  l, UArbInt    const& r);
  template <class T> ArbInt  operator ^ (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> UArbInt operator ^ (UArbInt    const& l, T          const  r);
  template <class T> ArbInt  operator ^ (ArbInt     const& l, T          const  r);
  template <class T> UArbInt operator ^ (T          const  l, UArbInt    const& r);
  template <class T> ArbInt  operator ^ (T          const  l, ArbInt     const& r);
  
  // Arithmetic assignment operators.
                     UArbInt&    operator *= (UArbInt&    l, UArbInt    const& r);
                     ArbInt&     operator *= (ArbInt&     l, ArbInt     const& r);
                     UArbInt&    operator *= (UArbInt&    l, ArbInt     const& r);
                     ArbInt&     operator *= (ArbInt&     l, UArbInt    const& r);
  template <class T> UArbInt&    operator *= (UArbInt&    l, SafeInt<T> const  r);
  template <class T> ArbInt&     operator *= (ArbInt&     l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator *= (SafeInt<T>& l, UArbInt    const& r);
  template <class T> SafeInt<T>& operator *= (SafeInt<T>& l, ArbInt     const& r);
  template <class T> UArbInt&    operator *= (UArbInt&    l, T          const  r);
  template <class T> ArbInt&     operator *= (ArbInt&     l, T          const  r);
  template <class T> T&          operator *= (T&          l, UArbInt    const& r);
  template <class T> T&          operator *= (T&          l, ArbInt     const& r);
                     UArbInt&    operator /= (UArbInt&    l, UArbInt    const& r);
                     ArbInt&     operator /= (ArbInt&     l, ArbInt     const& r);
                     UArbInt&    operator /= (UArbInt&    l, ArbInt     const& r);
                     ArbInt&     operator /= (ArbInt&     l, UArbInt    const& r);
  template <class T> UArbInt&    operator /= (UArbInt&    l, SafeInt<T> const  r);
  template <class T> ArbInt&     operator /= (ArbInt&     l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator /= (SafeInt<T>& l, UArbInt    const& r);
  template <class T> SafeInt<T>& operator /= (SafeInt<T>& l, ArbInt     const& r);
  template <class T> UArbInt&    operator /= (UArbInt&    l, T          const  r);
  template <class T> ArbInt&     operator /= (ArbInt&     l, T          const  r);
  template <class T> T&          operator /= (T&          l, UArbInt    const& r);
  template <class T> T&          operator /= (T&          l, ArbInt     const& r);
                     UArbInt&    operator %= (UArbInt&    l, UArbInt    const& r);
                     ArbInt&     operator %= (ArbInt&     l, ArbInt     const& r);
                     UArbInt&    operator %= (UArbInt&    l, ArbInt     const& r);
                     ArbInt&     operator %= (ArbInt&     l, UArbInt    const& r);
  template <class T> UArbInt&    operator %= (UArbInt&    l, SafeInt<T> const  r);
  template <class T> ArbInt&     operator %= (ArbInt&     l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator %= (SafeInt<T>& l, UArbInt    const& r);
  template <class T> SafeInt<T>& operator %= (SafeInt<T>& l, ArbInt     const& r);
  template <class T> UArbInt&    operator %= (UArbInt&    l, T          const  r);
  template <class T> ArbInt&     operator %= (ArbInt&     l, T          const  r);
  template <class T> T&          operator %= (T&          l, UArbInt    const& r);
  template <class T> T&          operator %= (T&          l, ArbInt     const& r);
                     UArbInt&    operator += (UArbInt&    l, UArbInt    const& r);
                     ArbInt&     operator += (ArbInt&     l, ArbInt     const& r);
                     UArbInt&    operator += (UArbInt&    l, ArbInt     const& r);
                     ArbInt&     operator += (ArbInt&     l, UArbInt    const& r);
  template <class T> UArbInt&    operator += (UArbInt&    l, SafeInt<T> const  r);
  template <class T> ArbInt&     operator += (ArbInt&     l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator += (SafeInt<T>& l, UArbInt    const& r);
  template <class T> SafeInt<T>& operator += (SafeInt<T>& l, ArbInt     const& r);
  template <class T> UArbInt&    operator += (UArbInt&    l, T          const  r);
  template <class T> ArbInt&     operator += (ArbInt&     l, T          const  r);
  template <class T> T&          operator += (T&          l, UArbInt    const& r);
  template <class T> T&          operator += (T&          l, ArbInt     const& r);
                     UArbInt&    operator -= (UArbInt&    l, UArbInt    const& r);
                     ArbInt&     operator -= (ArbInt&     l, ArbInt     const& r);
                     UArbInt&    operator -= (UArbInt&    l, ArbInt     const& r);
                     ArbInt&     operator -= (ArbInt&     l, UArbInt    const& r);
  template <class T> UArbInt&    operator -= (UArbInt&    l, SafeInt<T> const  r);
  template <class T> ArbInt&     operator -= (ArbInt&     l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator -= (SafeInt<T>& l, UArbInt    const& r);
  template <class T> SafeInt<T>& operator -= (SafeInt<T>& l, ArbInt     const& r);
  template <class T> UArbInt&    operator -= (UArbInt&    l, T          const  r);
  template <class T> ArbInt&     operator -= (ArbInt&     l, T          const  r);
  template <class T> T&          operator -= (T&          l, UArbInt    const& r);
  template <class T> T&          operator -= (T&          l, ArbInt     const& r);
  
  // Bit shift assignment operators.
                     UArbInt&    operator <<= (UArbInt&    l, UArbInt    const& r);
                     ArbInt&     operator <<= (ArbInt&     l, ArbInt     const& r);
                     UArbInt&    operator <<= (UArbInt&    l, ArbInt     const& r);
                     ArbInt&     operator <<= (ArbInt&     l, UArbInt    const& r);
  template <class T> UArbInt&    operator <<= (UArbInt&    l, SafeInt<T> const  r);
  template <class T> ArbInt&     operator <<= (ArbInt&     l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator <<= (SafeInt<T>& l, UArbInt    const& r);
  template <class T> SafeInt<T>& operator <<= (SafeInt<T>& l, ArbInt     const& r);
  template <class T> UArbInt&    operator <<= (UArbInt&    l, T          const  r);
  template <class T> ArbInt&     operator <<= (ArbInt&     l, T          const  r);
  template <class T> T&          operator <<= (T&          l, UArbInt    const& r);
  template <class T> T&          operator <<= (T&          l, ArbInt     const& r);
                     UArbInt&    operator >>= (UArbInt&    l, UArbInt    const& r);
                     ArbInt&     operator >>= (ArbInt&     l, ArbInt     const& r);
                     UArbInt&    operator >>= (UArbInt&    l, ArbInt     const& r);
                     ArbInt&     operator >>= (ArbInt&     l, UArbInt    const& r);
  template <class T> UArbInt&    operator >>= (UArbInt&    l, SafeInt<T> const  r);
  template <class T> ArbInt&     operator >>= (ArbInt&     l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator >>= (SafeInt<T>& l, UArbInt    const& r);
  template <class T> SafeInt<T>& operator >>= (SafeInt<T>& l, ArbInt     const& r);
  template <class T> UArbInt&    operator >>= (UArbInt&    l, T          const  r);
  template <class T> ArbInt&     operator >>= (ArbInt&     l, T          const  r);
  template <class T> T&          operator >>= (T&          l, UArbInt    const& r);
  template <class T> T&          operator >>= (T&          l, ArbInt     const& r);
  
  // Bitwise assignment operators.
                     UArbInt&    operator &= (UArbInt&    l, UArbInt    const& r);
                     ArbInt&     operator &= (ArbInt&     l, ArbInt     const& r);
                     UArbInt&    operator &= (UArbInt&    l, ArbInt     const& r);
                     ArbInt&     operator &= (ArbInt&     l, UArbInt    const& r);
  template <class T> UArbInt&    operator &= (UArbInt&    l, SafeInt<T> const  r);
  template <class T> ArbInt&     operator &= (ArbInt&     l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator &= (SafeInt<T>& l, UArbInt    const& r);
  template <class T> SafeInt<T>& operator &= (SafeInt<T>& l, ArbInt     const& r);
  template <class T> UArbInt&    operator &= (UArbInt&    l, T          const  r);
  template <class T> ArbInt&     operator &= (ArbInt&     l, T          const  r);
  template <class T> T&          operator &= (T&          l, UArbInt    const& r);
  template <class T> T&          operator &= (T&          l, ArbInt     const& r);
                     UArbInt&    operator |= (UArbInt&    l, UArbInt    const& r);
                     ArbInt&     operator |= (ArbInt&     l, ArbInt     const& r);
                     UArbInt&    operator |= (UArbInt&    l, ArbInt     const& r);
                     ArbInt&     operator |= (ArbInt&     l, UArbInt    const& r);
  template <class T> UArbInt&    operator |= (UArbInt&    l, SafeInt<T> const  r);
  template <class T> ArbInt&     operator |= (ArbInt&     l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator |= (SafeInt<T>& l, UArbInt    const& r);
  template <class T> SafeInt<T>& operator |= (SafeInt<T>& l, ArbInt     const& r);
  template <class T> UArbInt&    operator |= (UArbInt&    l, T          const  r);
  template <class T> ArbInt&     operator |= (ArbInt&     l, T          const  r);
  template <class T> T&          operator |= (T&          l, UArbInt    const& r);
  template <class T> T&          operator |= (T&          l, ArbInt     const& r);
                     UArbInt&    operator ^= (UArbInt&    l, UArbInt    const& r);
                     ArbInt&     operator ^= (ArbInt&     l, ArbInt     const& r);
                     UArbInt&    operator ^= (UArbInt&    l, ArbInt     const& r);
                     ArbInt&     operator ^= (ArbInt&     l, UArbInt    const& r);
  template <class T> UArbInt&    operator ^= (UArbInt&    l, SafeInt<T> const  r);
  template <class T> ArbInt&     operator ^= (ArbInt&     l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator ^= (SafeInt<T>& l, UArbInt    const& r);
  template <class T> SafeInt<T>& operator ^= (SafeInt<T>& l, ArbInt     const& r);
  template <class T> UArbInt&    operator ^= (UArbInt&    l, T          const  r);
  template <class T> ArbInt&     operator ^= (ArbInt&     l, T          const  r);
  template <class T> T&          operator ^= (T&          l, UArbInt    const& r);
  template <class T> T&          operator ^= (T&          l, ArbInt     const& r);
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************
   * UArbInt
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Default constructor.
   */
  inline UArbInt::UArbInt () : _base(10U) {}
  
  /*
   * Copy constructor.
   */
  inline UArbInt::UArbInt (UArbInt const& number) : _digits(number._digits), _base(number._base) {}
  
  /*
   * String conversion constructor.
   */
  inline UArbInt::UArbInt (std::string const& number) : _base(10U) { set(number); }
  
  /*
   * Native type conversion constructor.
   */
  template <class T> inline UArbInt::UArbInt (T const number) : _base(10U) { set(number); }
  
  /*
   * Increment / decrement operators.
   */
  inline UArbInt& UArbInt::operator ++ (   ) { return op_add(1U);                                }
  inline UArbInt  UArbInt::operator ++ (int) { UArbInt retval(*this); op_add(1U); return retval; }
  inline UArbInt& UArbInt::operator -- (   ) { return op_sub(1U);                                }
  inline UArbInt  UArbInt::operator -- (int) { UArbInt retval(*this); op_sub(1U); return retval; }
  
  /*
   * Unary sign operators.
   */
  inline UArbInt UArbInt::operator + () const { return *this;             }
  inline UArbInt UArbInt::operator - () const { throw Errors::Negative(); }
  
  /*
   * Bitwise compliment.
   */
  inline UArbInt UArbInt::operator ~ () const { return UArbInt(*this).op_bit_cpm(); }
  
  /*
   * Casting operators.
   */
  inline UArbInt::operator bool           () const { return !isZero();                   }
  inline UArbInt::operator char           () const { return Value<char              >(); }
  inline UArbInt::operator signed   char  () const { return Value<signed   char     >(); }
  inline UArbInt::operator unsigned char  () const { return Value<unsigned char     >(); }
  inline UArbInt::operator wchar_t        () const { return Value<wchar_t           >(); }
  inline UArbInt::operator short          () const { return Value<signed   short int>(); }
  inline UArbInt::operator unsigned short () const { return Value<unsigned short int>(); }
  inline UArbInt::operator int            () const { return Value<signed   int      >(); }
  inline UArbInt::operator unsigned       () const { return Value<unsigned int      >(); }
  inline UArbInt::operator long           () const { return Value<signed   long int >(); }
  inline UArbInt::operator unsigned long  () const { return Value<unsigned long int >(); }
  
  /*
   * Assignment operator.
   */
                     inline UArbInt& UArbInt::operator = (UArbInt     const& number) { set(number); return *this; }
                     inline UArbInt& UArbInt::operator = (std::string const& number) { set(number); return *this; }
  template <class T> inline UArbInt& UArbInt::operator = (T           const  number) { set(number); return *this; }
  
  /*
   * Get the base of this number.
   */
  inline UArbInt::value_type UArbInt::Base () const { return _base; }
  
  /*
   * Get the value of this number.
   */
  template <class T> T UArbInt::Value () const {
    SafeInt<T> retval   ;
    SafeInt<T> mag   (1);
    for (typename _DigsT::const_iterator i = _digits.begin(); i != _digits.end(); ++i) {
      try {
        retval += *i * mag;
        if (i != _digits.end() - 1) {
          mag *= s_digitbase;
        }
      } catch (typename SafeInt<T>::Errors::Overflow&) {
        throw UArbInt::Errors::ScalarOverflow();
      }
    }
    return retval;
  }
  
  /*
   * Set the base of this number.
   */
  inline UArbInt& UArbInt::Base (value_type const base) {
    if (base < 2) {
      throw Errors::BaseOutOfRangeMin();
    }
    if (base > s_digitbase) {
      throw Errors::BaseOutOfRangeMax();
    }
    _base = base;
    return *this;
  }
  
  /*
   * Set the value of this number.
   */
  template <class T> inline UArbInt& UArbInt::Value (T const number) { set(number); return *this; }
  
  /*
   * Set this number from another UArbInt.
   */
  inline UArbInt& UArbInt::set (UArbInt const& number) { _digits = number._digits; return *this; }
  
  /*
   * Set this number from another number.
   */
  template <class T> inline UArbInt& UArbInt::set (SafeInt<T> const number) { _Set<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline UArbInt& UArbInt::set (T          const number) { _Set<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  
  /*
   * Make this number an exact bitwise copy.
   */
  template <class T> UArbInt& UArbInt::setBitwise (SafeInt<T> const number) { _SetBitwise<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> UArbInt& UArbInt::setBitwise (T          const number) { _SetBitwise<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  
  /*
   * Push a string onto the back of this number.
   */
                     inline void UArbInt::push_back (UArbInt    const& number) { push_back(UArbInt(number).Base(_base).to_string()); }
  template <class T> inline void UArbInt::push_back (SafeInt<T> const  number) { push_back(static_cast<T>(number));                  }
  template <class T> inline void UArbInt::push_back (T          const  number) { push_back(DAC::to_string(number));                  }
  
  /*
   * Convert this number to a string.
   */
  inline std::string const& UArbInt::to_string (value_type const base) const { return to_string(_strbuf, base); }
  
  /*
   * Arithmetic operator backends.
   */
  template <class T> inline UArbInt& UArbInt::op_mul (SafeInt<T> const number)                           { _Mul<T, _GetNumType<T>::value>::op(*this, number);            return *this; }
  template <class T> inline UArbInt& UArbInt::op_mul (T          const number)                           { _Mul<T, _GetNumType<T>::value>::op(*this, number);            return *this; }
  template <class T> inline UArbInt& UArbInt::op_div (SafeInt<T> const number, UArbInt* const remainder) { _Div<T, _GetNumType<T>::value>::op(*this, number, remainder); return *this; }
  template <class T> inline UArbInt& UArbInt::op_div (T          const number, UArbInt* const remainder) { _Div<T, _GetNumType<T>::value>::op(*this, number, remainder); return *this; }
  template <class T> inline UArbInt& UArbInt::op_mod (SafeInt<T> const number)                           { _Mod<T, _GetNumType<T>::value>::op(*this, number);            return *this; }
  template <class T> inline UArbInt& UArbInt::op_mod (T          const number)                           { _Mod<T, _GetNumType<T>::value>::op(*this, number);            return *this; }
  template <class T> inline UArbInt& UArbInt::op_add (SafeInt<T> const number)                           { _Add<T, _GetNumType<T>::value>::op(*this, number);            return *this; }
  template <class T> inline UArbInt& UArbInt::op_add (T          const number)                           { _Add<T, _GetNumType<T>::value>::op(*this, number);            return *this; }
  template <class T> inline UArbInt& UArbInt::op_sub (SafeInt<T> const number)                           { _Sub<T, _GetNumType<T>::value>::op(*this, number);            return *this; }
  template <class T> inline UArbInt& UArbInt::op_sub (T          const number)                           { _Sub<T, _GetNumType<T>::value>::op(*this, number);            return *this; }
  
  /*
   * Shift left, shift right.
   */
                     inline UArbInt& UArbInt::op_shl (UArbInt    const& number) { _shift(number, _DIR_L);                            return *this; }
  template <class T> inline UArbInt& UArbInt::op_shl (SafeInt<T> const  number) { _ShL<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline UArbInt& UArbInt::op_shl (T          const  number) { _ShL<T, _GetNumType<T>::value>::op(*this, number); return *this; }
                     inline UArbInt& UArbInt::op_shr (UArbInt    const& number) { _shift(number, _DIR_R);                            return *this; }
  template <class T> inline UArbInt& UArbInt::op_shr (SafeInt<T> const  number) { _ShR<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline UArbInt& UArbInt::op_shr (T          const  number) { _ShR<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  
  /*
   * Master comparison operator.
   */
  template <class T> inline int UArbInt::op_compare (SafeInt<T> const number) const { return _Compare<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline int UArbInt::op_compare (T          const number) const { return _Compare<T, _GetNumType<T>::value>::op(*this, number); }
  
  /*
   * Comparison operators.
   */
                     inline bool UArbInt::op_gt (UArbInt    const& number) const { return op_compare(number) > 0;                           }
  template <class T> inline bool UArbInt::op_gt (SafeInt<T> const  number) const { return _GT<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool UArbInt::op_gt (T          const  number) const { return _GT<T, _GetNumType<T>::value>::op(*this, number); }
                     inline bool UArbInt::op_ge (UArbInt    const& number) const { return !op_lt(number);                                   }
  template <class T> inline bool UArbInt::op_ge (SafeInt<T> const  number) const { return _GE<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool UArbInt::op_ge (T          const  number) const { return _GE<T, _GetNumType<T>::value>::op(*this, number); }
                     inline bool UArbInt::op_lt (UArbInt    const& number) const { return op_compare(number) < 0;                           }
  template <class T> inline bool UArbInt::op_lt (SafeInt<T> const  number) const { return _LT<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool UArbInt::op_lt (T          const  number) const { return _LT<T, _GetNumType<T>::value>::op(*this, number); }
                     inline bool UArbInt::op_le (UArbInt    const& number) const { return !op_gt(number);                                   }
  template <class T> inline bool UArbInt::op_le (SafeInt<T> const  number) const { return _LE<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool UArbInt::op_le (T          const  number) const { return _LE<T, _GetNumType<T>::value>::op(*this, number); }
                     inline bool UArbInt::op_eq (UArbInt    const& number) const { return op_compare(number) == 0;                          }
  template <class T> inline bool UArbInt::op_eq (SafeInt<T> const  number) const { return _EQ<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool UArbInt::op_eq (T          const  number) const { return _EQ<T, _GetNumType<T>::value>::op(*this, number); }
                     inline bool UArbInt::op_ne (UArbInt    const& number) const { return !op_eq(number);                                   }
  template <class T> inline bool UArbInt::op_ne (SafeInt<T> const  number) const { return _NE<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool UArbInt::op_ne (T          const  number) const { return _NE<T, _GetNumType<T>::value>::op(*this, number); }
  
  /*
   * Bitwise operator backends.
   */
  template <class T> inline UArbInt& UArbInt::op_bit_and (SafeInt<T> const number) { _Bit_AND<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline UArbInt& UArbInt::op_bit_and (T          const number) { _Bit_AND<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline UArbInt& UArbInt::op_bit_ior (SafeInt<T> const number) { _Bit_IOR<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline UArbInt& UArbInt::op_bit_ior (T          const number) { _Bit_IOR<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline UArbInt& UArbInt::op_bit_xor (SafeInt<T> const number) { _Bit_XOR<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline UArbInt& UArbInt::op_bit_xor (T          const number) { _Bit_XOR<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  
  /*
   * Tests if this number is equal to zero.
   */
  inline bool UArbInt::isZero () const { return _digits.empty(); }
  
  /*
   * Tests if this number is odd or even.
   */
  inline bool UArbInt::isOdd  () const { return !isZero() && (_digits.front() & 1); }
  inline bool UArbInt::isEven () const { return !isOdd();                            }
  
  /*
   * Get the number of digits in this number.
   */
  inline size_t UArbInt::numDigits () const { return _digits.size(); }
  
  /*
   * Get the number of bits in this number.
   */
  inline UArbInt UArbInt::bitsInNumber () const { return (_digits.empty() ? UArbInt(0) : UArbInt(_digits.size()) * s_digitbits - (s_digitbits - SafeInt<_DigT>(_digits.back()).bitsInNumber())); }
  
  /*
   * Placeholder for automatic pow conversion.
   */
  template <class T> inline UArbInt UArbInt::pow (T const exp) const { return pow(UArbInt(exp)); }
  
  // Access a specific digit.
  inline UArbInt::value_type UArbInt::get_digit (size_t const digit) const {
    if (digit >= _digits.size()) {
      return 0;
    }
    return _digits[digit];
  }
  
  // Access a specific bit.
  inline bool UArbInt::get_bit (UArbInt const& bit) const { return get_bit(bit / s_digitbits, bit % s_digitbits); }
  inline bool UArbInt::get_bit (size_t const digit, size_t const bit) const {
    if (bit < 1) {
      return false;
    }
    return get_digit(digit) & 1 << bit - 1;
  }
  
  /*
   * Trim insignificant zeros.
   */
  inline UArbInt& UArbInt::_trimZeros () { s_trimZerosE(_digits); return *this; }
  
  /*
   * Trim leading and trailing zeros from a given container.
   */
  template <class T> inline void UArbInt::s_trimZeros (T& c) { s_trimZerosB(c); s_trimZerosE(c); }
  
  /*
   * Trim leading zeros from a given container.
   */
  template <class T> void UArbInt::s_trimZerosB (T& c) {
    
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
  
  /*
   * Trim trailing zeros from a given container.
   */
  template <class T> void UArbInt::s_trimZerosE (T& c) {
    
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
  
  /*
   * Do long division on a given container in the specified base. Divisor
   * type must be 2x base. This is not checked, so be careful!
   */
  template <class DivndT, class DivorT> DivorT UArbInt::s_longDiv (DivndT& divnd, DivorT const divor, value_type const base) {
    
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
      quotient.insert(quotient.begin(), dquot);
      
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
    return dgroup / base;
    
  }
  
  /*
   * Convert a container from one base to another. Frombase must be
   * 2^(bits/2) of from or smaller, tobase must be 2^(bits/2) of to or
   * smaller.
   */
  template <class FT, class TT> void UArbInt::s_baseConv (FT const& from, value_type const frombase, TT& to, value_type const tobase) {
    
    // Empty out target container.
    to.clear();
    
    // Temporary, since work will be done in place.
    FT tmp_from(from);
    
    // Convert base by storing the remainder of repeated division by the
    // base that we will be converting to, in the base that we are
    // converting from. Least significant digits come out first.
    while (tmp_from.size() > 0) {
      to.push_back(s_longDiv(tmp_from, tobase, frombase));
    }
    
  }
  
  /*
   * Determine number type.
   */
  template <class T> UArbInt::_NumType const UArbInt::_GetNumType<T>::value =
    std::numeric_limits<T>::is_specialized ? (
      std::numeric_limits<T>::is_integer ? (
        std::numeric_limits<T>::is_signed ? (
          _NUM_SINT
        ) : (
          _NUM_UINT
        )
      ) : (
        _NUM_UNKNOWN
      )
    ) : (
      _NUM_UNKNOWN
    )
  ;
  
  /*
   * Set from an unsigned integer type.
   */
  template <class T> void UArbInt::_Set<T, UArbInt::_NUM_UINT>::op (UArbInt& l, SafeInt<T> const r) {
    
    // Work area.
    UArbInt::_DigsT new_digits;
    
    // Only something to set if the number is not zero.
    if (r != 0) {
      
      // If the number is less than base, simply set it.
      if (r < s_digitbase) {
        
        // Easy :)
        new_digits.push_back(r);
        
      // Otherwise do full conversion.
      } else {
        
        // Convert with repeated division.
        SafeInt<T> tmp = r;
        while (tmp) {
          new_digits.push_back(tmp % s_digitbase);
          tmp /= s_digitbase;
        }
        
      }
    }
    
    // Swap in the new digits and return.
    l._digits.swap(new_digits);
    
  }
  template <class T> inline void UArbInt::_Set<T, UArbInt::_NUM_UINT>::op (UArbInt& l, T const r) { UArbInt::_Set<T, UArbInt::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Set from a signed integer type.
   */
  template <class T> void UArbInt::_Set<T, UArbInt::_NUM_SINT>::op (UArbInt& l, SafeInt<T> const r) {
    
    // If we were sent a negative number, error.
    if (r < 0) {
      throw Errors::Negative();
    }
    
    // Set.
    UArbInt::_Set<T, UArbInt::_NUM_UINT>::op(l, r);
    
  }
  template <class T> inline void UArbInt::_Set<T, UArbInt::_NUM_SINT>::op (UArbInt& l, T const r) { UArbInt::_Set<T, UArbInt::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Set a bitwise copy from an unsigned integer type.
   */
  template <class T> inline void UArbInt::_SetBitwise<T, UArbInt::_NUM_UINT>::op (UArbInt& l, SafeInt<T> const r) { UArbInt::_Set<T, UArbInt::_NUM_UINT>::op(l, r);                    }
  template <class T> inline void UArbInt::_SetBitwise<T, UArbInt::_NUM_UINT>::op (UArbInt& l, T          const r) { UArbInt::_SetBitwise<T, UArbInt::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Set a bitwise copy from a signed integer type.
   */
  template <class T> void UArbInt::_SetBitwise<T, UArbInt::_NUM_SINT>::op (UArbInt& l, SafeInt<T> const r) {
    
    if (r >= 0) {
      UArbInt::_Set<T, UArbInt::_NUM_UINT>::op(l, r);
      return;
    }
    
    // Work area.
    UArbInt::_DigsT new_digits;
    
    // Move bits into place. If we have more bits than can be held in a digit,
    // iterate through, otherwise do it the easy way.
    if (r.bitsInNumber() < s_digitbits) {
      new_digits.push_back(SafeInt<_DigT>().setBitwise(r));
    } else {
      // <= is used here because the sign is not counted in ::digits.
      // Why is ::digits signed? When is the number of digits in a number
      // ever going to be less than zero? Is there a number type out there
      // that makes you forget what you put in it? Jesus. Anyway, that's why
      // this cast is here. If somebody has one of these forgetful numbers I'm
      // pretty sure the universe will implode anyway, so it should be OK.
      for (unsigned int bitpos = 0; bitpos <= static_cast<unsigned int>(std::numeric_limits<T>::digits); bitpos += s_digitbits) {
        new_digits.push_back(SafeInt<_DigT>().setBitwise(r >> bitpos));
      }
    }
    
    // Swap in the new digits and return.
    l._digits = new_digits;
    
  }
  template <class T> inline void UArbInt::_SetBitwise<T, UArbInt::_NUM_SINT>::op (UArbInt& l, T const r) { UArbInt::_SetBitwise<T, UArbInt::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Multiply by an unsigned integer type.
   */
  template <class T> void UArbInt::_Mul<T, UArbInt::_NUM_UINT>::op (UArbInt& l, SafeInt<T> const r) {
    
    // If the number is too large to do in one step, resort to conversion to
    // UArbInt then multiply.
    if (r >= s_digitbase) {
      l.op_mul(UArbInt(r));
      return;
    }
    
    // If we were sent zero, blank the digits.
    if (r == 0) {
      l._digits.clear();
      return;
    }
    
    // If we were sent 1, easy.
    if (r == 1) {
      return;
    }
    
    // Work area;
    UArbInt retval;
    
    // Multiply like 3rd grade.
    for (UArbInt::_DigsT::size_type i = 0; i != l._digits.size(); ++i) {
      
      // Create a new digit if necessary.
      if (retval._digits.size() == i) {
        retval._digits.push_back(0);
      }
      
      // Multiply the digit and carry.
      retval._digits[i] += l._digits[i] * r;
      retval._carry(i);
      
    }
    
    // Move the digits into place and return.
    l._digits.swap(retval._digits);
    
  }
  template <class T> inline void UArbInt::_Mul<T, UArbInt::_NUM_UINT>::op (UArbInt& l, T const r) { UArbInt::_Mul<T, UArbInt::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Multiply by a signed integer type.
   */
  template <class T> void UArbInt::_Mul<T, UArbInt::_NUM_SINT>::op (UArbInt& l, SafeInt<T> const r) {
    
    // If we were sent a negative number, error.
    if (r < 0) {
      throw Errors::Negative();
    }
    
    // OK, now multiply.
    UArbInt::_Mul<T, UArbInt::_NUM_UINT>::op(l, r);
    
  }
  template <class T> inline void UArbInt::_Mul<T, UArbInt::_NUM_SINT>::op (UArbInt& l, T const r) { UArbInt::_Mul<T, UArbInt::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Divide by an unsigned integer type.
   */
  template <class T> void UArbInt::_Div<T, UArbInt::_NUM_UINT>::op (UArbInt& l, SafeInt<T> const r, UArbInt* const remainder) {
    
    // Cannot divide by zero.
    if (r == 0) {
      throw UArbInt::Errors::DivByZero();
    }
    
    // If the number is too large to do in one step, resort to conversion to
    // UArbInt then divide.
    if (r >= UArbInt::s_digitbase) {
      l.op_div(UArbInt(r), remainder);
      return;
    }
    
    // If dividing by 1, easy, remainder is zero and return.
    if (r == 1) {
      if (remainder) {
        *remainder = 0;
      }
      return;
    }
    
    // Already wrote the long division algorithm, reuse it. Different calls
    // for remainder or not.
    if (remainder) {
      *remainder = s_longDiv(l._digits, static_cast<T>(r), s_digitbase);
    } else {
      s_longDiv(l._digits, static_cast<T>(r), s_digitbase);
    }
    
  }
  template <class T> inline void UArbInt::_Div<T, UArbInt::_NUM_UINT>::op (UArbInt& l, T const r, UArbInt* const remainder) { UArbInt::_Div<T, UArbInt::_NUM_UINT>::op(l, SafeInt<T>(r), remainder); }
  
  /*
   * Divide by a signed integer type.
   */
  template <class T> void UArbInt::_Div<T, UArbInt::_NUM_SINT>::op (UArbInt& l, SafeInt<T> const r, UArbInt* const remainder) {
    
    // Cannot return a negative number.
    if (r < 0) {
      throw UArbInt::Errors::Negative();
    }
    
    // OK, now divide.
    UArbInt::_Div<T, UArbInt::_NUM_UINT>::op(l, r, remainder);
    
  }
  template <class T> inline void UArbInt::_Div<T, UArbInt::_NUM_SINT>::op (UArbInt& l, T const r, UArbInt* const remainder) { UArbInt::_Div<T, UArbInt::_NUM_SINT>::op(l, SafeInt<T>(r), remainder); }
  
  /*
   * Modulo divide by an unsigned integer type.
   */
  template <class T> void UArbInt::_Mod<T, UArbInt::_NUM_UINT>::op (UArbInt& l, SafeInt<T> const r) {
    
    // Work area.
    UArbInt retval;
    UArbInt quotient(l);
    
    // Do division.
    quotient.op_div(r, &retval);
    
    // Copy remainder into place and return.
    l._digits = retval._digits;
    
  }
  template <class T> inline void UArbInt::_Mod<T, UArbInt::_NUM_UINT>::op (UArbInt& l, T const r) { UArbInt::_Mod<T, UArbInt::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Modulo divide by a signed integer type.
   */
  template <class T> void UArbInt::_Mod<T, UArbInt::_NUM_SINT>::op (UArbInt& l, SafeInt<T> const r) {
    
    // Modulo division by a negative number does not affect the sign of the
    // result, so use the absolute value. Catch the rare circumstance where
    // the data type's minimum value is sent and convert to UArbInt before
    // dividing. Negate before converting with the ~x + 1 trick. This is a
    // hack, but there is no better way to get around type limit without
    // making this function aware of type sizes.
    if (r < 0) {
      try {
        UArbInt::_Mod<T, UArbInt::_NUM_UINT>::op(l, -r);
      } catch (typename SafeInt<T>::Errors::Overflow) {
        l.op_mod(UArbInt(~r) + 1);
      }
    } else {
      UArbInt::_Mod<T, UArbInt::_NUM_UINT>::op(l, r);
    }
    
  }
  template <class T> inline void UArbInt::_Mod<T, UArbInt::_NUM_SINT>::op (UArbInt& l, T const r) { UArbInt::_Mod<T, UArbInt::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Add an unsigned integer type.
   */
  template <class T> void UArbInt::_Add<T, UArbInt::_NUM_UINT>::op (UArbInt& l, SafeInt<T> const r) {
    
    // If the number is too large to add directly, resort to conversion to
    // UArbInt then add.
    if (r >= s_digitbase) {
      l.op_add(UArbInt(r));
      return;
    }
    
    // Nothing to do if adding 0.
    if (r == 0) {
      return;
    }
    
    // Add and carry or create a digit if necessary.
    if (l._digits.empty()) {
      l._digits.push_back(r);
    } else {
      l._digits[0] += r;
      l._carry(0);
    }
    
  }
  template <class T> inline void UArbInt::_Add<T, UArbInt::_NUM_UINT>::op (UArbInt& l, T const r) { UArbInt::_Add<T, UArbInt::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Add a signed integer type.
   */
  template <class T> void UArbInt::_Add<T, UArbInt::_NUM_SINT>::op (UArbInt& l, SafeInt<T> const r) {
    
    // If adding a negative, subtract the opposite. Use the same trick as in
    // _Mod to convert to a positive number.
    if (r < 0) {
      try {
        UArbInt::_Sub<T, UArbInt::_NUM_UINT>::op(l, -r);
      } catch (typename SafeInt<T>::Errors::Overflow) {
        l.op_sub(UArbInt(~r) + 1);
      }
      
    // Otherwise add normally.
    } else {
      UArbInt::_Add<T, UArbInt::_NUM_UINT>::op(l, r);
    }
    
  }
  template <class T> inline void UArbInt::_Add<T, UArbInt::_NUM_SINT>::op (UArbInt& l, T const r) { UArbInt::_Add<T, UArbInt::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Subtract an unsigned integer type.
   */
  template <class T> void UArbInt::_Sub<T, UArbInt::_NUM_UINT>::op (UArbInt& l, SafeInt<T> const r) {
    
    // If the number is too large to subtract directly, resort to conversion
    // to UArbInt then subtract. Convert any errors thrown.
    if (r >= UArbInt::s_digitbase) {
      l.op_sub(UArbInt(r));
      return;
    }
    
    // Nothing to do if subtracting 0.
    if (l == 0) {
      return;
    }
    
    // If subtraction will result in a negative, error.
    if (r > l) {
      throw UArbInt::Errors::Negative();
    }
    
    // Borrow if necessary and subtract.
    if (l._digits[0] < r) {
      l._borrow(0);
    }
    l._digits[0] -= r;
    
    // Trim zeros.
    l._trimZeros();
    
  }
  template <class T> inline void UArbInt::_Sub<T, UArbInt::_NUM_UINT>::op (UArbInt& l, T const r) { UArbInt::_Sub<T, UArbInt::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Subtract a signed integer type.
   */
  template <class T> void UArbInt::_Sub<T, UArbInt::_NUM_SINT>::op (UArbInt& l, SafeInt<T> const r) {
        
    // If subtracting a negative, add the opposite.
    if (r < 0) {
      
      //  Use the same trick as in _Mod to convert to a positive number.
      try {
        UArbInt::_Add<T, UArbInt::_NUM_UINT>::op(l, -r);
      } catch (typename SafeInt<T>::Errors::Overflow) {
        l.op_add(UArbInt(~r) + 1);
      }
      
    // Otherwise subtract normally.
    } else {
      UArbInt::_Sub<T, UArbInt::_NUM_UINT>::op(l, r);
    }
    
  }
  template <class T> inline void UArbInt::_Sub<T, UArbInt::_NUM_SINT>::op (UArbInt& l, T const r) { UArbInt::_Sub<T, UArbInt::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Shift by an unsigned integer type.
   */
  template <class T> void UArbInt::_Shift<T, UArbInt::_NUM_UINT>::op (UArbInt& l, SafeInt<T> const r, _Dir const dir) {
    
    // Only shift if it is needed.
    if (l && r) {
      
      // Convert bits to digits if the number of bits requested meets or
      // exceeds the number of bits in a digit.
      SafeInt<size_t> bits  ;
      SafeInt<size_t> digits;
      SafeInt<size_t> tmp   ;
      if (r >= s_digitbits) {
        // These types could be different. I don't think they _ever_ will be,
        // but they could. If they were, and an error of an unexpected type
        // was thrown, bad things. Not going to worry about it for now.
        try {
          digits = r / s_digitbits;
          bits = r - digits * s_digitbits;
        } catch (typename SafeInt<size_t>::Errors::Overflow) {
          throw Errors::Overrun();
        }
      } else {
        bits = r;
      }
      
      // Shift.
      l._shiftDigits(digits, dir);
      l._shiftBits  (bits,   dir);
      
    }
    
  }
  template <class T> inline void UArbInt::_Shift<T, UArbInt::_NUM_UINT>::op (UArbInt& l, T const r, _Dir const dir) { UArbInt::_Shift<T, UArbInt::_NUM_UINT>::op(l, SafeInt<T>(r), dir); }
  
  /*
   * Shift by a signed integer type.
   */
  template <class T> void UArbInt::_Shift<T, UArbInt::_NUM_SINT>::op (UArbInt& l, SafeInt<T> const r, _Dir const dir) {
    
    // If shifting a negative, shift abs in the opposite direction.
    if (r < 0) {
      
      // Opposite shift direction.
      _Dir tmpdir = (dir == _DIR_L) ? _DIR_R : _DIR_L;
      
      // Convert to a positive number.
      UArbInt::_Shift<T, UArbInt::_NUM_UINT>::op(l, -r, tmpdir);
      
    // Otherwise shift nomally.
    } else {
      UArbInt::_Shift<T, UArbInt::_NUM_UINT>::op(l, r, dir);
    }
    
  }
  template <class T> inline void UArbInt::_Shift<T, UArbInt::_NUM_SINT>::op (UArbInt& l, T const r, _Dir const dir) { UArbInt::_Shift<T, UArbInt::_NUM_SINT>::op(l, SafeInt<T>(r), dir); }
  
  /*
   * Shift left.
   */
  template <class T> inline void UArbInt::_ShL<T, UArbInt::_NUM_UINT>::op (UArbInt& l, SafeInt<T> const r) { UArbInt::_Shift<T, UArbInt::_NUM_UINT>::op(l, r, _DIR_L); }
  template <class T> inline void UArbInt::_ShL<T, UArbInt::_NUM_UINT>::op (UArbInt& l, T          const r) { UArbInt::_Shift<T, UArbInt::_NUM_UINT>::op(l, r, _DIR_L); }
  template <class T> inline void UArbInt::_ShL<T, UArbInt::_NUM_SINT>::op (UArbInt& l, SafeInt<T> const r) { UArbInt::_Shift<T, UArbInt::_NUM_SINT>::op(l, r, _DIR_L); }
  template <class T> inline void UArbInt::_ShL<T, UArbInt::_NUM_SINT>::op (UArbInt& l, T          const r) { UArbInt::_Shift<T, UArbInt::_NUM_SINT>::op(l, r, _DIR_L); }
  
  /*
   * Shift right.
   */
  template <class T> inline void UArbInt::_ShR<T, UArbInt::_NUM_UINT>::op (UArbInt& l, SafeInt<T> const r) { UArbInt::_Shift<T, UArbInt::_NUM_UINT>::op(l, r, _DIR_R); }
  template <class T> inline void UArbInt::_ShR<T, UArbInt::_NUM_UINT>::op (UArbInt& l, T          const r) { UArbInt::_Shift<T, UArbInt::_NUM_UINT>::op(l, r, _DIR_R); }
  template <class T> inline void UArbInt::_ShR<T, UArbInt::_NUM_SINT>::op (UArbInt& l, SafeInt<T> const r) { UArbInt::_Shift<T, UArbInt::_NUM_SINT>::op(l, r, _DIR_R); }
  template <class T> inline void UArbInt::_ShR<T, UArbInt::_NUM_SINT>::op (UArbInt& l, T          const r) { UArbInt::_Shift<T, UArbInt::_NUM_SINT>::op(l, r, _DIR_R); }
  
  /*
   * Master comparison operator to unsigned integer type.
   */
  template <class T> int UArbInt::_Compare<T, UArbInt::_NUM_UINT>::op (UArbInt const& l, SafeInt<T> const r) {
    
    // If the number is too large to compare directly, resort to conversion to
    // UArbInt then compare.
    if (r >= UArbInt::s_digitbase) {
      return l.op_compare(UArbInt(r));
    }
    
    // Check for zero.
    if (l.isZero()) {
      if (r == 0) {
        return 0;
      } else {
        return -1;
      }
    }
    
    // Check for container larger than 1 digit.
    if (l._digits.size() > 1) {
      return 1;
    }
    
    // Compare the number.
    if (l._digits[0] > r) {
      return 1;
    }
    if (l._digits[0] < r) {
      return -1;
    }
    return 0;
    
  }
  template <class T> inline int UArbInt::_Compare<T, UArbInt::_NUM_UINT>::op (UArbInt const&l, T const r) { return UArbInt::_Compare<T, UArbInt::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Master comparison operator to signed integer type.
   */
  template <class T> inline int UArbInt::_Compare<T, UArbInt::_NUM_SINT>::op (UArbInt const& l, SafeInt<T> const r) {
    
    // Always greater than negative numbers.
    if (r < 0) {
      return 1;
    }
    
    // Remaining tests are sign-independent.
    return UArbInt::_Compare<T, UArbInt::_NUM_UINT>::op(l, r);
    
  }
  
  /*
   * Greater than.
   */
  template <class T> inline bool UArbInt::_GT<T, UArbInt::_NUM_UINT>::op (UArbInt const& l, SafeInt<T> const r) { return _Compare<T, _NUM_UINT>::op(l, r) > 0; }
  template <class T> inline bool UArbInt::_GT<T, UArbInt::_NUM_UINT>::op (UArbInt const& l, T          const r) { return _GT<T, _NUM_UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline bool UArbInt::_GT<T, UArbInt::_NUM_SINT>::op (UArbInt const& l, SafeInt<T> const r) { return _Compare<T, _NUM_SINT>::op(l, r) > 0; }
  template <class T> inline bool UArbInt::_GT<T, UArbInt::_NUM_SINT>::op (UArbInt const& l, T          const r) { return _GT<T, _NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Greater than or equal to.
   */
  template <class T> inline bool UArbInt::_GE<T, UArbInt::_NUM_UINT>::op (UArbInt const& l, SafeInt<T> const r) { return !_LT<T, _NUM_UINT>::op(l, r); }
  template <class T> inline bool UArbInt::_GE<T, UArbInt::_NUM_UINT>::op (UArbInt const& l, T          const r) { return !_LT<T, _NUM_UINT>::op(l, r); }
  template <class T> inline bool UArbInt::_GE<T, UArbInt::_NUM_SINT>::op (UArbInt const& l, SafeInt<T> const r) { return !_LT<T, _NUM_SINT>::op(l, r); }
  template <class T> inline bool UArbInt::_GE<T, UArbInt::_NUM_SINT>::op (UArbInt const& l, T          const r) { return !_LT<T, _NUM_SINT>::op(l, r); }
  
  /*
   * Less than.
   */
  template <class T> inline bool UArbInt::_LT<T, UArbInt::_NUM_UINT>::op (UArbInt const& l, SafeInt<T> const r) { return _Compare<T, _NUM_UINT>::op(l, r) < 0; }
  template <class T> inline bool UArbInt::_LT<T, UArbInt::_NUM_UINT>::op (UArbInt const& l, T          const r) { return _LT<T, _NUM_UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline bool UArbInt::_LT<T, UArbInt::_NUM_SINT>::op (UArbInt const& l, SafeInt<T> const r) { return _Compare<T, _NUM_SINT>::op(l, r) < 0; }
  template <class T> inline bool UArbInt::_LT<T, UArbInt::_NUM_SINT>::op (UArbInt const& l, T          const r) { return _LT<T, _NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Less than or equal to.
   */
  template <class T> inline bool UArbInt::_LE<T, UArbInt::_NUM_UINT>::op (UArbInt const& l, SafeInt<T> const r) { return !_GT<T, _NUM_UINT>::op(l, r); }
  template <class T> inline bool UArbInt::_LE<T, UArbInt::_NUM_UINT>::op (UArbInt const& l, T          const r) { return !_GT<T, _NUM_UINT>::op(l, r); }
  template <class T> inline bool UArbInt::_LE<T, UArbInt::_NUM_SINT>::op (UArbInt const& l, SafeInt<T> const r) { return !_GT<T, _NUM_SINT>::op(l, r); }
  template <class T> inline bool UArbInt::_LE<T, UArbInt::_NUM_SINT>::op (UArbInt const& l, T          const r) { return !_GT<T, _NUM_SINT>::op(l, r); }
  
  /*
   * Equal to.
   */
  template <class T> inline bool UArbInt::_EQ<T, UArbInt::_NUM_UINT>::op (UArbInt const& l, SafeInt<T> const r) { return _Compare<T, _NUM_UINT>::op(l, r) == 0; }
  template <class T> inline bool UArbInt::_EQ<T, UArbInt::_NUM_UINT>::op (UArbInt const& l, T          const r) { return _EQ<T, _NUM_UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline bool UArbInt::_EQ<T, UArbInt::_NUM_SINT>::op (UArbInt const& l, SafeInt<T> const r) { return _Compare<T, _NUM_SINT>::op(l, r) == 0; }
  template <class T> inline bool UArbInt::_EQ<T, UArbInt::_NUM_SINT>::op (UArbInt const& l, T          const r) { return _EQ<T, _NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Not equal.
   */
  template <class T> inline bool UArbInt::_NE<T, UArbInt::_NUM_UINT>::op (UArbInt const& l, SafeInt<T> const r) { return !_EQ<T, _NUM_UINT>::op(l, r); }
  template <class T> inline bool UArbInt::_NE<T, UArbInt::_NUM_UINT>::op (UArbInt const& l, T          const r) { return !_EQ<T, _NUM_UINT>::op(l, r); }
  template <class T> inline bool UArbInt::_NE<T, UArbInt::_NUM_SINT>::op (UArbInt const& l, SafeInt<T> const r) { return !_EQ<T, _NUM_SINT>::op(l, r); }
  template <class T> inline bool UArbInt::_NE<T, UArbInt::_NUM_SINT>::op (UArbInt const& l, T          const r) { return !_EQ<T, _NUM_SINT>::op(l, r); }
  
  /*
   * Bitwise AND with an unsigned integer type.
   */
  template <class T> void UArbInt::_Bit_AND<T, UArbInt::_NUM_UINT>::op (UArbInt& l, SafeInt<T> const r) {
    
    // If the number is too large to AND directly, resort to conversion to
    // UArbInt then AND.
    if (r >= s_digitbase) {
      l.op_bit_and(UArbInt(r));
      return;
    }
    
    // If this number is 0, nothing needs to be done.
    if (l.isZero()) {
      return;
    }
    
    // Only one digit can be ANDed.
    l._digits.resize(1);
    l._digits[0] &= r;
    
  }
  template <class T> inline void UArbInt::_Bit_AND<T, UArbInt::_NUM_UINT>::op (UArbInt& l, T const r) { UArbInt::_Bit_AND<T, UArbInt::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Bitwise AND with a signed integer type.
   */
  template <class T> void UArbInt::_Bit_AND<T, UArbInt::_NUM_SINT>::op (UArbInt& l, SafeInt<T> const r) {
    
    // If negative, convert to UArbInt and AND.
    if (r < 0) {
      l.op_bit_and(UArbInt().setBitwise(r));
      return;
    }
    
    // AND normally.
    UArbInt::_Bit_AND<T, UArbInt::_NUM_UINT>::op(l, r);
    
  }
  template <class T> inline void UArbInt::_Bit_AND<T, UArbInt::_NUM_SINT>::op (UArbInt& l, T const r) { UArbInt::_Bit_AND<T, UArbInt::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Bitwise inclusive OR with an unsigned integer type.
   */
  template <class T> void UArbInt::_Bit_IOR<T, UArbInt::_NUM_UINT>::op (UArbInt& l, SafeInt<T> const r) {
    
    // If the number is too large to OR directly, resort to conversion to
    // UArbInt then OR.
    if (r >= s_digitbase) {
      l.op_bit_ior(UArbInt(r));
      return;
    }
    
    // If this number is 0, simply set with r.
    if (l.isZero()) {
      l.set(r);
      return;
    }
    
    // OR.
    l._digits[0] |= r;
    
  }
  template <class T> inline void UArbInt::_Bit_IOR<T, UArbInt::_NUM_UINT>::op (UArbInt& l, T const r) { UArbInt::_Bit_IOR<T, UArbInt::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Bitwise inclusive OR with a signed integer type.
   */
  template <class T> void UArbInt::_Bit_IOR<T, UArbInt::_NUM_SINT>::op (UArbInt& l, SafeInt<T> const r) {
    
    // If negative, convert to UArbInt and OR.
    if (r < 0) {
      l.op_bit_ior(UArbInt().setBitwise(r));
      return;
    }
    
    // OR normally.
    UArbInt::_Bit_IOR<T, UArbInt::_NUM_UINT>::op(l, r);
    
  }
  template <class T> inline void UArbInt::_Bit_IOR<T, UArbInt::_NUM_SINT>::op (UArbInt& l, T const r) { UArbInt::_Bit_IOR<T, UArbInt::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Bitwise exclusive OR with an unsigned integer type.
   */
  template <class T> void UArbInt::_Bit_XOR<T, UArbInt::_NUM_UINT>::op (UArbInt& l, SafeInt<T> const r) {
    
    // If the number is too large to OR directly, resort to conversion to
    // UArbInt then OR.
    if (r >= s_digitbase) {
      l.op_bit_xor(UArbInt(r));
      return;
    }
    
    // If this number is 0, simply set with r.
    if (l.isZero()) {
      l.set(r);
      return;
    }
    
    // XOR.
    l._digits[0] ^= r;
    
  }
  template <class T> inline void UArbInt::_Bit_XOR<T, UArbInt::_NUM_UINT>::op (UArbInt& l, T const r) { UArbInt::_Bit_XOR<T, UArbInt::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Bitwise exclusive OR with a signed integer type.
   */
  template <class T> void UArbInt::_Bit_XOR<T, UArbInt::_NUM_SINT>::op (UArbInt& l, SafeInt<T> const r) {
    
    // If negative, convert to UArbInt and XOR.
    if (r < 0) {
      l.op_bit_xor(UArbInt().setBitwise(r));
      return;
    }
    
    // XOR normally.
    UArbInt::_Bit_XOR<T, UArbInt::_NUM_UINT>::op(l, r);
    
  }
  
  /***************************************************************************
   * ArbInt
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Default constructor.
   */
  inline ArbInt::ArbInt () : _sign(true) {}
  
  /*
   * Copy constructor.
   */
  inline ArbInt::ArbInt (ArbInt const& number) : _sign(number._sign), _digits(number._digits) {}
  
  /*
   * UArbInt conversion constructor.
   */
  inline ArbInt::ArbInt (UArbInt const& number) : _sign(true), _digits(number) {}
  
  /*
   * String conversion constructor.
   */
  inline ArbInt::ArbInt (std::string const& number) { set(number); }
  
  /*
   * Native type conversion constructor.
   */
  template <class T> inline ArbInt::ArbInt (T const number) : _sign(number >= 0), _digits(abs(number)) {}
  
  /*
   * Increment / decrement operators.
   */
  inline ArbInt& ArbInt::operator ++ (   ) { return op_add(1U);                               }
  inline ArbInt  ArbInt::operator ++ (int) { ArbInt retval(*this); op_add(1U); return retval; }
  inline ArbInt& ArbInt::operator -- (   ) { return op_sub(1U);                               }
  inline ArbInt  ArbInt::operator -- (int) { ArbInt retval(*this); op_sub(1U); return retval; }
  
  /*
   * Unary sign operators.
   */
  inline ArbInt ArbInt::operator + () const { return *this;                                               }
  inline ArbInt ArbInt::operator - () const { ArbInt retval(*this); retval._sign = !_sign; return retval; }
  
  /*
   * Bitwise compliment.
   */
  inline ArbInt ArbInt::operator ~ () const { return ArbInt(*this).op_bit_cpm(); }
  
  /*
   * Casting operators.
   */
  inline ArbInt::operator bool           () const { return !isZero();               }
  inline ArbInt::operator char           () const { return Value<char          >(); }
  inline ArbInt::operator signed char    () const { return Value<signed char   >(); }
  inline ArbInt::operator unsigned char  () const { return Value<unsigned char >(); }
  inline ArbInt::operator wchar_t        () const { return Value<wchar_t       >(); }
  inline ArbInt::operator short          () const { return Value<short         >(); }
  inline ArbInt::operator unsigned short () const { return Value<unsigned short>(); }
  inline ArbInt::operator int            () const { return Value<int           >(); }
  
  inline ArbInt::operator long           () const { return Value<long          >(); }
  inline ArbInt::operator unsigned long  () const { return Value<unsigned long >(); }
  
  /*
   * Assignment operator.
   */
                     inline ArbInt& ArbInt::operator = (ArbInt      const& number) { set(number); return *this; }
                     inline ArbInt& ArbInt::operator = (UArbInt     const& number) { set(number); return *this; }
                     inline ArbInt& ArbInt::operator = (std::string const& number) { set(number); return *this; }
  template <class T> inline ArbInt& ArbInt::operator = (T           const  number) { set(number); return *this; }
  
  /*
   * Get the base.
   */
  inline ArbInt::value_type ArbInt::Base () const { return _digits.Base(); }
  
  /*
   * Get the value.
   */
  template <class T> inline T ArbInt::Value () const {
    try {
      SafeInt<T> retval(_digits);
      if (retval >= 0) {
        return retval;
      } else {
        return -retval;
      }
    } catch (typename SafeInt<T>::Errors::Overflow&) {
      throw ArbInt::Errors::ScalarOverflow();
    } catch (UArbInt::Errors::ScalarOverflow&) {
      throw ArbInt::Errors::ScalarOverflow();
    }
  }
  
  /*
   * Set the base.
   */
  inline ArbInt& ArbInt::Base (value_type const base) {
    try {
      _digits.Base(base);
    } catch (UArbInt::Errors::BaseOutOfRangeMin&) {
      throw ArbInt::Errors::BaseOutOfRangeMin();
    } catch (UArbInt::Errors::BaseOutOfRangeMax&) {
      throw ArbInt::Errors::BaseOutOfRangeMax();
    }
    return *this;
  }
  
  /*
   * Set the value.
   */
  template <class T> inline ArbInt& ArbInt::Value (T const number) { set(number); return *this; }
  
  /*
   * Reset to just-constructed state.
   */
  inline void ArbInt::clear () { _digits.clear(); _sign = false; }
  
  /*
   * Copy another number.
   */
  inline void ArbInt::copy (ArbInt const& number) { _digits.copy(number._digits); _sign = number._sign; }
  
  /*
   * Swap this number with another number.
   */
  inline void ArbInt::swap (ArbInt& number) {
    _digits.swap(number._digits);
    std::swap(_sign, number._sign);
  }
  
  /*
   * Set from another ArbInt.
   */
  inline ArbInt& ArbInt::set (ArbInt const& number) { _digits = number._digits; _sign = number._sign; return *this; }
  
  /*
   * Set from a UArbInt.
   */
  inline ArbInt& ArbInt::set (UArbInt const& number) { _digits = number; _sign = false; return *this; }
  
  /*
   * Set from another number.
   */
  template <class T> inline ArbInt& ArbInt::set (SafeInt<T> const number) { _Set<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline ArbInt& ArbInt::set (T          const number) { _Set<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  
  /*
   * Make an exact bitwise copy.
   */
  template <class T> inline ArbInt& ArbInt::setBitwise (SafeInt<T> const number) { _SetBitwise<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline ArbInt& ArbInt::setBitwise (T          const number) { _SetBitwise<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  
  /*
   * Push a string onto the back of this number.
   */
                     inline void ArbInt::push_back (ArbInt     const& number) { push_back(ArbInt (number).Base(_digits.Base()).to_string()); }
                     inline void ArbInt::push_back (UArbInt    const& number) { push_back(UArbInt(number).Base(_digits.Base()).to_string()); }
  template <class T> inline void ArbInt::push_back (SafeInt<T> const  number) { push_back(static_cast<T>(number)                          ); }
  template <class T> inline void ArbInt::push_back (T          const  number) { push_back(DAC::to_string(number)                          ); }
  inline void ArbInt::push_back (std::string const& number) {
    if (number.length() > 1 && (number[0] == '-' || number[0] == '+')) {
      _digits.push_back(number.substr(1));
    } else {
      _digits.push_back(number);
    }
  }
  
  /*
   * Convert to string.
   */
  inline std::string& ArbInt::to_string (std::string& buffer, value_type const base) const {
    _digits.to_string(buffer, base);
    if (_sign) {
      buffer.insert(0, 1, '-');
    }
    return buffer;
  }
  inline std::string const& ArbInt::to_string (value_type const base) const { return to_string(_strbuf, base); }
  
  /*
   * Arithmetic operator backends.
   */
  template <class T> inline ArbInt& ArbInt::op_mul (SafeInt<T> const number                         ) { _Mul<T, _GetNumType<T>::value>::op(*this, number           ); return *this; }
  template <class T> inline ArbInt& ArbInt::op_mul (T          const number                         ) { _Mul<T, _GetNumType<T>::value>::op(*this, number           ); return *this; }
  template <class T> inline ArbInt& ArbInt::op_div (SafeInt<T> const number, ArbInt* const remainder) { _Div<T, _GetNumType<T>::value>::op(*this, number, remainder); return *this; }
  template <class T> inline ArbInt& ArbInt::op_div (T          const number, ArbInt* const remainder) { _Div<T, _GetNumType<T>::value>::op(*this, number, remainder); return *this; }
  template <class T> inline ArbInt& ArbInt::op_mod (SafeInt<T> const number                         ) { _Mod<T, _GetNumType<T>::value>::op(*this, number           ); return *this; }
  template <class T> inline ArbInt& ArbInt::op_mod (T          const number                         ) { _Mod<T, _GetNumType<T>::value>::op(*this, number           ); return *this; }
  template <class T> inline ArbInt& ArbInt::op_add (SafeInt<T> const number                         ) { _Add<T, _GetNumType<T>::value>::op(*this, number           ); return *this; }
  template <class T> inline ArbInt& ArbInt::op_add (T          const number                         ) { _Add<T, _GetNumType<T>::value>::op(*this, number           ); return *this; }
  template <class T> inline ArbInt& ArbInt::op_sub (SafeInt<T> const number                         ) { _Sub<T, _GetNumType<T>::value>::op(*this, number           ); return *this; }
  template <class T> inline ArbInt& ArbInt::op_sub (T          const number                         ) { _Sub<T, _GetNumType<T>::value>::op(*this, number           ); return *this; }
  
  /*
   * Bit shift operator backends.
   */
                     inline ArbInt& ArbInt::op_shl (ArbInt     const& number) { if (number < 0) { op_shr(number._digits); } else { op_shl(number._digits); } return *this; }
                     inline ArbInt& ArbInt::op_shl (UArbInt    const& number) { _digits <<= number;                                                          return *this; }
  template <class T> inline ArbInt& ArbInt::op_shl (SafeInt<T> const  number) { _ShL<T, _GetNumType<T>::value>::op(*this, number);                           return *this; }
  template <class T> inline ArbInt& ArbInt::op_shl (T          const  number) { _ShL<T, _GetNumType<T>::value>::op(*this, number);                           return *this; }
                     inline ArbInt& ArbInt::op_shr (ArbInt     const& number) { if (number < 0) { op_shl(number._digits); } else { op_shr(number._digits); } return *this; }
                     inline ArbInt& ArbInt::op_shr (UArbInt    const& number) { _digits >>= number;                                                          return *this; }
  template <class T> inline ArbInt& ArbInt::op_shr (SafeInt<T> const  number) { _ShR<T, _GetNumType<T>::value>::op(*this, number);                           return *this; }
  template <class T> inline ArbInt& ArbInt::op_shr (T          const  number) { _ShR<T, _GetNumType<T>::value>::op(*this, number);                           return *this; }
  
  /*
   * Master comparison operator.
   */
  template <class T> inline int ArbInt::op_compare (SafeInt<T> const number) const { return _Compare<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline int ArbInt::op_compare (T          const number) const { return _Compare<T, _GetNumType<T>::value>::op(*this, number); }
  
  /*
   * Comparison operator backends.
   */
                     inline bool ArbInt::op_gt (ArbInt     const& number) const { return op_compare(number) > 0;                           }
                     inline bool ArbInt::op_gt (UArbInt    const& number) const { return op_compare(number) > 0;                           }
  template <class T> inline bool ArbInt::op_gt (SafeInt<T> const  number) const { return _GT<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool ArbInt::op_gt (T          const  number) const { return _GT<T, _GetNumType<T>::value>::op(*this, number); }
                     inline bool ArbInt::op_ge (ArbInt     const& number) const { return !op_lt(number);                                   }
                     inline bool ArbInt::op_ge (UArbInt    const& number) const { return !op_lt(number);                                   }
  template <class T> inline bool ArbInt::op_ge (SafeInt<T> const  number) const { return _GE<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool ArbInt::op_ge (T          const  number) const { return _GE<T, _GetNumType<T>::value>::op(*this, number); }
                     inline bool ArbInt::op_lt (ArbInt     const& number) const { return op_compare(number) < 0;                           }
                     inline bool ArbInt::op_lt (UArbInt    const& number) const { return op_compare(number) < 0;                           }
  template <class T> inline bool ArbInt::op_lt (SafeInt<T> const  number) const { return _LT<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool ArbInt::op_lt (T          const  number) const { return _LT<T, _GetNumType<T>::value>::op(*this, number); }
                     inline bool ArbInt::op_le (ArbInt     const& number) const { return !op_gt(number);                                   }
                     inline bool ArbInt::op_le (UArbInt    const& number) const { return !op_gt(number);                                   }
  template <class T> inline bool ArbInt::op_le (SafeInt<T> const  number) const { return _LE<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool ArbInt::op_le (T          const  number) const { return _LE<T, _GetNumType<T>::value>::op(*this, number); }
                     inline bool ArbInt::op_eq (ArbInt     const& number) const { return op_compare(number) == 0;                          }
                     inline bool ArbInt::op_eq (UArbInt    const& number) const { return op_compare(number) == 0;                          }
  template <class T> inline bool ArbInt::op_eq (SafeInt<T> const  number) const { return _EQ<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool ArbInt::op_eq (T          const  number) const { return _EQ<T, _GetNumType<T>::value>::op(*this, number); }
                     inline bool ArbInt::op_ne (ArbInt     const& number) const { return !op_eq(number);                                   }
                     inline bool ArbInt::op_ne (UArbInt    const& number) const { return !op_eq(number);                                   }
  template <class T> inline bool ArbInt::op_ne (SafeInt<T> const  number) const { return _NE<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool ArbInt::op_ne (T          const  number) const { return _NE<T, _GetNumType<T>::value>::op(*this, number); }
  
  /*
   * Bitwise operator backends.
   */
  template <class T> inline ArbInt& ArbInt::op_bit_and (SafeInt<T> const  number) { return _Bit_AND<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline ArbInt& ArbInt::op_bit_and (T          const  number) { return _Bit_AND<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline ArbInt& ArbInt::op_bit_ior (SafeInt<T> const  number) { return _Bit_IOR<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline ArbInt& ArbInt::op_bit_ior (T          const  number) { return _Bit_IOR<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline ArbInt& ArbInt::op_bit_xor (SafeInt<T> const  number) { return _Bit_XOR<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline ArbInt& ArbInt::op_bit_xor (T          const  number) { return _Bit_XOR<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  
  /*
   * Return information about this number.
   */
  inline bool   ArbInt::isEven     () const { return _digits.isEven();    }
  inline bool   ArbInt::isOdd      () const { return _digits.isOdd ();    }
  inline bool   ArbInt::isZero     () const { return _digits.isZero();    }
  inline bool   ArbInt::isPositive () const { return !_sign;              }
  inline bool   ArbInt::isNegative () const { return _sign;               }
  inline size_t ArbInt::numDigits  () const { return _digits.numDigits(); }
  
  /*
   * Convert to UArbInt.
   */
  UArbInt ArbInt::toUArbInt () const { return _digits; }
  
  /*
   * Raise this number to a power.
   */
  ArbInt ArbInt::pow (UArbInt const& exp) const {
    ArbInt retval;
    retval._digits = _digits.pow(exp);
    retval._sign   = _sign && exp & 1U;
    return retval;
  }
  template <class T> ArbInt ArbInt::pow (T const exp) const {
    ArbInt retval;
    retval._digits = _digits.pow(exp);
    retval._sign   = _sign && exp & 1U;
    return retval;
  }
  
  /*
   * Access a specific digit.
   */
  inline ArbInt::value_type ArbInt::get_digit (size_t const digit) const { return _digits.get_digit(digit); }
  
  /*
   * Make sure we are not attempting to set -0.
   */
  inline void ArbInt::_check_sign () { if (!_digits) { _sign = false; } }
  
  /*
   * Set.
   */
  template <class T> inline void ArbInt::_Set<T, ArbInt::_NUM_UINT>::op (ArbInt& l, SafeInt<T> const r) { l._digits = r; l._sign = false;           }
  template <class T> inline void ArbInt::_Set<T, ArbInt::_NUM_UINT>::op (ArbInt& l, T          const r) { _Set<T, _NUM_UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline void ArbInt::_Set<T, ArbInt::_NUM_SINT>::op (ArbInt& l, SafeInt<T> const r) { l._digits = r.abs(); l._sign = r < 0;     }
  template <class T> inline void ArbInt::_Set<T, ArbInt::_NUM_SINT>::op (ArbInt& l, T          const r) { _Set<T, _NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Set a bitwise copy.
   */
  template <class T> inline void ArbInt::_SetBitwise<T, ArbInt::_NUM_UINT>::op (ArbInt& l, SafeInt<T> const r) { l._digits = r; l._sign = false;                  }
  template <class T> inline void ArbInt::_SetBitwise<T, ArbInt::_NUM_UINT>::op (ArbInt& l, T          const r) { _SetBitwise<T, _NUM_UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline void ArbInt::_SetBitwise<T, ArbInt::_NUM_SINT>::op (ArbInt& l, SafeInt<T> const r) { l._digits = r.abs(); l._sign = r < 0;            }
  template <class T> inline void ArbInt::_SetBitwise<T, ArbInt::_NUM_SINT>::op (ArbInt& l, T          const r) { _SetBitwise<T, _NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Multiply.
   */
  template <class T> inline void ArbInt::_Mul<T, ArbInt::_NUM_UINT>::op (ArbInt& l, SafeInt<T> const r) { l._digits *= r; l._check_sign();                                   }
  template <class T> inline void ArbInt::_Mul<T, ArbInt::_NUM_UINT>::op (ArbInt& l, T          const r) { _Mul<T, _NUM_UINT>::op(l, SafeInt<T>(r));                          }
  template <class T> inline void ArbInt::_Mul<T, ArbInt::_NUM_SINT>::op (ArbInt& l, SafeInt<T> const r) { l._digits *= r.abs(); l._sign = l._sign != r < 0; l._check_sign(); }
  template <class T> inline void ArbInt::_Mul<T, ArbInt::_NUM_SINT>::op (ArbInt& l, T          const r) { _Mul<T, _NUM_SINT>::op(l, SafeInt<T>(r));                          }
  
  /*
   * Divide.
   */
  template <class T> void ArbInt::_Div<T, ArbInt::_NUM_UINT>::op (ArbInt& l, SafeInt<T> const r) {
    if (r == 0U) {
      throw Errors::DivByZero();
    }
    l._digits /= r;
    l._check_sign();
  }
  template <class T> inline void ArbInt::_Div<T, ArbInt::_NUM_UINT>::op (ArbInt& l, T const r) { _Div<T, _NUM_UINT>::op(l, SafeInt<T>(r)); }
  template <class T> void ArbInt::_Div<T, ArbInt::_NUM_SINT>::op (ArbInt& l, SafeInt<T> const r) {
    if (r == 0) {
      throw Errors::DivByZero();
    }
    l._digits /= r.abs();
    l._sign    = l._sign != r < 0;
    l._check_sign();
  }
  
  /*
   * Modulo divide.
   */
  template <class T> void ArbInt::_Mod<T, ArbInt::_NUM_UINT>::op (ArbInt& l, SafeInt<T> const r) {
    if (r == 0U) {
      throw Errors::DivByZero();
    }
    l._digits %= r;
    l._check_sign();
  }
  template <class T> inline void ArbInt::_Mod<T, ArbInt::_NUM_UINT>::op (ArbInt& l, T const r) { _Mod<T, _NUM_UINT>::op(l, SafeInt<T>(r)); }
  template <class T> void ArbInt::_Mod<T, ArbInt::_NUM_SINT>::op (ArbInt& l, SafeInt<T> const r) {
    if (r == 0U) {
      throw Errors::DivByZero();
    }
    l._digits %= r.abs();
    l._check_sign();
  }
  template <class T> inline void ArbInt::_Mod<T, ArbInt::_NUM_SINT>::op (ArbInt& l, T const r) { _Mod<T, _NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Add.
   */
  template <class T> void ArbInt::_Add<T, ArbInt::_NUM_UINT>::op (ArbInt& l, SafeInt<T> const r) {
    if (l._sign) {
      if (r > l._digits) {
        l._digits = r - l._digits;
        l._sign   = !l._sign;
      } else {
        l._digits -= r;
      }
    } else {
      l._digits += r;
    }
    l._check_sign();
  }
  template <class T> inline void ArbInt::_Add<T, ArbInt::_NUM_UINT>::op (ArbInt& l, T const r) { _Add<T, _NUM_UINT>::op(l, SafeInt<T>(r)); }
  template <class T> void ArbInt::_Add<T, ArbInt::_NUM_SINT>::op (ArbInt& l, SafeInt<T> const r) {
    if (l._sign == r < 0) {
      l._digits += r.abs();
    } else {
      if (r.abs() > l._digits) {
        l._digits = r.abs() - l._digits();
        l._sign   = !l._sign;
      } else {
        l._digits -= r.abs();
      }
    }
    l._check_sign();
  }
  template <class T> inline void ArbInt::_Add<T, ArbInt::_NUM_SINT>::op (ArbInt& l, T const r) { _Add<T, _NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Subtract.
   */
  template <class T> void ArbInt::_Sub<T, ArbInt::_NUM_UINT>::op (ArbInt& l, SafeInt<T> const r) {
    if (l._sign) {
      l._digits += r;
    } else {
      if (r > l._digits) {
        l._digits = r - l._digits;
        l._sign   = !l._sign;
      } else {
        l._digits -= r;
      }
    }
    l._check_sign();
  }
  template <class T> inline void ArbInt::_Sub<T, ArbInt::_NUM_UINT>::op (ArbInt& l, T const r) { _Sub<T, _NUM_UINT>::op(l, SafeInt<T>(r)); }
  template <class T> void ArbInt::_Sub<T, ArbInt::_NUM_SINT>::op (ArbInt& l, SafeInt<T> const r) {
    if (l._sign == r < 0) {
      if (r.abs() > l._digits) {
        l._digits = r.abs() - l._digits;
        l._sign   = !l._sign;
      } else {
        l._digits -= r.abs();
      }
    } else {
      l._digits += r.abs();
    }
    l._check_sign();
  }
  template <class T> inline void ArbInt::_Sub<T, ArbInt::_NUM_SINT>::op (ArbInt& l, T const r) { _Sub<T, _NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Shift.
   */
  /*
  template <class T> inline void ArbInt::_Shift<T, ArbInt::_NUM_UINT>::op (ArbInt& l, SafeInt<T> const r, _Dir const dir) {
    switch (dir) {
      case _DIR_L: l <<= r; break;
      case _DIR_R: l >>= r; break;
    }
  }
  template <class T> inline void ArbInt::_Shift<T, ArbInt::_NUM_UINT>::op (ArbInt& l, T const r, _Dir const dir) { _Shift<T, _NUM_UINT>::op(l, SafeInt<T>(r), dir); }
  template <class T> inline void ArbInt::_Shift<T, ArbInt::_NUM_SINT>::op (ArbInt& l, SafeInt<T> const r, _Dir const dir) {
    switch (dir) {
      case _DIR_L: l <<= r; break;
      case _DIR_R: l >>= r; break;
    }
  }
  */
  
  /*
   * Shift left.
   */
  template <class T> inline void ArbInt::_ShL<T, ArbInt::_NUM_UINT>::op (ArbInt& l, SafeInt<T> const r) { l._digits <<= r; l._check_sign(); }
  template <class T> inline void ArbInt::_ShL<T, ArbInt::_NUM_UINT>::op (ArbInt& l, T          const r) { l._digits <<= r; l._check_sign(); }
  template <class T> inline void ArbInt::_ShL<T, ArbInt::_NUM_SINT>::op (ArbInt& l, SafeInt<T> const r) { l._digits <<= r; l._check_sign(); }
  template <class T> inline void ArbInt::_ShL<T, ArbInt::_NUM_SINT>::op (ArbInt& l, T          const r) { l._digits <<= r; l._check_sign(); }
  
  /*
   * Shift left.
   */
  template <class T> inline void ArbInt::_ShR<T, ArbInt::_NUM_UINT>::op (ArbInt& l, SafeInt<T> const r) { l._digits >>= r; l._check_sign(); }
  template <class T> inline void ArbInt::_ShR<T, ArbInt::_NUM_UINT>::op (ArbInt& l, T          const r) { l._digits >>= r; l._check_sign(); }
  template <class T> inline void ArbInt::_ShR<T, ArbInt::_NUM_SINT>::op (ArbInt& l, SafeInt<T> const r) { l._digits >>= r; l._check_sign(); }
  template <class T> inline void ArbInt::_ShR<T, ArbInt::_NUM_SINT>::op (ArbInt& l, T          const r) { l._digits >>= r; l._check_sign(); }
  
  /*
   * Master comparison.
   */
  template <class T> inline int ArbInt::_Compare<T, ArbInt::_NUM_UINT>::op (ArbInt const& l, SafeInt<T> const r) {
    
    // Check sign first.
    if (l._sign) {
      return 1;
    }
    
    // Now compare digits.
    return l._digits.op_compare(r);
    
  }
  template <class T> inline int ArbInt::_Compare<T, ArbInt::_NUM_UINT>::op (ArbInt const& l, T const r) { return _Compare<T, _NUM_UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline int ArbInt::_Compare<T, ArbInt::_NUM_SINT>::op (ArbInt const& l, SafeInt<T> const r) {
    
    // Check sign first.
    if (l._sign) {
      if (r >= 0) {
        return 1;
      }
    } else {
      if (r < 0) {
        return -1;
      }
    }
    
    // Now compare digits.
    return l._digits.op_compare(r);
    
  }
  template <class T> inline int ArbInt::_Compare<T, ArbInt::_NUM_SINT>::op (ArbInt const& l, T const r) { return _Compare<T, _NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Greater than.
   */
  template <class T> inline bool ArbInt::_GT<T, ArbInt::_NUM_UINT>::op (ArbInt const& l, SafeInt<T> const r) { return _Compare<T, _NUM_UINT>::op(l, r) > 0;    }
  template <class T> inline bool ArbInt::_GT<T, ArbInt::_NUM_UINT>::op (ArbInt const& l, T          const r) { return _GT<T, _NUM_UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline bool ArbInt::_GT<T, ArbInt::_NUM_SINT>::op (ArbInt const& l, SafeInt<T> const r) { return _Compare<T, _NUM_SINT>::op(l, r) > 0;    }
  template <class T> inline bool ArbInt::_GT<T, ArbInt::_NUM_SINT>::op (ArbInt const& l, T          const r) { return _GT<T, _NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Greater than or equal to.
   */
  template <class T> inline bool ArbInt::_GE<T, ArbInt::_NUM_UINT>::op (ArbInt const& l, SafeInt<T> const r) { return !_LT<T, _NUM_UINT>::op(l, r); }
  template <class T> inline bool ArbInt::_GE<T, ArbInt::_NUM_UINT>::op (ArbInt const& l, T          const r) { return !_LT<T, _NUM_UINT>::op(l, r); }
  template <class T> inline bool ArbInt::_GE<T, ArbInt::_NUM_SINT>::op (ArbInt const& l, SafeInt<T> const r) { return !_LT<T, _NUM_SINT>::op(l, r); }
  template <class T> inline bool ArbInt::_GE<T, ArbInt::_NUM_SINT>::op (ArbInt const& l, T          const r) { return !_LT<T, _NUM_SINT>::op(l, r); }
  
  /*
   * Less than.
   */
  template <class T> inline bool ArbInt::_LT<T, ArbInt::_NUM_UINT>::op (ArbInt const& l, SafeInt<T> const r) { return _Compare<T, _NUM_UINT>::op(l, r) < 0;    }
  template <class T> inline bool ArbInt::_LT<T, ArbInt::_NUM_UINT>::op (ArbInt const& l, T          const r) { return _LT<T, _NUM_UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline bool ArbInt::_LT<T, ArbInt::_NUM_SINT>::op (ArbInt const& l, SafeInt<T> const r) { return _Compare<T, _NUM_SINT>::op(l, r) < 0;    }
  template <class T> inline bool ArbInt::_LT<T, ArbInt::_NUM_SINT>::op (ArbInt const& l, T          const r) { return _LT<T, _NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Greater than or equal to.
   */
  template <class T> inline bool ArbInt::_LE<T, ArbInt::_NUM_UINT>::op (ArbInt const& l, SafeInt<T> const r) { return !_GT<T, _NUM_UINT>::op(l, r); }
  template <class T> inline bool ArbInt::_LE<T, ArbInt::_NUM_UINT>::op (ArbInt const& l, T          const r) { return !_GT<T, _NUM_UINT>::op(l, r); }
  template <class T> inline bool ArbInt::_LE<T, ArbInt::_NUM_SINT>::op (ArbInt const& l, SafeInt<T> const r) { return !_GT<T, _NUM_SINT>::op(l, r); }
  template <class T> inline bool ArbInt::_LE<T, ArbInt::_NUM_SINT>::op (ArbInt const& l, T          const r) { return !_GT<T, _NUM_SINT>::op(l, r); }
  
  /*
   * Equal to.
   */
  template <class T> inline bool ArbInt::_EQ<T, ArbInt::_NUM_UINT>::op (ArbInt const& l, SafeInt<T> const r) { return _Compare<T, _NUM_UINT>::op(l, r) == 0;   }
  template <class T> inline bool ArbInt::_EQ<T, ArbInt::_NUM_UINT>::op (ArbInt const& l, T          const r) { return _EQ<T, _NUM_UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline bool ArbInt::_EQ<T, ArbInt::_NUM_SINT>::op (ArbInt const& l, SafeInt<T> const r) { return _Compare<T, _NUM_SINT>::op(l, r) == 0;   }
  template <class T> inline bool ArbInt::_EQ<T, ArbInt::_NUM_SINT>::op (ArbInt const& l, T          const r) { return _EQ<T, _NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Greater than or equal to.
   */
  template <class T> inline bool ArbInt::_NE<T, ArbInt::_NUM_UINT>::op (ArbInt const& l, SafeInt<T> const r) { return !_EQ<T, _NUM_UINT>::op(l, r); }
  template <class T> inline bool ArbInt::_NE<T, ArbInt::_NUM_UINT>::op (ArbInt const& l, T          const r) { return !_EQ<T, _NUM_UINT>::op(l, r); }
  template <class T> inline bool ArbInt::_NE<T, ArbInt::_NUM_SINT>::op (ArbInt const& l, SafeInt<T> const r) { return !_EQ<T, _NUM_SINT>::op(l, r); }
  template <class T> inline bool ArbInt::_NE<T, ArbInt::_NUM_SINT>::op (ArbInt const& l, T          const r) { return !_EQ<T, _NUM_SINT>::op(l, r); }
  
  /*
   * Bitwise AND.
   */
  template <class T> inline void ArbInt::_Bit_AND<T, ArbInt::_NUM_UINT>::op (ArbInt& l, SafeInt<T> const r) {
    l._digits &= r;
    l._sign    = false;
  }
  template <class T> inline void ArbInt::_Bit_AND<T, ArbInt::_NUM_UINT>::op (ArbInt& l, T const r) { _Bit_AND<T, _NUM_UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline void ArbInt::_Bit_AND<T, ArbInt::_NUM_SINT>::op (ArbInt& l, SafeInt<T> const r) {
    l._digits &= r.abs();
    l._sign    = l._sign && r < 0;
    _check_sign();
  }
  template <class T> inline void ArbInt::_Bit_AND<T, ArbInt::_NUM_SINT>::op (ArbInt& l, T const r) { _Bit_AND<T, _NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Bitwise inclusive OR.
   */
  template <class T> inline void ArbInt::_Bit_IOR<T, ArbInt::_NUM_UINT>::op (ArbInt& l, SafeInt<T> const r) {
    l._digits |= r;
    _check_sign();
  }
  template <class T> inline void ArbInt::_Bit_IOR<T, ArbInt::_NUM_UINT>::op (ArbInt& l, T const r) { _Bit_IOR<T, _NUM_UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline void ArbInt::_Bit_IOR<T, ArbInt::_NUM_SINT>::op (ArbInt& l, SafeInt<T> const r) {
    l._digits |= r.abs();
    l._sign    = r._sign || r < 0;
    _check_sign();
  }
  template <class T> inline void ArbInt::_Bit_IOR<T, ArbInt::_NUM_SINT>::op (ArbInt& l, T const r) { _Bit_IOR<T, _NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Bitwise exclusive OR.
   */
  template <class T> inline void ArbInt::_Bit_XOR<T, ArbInt::_NUM_UINT>::op (ArbInt& l, SafeInt<T> const r) {
    l._digits ^= r;
    _check_sign();
  }
  template <class T> inline void ArbInt::_Bit_XOR<T, ArbInt::_NUM_UINT>::op (ArbInt& l, T const r) { _Bit_XOR<T, _NUM_UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline void ArbInt::_Bit_XOR<T, ArbInt::_NUM_SINT>::op (ArbInt& l, SafeInt<T> const r) {
    l._digits ^= r.abs();
    l._sign    = r._sign != r < 0;
    _check_sign();
  }
  template <class T> inline void ArbInt::_Bit_XOR<T, ArbInt::_NUM_SINT>::op (ArbInt& l, T const r) { _Bit_XOR<T, _NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /***************************************************************************
   * Operators.
   ***************************************************************************/
  
  /*
   * Stream I/O operators.
   */
  inline std::ostream&       operator << (std::ostream&       l, UArbInt const& r) { l << r.to_string();                                 return l; }
  inline std::ostringstream& operator << (std::ostringstream& l, UArbInt const& r) { l << r.to_string();                                 return l; }
  inline std::istream&       operator >> (std::istream&       l, UArbInt&       r) { std::string input; std::cin >> input; r.set(input); return l; }
  inline std::ostream&       operator << (std::ostream&       l, ArbInt  const& r) { l << r.to_string();                                 return l; }
  inline std::ostringstream& operator << (std::ostringstream& l, ArbInt  const& r) { l << r.to_string();                                 return l; }
  inline std::istream&       operator >> (std::istream&       l, ArbInt&        r) { std::string input; std::cin >> input; r.set(input); return l; }
  
  /*
   * Arithmetic operators.
   */
                     inline UArbInt operator * (UArbInt    const& l, UArbInt    const& r) { return UArbInt(l).op_mul(r); }
                     inline ArbInt  operator * (ArbInt     const& l, ArbInt     const& r) { return ArbInt (l).op_mul(r); }
                     inline ArbInt  operator * (UArbInt    const& l, ArbInt     const& r) { return ArbInt (l).op_mul(r); }
                     inline ArbInt  operator * (ArbInt     const& l, UArbInt    const& r) { return ArbInt (l).op_mul(r); }
  template <class T> inline UArbInt operator * (UArbInt    const& l, SafeInt<T> const  r) { return UArbInt(l).op_mul(r); }
  template <class T> inline ArbInt  operator * (ArbInt     const& l, SafeInt<T> const  r) { return ArbInt (l).op_mul(r); }
  template <class T> inline UArbInt operator * (SafeInt<T> const  l, UArbInt    const& r) { return UArbInt(r).op_mul(l); }
  template <class T> inline ArbInt  operator * (SafeInt<T> const  l, ArbInt     const& r) { return ArbInt (r).op_mul(l); }
  template <class T> inline UArbInt operator * (UArbInt    const& l, T          const  r) { return UArbInt(l).op_mul(r); }
  template <class T> inline ArbInt  operator * (ArbInt     const& l, T          const  r) { return ArbInt (l).op_mul(r); }
  template <class T> inline UArbInt operator * (T          const  l, UArbInt    const& r) { return UArbInt(r).op_mul(l); }
  template <class T> inline ArbInt  operator * (T          const  l, ArbInt     const& r) { return ArbInt (r).op_mul(l); }
                     inline UArbInt operator / (UArbInt    const& l, UArbInt    const& r) { return UArbInt(l).op_div(r); }
                     inline ArbInt  operator / (ArbInt     const& l, ArbInt     const& r) { return ArbInt (l).op_div(r); }
                     inline ArbInt  operator / (UArbInt    const& l, ArbInt     const& r) { return ArbInt (l).op_div(r); }
                     inline ArbInt  operator / (ArbInt     const& l, UArbInt    const& r) { return ArbInt (l).op_div(r); }
  template <class T> inline UArbInt operator / (UArbInt    const& l, SafeInt<T> const  r) { return UArbInt(l).op_div(r); }
  template <class T> inline ArbInt  operator / (ArbInt     const& l, SafeInt<T> const  r) { return ArbInt (l).op_div(r); }
  template <class T> inline UArbInt operator / (SafeInt<T> const  l, UArbInt    const& r) { return UArbInt(l).op_div(r); }
  template <class T> inline ArbInt  operator / (SafeInt<T> const  l, ArbInt     const& r) { return ArbInt (l).op_div(r); }
  template <class T> inline UArbInt operator / (UArbInt    const& l, T          const  r) { return UArbInt(l).op_div(r); }
  template <class T> inline ArbInt  operator / (ArbInt     const& l, T          const  r) { return ArbInt (l).op_div(r); }
  template <class T> inline UArbInt operator / (T          const  l, UArbInt    const& r) { return UArbInt(l).op_div(r); }
  template <class T> inline ArbInt  operator / (T          const  l, ArbInt     const& r) { return ArbInt (l).op_div(r); }
                     inline UArbInt operator % (UArbInt    const& l, UArbInt    const& r) { return UArbInt(l).op_mod(r); }
                     inline ArbInt  operator % (ArbInt     const& l, ArbInt     const& r) { return ArbInt (l).op_mod(r); }
                     inline ArbInt  operator % (UArbInt    const& l, ArbInt     const& r) { return ArbInt (l).op_mod(r); }
                     inline ArbInt  operator % (ArbInt     const& l, UArbInt    const& r) { return ArbInt (l).op_mod(r); }
  template <class T> inline UArbInt operator % (UArbInt    const& l, SafeInt<T> const  r) { return UArbInt(l).op_mod(r); }
  template <class T> inline ArbInt  operator % (ArbInt     const& l, SafeInt<T> const  r) { return ArbInt (l).op_mod(r); }
  template <class T> inline UArbInt operator % (SafeInt<T> const  l, UArbInt    const& r) { return UArbInt(l).op_mod(r); }
  template <class T> inline ArbInt  operator % (SafeInt<T> const  l, ArbInt     const& r) { return ArbInt (l).op_mod(r); }
  template <class T> inline UArbInt operator % (UArbInt    const& l, T          const  r) { return UArbInt(l).op_mod(r); }
  template <class T> inline ArbInt  operator % (ArbInt     const& l, T          const  r) { return ArbInt (l).op_mod(r); }
  template <class T> inline UArbInt operator % (T          const  l, UArbInt    const& r) { return UArbInt(l).op_mod(r); }
  template <class T> inline ArbInt  operator % (T          const  l, ArbInt     const& r) { return ArbInt (l).op_mod(r); }
                     inline UArbInt operator + (UArbInt    const& l, UArbInt    const& r) { return UArbInt(l).op_add(r); }
                     inline ArbInt  operator + (ArbInt     const& l, ArbInt     const& r) { return ArbInt (l).op_add(r); }
                     inline ArbInt  operator + (UArbInt    const& l, ArbInt     const& r) { return ArbInt (l).op_add(r); }
                     inline ArbInt  operator + (ArbInt     const& l, UArbInt    const& r) { return ArbInt (l).op_add(r); }
  template <class T> inline UArbInt operator + (UArbInt    const& l, SafeInt<T> const  r) { return UArbInt(l).op_add(r); }
  template <class T> inline ArbInt  operator + (ArbInt     const& l, SafeInt<T> const  r) { return ArbInt (l).op_add(r); }
  template <class T> inline UArbInt operator + (SafeInt<T> const  l, UArbInt    const& r) { return UArbInt(r).op_add(l); }
  template <class T> inline ArbInt  operator + (SafeInt<T> const  l, ArbInt     const& r) { return ArbInt (r).op_add(l); }
  template <class T> inline UArbInt operator + (UArbInt    const& l, T          const  r) { return UArbInt(l).op_add(r); }
  template <class T> inline ArbInt  operator + (ArbInt     const& l, T          const  r) { return ArbInt (l).op_add(r); }
  template <class T> inline UArbInt operator + (T          const  l, UArbInt    const& r) { return UArbInt(r).op_add(l); }
  template <class T> inline ArbInt  operator + (T          const  l, ArbInt     const& r) { return ArbInt (r).op_add(l); }
                     inline UArbInt operator - (UArbInt    const& l, UArbInt    const& r) { return UArbInt(l).op_sub(r); }
                     inline ArbInt  operator - (ArbInt     const& l, ArbInt     const& r) { return ArbInt (l).op_sub(r); }
                     inline ArbInt  operator - (UArbInt    const& l, ArbInt     const& r) { return ArbInt (l).op_sub(r); }
                     inline ArbInt  operator - (ArbInt     const& l, UArbInt    const& r) { return ArbInt (l).op_sub(r); }
  template <class T> inline UArbInt operator - (UArbInt    const& l, SafeInt<T> const  r) { return UArbInt(l).op_sub(r); }
  template <class T> inline ArbInt  operator - (ArbInt     const& l, SafeInt<T> const  r) { return ArbInt (l).op_sub(r); }
  template <class T> inline UArbInt operator - (SafeInt<T> const  l, UArbInt    const& r) { return UArbInt(l).op_sub(r); }
  template <class T> inline ArbInt  operator - (SafeInt<T> const  l, ArbInt     const& r) { return ArbInt (l).op_sub(r); }
  template <class T> inline UArbInt operator - (UArbInt    const& l, T          const  r) { return UArbInt(l).op_sub(r); }
  template <class T> inline ArbInt  operator - (ArbInt     const& l, T          const  r) { return ArbInt (l).op_sub(r); }
  template <class T> inline UArbInt operator - (T          const  l, UArbInt    const& r) { return UArbInt(l).op_sub(r); }
  template <class T> inline ArbInt  operator - (T          const  l, ArbInt     const& r) { return ArbInt (l).op_sub(r); }
  
  /*
   * Bit shift operators.
   */
                     inline UArbInt operator << (UArbInt    const& l, UArbInt    const& r) { return UArbInt(l).op_shl(r); }
                     inline ArbInt  operator << (ArbInt     const& l, ArbInt     const& r) { return ArbInt (l).op_shl(r); }
                     inline ArbInt  operator << (UArbInt    const& l, ArbInt     const& r) { return ArbInt (l).op_shl(r); }
                     inline ArbInt  operator << (ArbInt     const& l, UArbInt    const& r) { return ArbInt (l).op_shl(r); }
  template <class T> inline UArbInt operator << (UArbInt    const& l, SafeInt<T> const  r) { return UArbInt(l).op_shl(r); }
  template <class T> inline ArbInt  operator << (ArbInt     const& l, SafeInt<T> const  r) { return ArbInt (l).op_shl(r); }
  template <class T> inline UArbInt operator << (SafeInt<T> const  l, UArbInt    const& r) { return UArbInt(l).op_shl(r); }
  template <class T> inline ArbInt  operator << (SafeInt<T> const  l, ArbInt     const& r) { return ArbInt (l).op_shl(r); }
  template <class T> inline UArbInt operator << (UArbInt    const& l, T          const  r) { return UArbInt(l).op_shl(r); }
  template <class T> inline ArbInt  operator << (ArbInt     const& l, T          const  r) { return ArbInt (l).op_shl(r); }
  template <class T> inline UArbInt operator << (T          const  l, UArbInt    const& r) { return UArbInt(l).op_shl(r); }
  template <class T> inline ArbInt  operator << (T          const  l, ArbInt     const& r) { return ArbInt (l).op_shl(r); }
                     inline UArbInt operator >> (UArbInt    const& l, UArbInt    const& r) { return UArbInt(l).op_shr(r); }
                     inline ArbInt  operator >> (ArbInt     const& l, ArbInt     const& r) { return ArbInt (l).op_shr(r); }
                     inline ArbInt  operator >> (UArbInt    const& l, ArbInt     const& r) { return ArbInt (l).op_shr(r); }
                     inline ArbInt  operator >> (ArbInt     const& l, UArbInt    const& r) { return ArbInt (l).op_shr(r); }
  template <class T> inline UArbInt operator >> (UArbInt    const& l, SafeInt<T> const  r) { return UArbInt(l).op_shr(r); }
  template <class T> inline ArbInt  operator >> (ArbInt     const& l, SafeInt<T> const  r) { return ArbInt (l).op_shr(r); }
  template <class T> inline UArbInt operator >> (SafeInt<T> const  l, UArbInt    const& r) { return UArbInt(l).op_shr(r); }
  template <class T> inline ArbInt  operator >> (SafeInt<T> const  l, ArbInt     const& r) { return ArbInt (l).op_shr(r); }
  template <class T> inline UArbInt operator >> (UArbInt    const& l, T          const  r) { return UArbInt(l).op_shr(r); }
  template <class T> inline ArbInt  operator >> (ArbInt     const& l, T          const  r) { return ArbInt (l).op_shr(r); }
  template <class T> inline UArbInt operator >> (T          const  l, UArbInt    const& r) { return UArbInt(l).op_shr(r); }
  template <class T> inline ArbInt  operator >> (T          const  l, ArbInt     const& r) { return ArbInt (l).op_shr(r); }
  
  /*
   * Comparsion operators.
   */
                     inline bool operator >  (UArbInt    const& l, UArbInt    const& r) { return  l.op_gt(r); }
                     inline bool operator >  (ArbInt     const& l, ArbInt     const& r) { return  l.op_gt(r); }
                     inline bool operator >  (UArbInt    const& l, ArbInt     const& r) { return  l.op_gt(r); }
                     inline bool operator >  (ArbInt     const& l, UArbInt    const& r) { return  l.op_gt(r); }
  template <class T> inline bool operator >  (UArbInt    const& l, SafeInt<T> const  r) { return  l.op_gt(r); }
  template <class T> inline bool operator >  (ArbInt     const& l, SafeInt<T> const  r) { return  l.op_gt(r); }
  template <class T> inline bool operator >  (SafeInt<T> const  l, UArbInt    const& r) { return !r.op_ge(l); }
  template <class T> inline bool operator >  (SafeInt<T> const  l, ArbInt     const& r) { return !r.op_ge(l); }
  template <class T> inline bool operator >  (UArbInt    const& l, T          const  r) { return  l.op_gt(r); }
  template <class T> inline bool operator >  (ArbInt     const& l, T          const  r) { return  l.op_gt(r); }
  template <class T> inline bool operator >  (T          const  l, UArbInt    const& r) { return !r.op_ge(l); }
  template <class T> inline bool operator >  (T          const  l, ArbInt     const& r) { return !r.op_ge(l); }
                     inline bool operator >= (UArbInt    const& l, UArbInt    const& r) { return  l.op_ge(r); }
                     inline bool operator >= (ArbInt     const& l, ArbInt     const& r) { return  l.op_ge(r); }
                     inline bool operator >= (UArbInt    const& l, ArbInt     const& r) { return  l.op_ge(r); }
                     inline bool operator >= (ArbInt     const& l, UArbInt    const& r) { return  l.op_ge(r); }
  template <class T> inline bool operator >= (UArbInt    const& l, SafeInt<T> const  r) { return  l.op_ge(r); }
  template <class T> inline bool operator >= (ArbInt     const& l, SafeInt<T> const  r) { return  l.op_ge(r); }
  template <class T> inline bool operator >= (SafeInt<T> const  l, UArbInt    const& r) { return !r.op_gt(l); }
  template <class T> inline bool operator >= (SafeInt<T> const  l, ArbInt     const& r) { return !r.op_gt(l); }
  template <class T> inline bool operator >= (UArbInt    const& l, T          const  r) { return  l.op_ge(r); }
  template <class T> inline bool operator >= (ArbInt     const& l, T          const  r) { return  l.op_ge(r); }
  template <class T> inline bool operator >= (T          const  l, UArbInt    const& r) { return !r.op_gt(l); }
  template <class T> inline bool operator >= (T          const  l, ArbInt     const& r) { return !r.op_gt(l); }
                     inline bool operator <  (UArbInt    const& l, UArbInt    const& r) { return  l.op_lt(r); }
                     inline bool operator <  (ArbInt     const& l, ArbInt     const& r) { return  l.op_lt(r); }
                     inline bool operator <  (UArbInt    const& l, ArbInt     const& r) { return  l.op_lt(r); }
                     inline bool operator <  (ArbInt     const& l, UArbInt    const& r) { return  l.op_lt(r); }
  template <class T> inline bool operator <  (UArbInt    const& l, SafeInt<T> const  r) { return  l.op_lt(r); }
  template <class T> inline bool operator <  (ArbInt     const& l, SafeInt<T> const  r) { return  l.op_lt(r); }
  template <class T> inline bool operator <  (SafeInt<T> const  l, UArbInt    const& r) { return !r.op_le(l); }
  template <class T> inline bool operator <  (SafeInt<T> const  l, ArbInt     const& r) { return !r.op_le(l); }
  template <class T> inline bool operator <  (UArbInt    const& l, T          const  r) { return  l.op_lt(r); }
  template <class T> inline bool operator <  (ArbInt     const& l, T          const  r) { return  l.op_lt(r); }
  template <class T> inline bool operator <  (T          const  l, UArbInt    const& r) { return !r.op_le(l); }
  template <class T> inline bool operator <  (T          const  l, ArbInt     const& r) { return !r.op_le(l); }
                     inline bool operator <= (UArbInt    const& l, UArbInt    const& r) { return  l.op_le(r); }
                     inline bool operator <= (ArbInt     const& l, ArbInt     const& r) { return  l.op_le(r); }
                     inline bool operator <= (UArbInt    const& l, ArbInt     const& r) { return  l.op_le(r); }
                     inline bool operator <= (ArbInt     const& l, UArbInt    const& r) { return  l.op_le(r); }
  template <class T> inline bool operator <= (UArbInt    const& l, SafeInt<T> const  r) { return  l.op_le(r); }
  template <class T> inline bool operator <= (ArbInt     const& l, SafeInt<T> const  r) { return  l.op_le(r); }
  template <class T> inline bool operator <= (SafeInt<T> const  l, UArbInt    const& r) { return !r.op_lt(l); }
  template <class T> inline bool operator <= (SafeInt<T> const  l, ArbInt     const& r) { return !r.op_lt(l); }
  template <class T> inline bool operator <= (UArbInt    const& l, T          const  r) { return  l.op_le(r); }
  template <class T> inline bool operator <= (ArbInt     const& l, T          const  r) { return  l.op_le(r); }
  template <class T> inline bool operator <= (T          const  l, UArbInt    const& r) { return !r.op_lt(l); }
  template <class T> inline bool operator <= (T          const  l, ArbInt     const& r) { return !r.op_lt(l); }
                     inline bool operator == (UArbInt    const& l, UArbInt    const& r) { return  l.op_eq(r); }
                     inline bool operator == (ArbInt     const& l, ArbInt     const& r) { return  l.op_eq(r); }
                     inline bool operator == (UArbInt    const& l, ArbInt     const& r) { return  l.op_eq(r); }
                     inline bool operator == (ArbInt     const& l, UArbInt    const& r) { return  l.op_eq(r); }
  template <class T> inline bool operator == (UArbInt    const& l, SafeInt<T> const  r) { return  l.op_eq(r); }
  template <class T> inline bool operator == (ArbInt     const& l, SafeInt<T> const  r) { return  l.op_eq(r); }
  template <class T> inline bool operator == (SafeInt<T> const  l, UArbInt    const& r) { return  r.op_eq(l); }
  template <class T> inline bool operator == (SafeInt<T> const  l, ArbInt     const& r) { return  r.op_eq(l); }
  template <class T> inline bool operator == (UArbInt    const& l, T          const  r) { return  l.op_eq(r); }
  template <class T> inline bool operator == (ArbInt     const& l, T          const  r) { return  l.op_eq(r); }
  template <class T> inline bool operator == (T          const  l, UArbInt    const& r) { return  r.op_eq(l); }
  template <class T> inline bool operator == (T          const  l, ArbInt     const& r) { return  r.op_eq(l); }
                     inline bool operator != (UArbInt    const& l, UArbInt    const& r) { return  l.op_ne(r); }
                     inline bool operator != (ArbInt     const& l, ArbInt     const& r) { return  l.op_ne(r); }
                     inline bool operator != (UArbInt    const& l, ArbInt     const& r) { return  l.op_ne(r); }
                     inline bool operator != (ArbInt     const& l, UArbInt    const& r) { return  l.op_ne(r); }
  template <class T> inline bool operator != (UArbInt    const& l, SafeInt<T> const  r) { return  l.op_ne(r); }
  template <class T> inline bool operator != (ArbInt     const& l, SafeInt<T> const  r) { return  l.op_ne(r); }
  template <class T> inline bool operator != (SafeInt<T> const  l, UArbInt    const& r) { return  r.op_ne(l); }
  template <class T> inline bool operator != (SafeInt<T> const  l, ArbInt     const& r) { return  r.op_ne(l); }
  template <class T> inline bool operator != (UArbInt    const& l, T          const  r) { return  l.op_ne(r); }
  template <class T> inline bool operator != (ArbInt     const& l, T          const  r) { return  l.op_ne(r); }
  template <class T> inline bool operator != (T          const  l, UArbInt    const& r) { return  r.op_ne(l); }
  template <class T> inline bool operator != (T          const  l, ArbInt     const& r) { return  r.op_ne(l); }
  
  /*
   * Bitwise operators.
   */
                     inline UArbInt operator & (UArbInt    const& l, UArbInt    const& r) { return UArbInt(l).op_bit_and(r); }
                     inline ArbInt  operator & (ArbInt     const& l, ArbInt     const& r) { return ArbInt (l).op_bit_and(r); }
                     inline ArbInt  operator & (UArbInt    const& l, ArbInt     const& r) { return ArbInt (l).op_bit_and(r); }
                     inline ArbInt  operator & (ArbInt     const& l, UArbInt    const& r) { return ArbInt (l).op_bit_and(r); }
  template <class T> inline UArbInt operator & (UArbInt    const& l, SafeInt<T> const  r) { return UArbInt(l).op_bit_and(r); }
  template <class T> inline ArbInt  operator & (ArbInt     const& l, SafeInt<T> const  r) { return ArbInt (l).op_bit_and(r); }
  template <class T> inline UArbInt operator & (SafeInt<T> const  l, UArbInt    const& r) { return UArbInt(r).op_bit_and(l); }
  template <class T> inline ArbInt  operator & (SafeInt<T> const  l, ArbInt     const& r) { return ArbInt (r).op_bit_and(l); }
  template <class T> inline UArbInt operator & (UArbInt    const& l, T          const  r) { return UArbInt(l).op_bit_and(r); }
  template <class T> inline ArbInt  operator & (ArbInt     const& l, T          const  r) { return ArbInt (l).op_bit_and(r); }
  template <class T> inline UArbInt operator & (T          const  l, UArbInt    const& r) { return UArbInt(r).op_bit_and(l); }
  template <class T> inline ArbInt  operator & (T          const  l, ArbInt     const& r) { return ArbInt (r).op_bit_and(l); }
                     inline UArbInt operator | (UArbInt    const& l, UArbInt    const& r) { return UArbInt(l).op_bit_ior(r); }
                     inline ArbInt  operator | (ArbInt     const& l, ArbInt     const& r) { return ArbInt (l).op_bit_ior(r); }
                     inline ArbInt  operator | (UArbInt    const& l, ArbInt     const& r) { return ArbInt (l).op_bit_ior(r); }
                     inline ArbInt  operator | (ArbInt     const& l, UArbInt    const& r) { return ArbInt (l).op_bit_ior(r); }
  template <class T> inline UArbInt operator | (UArbInt    const& l, SafeInt<T> const  r) { return UArbInt(l).op_bit_ior(r); }
  template <class T> inline ArbInt  operator | (ArbInt     const& l, SafeInt<T> const  r) { return ArbInt (l).op_bit_ior(r); }
  template <class T> inline UArbInt operator | (SafeInt<T> const  l, UArbInt    const& r) { return UArbInt(r).op_bit_ior(l); }
  template <class T> inline ArbInt  operator | (SafeInt<T> const  l, ArbInt     const& r) { return ArbInt (r).op_bit_ior(l); }
  template <class T> inline UArbInt operator | (UArbInt    const& l, T          const  r) { return UArbInt(l).op_bit_ior(r); }
  template <class T> inline ArbInt  operator | (ArbInt     const& l, T          const  r) { return ArbInt (l).op_bit_ior(r); }
  template <class T> inline UArbInt operator | (T          const  l, UArbInt    const& r) { return UArbInt(r).op_bit_ior(l); }
  template <class T> inline ArbInt  operator | (T          const  l, ArbInt     const& r) { return ArbInt (r).op_bit_ior(l); }
                     inline UArbInt operator ^ (UArbInt    const& l, UArbInt    const& r) { return UArbInt(l).op_bit_xor(r); }
                     inline ArbInt  operator ^ (ArbInt     const& l, ArbInt     const& r) { return ArbInt (l).op_bit_xor(r); }
                     inline ArbInt  operator ^ (UArbInt    const& l, ArbInt     const& r) { return ArbInt (l).op_bit_xor(r); }
                     inline ArbInt  operator ^ (ArbInt     const& l, UArbInt    const& r) { return ArbInt (l).op_bit_xor(r); }
  template <class T> inline UArbInt operator ^ (UArbInt    const& l, SafeInt<T> const  r) { return UArbInt(l).op_bit_xor(r); }
  template <class T> inline ArbInt  operator ^ (ArbInt     const& l, SafeInt<T> const  r) { return ArbInt (l).op_bit_xor(r); }
  template <class T> inline UArbInt operator ^ (SafeInt<T> const  l, UArbInt    const& r) { return UArbInt(r).op_bit_xor(l); }
  template <class T> inline ArbInt  operator ^ (SafeInt<T> const  l, ArbInt     const& r) { return ArbInt (r).op_bit_xor(l); }
  template <class T> inline UArbInt operator ^ (UArbInt    const& l, T          const  r) { return UArbInt(l).op_bit_xor(r); }
  template <class T> inline ArbInt  operator ^ (ArbInt     const& l, T          const  r) { return ArbInt (l).op_bit_xor(r); }
  template <class T> inline UArbInt operator ^ (T          const  l, UArbInt    const& r) { return UArbInt(r).op_bit_xor(l); }
  template <class T> inline ArbInt  operator ^ (T          const  l, ArbInt     const& r) { return ArbInt (r).op_bit_xor(l); }
  
  /*
   * Arithmetic assignment operators.
   */
                     inline UArbInt&    operator *= (UArbInt&    l, UArbInt    const& r) { return l.op_mul(r);                 }
                     inline ArbInt&     operator *= (ArbInt&     l, ArbInt     const& r) { return l.op_mul(r);                 }
                     inline UArbInt&    operator *= (UArbInt&    l, ArbInt     const& r) { l = ArbInt(l).op_mul(r); return l;  }
                     inline ArbInt&     operator *= (ArbInt&     l, UArbInt    const& r) { return l.op_mul(r);                 }
  template <class T> inline UArbInt&    operator *= (UArbInt&    l, SafeInt<T> const  r) { return l.op_mul(r);                 }
  template <class T> inline ArbInt&     operator *= (ArbInt&     l, SafeInt<T> const  r) { return l.op_mul(r);                 }
  template <class T> inline SafeInt<T>& operator *= (SafeInt<T>& l, UArbInt    const& r) { l = UArbInt(l).op_mul(r); return l; }
  template <class T> inline SafeInt<T>& operator *= (SafeInt<T>& l, ArbInt     const& r) { l = ArbInt (l).op_mul(r); return l; }
  template <class T> inline UArbInt&    operator *= (UArbInt&    l, T          const  r) { return l.op_mul(r);                 }
  template <class T> inline ArbInt&     operator *= (ArbInt&     l, T          const  r) { return l.op_mul(r);                 }
  template <class T> inline T&          operator *= (T&          l, UArbInt    const& r) { l = UArbInt(l).op_mul(r); return l; }
  template <class T> inline T&          operator *= (T&          l, ArbInt     const& r) { l = ArbInt (l).op_mul(r); return l; }
                     inline UArbInt&    operator /= (UArbInt&    l, UArbInt    const& r) { return l.op_div(r);                 }
                     inline ArbInt&     operator /= (ArbInt&     l, ArbInt     const& r) { return l.op_div(r);                 }
                     inline UArbInt&    operator /= (UArbInt&    l, ArbInt     const& r) { l = ArbInt(l).op_div(r); return l;  }
                     inline ArbInt&     operator /= (ArbInt&     l, UArbInt    const& r) { return l.op_div(r);                 }
  template <class T> inline UArbInt&    operator /= (UArbInt&    l, SafeInt<T> const  r) { return l.op_div(r);                 }
  template <class T> inline ArbInt&     operator /= (ArbInt&     l, SafeInt<T> const  r) { return l.op_div(r);                 }
  template <class T> inline SafeInt<T>& operator /= (SafeInt<T>& l, UArbInt    const& r) { l = UArbInt(l).op_div(r); return l; }
  template <class T> inline SafeInt<T>& operator /= (SafeInt<T>& l, ArbInt     const& r) { l = ArbInt (l).op_div(r); return l; }
  template <class T> inline UArbInt&    operator /= (UArbInt&    l, T          const  r) { return l.op_div(r);                 }
  template <class T> inline ArbInt&     operator /= (ArbInt&     l, T          const  r) { return l.op_div(r);                 }
  template <class T> inline T&          operator /= (T&          l, UArbInt    const& r) { l = UArbInt(l).op_div(r); return l; }
  template <class T> inline T&          operator /= (T&          l, ArbInt     const& r) { l = ArbInt (l).op_div(r); return l; }
                     inline UArbInt&    operator %= (UArbInt&    l, UArbInt    const& r) { return l.op_mod(r);                 }
                     inline ArbInt&     operator %= (ArbInt&     l, ArbInt     const& r) { return l.op_mod(r);                 }
                     inline UArbInt&    operator %= (UArbInt&    l, ArbInt     const& r) { l = ArbInt(l).op_mod(r); return l;  }
                     inline ArbInt&     operator %= (ArbInt&     l, UArbInt    const& r) { return l.op_mod(r);                 }
  template <class T> inline UArbInt&    operator %= (UArbInt&    l, SafeInt<T> const  r) { return l.op_mod(r);                 }
  template <class T> inline ArbInt&     operator %= (ArbInt&     l, SafeInt<T> const  r) { return l.op_mod(r);                 }
  template <class T> inline SafeInt<T>& operator %= (SafeInt<T>& l, UArbInt    const& r) { l = UArbInt(l).op_mod(r); return l; }
  template <class T> inline SafeInt<T>& operator %= (SafeInt<T>& l, ArbInt     const& r) { l = ArbInt (l).op_mod(r); return l; }
  template <class T> inline UArbInt&    operator %= (UArbInt&    l, T          const  r) { return l.op_mod(r);                 }
  template <class T> inline ArbInt&     operator %= (ArbInt&     l, T          const  r) { return l.op_mod(r);                 }
  template <class T> inline T&          operator %= (T&          l, UArbInt    const& r) { l = UArbInt(l).op_mod(r); return l; }
  template <class T> inline T&          operator %= (T&          l, ArbInt     const& r) { l = ArbInt (l).op_mod(r); return l; }
                     inline UArbInt&    operator += (UArbInt&    l, UArbInt    const& r) { return l.op_add(r);                 }
                     inline ArbInt&     operator += (ArbInt&     l, ArbInt     const& r) { return l.op_add(r);                 }
                     inline UArbInt&    operator += (UArbInt&    l, ArbInt     const& r) { l = ArbInt(l).op_add(r); return l;  }
                     inline ArbInt&     operator += (ArbInt&     l, UArbInt    const& r) { return l.op_add(r);                 }
  template <class T> inline UArbInt&    operator += (UArbInt&    l, SafeInt<T> const  r) { return l.op_add(r);                 }
  template <class T> inline ArbInt&     operator += (ArbInt&     l, SafeInt<T> const  r) { return l.op_add(r);                 }
  template <class T> inline SafeInt<T>& operator += (SafeInt<T>& l, UArbInt    const& r) { l = UArbInt(l).op_add(r); return l; }
  template <class T> inline SafeInt<T>& operator += (SafeInt<T>& l, ArbInt     const& r) { l = ArbInt (l).op_add(r); return l; }
  template <class T> inline UArbInt&    operator += (UArbInt&    l, T          const  r) { return l.op_add(r);                 }
  template <class T> inline ArbInt&     operator += (ArbInt&     l, T          const  r) { return l.op_add(r);                 }
  template <class T> inline T&          operator += (T&          l, UArbInt    const& r) { l = UArbInt(l).op_add(r); return l; }
  template <class T> inline T&          operator += (T&          l, ArbInt     const& r) { l = ArbInt (l).op_add(r); return l; }
                     inline UArbInt&    operator -= (UArbInt&    l, UArbInt    const& r) { return l.op_sub(r);                 }
                     inline ArbInt&     operator -= (ArbInt&     l, ArbInt     const& r) { return l.op_sub(r);                 }
                     inline UArbInt&    operator -= (UArbInt&    l, ArbInt     const& r) { l = ArbInt(l).op_sub(r); return l;  }
                     inline ArbInt&     operator -= (ArbInt&     l, UArbInt    const& r) { return l.op_sub(r);                 }
  template <class T> inline UArbInt&    operator -= (UArbInt&    l, SafeInt<T> const  r) { return l.op_sub(r);                 }
  template <class T> inline ArbInt&     operator -= (ArbInt&     l, SafeInt<T> const  r) { return l.op_sub(r);                 }
  template <class T> inline SafeInt<T>& operator -= (SafeInt<T>& l, UArbInt    const& r) { l = UArbInt(l).op_sub(r); return l; }
  template <class T> inline SafeInt<T>& operator -= (SafeInt<T>& l, ArbInt     const& r) { l = ArbInt (l).op_sub(r); return l; }
  template <class T> inline UArbInt&    operator -= (UArbInt&    l, T          const  r) { return l.op_sub(r);                 }
  template <class T> inline ArbInt&     operator -= (ArbInt&     l, T          const  r) { return l.op_sub(r);                 }
  template <class T> inline T&          operator -= (T&          l, UArbInt    const& r) { l = UArbInt(l).op_sub(r); return l; }
  template <class T> inline T&          operator -= (T&          l, ArbInt     const& r) { l = ArbInt (l).op_sub(r); return l; }
  
  /*
   * Bit shift assignment operators.
   */
                     inline UArbInt&    operator <<= (UArbInt&    l, UArbInt    const& r) { return l.op_shl(r);                 }
                     inline ArbInt&     operator <<= (ArbInt&     l, ArbInt     const& r) { return l.op_shl(r);                 }
                     inline UArbInt&    operator <<= (UArbInt&    l, ArbInt     const& r) { l = ArbInt(l).op_shl(r); return l;  }
                     inline ArbInt&     operator <<= (ArbInt&     l, UArbInt    const& r) { return l.op_shl(r);                 }
  template <class T> inline UArbInt&    operator <<= (UArbInt&    l, SafeInt<T> const  r) { return l.op_shl(r);                 }
  template <class T> inline ArbInt&     operator <<= (ArbInt&     l, SafeInt<T> const  r) { return l.op_shl(r);                 }
  template <class T> inline SafeInt<T>& operator <<= (SafeInt<T>& l, UArbInt    const& r) { l = UArbInt(l).op_shl(r); return l; }
  template <class T> inline SafeInt<T>& operator <<= (SafeInt<T>& l, ArbInt     const& r) { l = ArbInt (l).op_shl(r); return l; }
  template <class T> inline UArbInt&    operator <<= (UArbInt&    l, T          const  r) { return l.op_shl(r);                 }
  template <class T> inline ArbInt&     operator <<= (ArbInt&     l, T          const  r) { return l.op_shl(r);                 }
  template <class T> inline T&          operator <<= (T&          l, UArbInt    const& r) { l = UArbInt(l).op_shl(r); return l; }
  template <class T> inline T&          operator <<= (T&          l, ArbInt     const& r) { l = ArbInt (l).op_shl(r); return l; }
                     inline UArbInt&    operator >>= (UArbInt&    l, UArbInt    const& r) { return l.op_shr(r);                 }
                     inline ArbInt&     operator >>= (ArbInt&     l, ArbInt     const& r) { return l.op_shr(r);                 }
                     inline UArbInt&    operator >>= (UArbInt&    l, ArbInt     const& r) { l = ArbInt(l).op_shr(r); return l;  }
                     inline ArbInt&     operator >>= (ArbInt&     l, UArbInt    const& r) { return l.op_shr(r);                 }
  template <class T> inline UArbInt&    operator >>= (UArbInt&    l, SafeInt<T> const  r) { return l.op_shr(r);                 }
  template <class T> inline ArbInt&     operator >>= (ArbInt&     l, SafeInt<T> const  r) { return l.op_shr(r);                 }
  template <class T> inline SafeInt<T>& operator >>= (SafeInt<T>& l, UArbInt    const& r) { l = UArbInt(l).op_shr(r); return l; }
  template <class T> inline SafeInt<T>& operator >>= (SafeInt<T>& l, ArbInt     const& r) { l = ArbInt (l).op_shr(r); return l; }
  template <class T> inline UArbInt&    operator >>= (UArbInt&    l, T          const  r) { return l.op_shr(r);                 }
  template <class T> inline ArbInt&     operator >>= (ArbInt&     l, T          const  r) { return l.op_shr(r);                 }
  template <class T> inline T&          operator >>= (T&          l, UArbInt    const& r) { l = UArbInt(l).op_shr(r); return l; }
  template <class T> inline T&          operator >>= (T&          l, ArbInt     const& r) { l = ArbInt (l).op_shr(r); return l; }
  
  /*
   * Bitwise assignment operators.
   */
                     inline UArbInt&    operator &= (UArbInt&    l, UArbInt    const& r) { return l.op_bit_and(r);                 }
                     inline ArbInt&     operator &= (ArbInt&     l, ArbInt     const& r) { return l.op_bit_and(r);                 }
                     inline UArbInt&    operator &= (UArbInt&    l, ArbInt     const& r) { l = ArbInt(l).op_bit_and(r); return l;  }
                     inline ArbInt&     operator &= (ArbInt&     l, UArbInt    const& r) { return l.op_bit_and(r);                 }
  template <class T> inline UArbInt&    operator &= (UArbInt&    l, SafeInt<T> const  r) { return l.op_bit_and(r);                 }
  template <class T> inline ArbInt&     operator &= (ArbInt&     l, SafeInt<T> const  r) { return l.op_bit_and(r);                 }
  template <class T> inline SafeInt<T>& operator &= (SafeInt<T>& l, UArbInt    const& r) { l = UArbInt(l).op_bit_and(r); return l; }
  template <class T> inline SafeInt<T>& operator &= (SafeInt<T>& l, ArbInt     const& r) { l = ArbInt (l).op_bit_and(r); return l; }
  template <class T> inline UArbInt&    operator &= (UArbInt&    l, T          const  r) { return l.op_bit_and(r);                 }
  template <class T> inline ArbInt&     operator &= (ArbInt&     l, T          const  r) { return l.op_bit_and(r);                 }
  template <class T> inline T&          operator &= (T&          l, UArbInt    const& r) { l = UArbInt(l).op_bit_and(r); return l; }
  template <class T> inline T&          operator &= (T&          l, ArbInt     const& r) { l = ArbInt (l).op_bit_and(r); return l; }
                     inline UArbInt&    operator |= (UArbInt&    l, UArbInt    const& r) { return l.op_bit_ior(r);                 }
                     inline ArbInt&     operator |= (ArbInt&     l, ArbInt     const& r) { return l.op_bit_ior(r);                 }
                     inline UArbInt&    operator |= (UArbInt&    l, ArbInt     const& r) { l = ArbInt(l).op_bit_ior(r); return l;  }
                     inline ArbInt&     operator |= (ArbInt&     l, UArbInt    const& r) { return l.op_bit_ior(r);                 }
  template <class T> inline UArbInt&    operator |= (UArbInt&    l, SafeInt<T> const  r) { return l.op_bit_ior(r);                 }
  template <class T> inline ArbInt&     operator |= (ArbInt&     l, SafeInt<T> const  r) { return l.op_bit_ior(r);                 }
  template <class T> inline SafeInt<T>& operator |= (SafeInt<T>& l, UArbInt    const& r) { l = UArbInt(l).op_bit_ior(r); return l; }
  template <class T> inline SafeInt<T>& operator |= (SafeInt<T>& l, ArbInt     const& r) { l = ArbInt (l).op_bit_ior(r); return l; }
  template <class T> inline UArbInt&    operator |= (UArbInt&    l, T          const  r) { return l.op_bit_ior(r);                 }
  template <class T> inline ArbInt&     operator |= (ArbInt&     l, T          const  r) { return l.op_bit_ior(r);                 }
  template <class T> inline T&          operator |= (T&          l, UArbInt    const& r) { l = UArbInt(l).op_bit_ior(r); return l; }
  template <class T> inline T&          operator |= (T&          l, ArbInt     const& r) { l = ArbInt (l).op_bit_ior(r); return l; }
                     inline UArbInt&    operator ^= (UArbInt&    l, UArbInt    const& r) { return l.op_bit_xor(r);                 }
                     inline ArbInt&     operator ^= (ArbInt&     l, ArbInt     const& r) { return l.op_bit_xor(r);                 }
                     inline UArbInt&    operator ^= (UArbInt&    l, ArbInt     const& r) { l = ArbInt(l).op_bit_xor(r); return l;  }
                     inline ArbInt&     operator ^= (ArbInt&     l, UArbInt    const& r) { return l.op_bit_xor(r);                 }
  template <class T> inline UArbInt&    operator ^= (UArbInt&    l, SafeInt<T> const  r) { return l.op_bit_xor(r);                 }
  template <class T> inline ArbInt&     operator ^= (ArbInt&     l, SafeInt<T> const  r) { return l.op_bit_xor(r);                 }
  template <class T> inline SafeInt<T>& operator ^= (SafeInt<T>& l, UArbInt    const& r) { l = UArbInt(l).op_bit_xor(r); return l; }
  template <class T> inline SafeInt<T>& operator ^= (SafeInt<T>& l, ArbInt     const& r) { l = ArbInt (l).op_bit_xor(r); return l; }
  template <class T> inline UArbInt&    operator ^= (UArbInt&    l, T          const  r) { return l.op_bit_xor(r);                 }
  template <class T> inline ArbInt&     operator ^= (ArbInt&     l, T          const  r) { return l.op_bit_xor(r);                 }
  template <class T> inline T&          operator ^= (T&          l, UArbInt    const& r) { l = UArbInt(l).op_bit_xor(r); return l; }
  template <class T> inline T&          operator ^= (T&          l, ArbInt     const& r) { l = ArbInt (l).op_bit_xor(r); return l; }
  
}

#endif
