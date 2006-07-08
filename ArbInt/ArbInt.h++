/*****************************************************************************
 * ArbInt
 *****************************************************************************
 * ArbInt interface.
 *****************************************************************************/

// Include guard.
#if !defined(ARBINT_u98h4uh98u)
  #define ARBINT_u98h4uh98u

// Standard includes.
  #include <string>
  #include <iostream>

// System includes.
  #include <UArbInt.h++>
  #include <Exception.h++>
  #include <to_string.h++>

// Namespace wrapping.
namespace DAC {
  
  /***************************************************************************
   * ArbInt
   ***************************************************************************
   * Arbitrary length integer.
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
                  std::string tmpmsg(_problem + " at position " + DAC::to_string(SafeInt<std::string::size_type(_position) + 1) + ".");
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
      
      // Return information about this number.
      bool   isEven       () const;
      bool   isOdd        () const;
      bool   isZero       () const;
      bool   isPositive   () const;
      bool   isNegative   () const;
      size_t bitsInNumber () const;
      
      // Convert.
      UArbInt toUArbInt () const;
      
      // Raise this number to a power.
                         ArbInt pow (ArbInt  const& exp);
                         ArbInt pow (UArbInt const& exp);
      template <class T> ArbInt pow (T       const  exp);
      
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
      
      // Shift.
      template <class T, _NumType> class _Shift;
      template <class T> class _Shift<T, _NUM_UINT> { public: static void op (ArbInt& l, SafeInt<T> const r, _Dir const dir); static void op (ArbInt& l, T const r); };
      template <class T> class _Shift<T, _NUM_SINT> { public: static void op (ArbInt& l, SafeInt<T> const r, _Dir const dir); static void op (ArbInt& l, T const r); };
      
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
      template <class T> class _Compare<T, _NUM_UINT> { public: static void op (ArbInt const& l, SafeInt<T> const r); static void op (ArbInt const& l, T const r); };
      template <class T> class _Compare<T, _NUM_SINT> { public: static void op (ArbInt const& l, SafeInt<T> const r); static void op (ArbInt const& l, T const r); };
      
      // Greater than.
      template <class T, _NumType> class _GT;
      template <class T> class _GT<T, _NUM_UINT> { public: static void op (ArbInt const& l, SafeInt<T> const r); static void op (ArbInt const& l, T const r); };
      template <class T> class _GT<T, _NUM_SINT> { public: static void op (ArbInt const& l, SafeInt<T> const r); static void op (ArbInt const& l, T const r); };
      
      // Greater than or equal to.
      template <class T, _NumType> class _GE;
      template <class T> class _GE<T, _NUM_UINT> { public: static void op (ArbInt const& l, SafeInt<T> const r); static void op (ArbInt const& l, T const r); };
      template <class T> class _GE<T, _NUM_SINT> { public: static void op (ArbInt const& l, SafeInt<T> const r); static void op (ArbInt const& l, T const r); };
      
      // Less than.
      template <class T, _NumType> class _LT;
      template <class T> class _LT<T, _NUM_UINT> { public: static void op (ArbInt const& l, SafeInt<T> const r); static void op (ArbInt const& l, T const r); };
      template <class T> class _LT<T, _NUM_SINT> { public: static void op (ArbInt const& l, SafeInt<T> const r); static void op (ArbInt const& l, T const r); };
      
      // Less than or equal to.
      template <class T, _NumType> class _LE;
      template <class T> class _LE<T, _NUM_UINT> { public: static void op (ArbInt const& l, SafeInt<T> const r); static void op (ArbInt const& l, T const r); };
      template <class T> class _LE<T, _NUM_SINT> { public: static void op (ArbInt const& l, SafeInt<T> const r); static void op (ArbInt const& l, T const r); };
      
      // Equal to.
      template <class T, _NumType> class _EQ;
      template <class T> class _EQ<T, _NUM_UINT> { public: static void op (ArbInt const& l, SafeInt<T> const r); static void op (ArbInt const& l, T const r); };
      template <class T> class _EQ<T, _NUM_SINT> { public: static void op (ArbInt const& l, SafeInt<T> const r); static void op (ArbInt const& l, T const r); };
      
      // Not equal.
      template <class T, _NumType> class _NE;
      template <class T> class _NE<T, _NUM_UINT> { public: static void op (ArbInt const& l, SafeInt<T> const r); static void op (ArbInt const& l, T const r); };
      template <class T> class _NE<T, _NUM_SINT> { public: static void op (ArbInt const& l, SafeInt<T> const r); static void op (ArbInt const& l, T const r); };
      
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
      
      // Sign.
      bool _positive;
      
      // The number.
      UArbInt _digits;
      
      // Temporary buffer for string output.
      mutable std::string _strbuf;
      
  };
  
  /***************************************************************************
   * Operators.
   ***************************************************************************/
  
  // Stream I/O operators.
  std::ostream&       operator << (std::ostream&       l, ArbInt const& r);
  std::ostringstream& operator << (std::ostringstream& l, ArbInt const& r);
  std::istream&       operator >> (std::istream&       l, ArbInt&       r);
  
