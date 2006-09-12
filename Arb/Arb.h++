/*****************************************************************************
 * Arb.h++
 *****************************************************************************
 * Interface for class Arb.
 *****************************************************************************/

// Include guard.
#if !defined(ARB_43693478di8u7do)
  #define ARB_43693478di8u7do

// STL includes.
  #include <iostream>
  #include <sstream>
  #include <string>
  #include <limits>

// System includes.
  #include <SafeInt.h++>
  #include <to_string.h++>
  #include <ArbInt.h++>
  #include <abs.h++>
  #include <NumInfo.h++>

// Namespace wrapping.
namespace DAC {
  
  /*************************************************************************
   * Arb
   *************************************************************************
   * This is an arbitrary precision number class.
   *************************************************************************/
  class Arb {
    
    /*
     * Public members.
     */
    public:
      
      /*********************************************************************/
      // Typedefs.
      
      // This is the type that controls where the radix point is in a fixed
      // decimal number.
      typedef unsigned int PointPosT;
      
      // Use this type when specifying a base.
      typedef unsigned int BaseT;
      
      /*********************************************************************/
      // Enums
      
      // Format of number when output.
      enum OutputFormat {
        FMT_RADIX,
        FMT_FRACTION,
        FMT_BOTH,
        FMT_DEFAULT
      };
      
      // Rounding method.
      enum RoundMethod {
        ROUND_NORMAL,
        ROUND_EVEN,
        ROUND_UP,
        ROUND_DOWN,
        ROUND_TOWARD_ZERO,
        ROUND_FROM_ZERO,
        ROUND_DEFAULT
      };
      
      // Type of fixed number {
      enum FixType {
        FIX_RADIX,
        FIX_DENOM
      };
      
      /***********************************************************************/
      // Errors.
      class Errors {
        
        public:
    
          // All Arb errors are based off of this.
          class Base : public Exception { public: virtual char const* what () const throw() { return "Undefined error in Arb."; }; };
          
          // Bad format.
          class BadFormat : public Base {
            public:
              virtual char const* what () const throw() {
                try {
                  std::string tmpmsg(std::string(_problem) + " at position " + DAC::to_string(SafeInt<std::string::size_type>(_position) + 1) + ".");
                  return Exception::buffer_message(tmpmsg);
                } catch (...) {
                  return "Bad format. Error creating message string.";
                }
              };
              virtual ~BadFormat () throw() {};
              BadFormat& Problem  (char const*            const problem ) { _problem  = problem ; return *this; };
              BadFormat& Position (std::string::size_type const position) { _position = position; return *this; };
              char const*            Problem  () const { return _problem.c_str(); };
              std::string::size_type Position () const { return _position       ; };
            private:
              std::string            _problem;
              std::string::size_type _position;
          };
          
          // Denominator is out of bounds.
          class OOBDenom : public Base { public: virtual char const* what () const throw() { return "Denominator must be greater than 0."; }; };

          // Divide by zero.
          class DivByZero : public Base { public: virtual char const* what () const throw() { return "Divide by zero."; }; };
          
          // Operation results in a complex number.
          class Complex : public Base { public: virtual char const* what () const throw() { return "Even roots of negative numbers can only be complex numbers."; }; };
          
          // Integer-only operation attempted on a non-integer.
          class NonInteger : public Base { public: virtual char const* what () const throw() { return "An integer operation was attempted on a non-integer number."; }; };
          
          // Attempt to stuff Arb into a type that will not fit.
          class ScalarOverflow : public Base { public: virtual char const* what () const throw() { return "Arb overflows requested scalar type."; }; };
          
          // Attempt to move Arb into a number that won't see it.
          class ScalarUnderflow : public Base { public: virtual char const* what () const throw() { return "Arb underflows requested scalar type."; }; };
          
          // Attempt to set Arb with an invalid floating-point number.
          class InvalidFloat     : public Base         { public: virtual char const* what () const throw() { return "Attempt to set Arb from an invalid floating-point number."; }; };
          class Infinity         : public InvalidFloat { public: virtual char const* what () const throw() { return "Attempt to set Arb to infinity.";                           }; };
          class PositiveInfinity : public Infinity     { public: virtual char const* what () const throw() { return "Attempt to set Arb to positive infinity.";                  }; };
          class NegativeInfinity : public Infinity     { public: virtual char const* what () const throw() { return "Attempt to set Arb to negative infinity.";                  }; };
          class NaN              : public InvalidFloat { public: virtual char const* what () const throw() { return "Attempt to set Arb to NaN.";                                }; };
        
        // Private constructor so this cannot be instantiated.
        private:
          Errors ();
          Errors (Errors const&);
          Errors& operator = (Errors const&);
          
      };
      
      /*********************************************************************/
      // Function members.
      
      // Default constructor.
      Arb ();
      
      // Copy constructor.
      Arb (Arb const& number);
      
      // Conversion constructor.
                         explicit Arb (std::string const& number);
                         explicit Arb (ArbInt      const& number);
                         explicit Arb (UArbInt     const& number);
      template <class T> explicit Arb (T           const  number);
      
      // Increment / decrement operators.
      Arb& operator ++ ();
      Arb  operator ++ (int);
      Arb& operator -- ();
      Arb  operator -- (int);
      
      // Unary sign operators.
      Arb operator + () const;
      Arb operator - () const;
      
      // Casting operators.
      operator bool               () const;
      operator char               () const;
      operator signed   char      () const;
      operator unsigned char      () const;
      operator wchar_t            () const;
      operator signed   short int () const;
      operator unsigned short int () const;
      operator signed   int       () const;
      operator unsigned int       () const;
      operator signed   long int  () const;
      operator unsigned long int  () const;
      operator float              () const;
      operator double             () const;
      operator long double        () const;
      
      // Assignment operator.
                         Arb& operator = (Arb         const& number);
                         Arb& operator = (std::string const& number);
                         Arb& operator = (ArbInt      const& number);
                         Arb& operator = (UArbInt     const& number);
      template <class T> Arb& operator = (T           const  number);
      
      // Accessors.
                         Arb& Base     (BaseT                  const  base);
                         Arb& MaxRadix (std::string::size_type const  maxradix);
                         Arb& PointPos (PointPosT              const  pointpos);
                         Arb& Fixed    (bool                   const  fixed);
                         Arb& Fix      (FixType                const  fix);
                         Arb& Format   (OutputFormat           const  format);
                         Arb& Round    (RoundMethod            const  round);
                         Arb& FixQ     (Arb                    const& fixq);
      template <class T> Arb& Value    (T                      const  number);
                         BaseT                  Base     () const;
                         std::string::size_type MaxRadix () const;
                         PointPosT              PointPos () const;
                         bool                   Fixed    () const;
                         FixType                Fix      () const;
                         OutputFormat           Format   () const;
                         RoundMethod            Round    () const;
                         Arb                    FixQ     () const;
      template <class T> T                      Value    () const;
      
      // Reset to just-constructed defaults.
      void clear ();
      
      // Copy another number.
      void copy (Arb const& number);
      
      // Set the number.
                         void set (std::string const& number, bool const autobase = true);
                         void set (Arb         const& number                            );
                         void set (ArbInt      const& number                            );
                         void set (UArbInt     const& number                            );
      template <class T> void set (SafeInt<T>  const  number                            );
      template <class T> void set (T           const  number                            );
      
      // Convert to string.
      std::string      & to_string (std::string& buffer, OutputFormat const format = FMT_DEFAULT) const;
      std::string const& to_string (                     OutputFormat const format = FMT_DEFAULT) const;
      
      // Arithmetic operator backends.
                         Arb& op_mul (Arb        const& number);
                         Arb& op_mul (ArbInt     const& number);
                         Arb& op_mul (UArbInt    const& number);
      template <class T> Arb& op_mul (SafeInt<T> const  number);
      template <class T> Arb& op_mul (T          const  number);
                         Arb& op_div (Arb        const& number);
                         Arb& op_div (ArbInt     const& number);
                         Arb& op_div (UArbInt    const& number);
      template <class T> Arb& op_div (SafeInt<T> const  number);
      template <class T> Arb& op_div (T          const  number);
                         Arb& op_mod (Arb        const& number);
                         Arb& op_mod (ArbInt     const& number);
                         Arb& op_mod (UArbInt    const& number);
      template <class T> Arb& op_mod (SafeInt<T> const  number);
      template <class T> Arb& op_mod (T          const  number);
                         Arb& op_add (Arb        const& number);
                         Arb& op_add (ArbInt     const& number);
                         Arb& op_add (UArbInt    const& number);
      template <class T> Arb& op_add (SafeInt<T> const  number);
      template <class T> Arb& op_add (T          const  number);
                         Arb& op_sub (Arb        const& number);
                         Arb& op_sub (ArbInt     const& number);
                         Arb& op_sub (UArbInt    const& number);
      template <class T> Arb& op_sub (SafeInt<T> const  number);
      template <class T> Arb& op_sub (T          const  number);
      
      // Bit shift operator backends.
                         Arb& op_shl (Arb        const& number);
                         Arb& op_shl (ArbInt     const& number);
                         Arb& op_shl (UArbInt    const& number);
      template <class T> Arb& op_shl (SafeInt<T> const  number);
      template <class T> Arb& op_shl (T          const  number);
                         Arb& op_shr (Arb        const& number);
                         Arb& op_shr (ArbInt     const& number);
                         Arb& op_shr (UArbInt    const& number);
      template <class T> Arb& op_shr (SafeInt<T> const  number);
      template <class T> Arb& op_shr (T          const  number);
      
      // Master comparison operator.
                         int op_compare (Arb        const& number) const;
                         int op_compare (ArbInt     const& number) const;
                         int op_compare (UArbInt    const& number) const;
      template <class T> int op_compare (SafeInt<T> const  number) const;
      template <class T> int op_compare (T          const  number) const;
      
      // Comparison operator backends.
                         bool op_gt (Arb        const& number) const;
                         bool op_gt (ArbInt     const& number) const;
                         bool op_gt (UArbInt    const& number) const;
      template <class T> bool op_gt (SafeInt<T> const  number) const;
      template <class T> bool op_gt (T          const  number) const;
                         bool op_ge (Arb        const& number) const;
                         bool op_ge (ArbInt     const& number) const;
                         bool op_ge (UArbInt    const& number) const;
      template <class T> bool op_ge (SafeInt<T> const  number) const;
      template <class T> bool op_ge (T          const  number) const;
                         bool op_lt (Arb        const& number) const;
                         bool op_lt (ArbInt     const& number) const;
                         bool op_lt (UArbInt    const& number) const;
      template <class T> bool op_lt (SafeInt<T> const  number) const;
      template <class T> bool op_lt (T          const  number) const;
                         bool op_le (Arb        const& number) const;
                         bool op_le (ArbInt     const& number) const;
                         bool op_le (UArbInt    const& number) const;
      template <class T> bool op_le (SafeInt<T> const  number) const;
      template <class T> bool op_le (T          const  number) const;
                         bool op_eq (Arb        const& number) const;
                         bool op_eq (ArbInt     const& number) const;
                         bool op_eq (UArbInt    const& number) const;
      template <class T> bool op_eq (SafeInt<T> const  number) const;
      template <class T> bool op_eq (T          const  number) const;
                         bool op_ne (Arb        const& number) const;
                         bool op_ne (ArbInt     const& number) const;
                         bool op_ne (UArbInt    const& number) const;
      template <class T> bool op_ne (SafeInt<T> const  number) const;
      template <class T> bool op_ne (T          const  number) const;
      
      // Return information about this number.
      bool isInteger  () const;
      bool isPositive () const;
      bool isNegative () const;
      bool isZero     () const;
      bool isEven     () const;
      bool isOdd      () const;
      
      // Convert to an interger value. Do not confuse with int.
      Arb     ceil      () const;
      Arb     floor     () const;
      Arb     truncate  () const;
      Arb     toInt     () const;
      ArbInt  toArbInt  () const;
      UArbInt toUArbInt () const;
      
      // Raise this number to a power.
                         Arb pow (Arb const& exp) const;
      template <class T> Arb pow (T   const  exp) const;
      
      // Find the nth root of this number.
                         Arb root (Arb const& n) const;
      template <class T> Arb root (T   const  n) const;
      
      // Return the absolute value of this number.
      Arb abs () const;
      
    /*
     * Private members.
     */
    private:
      
      /*********************************************************************/
      // Data types.
      
      // Floating-point bitfields.
      struct _FloatBits {
        unsigned int mantissa : 23;
        unsigned int exponent :  8;
        unsigned int sign     :  1;
      };
      struct _DoubleBits {
        unsigned int mantissal;
        unsigned int mantissah : 20;
        unsigned int exponent  : 11;
        unsigned int sign      :  1;
      };
      struct _LongDoubleBits {
        unsigned int mantissal;
        unsigned int mantissah;
        unsigned int exponent  : 15;
        unsigned int sign      :  1;
      };
      union _FloatParts {
        float      number;
        _FloatBits bits;
      };
      union _DoubleParts {
        double      number;
        _DoubleBits bits;
      };
      union _LongDoubleParts {
        long double     number;
        _LongDoubleBits bits;
      };
      
      // Floating-point info.
      template <class T> class _FloatInfo {
        public:
          static unsigned int const mantissabits;
          static unsigned int const exponentbits;
          static unsigned int const bias;
      };
      
      // Directions.
      enum _Dir { _DIR_L, _DIR_R };
      
      // Set.
      template <class T, NumTypes::Type> class _Set;
      template <class T> class _Set<T, NumTypes::UINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Set<T, NumTypes::SINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Set<T, NumTypes::FLPT> { public:                                              static void op (Arb& l, T const r); };
      
      // Get.
      template <class T, NumTypes::Type> class _Get;
      template <class T> class _Get<T, NumTypes::UINT> { public: static void op (SafeInt<T>& l, Arb const& r); static void op (T& l, Arb const& r); };
      template <class T> class _Get<T, NumTypes::SINT> { public: static void op (SafeInt<T>& l, Arb const& r); static void op (T& l, Arb const& r); };
      template <class T> class _Get<T, NumTypes::FLPT> { public:                                               static void op (T& l, Arb const& r); };
      
      // Set a bitwise copy.
      template <class T, NumTypes::Type> class _SetBitwise;
      template <class T> class _SetBitwise<T, NumTypes::UINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _SetBitwise<T, NumTypes::SINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      
      // Multiply.
      template <class T, NumTypes::Type> class _Mul;
      template <class T> class _Mul<T, NumTypes::UINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Mul<T, NumTypes::SINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Mul<T, NumTypes::FLPT> { public:                                              static void op (Arb& l, T const r); };
      
      // Divide.
      template <class T, NumTypes::Type> class _Div;
      template <class T> class _Div<T, NumTypes::UINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Div<T, NumTypes::SINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Div<T, NumTypes::FLPT> { public:                                              static void op (Arb& l, T const r); };
      
      // Modulo divide.
      template <class T, NumTypes::Type> class _Mod;
      template <class T> class _Mod<T, NumTypes::UINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Mod<T, NumTypes::SINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Mod<T, NumTypes::FLPT> { public:                                              static void op (Arb& l, T const r); };
      
      // Add.
      template <class T, NumTypes::Type> class _Add;
      template <class T> class _Add<T, NumTypes::UINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Add<T, NumTypes::SINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Add<T, NumTypes::FLPT> { public:                                              static void op (Arb& l, T const r); };
      
      // Subtract.
      template <class T, NumTypes::Type> class _Sub;
      template <class T> class _Sub<T, NumTypes::UINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Sub<T, NumTypes::SINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Sub<T, NumTypes::FLPT> { public:                                              static void op (Arb& l, T const r); };
      
      // Shift.
      template <class T, NumTypes::Type> class _Shift;
      template <class T> class _Shift<T, NumTypes::UINT> { public: static void op (Arb& l, SafeInt<T> const r, _Dir const dir); static void op (Arb& l, T const r, _Dir const dir); };
      template <class T> class _Shift<T, NumTypes::SINT> { public: static void op (Arb& l, SafeInt<T> const r, _Dir const dir); static void op (Arb& l, T const r, _Dir const dir); };
      template <class T> class _Shift<T, NumTypes::FLPT> { public:                                                              static void op (Arb& l, T const r, _Dir const dir); };
      
      // Shift left.
      template <class T, NumTypes::Type> class _ShL;
      template <class T> class _ShL<T, NumTypes::UINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _ShL<T, NumTypes::SINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _ShL<T, NumTypes::FLPT> { public:                                              static void op (Arb& l, T const r); };
      
      // Shift right.
      template <class T, NumTypes::Type> class _ShR;
      template <class T> class _ShR<T, NumTypes::UINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _ShR<T, NumTypes::SINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _ShR<T, NumTypes::FLPT> { public:                                              static void op (Arb& l, T const r); };
      
      // Compare.
      template <class T, NumTypes::Type> class _Compare;
      template <class T> class _Compare<T, NumTypes::UINT> { public: static int op (Arb const& l, SafeInt<T> const r); static int op (Arb const& l, T const r); };
      template <class T> class _Compare<T, NumTypes::SINT> { public: static int op (Arb const& l, SafeInt<T> const r); static int op (Arb const& l, T const r); };
      template <class T> class _Compare<T, NumTypes::FLPT> { public:                                                   static int op (Arb const& l, T const r); };
      
      // Greater than.
      template <class T, NumTypes::Type> class _GT;
      template <class T> class _GT<T, NumTypes::UINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _GT<T, NumTypes::SINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _GT<T, NumTypes::FLPT> { public:                                                    static bool op (Arb const& l, T const r); };
      
      // Greater than or equal to.
      template <class T, NumTypes::Type> class _GE;
      template <class T> class _GE<T, NumTypes::UINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _GE<T, NumTypes::SINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _GE<T, NumTypes::FLPT> { public:                                                    static bool op (Arb const& l, T const r); };
      
      // Less than.
      template <class T, NumTypes::Type> class _LT;
      template <class T> class _LT<T, NumTypes::UINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _LT<T, NumTypes::SINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _LT<T, NumTypes::FLPT> { public:                                                    static bool op (Arb const& l, T const r); };
      
      // Less than or equal to.
      template <class T, NumTypes::Type> class _LE;
      template <class T> class _LE<T, NumTypes::UINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _LE<T, NumTypes::SINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _LE<T, NumTypes::FLPT> { public:                                                    static bool op (Arb const& l, T const r); };
      
      // Equal to.
      template <class T, NumTypes::Type> class _EQ;
      template <class T> class _EQ<T, NumTypes::UINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _EQ<T, NumTypes::SINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _EQ<T, NumTypes::FLPT> { public:                                                    static bool op (Arb const& l, T const r); };
      
      // Not equal to.
      template <class T, NumTypes::Type> class _NE;
      template <class T> class _NE<T, NumTypes::UINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _NE<T, NumTypes::SINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _NE<T, NumTypes::FLPT> { public:                                                    static bool op (Arb const& l, T const r); };
      
      /*********************************************************************/
      // Data members.
      
      // The number itself.
      ArbInt  _p; // Numerator.
      UArbInt _q; // Denominator.
      
      // Number properties.
      UArbInt                _fixq    ; // Fixed denominator.
      PointPosT              _pointpos; // Radix point position.
      bool                   _fix     ; // If true, fix the radix point.
      FixType                _fixtype ; // What to fix on, radix points or denominator.
      BaseT                  _base    ; // The base of the number.
      std::string::size_type _maxradix; // Radix digits to output.
      OutputFormat           _format  ; // Format to output this number.
      RoundMethod            _round   ; // Rounding method.
      
      // Temporary buffer for string output.
      mutable std::string _strbuf;
      
      /*********************************************************************/
      // Function members.
      
      // Common initialization tasks.
      void _init ();
      
      // Bit shift this number.
                         Arb& _shift (Arb        const& bits, _Dir const dir);
                         Arb& _shift (ArbInt     const& bits, _Dir const dir);
                         Arb& _shift (UArbInt    const& bits, _Dir const dir);
      template <class T> Arb& _shift (SafeInt<T> const  bits, _Dir const dir);
      template <class T> Arb& _shift (T          const  bits, _Dir const dir);
      
      // Reduce the number to its most compact representation.
      Arb& _reduce      ();
      Arb& _forcereduce (UArbInt const& q);
      
      // Normalize this number to another number.
      Arb& _normalize (Arb& number);
      
      // Convert this number to the range of a floating-point number type,
      // return the exponent of 2 that will convert this number back to the
      // original. I'm not happy with the syntax of this, so that combined
      // with the limited usefulness of this outside converting to a float,
      // which we already provide, it's staying private because it's bad.
      template <class T> UArbInt _toFloat (unsigned int& exponent) const;
      
  };
  
  /*************************************************************************
   * Operators.
   *************************************************************************/
  
  // Stream I/O operators.
  std::ostream&       operator << (std::ostream&       l, Arb  const& r);
  std::ostringstream& operator << (std::ostringstream& l, Arb  const& r);
  std::istream&       operator >> (std::istream&       l, Arb&        r);
  