  // Arithmetic operators.
                     ArbInt operator * (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator * (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator * (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator * (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator * (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator * (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator * (T          const  l, ArbInt     const& r);
                     ArbInt operator / (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator / (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator / (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator / (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator / (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator / (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator / (T          const  l, ArbInt     const& r);
                     ArbInt operator % (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator % (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator % (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator % (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator % (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator % (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator % (T          const  l, ArbInt     const& r);
                     ArbInt operator + (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator + (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator + (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator + (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator + (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator + (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator + (T          const  l, ArbInt     const& r);
                     ArbInt operator - (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator - (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator - (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator - (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator - (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator - (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator - (T          const  l, ArbInt     const& r);
  
  // Bit shift operators.
                     ArbInt operator << (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator << (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator << (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator << (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator << (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator << (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator << (T          const  l, ArbInt     const& r);
                     ArbInt operator >> (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator >> (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator >> (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator >> (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator >> (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator >> (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator >> (T          const  l, ArbInt     const& r);
  
  // Comparison operators.
                     bool operator >  (ArbInt     const& l, ArbInt     const& r);
                     bool operator >  (ArbInt     const& l, UArbInt    const& r);
                     bool operator >  (UArbInt    const& l, ArbInt     const& r);
  template <class T> bool operator >  (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> bool operator >  (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> bool operator >  (ArbInt     const& l, T          const  r);
  template <class T> bool operator >  (T          const  l, ArbInt     const& r);
                     bool operator >= (ArbInt     const& l, ArbInt     const& r);
                     bool operator >= (ArbInt     const& l, UArbInt    const& r);
                     bool operator >= (UArbInt    const& l, ArbInt     const& r);
  template <class T> bool operator >= (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> bool operator >= (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> bool operator >= (ArbInt     const& l, T          const  r);
  template <class T> bool operator >= (T          const  l, ArbInt     const& r);
                     bool operator <  (ArbInt     const& l, ArbInt     const& r);
                     bool operator <  (ArbInt     const& l, UArbInt    const& r);
                     bool operator <  (UArbInt    const& l, ArbInt     const& r);
  template <class T> bool operator <  (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> bool operator <  (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> bool operator <  (ArbInt     const& l, T          const  r);
  template <class T> bool operator <  (T          const  l, ArbInt     const& r);
                     bool operator <= (ArbInt     const& l, ArbInt     const& r);
                     bool operator <= (ArbInt     const& l, UArbInt    const& r);
                     bool operator <= (UArbInt    const& l, ArbInt     const& r);
  template <class T> bool operator <= (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> bool operator <= (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> bool operator <= (ArbInt     const& l, T          const  r);
  template <class T> bool operator <= (T          const  l, ArbInt     const& r);
                     bool operator == (ArbInt     const& l, ArbInt     const& r);
                     bool operator == (ArbInt     const& l, UArbInt    const& r);
                     bool operator == (UArbInt    const& l, ArbInt     const& r);
  template <class T> bool operator == (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> bool operator == (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> bool operator == (ArbInt     const& l, T          const  r);
  template <class T> bool operator == (T          const  l, ArbInt     const& r);
                     bool operator != (ArbInt     const& l, ArbInt     const& r);
                     bool operator != (ArbInt     const& l, UArbInt    const& r);
                     bool operator != (UArbInt    const& l, ArbInt     const& r);
  template <class T> bool operator != (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> bool operator != (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> bool operator != (ArbInt     const& l, T          const  r);
  template <class T> bool operator != (T          const  l, ArbInt     const& r);
  
  // Bitwise operators.
                     ArbInt operator & (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator & (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator & (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator & (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator & (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator & (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator & (T          const  l, ArbInt     const& r);
                     ArbInt operator | (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator | (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator | (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator | (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator | (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator | (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator | (T          const  l, ArbInt     const& r);
                     ArbInt operator ^ (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator ^ (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator ^ (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator ^ (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator ^ (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator ^ (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator ^ (T          const  l, ArbInt     const& r);
  
  // Arithmetic assignment operators.
                     ArbInt operator *= (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator *= (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator *= (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator *= (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator *= (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator *= (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator *= (T          const  l, ArbInt     const& r);
                     ArbInt operator /= (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator /= (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator /= (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator /= (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator /= (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator /= (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator /= (T          const  l, ArbInt     const& r);
                     ArbInt operator %= (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator %= (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator %= (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator %= (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator %= (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator %= (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator %= (T          const  l, ArbInt     const& r);
                     ArbInt operator += (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator += (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator += (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator += (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator += (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator += (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator += (T          const  l, ArbInt     const& r);
                     ArbInt operator -= (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator -= (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator -= (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator -= (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator -= (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator -= (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator -= (T          const  l, ArbInt     const& r);
  
  // Bit shift assignment operators.
                     ArbInt operator <<= (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator <<= (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator <<= (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator <<= (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator <<= (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator <<= (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator <<= (T          const  l, ArbInt     const& r);
                     ArbInt operator >>= (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator >>= (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator >>= (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator >>= (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator >>= (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator >>= (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator >>= (T          const  l, ArbInt     const& r);
  
  // Bitwise assignment operators.
                     ArbInt operator &= (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator &= (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator &= (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator &= (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator &= (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator &= (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator &= (T          const  l, ArbInt     const& r);
                     ArbInt operator |= (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator |= (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator |= (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator |= (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator |= (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator |= (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator |= (T          const  l, ArbInt     const& r);
                     ArbInt operator ^= (ArbInt     const& l, ArbInt     const& r);
                     ArbInt operator ^= (ArbInt     const& l, UArbInt    const& r);
                     ArbInt operator ^= (UArbInt    const& l, ArbInt     const& r);
  template <class T> ArbInt operator ^= (ArbInt     const& l, SafeInt<T> const  r);
  template <class T> ArbInt operator ^= (SafeInt<T> const  l, ArbInt     const& r);
  template <class T> ArbInt operator ^= (ArbInt     const& l, T          const  r);
  template <class T> ArbInt operator ^= (T          const  l, ArbInt     const& r);
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
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
  inline ArbInt::ArbInt (ArbInt const& number) : _sign(number.sign), _digits(number._digits) {}
  
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
  inline ArbInt ArbInt::operator + () const { return *this;                                                       }
  inline ArbInt ArbInt::operator - () const { ArbInt retval(*this); retval._positive = !_positive; return retval; }
  
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
  inline ArbInt::operator unsigned       () const { return Value<unsigned      >(); }
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
  inline ArbInt ArbInt::Base (value_type const base) {
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
  inline void ArbInt::clear () { _digits.clear(); _positive = true; }
  
  /*
   * Copy another number.
   */
  inline void ArbInt::copy (ArbInt const& number) { _digits.copy(number._digits); _positive = number._positive; }
  
  /*
   * Swap this number with another number.
   */
  inline void ArbInt::swap (ArbInt& number) {
    _digits.swap(number._digits);
    bool tmp(_positive);
    _positive = number._positive;
    number._positive = tmp;
  }
  
  /*
   * Set from another ArbInt.
   */
  inline ArbInt& ArbInt::set (ArbInt const& number) { _digits = number._digits; positive = number._positive; }
  
  /*
   * Set from a UArbInt.
   */
  inline ArbInt& ArbInt::set (UArbInt const& number) { _digits = number; positive = true; }
  
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
                     inline void ArbInt::push_back (ArbInt     const& number) { push_back(ArbInt(number).Base(_base).to_string() ); }
                     inline void ArbInt::push_back (UArbInt    const& number) { push_back(UArbInt(number).Base(_base).to_string()); }
  template <class T> inline void ArbInt::push_back (SafeInt<T> const  number) { push_back(static_cast<T>(number)                 ); }
  template <class T> inline void ArbInt::push_back (T          const  number) { push_back(DAC::to_string(number)                 ); }
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
  inline std::string& to_string (std::string& buffer, value_type const base = 0) const {
    digits.to_string(buffer, base);
    if (!_positive) {
      buffer.insert(0, 1, '-');
    }
    return buffer;
  }
  inline std::string const& to_string (value_type const base = 0) const { return to_string(_strbuf, base); }
  
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
  template <class T> inline ArbInt& ArbInt::op_sub (T          const number                         ) { _Sum<T, _GetNumType<T>::value>::op(*this, number           ); return *this; }
  
  /*
   * Bit shift operator backends.
   */
                     inline ArbInt& ArbInt::op_shl (ArbInt     const& number) { if (number < 0) { op_shr(number._digits); } else { op_shl(number._digits); } return *this; }
                     inline ArbInt& ArbInt::op_shl (UArbInt    const& number) { _shift(number, _DIR_L);                                                      return *this; }
  template <class T> inline ArbInt& ArbInt::op_shl (SafeInt<T> const  number) { _ShL<T, _GetNumType<T>::value>::op(*this, number);                           return *this; }
  template <class T> inline ArbInt& ArbInt::op_shl (T          const  number) { _ShL<T, _GetNumType<T>::value>::op(*this, number);                           return *this; }
                     inline ArbInt& ArbInt::op_shr (ArbInt     const& number) { if (number < 0) { op_shl(number._digits); } else { op_shr(number._digits); } return *this; }
                     inline ArbInt& ArbInt::op_shr (UArbInt    const& number) { _shift(number, _DIR_R);                                                      return *this; }
  template <class T> inline ArbInt& ArbInt::op_shr (SafeInt<T> const  number) { _ShR<T, _GetNumType<T>::value>::op(*this, number);                           return *this; }
  template <class T> inline ArbInt& ArbInt::op_shr (T          const  number) { _ShR<T, _GetNumType<T>::value>::op(*this, number);                           return *this; }
  
  /*
   * Master comparison operator.
   */
  template <class T> inline int ArbInt::op_compare (SafeInt<T> const number)  { return _Compare<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline int ArbInt::op_compare (T          const number)  { return _Compare<T, _GetNumType<T>::value>::op(*this, number); }
  
}

// End include guard.
#endif