  // Arithmetic operators.
                     Arb operator * (Arb        const& l, Arb        const& r);
                     Arb operator * (Arb        const& l, ArbInt     const& r);
                     Arb operator * (Arb        const& l, UArbInt    const& r);
                     Arb operator * (ArbInt     const& l, Arb        const& r);
                     Arb operator * (UArbInt    const& l, Arb        const& r);
  template <class T> Arb operator * (Arb        const& l, SafeInt<T> const  r);
  template <class T> Arb operator * (SafeInt<T> const  l, Arb        const& r);
  template <class T> Arb operator * (Arb        const& l, T          const  r);
  template <class T> Arb operator * (T          const  l, Arb        const& r);
                     Arb operator / (Arb        const& l, Arb        const& r);
                     Arb operator / (Arb        const& l, ArbInt     const& r);
                     Arb operator / (Arb        const& l, UArbInt    const& r);
                     Arb operator / (ArbInt     const& l, Arb        const& r);
                     Arb operator / (UArbInt    const& l, Arb        const& r);
  template <class T> Arb operator / (Arb        const& l, SafeInt<T> const  r);
  template <class T> Arb operator / (SafeInt<T> const  l, Arb        const& r);
  template <class T> Arb operator / (Arb        const& l, T          const  r);
  template <class T> Arb operator / (T          const  l, Arb        const& r);
                     Arb operator % (Arb        const& l, Arb        const& r);
                     Arb operator % (Arb        const& l, ArbInt     const& r);
                     Arb operator % (Arb        const& l, UArbInt    const& r);
                     Arb operator % (ArbInt     const& l, Arb        const& r);
                     Arb operator % (UArbInt    const& l, Arb        const& r);
  template <class T> Arb operator % (Arb        const& l, SafeInt<T> const  r);
  template <class T> Arb operator % (SafeInt<T> const  l, Arb        const& r);
  template <class T> Arb operator % (Arb        const& l, T          const  r);
  template <class T> Arb operator % (T          const  l, Arb        const& r);
                     Arb operator + (Arb        const& l, Arb        const& r);
                     Arb operator + (Arb        const& l, ArbInt     const& r);
                     Arb operator + (Arb        const& l, UArbInt    const& r);
                     Arb operator + (ArbInt     const& l, Arb        const& r);
                     Arb operator + (UArbInt    const& l, Arb        const& r);
  template <class T> Arb operator + (Arb        const& l, SafeInt<T> const  r);
  template <class T> Arb operator + (SafeInt<T> const  l, Arb        const& r);
  template <class T> Arb operator + (Arb        const& l, T          const  r);
  template <class T> Arb operator + (T          const  l, Arb        const& r);
                     Arb operator - (Arb        const& l, Arb        const& r);
                     Arb operator - (Arb        const& l, ArbInt     const& r);
                     Arb operator - (Arb        const& l, UArbInt    const& r);
                     Arb operator - (ArbInt     const& l, Arb        const& r);
                     Arb operator - (UArbInt    const& l, Arb        const& r);
  template <class T> Arb operator - (Arb        const& l, SafeInt<T> const  r);
  template <class T> Arb operator - (SafeInt<T> const  l, Arb        const& r);
  template <class T> Arb operator - (Arb        const& l, T          const  r);
  template <class T> Arb operator - (T          const  l, Arb        const& r);
  
  // Bit shift operators.
                     Arb operator << (Arb        const& l, Arb        const& r);
                     Arb operator << (Arb        const& l, ArbInt     const& r);
                     Arb operator << (Arb        const& l, UArbInt    const& r);
                     Arb operator << (ArbInt     const& l, Arb        const& r);
                     Arb operator << (UArbInt    const& l, Arb        const& r);
  template <class T> Arb operator << (Arb        const& l, SafeInt<T> const  r);
  template <class T> Arb operator << (SafeInt<T> const  l, Arb        const& r);
  template <class T> Arb operator << (Arb        const& l, T          const  r);
  template <class T> Arb operator << (T          const  l, Arb        const& r);
                     Arb operator >> (Arb        const& l, Arb        const& r);
                     Arb operator >> (Arb        const& l, ArbInt     const& r);
                     Arb operator >> (Arb        const& l, UArbInt    const& r);
                     Arb operator >> (ArbInt     const& l, Arb        const& r);
                     Arb operator >> (UArbInt    const& l, Arb        const& r);
  template <class T> Arb operator >> (Arb        const& l, SafeInt<T> const  r);
  template <class T> Arb operator >> (SafeInt<T> const  l, Arb        const& r);
  template <class T> Arb operator >> (Arb        const& l, T          const  r);
  template <class T> Arb operator >> (T          const  l, Arb        const& r);
  
  // Comparison operators.
                     bool operator >  (Arb        const& l, Arb        const& r);
                     bool operator >  (Arb        const& l, ArbInt     const& r);
                     bool operator >  (Arb        const& l, UArbInt    const& r);
                     bool operator >  (ArbInt     const& l, Arb        const& r);
                     bool operator >  (UArbInt    const& l, Arb        const& r);
  template <class T> bool operator >  (Arb        const& l, SafeInt<T> const  r);
  template <class T> bool operator >  (SafeInt<T> const  l, Arb        const& r);
  template <class T> bool operator >  (Arb        const& l, T          const  r);
  template <class T> bool operator >  (T          const  l, Arb        const& r);
                     bool operator >= (Arb        const& l, Arb        const& r);
                     bool operator >= (Arb        const& l, ArbInt     const& r);
                     bool operator >= (Arb        const& l, UArbInt    const& r);
                     bool operator >= (ArbInt     const& l, Arb        const& r);
                     bool operator >= (UArbInt    const& l, Arb        const& r);
  template <class T> bool operator >= (Arb        const& l, SafeInt<T> const  r);
  template <class T> bool operator >= (SafeInt<T> const  l, Arb        const& r);
  template <class T> bool operator >= (Arb        const& l, T          const  r);
  template <class T> bool operator >= (T          const  l, Arb        const& r);
                     bool operator <  (Arb        const& l, Arb        const& r);
                     bool operator <  (Arb        const& l, ArbInt     const& r);
                     bool operator <  (Arb        const& l, UArbInt    const& r);
                     bool operator <  (ArbInt     const& l, Arb        const& r);
                     bool operator <  (UArbInt    const& l, Arb        const& r);
  template <class T> bool operator <  (Arb        const& l, SafeInt<T> const  r);
  template <class T> bool operator <  (SafeInt<T> const  l, Arb        const& r);
  template <class T> bool operator <  (Arb        const& l, T          const  r);
  template <class T> bool operator <  (T          const  l, Arb        const& r);
                     bool operator <= (Arb        const& l, Arb        const& r);
                     bool operator <= (Arb        const& l, ArbInt     const& r);
                     bool operator <= (Arb        const& l, UArbInt    const& r);
                     bool operator <= (ArbInt     const& l, Arb        const& r);
                     bool operator <= (UArbInt    const& l, Arb        const& r);
  template <class T> bool operator <= (Arb        const& l, SafeInt<T> const  r);
  template <class T> bool operator <= (SafeInt<T> const  l, Arb        const& r);
  template <class T> bool operator <= (Arb        const& l, T          const  r);
  template <class T> bool operator <= (T          const  l, Arb        const& r);
                     bool operator == (Arb        const& l, Arb        const& r);
                     bool operator == (Arb        const& l, ArbInt     const& r);
                     bool operator == (Arb        const& l, UArbInt    const& r);
                     bool operator == (ArbInt     const& l, Arb        const& r);
                     bool operator == (UArbInt    const& l, Arb        const& r);
  template <class T> bool operator == (Arb        const& l, SafeInt<T> const  r);
  template <class T> bool operator == (SafeInt<T> const  l, Arb        const& r);
  template <class T> bool operator == (Arb        const& l, T          const  r);
  template <class T> bool operator == (T          const  l, Arb        const& r);
                     bool operator != (Arb        const& l, Arb        const& r);
                     bool operator != (Arb        const& l, ArbInt     const& r);
                     bool operator != (Arb        const& l, UArbInt    const& r);
                     bool operator != (ArbInt     const& l, Arb        const& r);
                     bool operator != (UArbInt    const& l, Arb        const& r);
  template <class T> bool operator != (Arb        const& l, SafeInt<T> const  r);
  template <class T> bool operator != (SafeInt<T> const  l, Arb        const& r);
  template <class T> bool operator != (Arb        const& l, T          const  r);
  template <class T> bool operator != (T          const  l, Arb        const& r);
  
  // Arithmetic assignment operators.
                     Arb&        operator *= (Arb&        l, Arb        const& r);
                     Arb&        operator *= (Arb&        l, ArbInt     const& r);
                     Arb&        operator *= (Arb&        l, UArbInt    const& r);
                     ArbInt&     operator *= (ArbInt&     l, Arb        const& r);
                     UArbInt&    operator *= (UArbInt&    l, Arb        const& r);
  template <class T> Arb&        operator *= (Arb&        l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator *= (SafeInt<T>& l, Arb        const& r);
  template <class T> Arb&        operator *= (Arb&        l, T          const  r);
  template <class T> T&          operator *= (T&          l, Arb        const& r);
                     Arb&        operator /= (Arb&        l, Arb        const& r);
                     Arb&        operator /= (Arb&        l, ArbInt     const& r);
                     Arb&        operator /= (Arb&        l, UArbInt    const& r);
                     ArbInt&     operator /= (ArbInt&     l, Arb        const& r);
                     UArbInt&    operator /= (UArbInt&    l, Arb        const& r);
  template <class T> Arb&        operator /= (Arb&        l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator /= (SafeInt<T>& l, Arb        const& r);
  template <class T> Arb&        operator /= (Arb&        l, T          const  r);
  template <class T> T&          operator /= (T&          l, Arb        const& r);
                     Arb&        operator %= (Arb&        l, Arb        const& r);
                     Arb&        operator %= (Arb&        l, ArbInt     const& r);
                     Arb&        operator %= (Arb&        l, UArbInt    const& r);
                     ArbInt&     operator %= (ArbInt&     l, Arb        const& r);
                     UArbInt&    operator %= (UArbInt&    l, Arb        const& r);
  template <class T> Arb&        operator %= (Arb&        l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator %= (SafeInt<T>& l, Arb        const& r);
  template <class T> Arb&        operator %= (Arb&        l, T          const  r);
  template <class T> T&          operator %= (T&          l, Arb        const& r);
                     Arb&        operator += (Arb&        l, Arb        const& r);
                     Arb&        operator += (Arb&        l, ArbInt     const& r);
                     Arb&        operator += (Arb&        l, UArbInt    const& r);
                     ArbInt&     operator += (ArbInt&     l, Arb        const& r);
                     UArbInt&    operator += (UArbInt&    l, Arb        const& r);
  template <class T> Arb&        operator += (Arb&        l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator += (SafeInt<T>& l, Arb        const& r);
  template <class T> Arb&        operator += (Arb&        l, T          const  r);
  template <class T> T&          operator += (T&          l, Arb        const& r);
                     Arb&        operator -= (Arb&        l, Arb        const& r);
                     Arb&        operator -= (Arb&        l, ArbInt     const& r);
                     Arb&        operator -= (Arb&        l, UArbInt    const& r);
                     ArbInt&     operator -= (ArbInt&     l, Arb        const& r);
                     UArbInt&    operator -= (UArbInt&    l, Arb        const& r);
  template <class T> Arb&        operator -= (Arb&        l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator -= (SafeInt<T>& l, Arb        const& r);
  template <class T> Arb&        operator -= (Arb&        l, T          const  r);
  template <class T> T&          operator -= (T&          l, Arb        const& r);
  
  // Bit shift assignment operators.
                     Arb&        operator <<= (Arb&        l, Arb        const& r);
                     Arb&        operator <<= (Arb&        l, ArbInt     const& r);
                     Arb&        operator <<= (Arb&        l, UArbInt    const& r);
                     ArbInt&     operator <<= (ArbInt&     l, Arb        const& r);
                     UArbInt&    operator <<= (UArbInt&    l, Arb        const& r);
  template <class T> Arb&        operator <<= (Arb&        l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator <<= (SafeInt<T>& l, Arb        const& r);
  template <class T> Arb&        operator <<= (Arb&        l, T          const  r);
  template <class T> T&          operator <<= (T&          l, Arb        const& r);
                     Arb&        operator >>= (Arb&        l, Arb        const& r);
                     Arb&        operator >>= (Arb&        l, ArbInt     const& r);
                     Arb&        operator >>= (Arb&        l, UArbInt    const& r);
                     ArbInt&     operator >>= (ArbInt&     l, Arb        const& r);
                     UArbInt&    operator >>= (UArbInt&    l, Arb        const& r);
  template <class T> Arb&        operator >>= (Arb&        l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator >>= (SafeInt<T>& l, Arb        const& r);
  template <class T> Arb&        operator >>= (Arb&        l, T          const  r);
  template <class T> T&          operator >>= (T&          l, Arb        const& r);
  
// Extensions to std::
} namespace std {
  
  DAC::Arb ceil  (DAC::Arb const& x);
  DAC::Arb floor (DAC::Arb const& x);
  
} namespace DAC {
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Conversion constructor.
   */
  template <class T> inline Arb::Arb (T const number) {
    
    // Construct fully.
    _init();
    
    // Set the number.
    set(number);
    
  }
  
  /*
   * Increment / decrement operators.
   */
  inline Arb& Arb::operator ++ ()    { return op_add(1);                            }
  inline Arb  Arb::operator ++ (int) { Arb retval(*this); op_add(1); return retval; }
  inline Arb& Arb::operator -- ()    { return op_sub(1);                            }
  inline Arb  Arb::operator -- (int) { Arb retval(*this); op_sub(1); return retval; }
  
  /*
   * Unary sign operators.
   */
  inline Arb Arb::operator + () const { return *this;                                             }
  inline Arb Arb::operator - () const { Arb retval(*this); retval._p = -retval._p; return retval; }
  
  /*
   * Casting operators.
   */
  inline Arb::operator bool               () const { return !isZero();                   }
  inline Arb::operator char               () const { return Value<char              >(); }
  inline Arb::operator signed   char      () const { return Value<signed   char     >(); }
  inline Arb::operator unsigned char      () const { return Value<unsigned char     >(); }
  inline Arb::operator wchar_t            () const { return Value<wchar_t           >(); }
  inline Arb::operator signed   short int () const { return Value<signed   short int>(); }
  inline Arb::operator unsigned short int () const { return Value<unsigned short int>(); }
  inline Arb::operator signed   int       () const { return Value<signed   int      >(); }
  inline Arb::operator unsigned int       () const { return Value<unsigned int      >(); }
  inline Arb::operator signed   long int  () const { return Value<signed   long int >(); }
  inline Arb::operator unsigned long int  () const { return Value<unsigned long int >(); }
  inline Arb::operator float              () const { return Value<float             >(); }
  inline Arb::operator double             () const { return Value<double            >(); }
  inline Arb::operator long double        () const { return Value<long double       >(); }
  
  /*
   * Assignment operator.
   */
                     inline Arb& Arb::operator = (Arb         const& number) { set(number); return *this; }
                     inline Arb& Arb::operator = (std::string const& number) { set(number); return *this; }
                     inline Arb& Arb::operator = (ArbInt      const& number) { set(number); return *this; }
                     inline Arb& Arb::operator = (UArbInt     const& number) { set(number); return *this; }
  template <class T> inline Arb& Arb::operator = (T           const  number) { set(number); return *this; }
  
  /*
   * The base of this number.
   */
  inline Arb::BaseT Arb::Base () const { return _base; }
  
  /*
   * The maximum number of radix digits to output.
   */
  inline std::string::size_type Arb::MaxRadix (                                     ) const { return _maxradix;                   }
  inline Arb&                   Arb::MaxRadix (std::string::size_type const maxradix)       { _maxradix = maxradix; return *this; }
  
  /*
   * The point position of a fixed-point number.
   */
  inline Arb::PointPosT Arb::PointPos () const { return _pointpos; }
  
  /*
   * Whether this is a fixed-point number or not.
   */
  inline bool Arb::Fixed () const { return _fix; }
  
  /*
   * The fix source for this number.
   */
  inline Arb::FixType Arb::Fix () const { return _fixtype; }
  
  /*
   * The output format of this number.
   */
  inline Arb::OutputFormat Arb::Format (                         ) const { return _format;                 }
  inline Arb&              Arb::Format (OutputFormat const format)       { _format = format; return *this; }
  
  /*
   * The round method of this number.
   */
  inline Arb::RoundMethod Arb::Round (                       ) const { return _round;                }
  inline Arb&             Arb::Round (RoundMethod const round)       { _round = round; return *this; }
  
  /*
   * The fixed quotient of this number.
   */
  inline Arb Arb::FixQ () const { return Arb(_fixq); }
  
  /*
   * The value of this number.
   */
  template <class T> inline T    Arb::Value (              ) const { T retval; _Get<T, NumType<T>::type>::op(retval, *this); return retval; }
  template <class T> inline Arb& Arb::Value (T const number)       { set(number); return *this;                                                  }
  
  /*
   * Set from a built-in type.
   */
  template <class T> inline void Arb::set (SafeInt<T> const number) { _Set<T, NumType<T>::type>::op(*this, number); }
  template <class T> inline void Arb::set (T          const number) { _Set<T, NumType<T>::type>::op(*this, number); }
  
  /*
   * Convert to a string with automatic buffering.
   */
  inline std::string const& Arb::to_string (OutputFormat const format) const { return to_string(_strbuf, format); }
  
  /*
   * Arithmetic operator backends.
   */
  template <class T> inline Arb& Arb::op_mul (SafeInt<T> const number) { _Mul<T, NumType<T>::type>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_mul (T          const number) { _Mul<T, NumType<T>::type>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_div (SafeInt<T> const number) { _Div<T, NumType<T>::type>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_div (T          const number) { _Div<T, NumType<T>::type>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_mod (SafeInt<T> const number) { _Mod<T, NumType<T>::type>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_mod (T          const number) { _Mod<T, NumType<T>::type>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_add (SafeInt<T> const number) { _Add<T, NumType<T>::type>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_add (T          const number) { _Add<T, NumType<T>::type>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_sub (SafeInt<T> const number) { _Sub<T, NumType<T>::type>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_sub (T          const number) { _Sub<T, NumType<T>::type>::op(*this, number); return *this; }
  
  /*
   * Shift left, shift right.
   */
                     inline Arb& Arb::op_shl (Arb        const& number) { _shift(number, _DIR_L);                       return *this; }
                     inline Arb& Arb::op_shl (ArbInt     const& number) { _shift(number, _DIR_L);                       return *this; }
                     inline Arb& Arb::op_shl (UArbInt    const& number) { _shift(number, _DIR_L);                       return *this; }
  template <class T> inline Arb& Arb::op_shl (SafeInt<T> const  number) { _ShL<T, NumType<T>::type>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_shl (T          const  number) { _ShL<T, NumType<T>::type>::op(*this, number); return *this; }
                     inline Arb& Arb::op_shr (Arb        const& number) { _shift(number, _DIR_R);                       return *this; }
                     inline Arb& Arb::op_shr (ArbInt     const& number) { _shift(number, _DIR_R);                       return *this; }
                     inline Arb& Arb::op_shr (UArbInt    const& number) { _shift(number, _DIR_R);                       return *this; }
  template <class T> inline Arb& Arb::op_shr (SafeInt<T> const  number) { _ShR<T, NumType<T>::type>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_shr (T          const  number) { _ShR<T, NumType<T>::type>::op(*this, number); return *this; }
  
  /*
   * Comparison operator backends.
   */
                     inline bool Arb::op_gt (Arb        const& number) const { return op_compare(number) > 0;                      }
                     inline bool Arb::op_gt (ArbInt     const& number) const { return op_compare(number) > 0;                      }
                     inline bool Arb::op_gt (UArbInt    const& number) const { return op_compare(number) > 0;                      }
  template <class T> inline bool Arb::op_gt (SafeInt<T> const  number) const { return _GT<T, NumType<T>::type>::op(*this, number); }
  template <class T> inline bool Arb::op_gt (T          const  number) const { return _GT<T, NumType<T>::type>::op(*this, number); }
                     inline bool Arb::op_ge (Arb        const& number) const { return !op_lt(number);                              }
                     inline bool Arb::op_ge (ArbInt     const& number) const { return !op_lt(number);                              }
                     inline bool Arb::op_ge (UArbInt    const& number) const { return !op_lt(number);                              }
  template <class T> inline bool Arb::op_ge (SafeInt<T> const  number) const { return _GE<T, NumType<T>::type>::op(*this, number); }
  template <class T> inline bool Arb::op_ge (T          const  number) const { return _GE<T, NumType<T>::type>::op(*this, number); }
                     inline bool Arb::op_lt (Arb        const& number) const { return op_compare(number) < 0;                      }
                     inline bool Arb::op_lt (ArbInt     const& number) const { return op_compare(number) < 0;                      }
                     inline bool Arb::op_lt (UArbInt    const& number) const { return op_compare(number) < 0;                      }
  template <class T> inline bool Arb::op_lt (SafeInt<T> const  number) const { return _LT<T, NumType<T>::type>::op(*this, number); }
  template <class T> inline bool Arb::op_lt (T          const  number) const { return _LT<T, NumType<T>::type>::op(*this, number); }
                     inline bool Arb::op_le (Arb        const& number) const { return !op_gt(number);                              }
                     inline bool Arb::op_le (ArbInt     const& number) const { return !op_gt(number);                              }
                     inline bool Arb::op_le (UArbInt    const& number) const { return !op_gt(number);                              }
  template <class T> inline bool Arb::op_le (SafeInt<T> const  number) const { return _LE<T, NumType<T>::type>::op(*this, number); }
  template <class T> inline bool Arb::op_le (T          const  number) const { return _LE<T, NumType<T>::type>::op(*this, number); }
                     inline bool Arb::op_eq (Arb        const& number) const { return op_compare(number) == 0;                     }
                     inline bool Arb::op_eq (ArbInt     const& number) const { return op_compare(number) == 0;                     }
                     inline bool Arb::op_eq (UArbInt    const& number) const { return op_compare(number) == 0;                     }
  template <class T> inline bool Arb::op_eq (SafeInt<T> const  number) const { return _EQ<T, NumType<T>::type>::op(*this, number); }
  template <class T> inline bool Arb::op_eq (T          const  number) const { return _EQ<T, NumType<T>::type>::op(*this, number); }
                     inline bool Arb::op_ne (Arb        const& number) const { return !op_eq(number);                              }
                     inline bool Arb::op_ne (ArbInt     const& number) const { return !op_eq(number);                              }
                     inline bool Arb::op_ne (UArbInt    const& number) const { return !op_eq(number);                              }
  template <class T> inline bool Arb::op_ne (SafeInt<T> const  number) const { return _NE<T, NumType<T>::type>::op(*this, number); }
  template <class T> inline bool Arb::op_ne (T          const  number) const { return _NE<T, NumType<T>::type>::op(*this, number); }
  
  /*
   * Return whether this number is an integer.
   */
  inline bool Arb::isInteger () const { return (_q == 1); }
  
  /*
   * Return whether this number is positive.
   */
  inline bool Arb::isPositive () const { return _p.isPositive() == 0; }
  inline bool Arb::isNegative () const { return !isPositive();        }
  
  /*
   * Return whether this number is equal to zero.
   */
  inline bool Arb::isZero () const { return _p.isZero(); }
  
  /*
   * Return whether this number is even or odd.
   */
  inline bool Arb::isOdd  () const { return (isInteger() && _p.isOdd ()); }
  inline bool Arb::isEven () const { return (isInteger() && _p.isEven()); }
  
  /*
   * Return the integer portion of this number.
   */
  inline Arb Arb::toInt () const { Arb retval(*this); retval._forcereduce(UArbInt(1)); return retval; }
  
  /*
   * Raise this number to a given power.
   */
  template <class T> inline Arb Arb::pow (T const exp) const { return pow(Arb(exp)); }
  
  /*
   * Get a given root of this number.
   */
  template <class T> inline Arb Arb::root (T const n) const { return root(Arb(n)); }
  
  /*
   * Return the absolute value of this number.
   */
  inline Arb Arb::abs () const { Arb retval(*this); retval._p = retval._p.abs(); return retval; }
  
  /*
   * Bit shift this number.
   */
  template <class T> inline Arb& Arb::_shift (SafeInt<T> const bits, _Dir const dir) { _Shift<T, NumType<T>::type>::op(*this, bits, dir); return *this; }
  template <class T> inline Arb& Arb::_shift (T          const bits, _Dir const dir) { _Shift<T, NumType<T>::type>::op(*this, bits, dir); return *this; }
  
  /*
   * Convert number to floating-point range and return exponent.
   */
  template <class T> UArbInt Arb::_toFloat (unsigned int& exponent) const {
    
    // Work area.
    Arb                   tmpnum;
    SafeInt<unsigned int> tmpexp;
    
    // Check for zero.
    if (isZero()) {
      exponent = 0;
      return tmpnum._p;
    }
    
    // Set tmpnum to *this here to avoid carrying over rounding or fixed-point
    // properties, all we are interested in here is the value. Also, only abs.
    tmpnum.set(abs());
    
    // Convert tmpnum to the range of 1 <= tmpnum < 2, save changes in
    // exponent so that tmpnum * 2^exponent == *this.
    UArbInt bits_p(tmpnum._p.bitsInNumber());
    UArbInt bits_q(tmpnum._q.bitsInNumber());
    bool   denorm = false;
    if (bits_p < bits_q) {
      
      // Get the difference in bits.
      UArbInt bitdiff(bits_q - bits_p);
      
      // If bitdiff >= bias, try to store this number as a denormalized
      // number.
      if (bitdiff >= _FloatInfo<T>::bias) {
        
        // See if we can keep the exponent within range and still have bits of
        // result left. If not, throw an exception, caller should deal with
        // underflow.
        if (bitdiff >= _FloatInfo<T>::bias + (_FloatInfo<T>::mantissabits - 1)) {
          throw Arb::Errors::ScalarUnderflow();
        }
        
        // We will have room, flag as denormalized.
        denorm = true;
        
      }
      
      // Shift p up until it has the same number of bits as q, then make sure
      // that 1 <= tmpnum < 2, unless this is a denormalized number. Set
      // exponent according to the number of bits shifted. If this is a
      // denormalized number, shift it the max we can with our max exponent.
      if (denorm) {
        tmpnum._p <<= _FloatInfo<T>::bias - 1;
        tmpexp      = 0;
      } else {
        tmpnum._p <<= bitdiff;
        tmpexp      = static_cast<unsigned int>(_FloatInfo<T>::bias - bitdiff);
        if (tmpnum._q > tmpnum._p) {
          if (tmpexp == 1) {
            --tmpexp;
          } else {
            tmpnum._p <<= 1;
            --tmpexp;
          }
        }
      }
      
    } else {
      
      // Get the difference in bits.
      UArbInt bitdiff(bits_p - bits_q);
      
      // If bitdiff > bias, we won't be able to store this number, it will
      // overflow the exponent.
      if (bitdiff > _FloatInfo<T>::bias) {
        throw Arb::Errors::ScalarOverflow();
      }
      
      // Shift q up until it has the same number of bits as p, then make sure
      // that 1 <= tmpnum < 2. Set exponent according to the number of bits
      // shifted. If q is still > p, shift p up instead of q back down to
      // prevent the rare case of p & q have an equal number of bits to begin
      // with and then losing precision by shifting it down. Should be a very
      // rare occurance.
      tmpnum._q <<= bitdiff;
      tmpexp      = static_cast<unsigned int>(_FloatInfo<T>::bias + bitdiff);
      if (tmpnum._q > tmpnum._p) {
        tmpnum._p <<= 1;
        --tmpexp;
      }
      
    }
    
    // Convert tmpnum to x/bits in mantissa.
    tmpnum._forcereduce(UArbInt(1) << _FloatInfo<T>::mantissabits - 1);
    
    // We done. Set the exponent and return the fraction.
    exponent = tmpexp;
    return tmpnum._p;
    
  }
  
  /*
   * Set from an integer type.
   */
  template <class T> void Arb::_Set<T, NumTypes::UINT>::op (Arb& l, SafeInt<T> const r) {
    
    // This number is a whole number.
    l._p = r;
    l._q = 1;
    
    // Reduce the fraction.
    l._reduce();
    
  }
  template <class T> inline void Arb::_Set<T, NumTypes::UINT>::op (Arb& l, T          const r) { _Set<T, NumTypes::UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline void Arb::_Set<T, NumTypes::SINT>::op (Arb& l, SafeInt<T> const r) { _Set<T, NumTypes::UINT>::op(l, r            ); }
  template <class T> inline void Arb::_Set<T, NumTypes::SINT>::op (Arb& l, T          const r) { _Set<T, NumTypes::SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Get as an integer.
   */
  template <class T> void Arb::_Get<T, NumTypes::UINT>::op (SafeInt<T>& l, Arb const& r) {
    
    // Make a temporary copy of r reduced to an integer.
    Arb tmp = r.toInt();
    
    // Catch and convert any errors.
    try {
      l = static_cast<T>(tmp._p);
      
    // Only error that should occur.
    } catch (ArbInt::Errors::ScalarOverflow&) {
      throw Errors::ScalarOverflow();
    }
    
  }
  template <class T> inline void Arb::_Get<T, NumTypes::UINT>::op (T&          l, Arb const& r) { SafeInt<T> tmp; _Get<T, NumTypes::UINT>::op(tmp, r); l = tmp; }
  template <class T> inline void Arb::_Get<T, NumTypes::SINT>::op (SafeInt<T>& l, Arb const& r) {                 _Get<T, NumTypes::UINT>::op(l  , r);          }
  template <class T> inline void Arb::_Get<T, NumTypes::SINT>::op (T&          l, Arb const& r) { SafeInt<T> tmp; _Get<T, NumTypes::SINT>::op(tmp, r); l = tmp; }
  
  /*
   * Multiply by an integer type.
   */
  template <class T> void Arb::_Mul<T, NumTypes::UINT>::op (Arb& l, SafeInt<T> const r) {
    
    // Multiplying 0 is easy.
    if (l == 0) {
      return;
    }
    
    // Multiplying by 1 is also easy.
    if (r == 1) {
      return;
    }
    
    // Multiply.
    l._p *= r;
    
    // Reduce.
    l._reduce();
    
  }
  template <class T> inline void Arb::_Mul<T, NumTypes::UINT>::op (Arb& l, T          const r) { _Mul<T, NumTypes::UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline void Arb::_Mul<T, NumTypes::SINT>::op (Arb& l, SafeInt<T> const r) { _Mul<T, NumTypes::UINT>::op(l, r            ); }
  template <class T> inline void Arb::_Mul<T, NumTypes::SINT>::op (Arb& l, T          const r) { _Mul<T, NumTypes::SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Multiply by a floating-point type.
   */
  template <class T> inline void Arb::_Mul<T, NumTypes::FLPT>::op (Arb& l, T const r) { l.op_mul(Arb(r)); }
  
  /*
   * Divide by an integer type.
   */
  template <class T> void Arb::_Div<T, NumTypes::UINT>::op (Arb& l, SafeInt<T> const r) {
    
    // Dividing 0 is easy.
    if (l == 0) {
      return;
    }
    
    // Dividing by 0 is verboten.
    if (r == 0) {
      throw Arb::Errors::DivByZero();
    }
    
    // Dividing by 1 is also easy.
    if (r == 1) {
      return;
    }
    
    // Divide.
    l._q *= r;
    
    // Reduce.
    l._reduce();
    
  }
  template <class T> inline void Arb::_Div<T, NumTypes::UINT>::op (Arb& l, T          const r) { _Div<T, NumTypes::UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline void Arb::_Div<T, NumTypes::SINT>::op (Arb& l, SafeInt<T> const r) { _Div<T, NumTypes::UINT>::op(l, r            ); }
  template <class T> inline void Arb::_Div<T, NumTypes::SINT>::op (Arb& l, T          const r) { _Div<T, NumTypes::SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Divide by a floating-point type.
   */
  template <class T> inline void Arb::_Div<T, NumTypes::FLPT>::op (Arb& l, T const r) { l.op_div(Arb(r)); }
  
  /*
   * Modulo divide by an integer type.
   */
  template <class T> void Arb::_Mod<T, NumTypes::UINT>::op (Arb& l, SafeInt<T> const r) {
    
    // Dividing 0 is easy.
    if (l == 0) {
      return;
    }
    
    // Dividing by 0 is verboten.
    if (r == 0) {
      throw Arb::Errors::DivByZero();
    }
    
    // Throw an error if both numbers are not integer.
    if (!l.isInteger()) {
      throw Errors::NonInteger();
    }
    
    // Modulo divide p.
    l._p %= r;
    
  }
  template <class T> inline void Arb::_Mod<T, NumTypes::UINT>::op (Arb& l, T          const r) { _Mod<T, NumTypes::UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline void Arb::_Mod<T, NumTypes::SINT>::op (Arb& l, SafeInt<T> const r) { _Mod<T, NumTypes::UINT>::op(l, r            ); }
  template <class T> inline void Arb::_Mod<T, NumTypes::SINT>::op (Arb& l, T          const r) { _Mod<T, NumTypes::SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Modulo divide by a floating-point type.
   */
  template <class T> inline void Arb::_Mod<T, NumTypes::FLPT>::op (Arb& l, T const r) { l.op_mod(Arb(r)); }
  
  /*
   * Add an integer type.
   */
  template <class T> void Arb::_Add<T, NumTypes::UINT>::op (Arb& l, SafeInt<T> const r) {
    
    // Adding to 0 is easy.
    if (l == 0) {
      l = r;
      return;
    }
    
    // Adding 0 is also easy.
    if (r == 0) {
      return;
    }
    
    // Add the easy way if l is an integer, otherwise scale.
    if (l.isInteger()) {
      l._p += r;
    } else {
      l._p += r * l._q;
      l._reduce();
    }
    
  }
  template <class T> inline void Arb::_Add<T, NumTypes::UINT>::op (Arb& l, T          const r) { _Add<T, NumTypes::UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline void Arb::_Add<T, NumTypes::SINT>::op (Arb& l, SafeInt<T> const r) { _Add<T, NumTypes::UINT>::op(l, r            ); }
  template <class T> inline void Arb::_Add<T, NumTypes::SINT>::op (Arb& l, T          const r) { _Add<T, NumTypes::SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Add a floating-point type.
   */
  template <class T> inline void Arb::_Add<T, NumTypes::FLPT>::op (Arb& l, T const r) { l.op_add(Arb(r)); }
  
  /*
   * Subtract an integer type.
   */
  template <class T> void Arb::_Sub<T, NumTypes::UINT>::op (Arb& l, SafeInt<T> const r) {
    
    // Subtracting 0 is easy.
    if (r == 0) {
      return;
    }
    
    // Subtracting from 0 is also easy.
    if (l == 0) {
      l = -r;
      return;
    }
    
    // Subtract the easy way if l is an integer, otherwise scale.
    if (l.isInteger()) {
      l._p -= r;
    } else {
      l._p -= r * l._q;
      l._reduce();
    }
    
  }
  template <class T> inline void Arb::_Sub<T, NumTypes::UINT>::op (Arb& l, T          const r) { _Sub<T, NumTypes::UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline void Arb::_Sub<T, NumTypes::SINT>::op (Arb& l, SafeInt<T> const r) { _Sub<T, NumTypes::UINT>::op(l, r            ); }
  template <class T> inline void Arb::_Sub<T, NumTypes::SINT>::op (Arb& l, T          const r) { _Sub<T, NumTypes::SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Subtract a floating-point type.
   */
  template <class T> inline void Arb::_Sub<T, NumTypes::FLPT>::op (Arb& l, T const r) { l.op_sub(Arb(r)); }
  
  /*
   * Shift by an unsigned integer type.
   */
  template <class T> void Arb::_Shift<T, NumTypes::UINT>::op (Arb& l, SafeInt<T> const r, _Dir const dir) {
    
    // Only work if necessary.
    if (!l || !r) {
      return;
    }
    
    // Shift left by left-shifting p. Shift right by left-shifting q.
    if (dir == _DIR_L) {
      l._p <<= r;
    } else {
      l._q <<= r;
    }
    
    // Reduce the fraction and we're done.
    l._reduce();
    
  }
  template <class T> inline void Arb::_Shift<T, NumTypes::UINT>::op (Arb& l, T          const r, _Dir const dir) { _Shift<T, NumTypes::UINT>::op(l, SafeInt<T>(r), dir); }
  template <class T> inline void Arb::_Shift<T, NumTypes::SINT>::op (Arb& l, SafeInt<T> const r, _Dir const dir) { _Shift<T, NumTypes::UINT>::op(l, r            , dir); }
  template <class T> inline void Arb::_Shift<T, NumTypes::SINT>::op (Arb& l, T          const r, _Dir const dir) { _Shift<T, NumTypes::SINT>::op(l, SafeInt<T>(r), dir); }
  
  /*
   * Shift by a floating-point type.
   */
  template <class T> inline void Arb::_Shift<T, NumTypes::FLPT>::op (Arb& l, T const r, _Dir const dir) { l._shift(Arb(r), dir); }
  
  /*
   * Shift left.
   */
  template <class T> inline void Arb::_ShL<T, NumTypes::UINT>::op (Arb& l, SafeInt<T> const r) { Arb::_Shift<T, NumTypes::UINT>::op(l, r, _DIR_L); }
  template <class T> inline void Arb::_ShL<T, NumTypes::UINT>::op (Arb& l, T          const r) { Arb::_Shift<T, NumTypes::UINT>::op(l, r, _DIR_L); }
  template <class T> inline void Arb::_ShL<T, NumTypes::SINT>::op (Arb& l, SafeInt<T> const r) { Arb::_Shift<T, NumTypes::SINT>::op(l, r, _DIR_L); }
  template <class T> inline void Arb::_ShL<T, NumTypes::SINT>::op (Arb& l, T          const r) { Arb::_Shift<T, NumTypes::SINT>::op(l, r, _DIR_L); }
  template <class T> inline void Arb::_ShL<T, NumTypes::FLPT>::op (Arb& l, T          const r) { Arb::_Shift<T, NumTypes::FLPT>::op(l, r, _DIR_L); }
  
  /*
   * Shift right.
   */
  template <class T> inline void Arb::_ShR<T, NumTypes::UINT>::op (Arb& l, SafeInt<T> const r) { Arb::_Shift<T, NumTypes::UINT>::op(l, r, _DIR_R); }
  template <class T> inline void Arb::_ShR<T, NumTypes::UINT>::op (Arb& l, T          const r) { Arb::_Shift<T, NumTypes::UINT>::op(l, r, _DIR_R); }
  template <class T> inline void Arb::_ShR<T, NumTypes::SINT>::op (Arb& l, SafeInt<T> const r) { Arb::_Shift<T, NumTypes::SINT>::op(l, r, _DIR_R); }
  template <class T> inline void Arb::_ShR<T, NumTypes::SINT>::op (Arb& l, T          const r) { Arb::_Shift<T, NumTypes::SINT>::op(l, r, _DIR_R); }
  template <class T> inline void Arb::_ShR<T, NumTypes::FLPT>::op (Arb& l, T          const r) { Arb::_Shift<T, NumTypes::FLPT>::op(l, r, _DIR_R); }
  
  /*
   * Compare to an integer type.
   */
  template <class T> int Arb::_Compare<T, NumTypes::UINT>::op (Arb const& l, SafeInt<T> const r) {
    
    // Compare the easy way if possible, otherwise scale.
    if (l.isInteger()) {
      return l._p.op_compare(r);
    } else {
      return l._p.op_compare(r * l._q);
    }
    
  }
  template <class T> inline int Arb::_Compare<T, NumTypes::UINT>::op (Arb const& l, T          const r) { return _Compare<T, NumTypes::UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline int Arb::_Compare<T, NumTypes::SINT>::op (Arb const& l, SafeInt<T> const r) { return _Compare<T, NumTypes::UINT>::op(l, r            ); }
  template <class T> inline int Arb::_Compare<T, NumTypes::SINT>::op (Arb const& l, T          const r) { return _Compare<T, NumTypes::SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Compare to a floating-point type.
   */
  template <class T> inline int Arb::_Compare<T, NumTypes::FLPT>::op (Arb const& l, T const r) { return l.op_compare(Arb(r)); }

  /*
   * Greater than an integer type.
   */
  template <class T> inline bool Arb::_GT<T, NumTypes::UINT>::op (Arb const& l, SafeInt<T> const r) { return l.op_compare(r) > 0;                          }
  template <class T> inline bool Arb::_GT<T, NumTypes::UINT>::op (Arb const& l, T          const r) { return _GT<T, NumTypes::UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline bool Arb::_GT<T, NumTypes::SINT>::op (Arb const& l, SafeInt<T> const r) { return _GT<T, NumTypes::UINT>::op(l, r            ); }
  template <class T> inline bool Arb::_GT<T, NumTypes::SINT>::op (Arb const& l, T          const r) { return _GT<T, NumTypes::SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Greater than a floating-point type.
   */
  template <class T> inline bool Arb::_GT<T, NumTypes::FLPT>::op (Arb const& l, T const r) { return l.op_gt(Arb(r)); }
  
  /*
   * Greater than or equal to an integer type.
   */
  template <class T> inline bool Arb::_GE<T, NumTypes::UINT>::op (Arb const& l, SafeInt<T> const r) { return !l.op_lt(r);                                  }
  template <class T> inline bool Arb::_GE<T, NumTypes::UINT>::op (Arb const& l, T          const r) { return _GE<T, NumTypes::UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline bool Arb::_GE<T, NumTypes::SINT>::op (Arb const& l, SafeInt<T> const r) { return _GE<T, NumTypes::UINT>::op(l, r            ); }
  template <class T> inline bool Arb::_GE<T, NumTypes::SINT>::op (Arb const& l, T          const r) { return _GE<T, NumTypes::SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Greater than or equal to a floating-point type.
   */
  template <class T> inline bool Arb::_GE<T, NumTypes::FLPT>::op (Arb const& l, T const r) { return l.op_ge(Arb(r)); }
  
  /*
   * Less than an integer type.
   */
  template <class T> inline bool Arb::_LT<T, NumTypes::UINT>::op (Arb const& l, SafeInt<T> const r) { return l.op_compare(r) < 0;                          }
  template <class T> inline bool Arb::_LT<T, NumTypes::UINT>::op (Arb const& l, T          const r) { return _LT<T, NumTypes::UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline bool Arb::_LT<T, NumTypes::SINT>::op (Arb const& l, SafeInt<T> const r) { return _LT<T, NumTypes::UINT>::op(l, r            ); }
  template <class T> inline bool Arb::_LT<T, NumTypes::SINT>::op (Arb const& l, T          const r) { return _LT<T, NumTypes::SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Greater than a floating-point type.
   */
  template <class T> inline bool Arb::_LT<T, NumTypes::FLPT>::op (Arb const& l, T const r) { return l.op_lt(Arb(r)); }
  
  /*
   * Less than or equal to an integer type.
   */
  template <class T> inline bool Arb::_LE<T, NumTypes::UINT>::op (Arb const& l, SafeInt<T> const r) { return !l.op_gt(r);                                  }
  template <class T> inline bool Arb::_LE<T, NumTypes::UINT>::op (Arb const& l, T          const r) { return _LE<T, NumTypes::UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline bool Arb::_LE<T, NumTypes::SINT>::op (Arb const& l, SafeInt<T> const r) { return _LE<T, NumTypes::UINT>::op(l, r            ); }
  template <class T> inline bool Arb::_LE<T, NumTypes::SINT>::op (Arb const& l, T          const r) { return _LE<T, NumTypes::SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Less than or equal to a floating-point type.
   */
  template <class T> inline bool Arb::_LE<T, NumTypes::FLPT>::op (Arb const& l, T const r) { return l.op_le(Arb(r)); }
  
  /*
   * Equal to an unsigned integer type.
   */
  template <class T> inline bool Arb::_EQ<T, NumTypes::UINT>::op (Arb const& l, SafeInt<T> const r) { return l.op_compare(r) == 0;                         }
  template <class T> inline bool Arb::_EQ<T, NumTypes::UINT>::op (Arb const& l, T          const r) { return _EQ<T, NumTypes::UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline bool Arb::_EQ<T, NumTypes::SINT>::op (Arb const& l, SafeInt<T> const r) { return _EQ<T, NumTypes::UINT>::op(l, r            ); }
  template <class T> inline bool Arb::_EQ<T, NumTypes::SINT>::op (Arb const& l, T          const r) { return _EQ<T, NumTypes::SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Equal to a floating-point type.
   */
  template <class T> inline bool Arb::_EQ<T, NumTypes::FLPT>::op (Arb const& l, T const r) { return l.op_eq(Arb(r)); }
  
  /*
   * Not equal to an unsigned integer type.
   */
  template <class T> inline bool Arb::_NE<T, NumTypes::UINT>::op (Arb const& l, SafeInt<T> const r) { return !l.op_eq(r);                                  }
  template <class T> inline bool Arb::_NE<T, NumTypes::UINT>::op (Arb const& l, T          const r) { return _NE<T, NumTypes::UINT>::op(l, SafeInt<T>(r)); }
  template <class T> inline bool Arb::_NE<T, NumTypes::SINT>::op (Arb const& l, SafeInt<T> const r) { return _NE<T, NumTypes::UINT>::op(l, r            ); }
  template <class T> inline bool Arb::_NE<T, NumTypes::SINT>::op (Arb const& l, T          const r) { return _NE<T, NumTypes::SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Not equal to a floating-point type.
   */
  template <class T> inline bool Arb::_NE<T, NumTypes::FLPT>::op (Arb const& l, T const r) { return l.op_ne(Arb(r)); }
  
  /***************************************************************************
   * Operators.
   ***************************************************************************/
  
  /*
   * Stream I/O operators.
   */
  inline std::ostream&       operator << (std::ostream&       l, Arb  const& r) { l << r.to_string();                                 return l; }
  inline std::ostringstream& operator << (std::ostringstream& l, Arb  const& r) { l << r.to_string();                                 return l; }
  inline std::istream&       operator >> (std::istream&       l, Arb&        r) { std::string input; std::cin >> input; r.set(input); return l; }
  
  /*
   * Arithmetic operators.
   */
                     inline Arb operator * (Arb        const& l, Arb        const& r) { return Arb(l).op_mul(r);    }
                     inline Arb operator * (Arb        const& l, ArbInt     const& r) { return Arb(l).op_mul(r);    }
                     inline Arb operator * (Arb        const& l, UArbInt    const& r) { return Arb(l).op_mul(r);    }
                     inline Arb operator * (ArbInt     const& l, Arb        const& r) { return Arb(r).op_mul(l);    }
                     inline Arb operator * (UArbInt    const& l, Arb        const& r) { return Arb(r).op_mul(l);    }
  template <class T> inline Arb operator * (Arb        const& l, SafeInt<T> const  r) { return Arb(l).op_mul(r);    }
  template <class T> inline Arb operator * (SafeInt<T> const  l, Arb        const& r) { return Arb(r).op_mul(l);    }
  template <class T> inline Arb operator * (Arb        const& l, T          const  r) { return Arb(l).op_mul(r);    }
  template <class T> inline Arb operator * (T          const  l, Arb        const& r) { return Arb(r).op_mul(l);    }
                     inline Arb operator / (Arb        const& l, Arb        const& r) { return Arb(l).op_div(r);    }
                     inline Arb operator / (Arb        const& l, ArbInt     const& r) { return Arb(l).op_div(r);    }
                     inline Arb operator / (Arb        const& l, UArbInt    const& r) { return Arb(l).op_div(r);    }
                     inline Arb operator / (ArbInt     const& l, Arb        const& r) { return Arb(l).op_div(r);    }
                     inline Arb operator / (UArbInt    const& l, Arb        const& r) { return Arb(l).op_div(r);    }
  template <class T> inline Arb operator / (Arb        const& l, SafeInt<T> const  r) { return Arb(l).op_div(r);    }
  template <class T> inline Arb operator / (SafeInt<T> const  l, Arb        const& r) { return Arb(l).op_div(r);    }
  template <class T> inline Arb operator / (Arb        const& l, T          const  r) { return Arb(l).op_div(r);    }
  template <class T> inline Arb operator / (T          const  l, Arb        const& r) { return Arb(l).op_div(r);    }
                     inline Arb operator % (Arb        const& l, Arb        const& r) { return Arb(l).op_mod(r);    }
                     inline Arb operator % (Arb        const& l, ArbInt     const& r) { return Arb(l).op_mod(r);    }
                     inline Arb operator % (Arb        const& l, UArbInt    const& r) { return Arb(l).op_mod(r);    }
                     inline Arb operator % (ArbInt     const& l, Arb        const& r) { return Arb(l).op_mod(r);    }
                     inline Arb operator % (UArbInt    const& l, Arb        const& r) { return Arb(l).op_mod(r);    }
  template <class T> inline Arb operator % (Arb        const& l, SafeInt<T> const  r) { return Arb(l).op_mod(r);    }
  template <class T> inline Arb operator % (SafeInt<T> const  l, Arb        const& r) { return Arb(l).op_mod(r);    }
  template <class T> inline Arb operator % (Arb        const& l, T          const  r) { return Arb(l).op_mod(r);    }
  template <class T> inline Arb operator % (T          const  l, Arb        const& r) { return Arb(l).op_mod(r);    }
                     inline Arb operator + (Arb        const& l, Arb        const& r) { return Arb(l).op_add(r);    }
                     inline Arb operator + (Arb        const& l, ArbInt     const& r) { return Arb(l).op_add(r);    }
                     inline Arb operator + (Arb        const& l, UArbInt    const& r) { return Arb(l).op_add(r);    }
                     inline Arb operator + (ArbInt     const& l, Arb        const& r) { return Arb(r).op_add(l);    }
                     inline Arb operator + (UArbInt    const& l, Arb        const& r) { return Arb(r).op_add(l);    }
  template <class T> inline Arb operator + (Arb        const& l, SafeInt<T> const  r) { return Arb(l).op_add(r);    }
  template <class T> inline Arb operator + (SafeInt<T> const  l, Arb        const& r) { return Arb(r).op_add(l);    }
  template <class T> inline Arb operator + (Arb        const& l, T          const  r) { return Arb(l).op_add(r);    }
  template <class T> inline Arb operator + (T          const  l, Arb        const& r) { return Arb(r).op_add(l);    }
                     inline Arb operator - (Arb        const& l, Arb        const& r) { return Arb(l).op_sub(r);    }
                     inline Arb operator - (Arb        const& l, ArbInt     const& r) { return Arb(l).op_sub(r);    }
                     inline Arb operator - (Arb        const& l, UArbInt    const& r) { return Arb(l).op_sub(r);    }
                     inline Arb operator - (ArbInt     const& l, Arb        const& r) { return -(Arb(r).op_sub(l)); }
                     inline Arb operator - (UArbInt    const& l, Arb        const& r) { return -(Arb(r).op_sub(l)); }
  template <class T> inline Arb operator - (Arb        const& l, SafeInt<T> const  r) { return Arb(l).op_sub(r);    }
  template <class T> inline Arb operator - (SafeInt<T> const  l, Arb        const& r) { return -(Arb(r).op_sub(l)); }
  template <class T> inline Arb operator - (Arb        const& l, T          const  r) { return Arb(l).op_sub(r);    }
  template <class T> inline Arb operator - (T          const  l, Arb        const& r) { return -(Arb(r).op_sub(l)); }
  
  /*
   * Bit shift operators.
   */
                     inline Arb operator << (Arb        const& l, Arb        const& r) { return Arb(l).op_shl(r); }
                     inline Arb operator << (Arb        const& l, ArbInt     const& r) { return Arb(l).op_shl(r); }
                     inline Arb operator << (Arb        const& l, UArbInt    const& r) { return Arb(l).op_shl(r); }
                     inline Arb operator << (ArbInt     const& l, Arb        const& r) { return Arb(l).op_shl(r); }
                     inline Arb operator << (UArbInt    const& l, Arb        const& r) { return Arb(l).op_shl(r); }
  template <class T> inline Arb operator << (Arb        const& l, SafeInt<T> const  r) { return Arb(l).op_shl(r); }
  template <class T> inline Arb operator << (SafeInt<T> const  l, Arb        const& r) { return Arb(l).op_shl(r); }
  template <class T> inline Arb operator << (Arb        const& l, T          const  r) { return Arb(l).op_shl(r); }
  template <class T> inline Arb operator << (T          const  l, Arb        const& r) { return Arb(l).op_shl(r); }
                     inline Arb operator >> (Arb        const& l, Arb        const& r) { return Arb(l).op_shr(r); }
                     inline Arb operator >> (Arb        const& l, ArbInt     const& r) { return Arb(l).op_shr(r); }
                     inline Arb operator >> (Arb        const& l, UArbInt    const& r) { return Arb(l).op_shr(r); }
                     inline Arb operator >> (ArbInt     const& l, Arb        const& r) { return Arb(l).op_shr(r); }
                     inline Arb operator >> (UArbInt    const& l, Arb        const& r) { return Arb(l).op_shr(r); }
  template <class T> inline Arb operator >> (Arb        const& l, SafeInt<T> const  r) { return Arb(l).op_shr(r); }
  template <class T> inline Arb operator >> (SafeInt<T> const  l, Arb        const& r) { return Arb(l).op_shr(r); }
  template <class T> inline Arb operator >> (Arb        const& l, T          const  r) { return Arb(l).op_shr(r); }
  template <class T> inline Arb operator >> (T          const  l, Arb        const& r) { return Arb(l).op_shr(r); }
  
  /*
   * Comparison operators.
   */
                     inline bool operator >  (Arb        const& l, Arb        const& r) { return  l.op_gt(r); }
                     inline bool operator >  (Arb        const& l, ArbInt     const& r) { return  l.op_gt(r); }
                     inline bool operator >  (Arb        const& l, UArbInt    const& r) { return  l.op_gt(r); }
                     inline bool operator >  (ArbInt     const& l, Arb        const& r) { return !r.op_ge(l); }
                     inline bool operator >  (UArbInt    const& l, Arb        const& r) { return !r.op_ge(l); }
  template <class T> inline bool operator >  (Arb        const& l, SafeInt<T> const  r) { return  l.op_gt(r); }
  template <class T> inline bool operator >  (SafeInt<T> const  l, Arb        const& r) { return !r.op_ge(l); }
  template <class T> inline bool operator >  (Arb        const& l, T          const  r) { return  l.op_gt(r); }
  template <class T> inline bool operator >  (T          const  l, Arb        const& r) { return !r.op_ge(l); }
                     inline bool operator >= (Arb        const& l, Arb        const& r) { return  l.op_ge(r); }
                     inline bool operator >= (Arb        const& l, ArbInt     const& r) { return  l.op_ge(r); }
                     inline bool operator >= (Arb        const& l, UArbInt    const& r) { return  l.op_ge(r); }
                     inline bool operator >= (ArbInt     const& l, Arb        const& r) { return !r.op_gt(l); }
                     inline bool operator >= (UArbInt    const& l, Arb        const& r) { return !r.op_gt(l); }
  template <class T> inline bool operator >= (Arb        const& l, SafeInt<T> const  r) { return  l.op_ge(r); }
  template <class T> inline bool operator >= (SafeInt<T> const  l, Arb        const& r) { return !r.op_gt(l); }
  template <class T> inline bool operator >= (Arb        const& l, T          const  r) { return  l.op_ge(r); }
  template <class T> inline bool operator >= (T          const  l, Arb        const& r) { return !r.op_gt(l); }
                     inline bool operator <  (Arb        const& l, Arb        const& r) { return  l.op_lt(r); }
                     inline bool operator <  (Arb        const& l, ArbInt     const& r) { return  l.op_lt(r); }
                     inline bool operator <  (Arb        const& l, UArbInt    const& r) { return  l.op_lt(r); }
                     inline bool operator <  (ArbInt     const& l, Arb        const& r) { return !r.op_le(l); }
                     inline bool operator <  (UArbInt    const& l, Arb        const& r) { return !r.op_le(l); }
  template <class T> inline bool operator <  (Arb        const& l, SafeInt<T> const  r) { return  l.op_lt(r); }
  template <class T> inline bool operator <  (SafeInt<T> const  l, Arb        const& r) { return !r.op_le(l); }
  template <class T> inline bool operator <  (Arb        const& l, T          const  r) { return  l.op_lt(r); }
  template <class T> inline bool operator <  (T          const  l, Arb        const& r) { return !r.op_le(l); }
                     inline bool operator <= (Arb        const& l, Arb        const& r) { return  l.op_le(r); }
                     inline bool operator <= (Arb        const& l, ArbInt     const& r) { return  l.op_le(r); }
                     inline bool operator <= (Arb        const& l, UArbInt    const& r) { return  l.op_le(r); }
                     inline bool operator <= (ArbInt     const& l, Arb        const& r) { return !r.op_lt(l); }
                     inline bool operator <= (UArbInt    const& l, Arb        const& r) { return !r.op_lt(l); }
  template <class T> inline bool operator <= (Arb        const& l, SafeInt<T> const  r) { return  l.op_le(r); }
  template <class T> inline bool operator <= (SafeInt<T> const  l, Arb        const& r) { return !r.op_lt(l); }
  template <class T> inline bool operator <= (Arb        const& l, T          const  r) { return  l.op_le(r); }
  template <class T> inline bool operator <= (T          const  l, Arb        const& r) { return !r.op_lt(l); }
                     inline bool operator == (Arb        const& l, Arb        const& r) { return  l.op_eq(r); }
                     inline bool operator == (Arb        const& l, ArbInt     const& r) { return  l.op_eq(r); }
                     inline bool operator == (Arb        const& l, UArbInt    const& r) { return  l.op_eq(r); }
                     inline bool operator == (ArbInt     const& l, Arb        const& r) { return  r.op_eq(l); }
                     inline bool operator == (UArbInt    const& l, Arb        const& r) { return  r.op_eq(l); }
  template <class T> inline bool operator == (Arb        const& l, SafeInt<T> const  r) { return  l.op_eq(r); }
  template <class T> inline bool operator == (SafeInt<T> const  l, Arb        const& r) { return  r.op_eq(l); }
  template <class T> inline bool operator == (Arb        const& l, T          const  r) { return  l.op_eq(r); }
  template <class T> inline bool operator == (T          const  l, Arb        const& r) { return  r.op_eq(l); }
                     inline bool operator != (Arb        const& l, Arb        const& r) { return  l.op_ne(r); }
                     inline bool operator != (Arb        const& l, ArbInt     const& r) { return  l.op_ne(r); }
                     inline bool operator != (Arb        const& l, UArbInt    const& r) { return  l.op_ne(r); }
                     inline bool operator != (ArbInt     const& l, Arb        const& r) { return  r.op_ne(l); }
                     inline bool operator != (UArbInt    const& l, Arb        const& r) { return  r.op_ne(l); }
  template <class T> inline bool operator != (Arb        const& l, SafeInt<T> const  r) { return  l.op_ne(r); }
  template <class T> inline bool operator != (SafeInt<T> const  l, Arb        const& r) { return  r.op_ne(l); }
  template <class T> inline bool operator != (Arb        const& l, T          const  r) { return  l.op_ne(r); }
  template <class T> inline bool operator != (T          const  l, Arb        const& r) { return  r.op_ne(l); }
  
  /*
   * Arithmetic assignment operators.
   */
                     inline Arb&        operator *= (Arb&        l, Arb        const& r) { return l.op_mul(r);                             }
                     inline Arb&        operator *= (Arb&        l, ArbInt     const& r) { return l.op_mul(r);                             }
                     inline Arb&        operator *= (Arb&        l, UArbInt    const& r) { return l.op_mul(r);                             }
                     inline ArbInt&     operator *= (ArbInt&     l, Arb        const& r) { l = Arb(r).op_mul(l).toArbInt (); return l;     }
                     inline UArbInt&    operator *= (UArbInt&    l, Arb        const& r) { l = Arb(r).op_mul(l).toUArbInt(); return l;     }
  template <class T> inline Arb&        operator *= (Arb&        l, SafeInt<T> const  r) { return l.op_mul(r);                             }
  template <class T> inline SafeInt<T>& operator *= (SafeInt<T>& l, Arb        const& r) { l = static_cast<T>(Arb(r).op_mul(l)); return l; }
  template <class T> inline Arb&        operator *= (Arb&        l, T          const  r) { return l.op_mul(r);                             }
  template <class T> inline T&          operator *= (T&          l, Arb        const& r) { l = Arb(r).op_mul(l); return l;                 }
                     inline Arb&        operator /= (Arb&        l, Arb        const& r) { return l.op_div(r);                             }
                     inline Arb&        operator /= (Arb&        l, ArbInt     const& r) { return l.op_div(r);                             }
                     inline Arb&        operator /= (Arb&        l, UArbInt    const& r) { return l.op_div(r);                             }
                     inline ArbInt&     operator /= (ArbInt&     l, Arb        const& r) { l = Arb(l).op_div(r).toArbInt (); return l;     }
                     inline UArbInt&    operator /= (UArbInt&    l, Arb        const& r) { l = Arb(l).op_div(r).toUArbInt(); return l;     }
  template <class T> inline Arb&        operator /= (Arb&        l, SafeInt<T> const  r) { return l.op_div(r);                             }
  template <class T> inline SafeInt<T>& operator /= (SafeInt<T>& l, Arb        const& r) { l = static_cast<T>(Arb(l).op_div(r)); return l; }
  template <class T> inline Arb&        operator /= (Arb&        l, T          const  r) { return l.op_div(r);                             }
  template <class T> inline T&          operator /= (T&          l, Arb        const& r) { l = Arb(l).op_div(r); return l;                 }
                     inline Arb&        operator %= (Arb&        l, Arb        const& r) { return l.op_mod(r);                             }
                     inline Arb&        operator %= (Arb&        l, ArbInt     const& r) { return l.op_mod(r);                             }
                     inline Arb&        operator %= (Arb&        l, UArbInt    const& r) { return l.op_mod(r);                             }
                     inline ArbInt&     operator %= (ArbInt&     l, Arb        const& r) { l = Arb(l).op_mod(r).toArbInt (); return l;     }
                     inline UArbInt&    operator %= (UArbInt&    l, Arb        const& r) { l = Arb(l).op_mod(r).toUArbInt(); return l;     }
  template <class T> inline Arb&        operator %= (Arb&        l, SafeInt<T> const  r) { return l.op_mod(r);                             }
  template <class T> inline SafeInt<T>& operator %= (SafeInt<T>& l, Arb        const& r) { l = static_cast<T>(Arb(l).op_mod(r)); return l; }
  template <class T> inline Arb&        operator %= (Arb&        l, T          const  r) { return l.op_mod(r);                             }
  template <class T> inline T&          operator %= (T&          l, Arb        const& r) { l = Arb(l).op_mod(r); return l;                 }
                     inline Arb&        operator += (Arb&        l, Arb        const& r) { return l.op_add(r);                             }
                     inline Arb&        operator += (Arb&        l, ArbInt     const& r) { return l.op_add(r);                             }
                     inline Arb&        operator += (Arb&        l, UArbInt    const& r) { return l.op_add(r);                             }
                     inline ArbInt&     operator += (ArbInt&     l, Arb        const& r) { l = Arb(r).op_add(l).toArbInt (); return l;     }
                     inline UArbInt&    operator += (UArbInt&    l, Arb        const& r) { l = Arb(r).op_add(l).toUArbInt(); return l;     }
  template <class T> inline Arb&        operator += (Arb&        l, SafeInt<T> const  r) { return l.op_add(r);                             }
  template <class T> inline SafeInt<T>& operator += (SafeInt<T>& l, Arb        const& r) { l = static_cast<T>(Arb(r).op_add(l)); return l; }
  template <class T> inline Arb&        operator += (Arb&        l, T          const  r) { return l.op_add(r);                             }
  template <class T> inline T&          operator += (T&          l, Arb        const& r) { l = Arb(r).op_add(l); return l;                 }
                     inline Arb&        operator -= (Arb&        l, Arb        const& r) { return l.op_sub(r);                             }
                     inline Arb&        operator -= (Arb&        l, ArbInt     const& r) { return l.op_sub(r);                             }
                     inline Arb&        operator -= (Arb&        l, UArbInt    const& r) { return l.op_sub(r);                             }
                     inline ArbInt&     operator -= (ArbInt&     l, Arb        const& r) { l = Arb(l).op_sub(r).toArbInt (); return l;     }
                     inline UArbInt&    operator -= (UArbInt&    l, Arb        const& r) { l = Arb(l).op_sub(r).toUArbInt(); return l;     }
  template <class T> inline Arb&        operator -= (Arb&        l, SafeInt<T> const  r) { return l.op_sub(r);                             }
  template <class T> inline SafeInt<T>& operator -= (SafeInt<T>& l, Arb        const& r) { l = static_cast<T>(Arb(l).op_sub(r)); return l; }
  template <class T> inline Arb&        operator -= (Arb&        l, T          const  r) { return l.op_sub(r);                             }
  template <class T> inline T&          operator -= (T&          l, Arb        const& r) { l = Arb(l).op_sub(r); return l;                 }
  
  /*
   * Bit shift assignment operators.
   */
                     inline Arb&        operator <<= (Arb&        l, Arb        const& r) { return l.op_shl(r);                             }
                     inline Arb&        operator <<= (Arb&        l, ArbInt     const& r) { return l.op_shl(r);                             }
                     inline Arb&        operator <<= (Arb&        l, UArbInt    const& r) { return l.op_shl(r);                             }
                     inline ArbInt&     operator <<= (ArbInt&     l, Arb        const& r) { l = Arb(l).op_shl(r).toArbInt (); return l;     }
                     inline UArbInt&    operator <<= (UArbInt&    l, Arb        const& r) { l = Arb(l).op_shl(r).toUArbInt(); return l;     }
  template <class T> inline Arb&        operator <<= (Arb&        l, SafeInt<T> const  r) { return l.op_shl(r);                             }
  template <class T> inline SafeInt<T>& operator <<= (SafeInt<T>& l, Arb        const& r) { l = static_cast<T>(Arb(l).op_shl(r)); return l; }
  template <class T> inline Arb&        operator <<= (Arb&        l, T          const  r) { return l.op_shl(r);                             }
  template <class T> inline T&          operator <<= (T&          l, Arb        const& r) { l = Arb(l).op_shl(r); return l;                 }
                     inline Arb&        operator >>= (Arb&        l, Arb        const& r) { return l.op_shr(r);                             }
                     inline Arb&        operator >>= (Arb&        l, ArbInt     const& r) { return l.op_shr(r);                             }
                     inline Arb&        operator >>= (Arb&        l, UArbInt    const& r) { return l.op_shr(r);                             }
                     inline ArbInt&     operator >>= (ArbInt&     l, Arb        const& r) { l = Arb(l).op_shr(r).toArbInt (); return l;     }
                     inline UArbInt&    operator >>= (UArbInt&    l, Arb        const& r) { l = Arb(l).op_shr(r).toUArbInt(); return l;     }
  template <class T> inline Arb&        operator >>= (Arb&        l, SafeInt<T> const  r) { return l.op_shr(r);                             }
  template <class T> inline SafeInt<T>& operator >>= (SafeInt<T>& l, Arb        const& r) { l = static_cast<T>(Arb(l).op_shr(r)); return l; }
  template <class T> inline Arb&        operator >>= (Arb&        l, T          const  r) { return l.op_shr(r);                             }
  template <class T> inline T&          operator >>= (T&          l, Arb        const& r) { l = Arb(l).op_shr(r); return l;                 }
  
/*****************************************************************************
 * Extensions to std::
 *****************************************************************************/
} namespace std {
  
  inline DAC::Arb ceil  (DAC::Arb const& x) { return x.ceil();  }
  inline DAC::Arb floor (DAC::Arb const& x) { return x.floor(); }
  
}

#endif
