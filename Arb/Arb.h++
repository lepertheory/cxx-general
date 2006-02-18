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
  #include <ReferencePointer.h++>
  #include <toString.h++>
  #include <ArbInt.h++>
  #include <abs.h++>

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
          class Base : public Exception {
            public:
              virtual char const* what () const throw() { return "Undefined error in Arb."; };
              virtual ~Base () throw() {};
          };
          
          // Bad format.
          class BadFormat : public Base {
            public:
              virtual char const* what () const throw() {
                try {
                  return (std::string(_problem) + " at position " + DAC::toString(SafeInt<std::string::size_type>(_position) + 1) + " in number \"" + *_number + "\".").c_str();
                } catch (...) {
                  return "Bad format. Error creating message string.";
                }
              };
              virtual ~BadFormat () throw() {};
              BadFormat& Problem  (char const*                        const problem)  throw() { _problem  = problem ; return *this; };
              BadFormat& Position (std::string::size_type             const position) throw() { _position = position; return *this; };
              BadFormat& Number   (ConstReferencePointer<std::string> const number)   throw() { _number   = number  ; return *this; };
              char const*                        Problem  () const throw() { return _problem.c_str(); };
              std::string::size_type             Position () const throw() { return _position       ; };
              ConstReferencePointer<std::string> Number   () const throw() { return _number         ; };
            private:
              std::string                        _problem;
              std::string::size_type             _position;
              ConstReferencePointer<std::string> _number;
          };
          
          // Divide by zero.
          class DivByZero : public Base {
            public:
              virtual char const* what () const throw() { return "Divide by zero."; };
          };
          template <class T, class U> class DivByZeroBinary : public DivByZero {
            public:
              virtual char const* what () const throw() {
                try {
                  return (DAC::toString(_l) + " " + std::string(_op) + " " + DAC::toString(_r) + ": Divide by zero.").c_str();
                } catch (...) {
                  return "Divide by zero. Error creating message string.";
                }
              };
              virtual ~DivByZeroBinary () throw() {};
              DivByZeroBinary& Left     (T           const l)  throw() { _l  = l ; return *this; };
              DivByZeroBinary& Operator (char const* const op) throw() { _op = op; return *this; };
              DivByZeroBinary& Right    (U           const r)  throw() { _r  = r ; return *this; };
              T           Left     () const throw() { return _l         ; };
              char const* Operator () const throw() { return _op.c_str(); };
              U           Right    () const throw() { return _r         ; };
            private:
              T           _l;
              std::string _op;
              U           _r;
          };
          
          // Operation results in a complex number.
          class Complex : public Base {
            public:
              virtual char const* what () const throw() { return "Even roots of negative numbers can only be complex numbers."; };
          };
          class ComplexRoot : public Complex {
            public:
              virtual char const* what () const throw() {
                try {
                  return (std::string("Attempting to take the even root ") + DAC::toString(*_root) + " of negative number " + DAC::toString(*_number) + " results in a complex number.").c_str();
                } catch (...) {
                  return "Even roots of negative numbers can only be complex numbers. Error creating message string.";
                }
              };
              ComplexRoot& Number (ConstReferencePointer<Arb> const number) throw() { _number = number; return *this; };
              ComplexRoot& Root   (ConstReferencePointer<Arb> const root)   throw() { _root   = root  ; return *this; };
              ConstReferencePointer<Arb> Number () const throw() { return _number; };
              ConstReferencePointer<Arb> Root   () const throw() { return _root  ; };
            private:
              ConstReferencePointer<Arb> _number;
              ConstReferencePointer<Arb> _root;
          };
          
          // Integer-only operation attempted on a non-integer.
          class NonInteger : public Base {
            public:
              virtual char const* what () const throw() { return "An integer operation was attempted on a non-integer number."; };
          };
          template <class T, class U> class NonIntegerBinary : public NonInteger {
            public:
              virtual char const* what () const throw() {
                try {
                  return (DAC::toString(_l) + " " + std::string(_op) + " " + DAC::toString(_r) + ": Integer operation attempted on a non-integer.").c_str();
                } catch (...) {
                  return "Integer operation attempted on a non-integer. Error creating message string.";
                }
              };
              virtual ~NonIntegerBinary () throw() {};
              NonIntegerBinary& Left     (T           const l)  throw() { _l  = l ; return *this; };
              NonIntegerBinary& Operator (char const* const op) throw() { _op = op; return *this; };
              NonIntegerBinary& Right    (U           const r)  throw() { _r  = r ; return *this; };
              T           Left     () const throw() { return _l         ; };
              char const* Operator () const throw() { return _op.c_str(); };
              U           Right    () const throw() { return _r         ; };
            private:
              T           _l;
              std::string _op;
              U           _r;
          };
          
          // Attempt to stuff Arb into a type that will not fit.
          class ScalarOverflow : public Base {
            public:
              virtual char const* what () const throw() { return "Arb overflows requested scalar type."; };
          };
          template <class T> class ScalarOverflowSpecialized : public ScalarOverflow {
            public:
              virtual char const* what () const throw() {
                try {
                  return (_number->toString() + ": Overflows requested scalar type's limit of " + DAC::toString(_limit) + ".").c_str();
                } catch (...) {
                  return "Arb overflows requested scalar type. Error creating message string.";
                }
              };
              ScalarOverflowSpecialized& Number (ConstReferencePointer<Arb> const number) throw() { _number = number; return *this; };
              ScalarOverflowSpecialized& Limit  (T                          const limit ) throw() { _limit  = limit ; return *this; };
              ConstReferencePointer<Arb> Number () const throw() { return _number; };
              T                          Limit  () const throw() { return _limit ; };
            private:
              ConstReferencePointer<Arb> _number;
              T                          _limit;
          };
          
          // Attempt to move Arb into a number that won't see it.
          class ScalarUnderflow : public Base {
            public:
              virtual char const* what () const throw() { return "Arb underflows requested scalar type."; };
          };
          template <class T> class ScalarUnderflowSpecialized : public ScalarUnderflow {
            public:
              virtual char const* what () const throw() {
                try {
                  return (_number->toString() + ": Underflows requested scalar type's limit of " + DAC::toString(_limit) + ".").c_str();
                } catch (...) {
                  return "Arb underflows requested scalar type. Error creating message string.";
                }
              };
              ScalarUnderflowSpecialized& Number (ConstReferencePointer<Arb> const number) throw() { _number = number; return *this; };
              ScalarUnderflowSpecialized& Limit  (T                          const limit ) throw() { _limit  = limit ; return *this; };
              ConstReferencePointer<Arb> Number () const throw() { return _number; };
              T                          Limit  () const throw() { return _limit ; };
            private:
              ConstReferencePointer<Arb> _number;
              T                          _limit ;
          };
          
          // Attempt to set Arb with an invalid floating-point number.
          class InvalidFloat : public Base {
            public:
              virtual char const* what () const throw() { return "Attempt to set Arb from an invalid floating-point number."; };
          };
          template <class T> class InvalidFloatSpecialized : public InvalidFloat {
            public:
              virtual char const* what () const throw() {
                try {
                  return ("Attempt to set Arb from an invalid floating-point number: " + DAC::toString(_number)).c_str();
                } catch (...) {
                  return "Attempt to set Arb from an invalid floating-point number. Error creating message string.";
                }
              };
              InvalidFloatSpecialized& Number (T const number) throw() { _number = number; return *this; };
              T Number () const throw() { return _number; };
            private:
              T _number;
          };
          template <class T> class Infinity : public InvalidFloatSpecialized<T> {
            public:
              virtual char const* what () const throw() {
                try {
                  return ("Attempt to set Arb to infinity: " + DAC::toString(this->Number())).c_str();
                } catch (...) {
                  return "Attempt to set Arb to infinity. Error creating message string.";
                }
              };
          };
          template <class T> class PositiveInfinity : public Infinity<T> {
            public:
              virtual char const* what () const throw() {
                try {
                  return ("Attempt to set Arb to positive infinity: " + DAC::toString(this->Number())).c_str();
                } catch (...) {
                  return "Attempt to set Arb to positive infinity. Error creating message string.";
                }
              };
          };
          template <class T> class NegativeInfinity : public Infinity<T> {
            public:
              virtual char const* what () const throw() {
                try {
                  return ("Attempt to set Arb to negative infinity: " + DAC::toString(this->Number())).c_str();
                } catch (...) {
                  return "Attempt to set Arb to negative infinity. Error creating message string.";
                }
              };
          };
          template <class T> class NaN : public InvalidFloatSpecialized<T> {
            public:
              virtual char const* what () const throw() {
                try {
                  return ("Attempt to set Arb to Not-a-Number: " + DAC::toString(this->Number())).c_str();
                } catch (...) {
                  return "Attempt to set Arb to Not-a-Number. Error creating message string.";
                }
              };
          };
        
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
      Arb (Arb const& number, bool const copynow = false);
      
      // Conversion constructor.
                         explicit Arb (std::string const& number);
                         explicit Arb (ArbInt      const& number);
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
      operator signed   char      () const;
      operator unsigned char      () const;
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
      Arb& clear ();
      
      // Copy another number.
      Arb& copy     (Arb const& number) throw();
      Arb& deepcopy (Arb const& number)        ;
      
      // Set the number.
                         Arb& set (std::string const& number);
                         Arb& set (Arb         const& number);
                         Arb& set (ArbInt      const& number);
      template <class T> Arb& set (SafeInt<T>  const  number);
      template <class T> Arb& set (T           const  number);
      
      // Convert to string.
      std::string toString (OutputFormat const format = FMT_DEFAULT) const;
      
      // Arithmetic operator backends.
                         Arb& op_mul (Arb        const& number);
                         Arb& op_mul (ArbInt     const& number);
      template <class T> Arb& op_mul (SafeInt<T> const  number);
      template <class T> Arb& op_mul (T          const  number);
                         Arb& op_div (Arb        const& number);
                         Arb& op_div (ArbInt     const& number);
      template <class T> Arb& op_div (SafeInt<T> const  number);
      template <class T> Arb& op_div (T          const  number);
                         Arb& op_mod (Arb        const& number);
                         Arb& op_mod (ArbInt     const& number);
      template <class T> Arb& op_mod (SafeInt<T> const  number);
      template <class T> Arb& op_mod (T          const  number);
                         Arb& op_add (Arb        const& number);
                         Arb& op_add (ArbInt     const& number);
      template <class T> Arb& op_add (SafeInt<T> const  number);
      template <class T> Arb& op_add (T          const  number);
                         Arb& op_sub (Arb        const& number);
                         Arb& op_sub (ArbInt     const& number);
      template <class T> Arb& op_sub (SafeInt<T> const  number);
      template <class T> Arb& op_sub (T          const  number);
      
      // Bit shift operator backends.
                         Arb& op_shl (Arb        const& number);
                         Arb& op_shl (ArbInt     const& number);
      template <class T> Arb& op_shl (SafeInt<T> const  number);
      template <class T> Arb& op_shl (T          const  number);
                         Arb& op_shr (Arb        const& number);
                         Arb& op_shr (ArbInt     const& number);
      template <class T> Arb& op_shr (SafeInt<T> const  number);
      template <class T> Arb& op_shr (T          const  number);
      
      // Comparison operator backends.
                         bool op_gt (Arb        const& number) const;
                         bool op_gt (ArbInt     const& number) const;
      template <class T> bool op_gt (SafeInt<T> const  number) const;
      template <class T> bool op_gt (T          const  number) const;
                         bool op_ge (Arb        const& number) const;
                         bool op_ge (ArbInt     const& number) const;
      template <class T> bool op_ge (SafeInt<T> const  number) const;
      template <class T> bool op_ge (T          const  number) const;
                         bool op_lt (Arb        const& number) const;
                         bool op_lt (ArbInt     const& number) const;
      template <class T> bool op_lt (SafeInt<T> const  number) const;
      template <class T> bool op_lt (T          const  number) const;
                         bool op_le (Arb        const& number) const;
                         bool op_le (ArbInt     const& number) const;
      template <class T> bool op_le (SafeInt<T> const  number) const;
      template <class T> bool op_le (T          const  number) const;
                         bool op_eq (Arb        const& number) const;
                         bool op_eq (ArbInt     const& number) const;
      template <class T> bool op_eq (SafeInt<T> const  number) const;
      template <class T> bool op_eq (T          const  number) const;
                         bool op_ne (Arb        const& number) const;
                         bool op_ne (ArbInt     const& number) const;
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
      Arb    ceil     () const;
      Arb    floor    () const;
      Arb    truncate () const;
      Arb    toInt    () const;
      ArbInt toArbInt () const;
      
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
      // Typedefs.
      
      // Forward declarations.
      class _Data;
      
      typedef ArbInt _DigsT; // Native digits type.
      
      typedef ReferencePointer<_Data> _DataPT; // Pointer to number data.
      
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
      
      // Number types.
      enum _NumType { _NUM_UINT, _NUM_SINT, _NUM_FLPT, _NUM_UNKNOWN };
      
      // Determine number type.
      template <class T> class _GetNumType { public: static _NumType const value; };
      
      // Set.
      template <class T, _NumType> class _Set;
      template <class T> class _Set<T, _NUM_UINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Set<T, _NUM_SINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Set<T, _NUM_FLPT> { public:                                              static void op (Arb& l, T const r); };
      
      // Get.
      template <class T, _NumType> class _Get;
      template <class T> class _Get<T, _NUM_UINT> { public: static void op (SafeInt<T>& l, Arb const& r); static void op (T& l, Arb const& r); };
      template <class T> class _Get<T, _NUM_SINT> { public: static void op (SafeInt<T>& l, Arb const& r); static void op (T& l, Arb const& r); };
      template <class T> class _Get<T, _NUM_FLPT> { public:                                               static void op (T& l, Arb const& r); };
      
      // Set a bitwise copy.
      template <class T, _NumType> class _SetBitwise;
      template <class T> class _SetBitwise<T, _NUM_UINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _SetBitwise<T, _NUM_SINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      
      // Multiply.
      template <class T, _NumType> class _Mul;
      template <class T> class _Mul<T, _NUM_UINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Mul<T, _NUM_SINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Mul<T, _NUM_FLPT> { public:                                              static void op (Arb& l, T const r); };
      
      // Divide.
      template <class T, _NumType> class _Div;
      template <class T> class _Div<T, _NUM_UINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Div<T, _NUM_SINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Div<T, _NUM_FLPT> { public:                                              static void op (Arb& l, T const r); };
      
      // Modulo divide.
      template <class T, _NumType> class _Mod;
      template <class T> class _Mod<T, _NUM_UINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Mod<T, _NUM_SINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Mod<T, _NUM_FLPT> { public:                                              static void op (Arb& l, T const r); };
      
      // Add.
      template <class T, _NumType> class _Add;
      template <class T> class _Add<T, _NUM_UINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Add<T, _NUM_SINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Add<T, _NUM_FLPT> { public:                                              static void op (Arb& l, T const r); };
      
      // Subtract.
      template <class T, _NumType> class _Sub;
      template <class T> class _Sub<T, _NUM_UINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Sub<T, _NUM_SINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _Sub<T, _NUM_FLPT> { public:                                              static void op (Arb& l, T const r); };
      
      // Shift.
      template <class T, _NumType> class _Shift;
      template <class T> class _Shift<T, _NUM_UINT> { public: static void op (Arb& l, SafeInt<T> const r, _Dir const dir); static void op (Arb& l, T const r, _Dir const dir); };
      template <class T> class _Shift<T, _NUM_SINT> { public: static void op (Arb& l, SafeInt<T> const r, _Dir const dir); static void op (Arb& l, T const r, _Dir const dir); };
      template <class T> class _Shift<T, _NUM_FLPT> { public:                                                              static void op (Arb& l, T const r, _Dir const dir); };
      
      // Shift left.
      template <class T, _NumType> class _ShL;
      template <class T> class _ShL<T, _NUM_UINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _ShL<T, _NUM_SINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _ShL<T, _NUM_FLPT> { public:                                              static void op (Arb& l, T const r); };
      
      // Shift right.
      template <class T, _NumType> class _ShR;
      template <class T> class _ShR<T, _NUM_UINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _ShR<T, _NUM_SINT> { public: static void op (Arb& l, SafeInt<T> const r); static void op (Arb& l, T const r); };
      template <class T> class _ShR<T, _NUM_FLPT> { public:                                              static void op (Arb& l, T const r); };
      
      // Greater than.
      template <class T, _NumType> class _GT;
      template <class T> class _GT<T, _NUM_UINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _GT<T, _NUM_SINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _GT<T, _NUM_FLPT> { public:                                                    static bool op (Arb const& l, T const r); };
      
      // Greater than or equal to.
      template <class T, _NumType> class _GE;
      template <class T> class _GE<T, _NUM_UINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _GE<T, _NUM_SINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _GE<T, _NUM_FLPT> { public:                                                    static bool op (Arb const& l, T const r); };
      
      // Less than.
      template <class T, _NumType> class _LT;
      template <class T> class _LT<T, _NUM_UINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _LT<T, _NUM_SINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _LT<T, _NUM_FLPT> { public:                                                    static bool op (Arb const& l, T const r); };
      
      // Less than or equal to.
      template <class T, _NumType> class _LE;
      template <class T> class _LE<T, _NUM_UINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _LE<T, _NUM_SINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _LE<T, _NUM_FLPT> { public:                                                    static bool op (Arb const& l, T const r); };
      
      // Equal to.
      template <class T, _NumType> class _EQ;
      template <class T> class _EQ<T, _NUM_UINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _EQ<T, _NUM_SINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _EQ<T, _NUM_FLPT> { public:                                                    static bool op (Arb const& l, T const r); };
      
      // Not equal to.
      template <class T, _NumType> class _NE;
      template <class T> class _NE<T, _NUM_UINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _NE<T, _NUM_SINT> { public: static bool op (Arb const& l, SafeInt<T> const r); static bool op (Arb const& l, T const r); };
      template <class T> class _NE<T, _NUM_FLPT> { public:                                                    static bool op (Arb const& l, T const r); };
      
      /*********************************************************************
       * _Data
       *********************************************************************
       * This holds all data for this number, copying this data structure to
       * another number will make a perfect copy of this number.
       *********************************************************************/
      class _Data {
        
        /*
         * Public members.
         */
        public:
          
          /*****************************************************************/
          // Data members.
          
          _DigsT p;    // Numerator.
          _DigsT q;    // Denominator.
          _DigsT fixq; // Fixed denominator.
          
          bool      positive; // True if the number is positive.
          PointPosT pointpos; // Radix point position.
          bool      fix;      // If true, fix the radix point.
          FixType   fixtype;  // What to fix on, radix points or denominator.
          BaseT     base;     // The base of the number.
          
          /*****************************************************************/
          // Function members.
          
          // Default constructor.
          _Data ();
          
          // Copy constructor.
          _Data (_Data const& data);
          
          // Assignment operator.
          _Data& operator = (_Data const& data);
          
          // Reset to just-constructed state.
          _Data& clear ();
          
          // Copy a given _Data object.
          _Data& copy (_Data const& data);
          
        /*
         * Private members.
         */
        private:
          
          /*****************************************************************/
          // Function members.
          
          // Common initialization routines.
          void _init ();
          
      };
      
      /*********************************************************************/
      // Data members.
      
      // The number.
      _DataPT _data;
      
      // Properties.
      std::string::size_type _maxradix; // Radix digits to output.
      OutputFormat           _format;   // Format to output this number.
      RoundMethod            _round;    // Rounding method.
      
      /*********************************************************************/
      // Function members.
      
      // Common initialization tasks.
      void _init ();
      
      // Bit shift this number.
                         Arb& _shift (Arb        const& bits, _Dir const dir);
                         Arb& _shift (ArbInt     const& bits, _Dir const dir);
      template <class T> Arb& _shift (SafeInt<T> const  bits, _Dir const dir);
      template <class T> Arb& _shift (T          const  bits, _Dir const dir);
      
      // Reduce the number to its most compact representation.
      Arb& _reduce      ();
      Arb& _forcereduce (_DigsT const& q);
      
      // Normalize this number to another number.
      Arb& _normalize (Arb& number);
      
      // Convert this number to the range of a floating-point number type,
      // return the exponent of 2 that will convert this number back to the
      // original. I'm not happy with the syntax of this, so that combined
      // with the limited usefulness of this outside converting to a float,
      // which we already provide, it's staying private because it's bad.
      template <class T> ArbInt _toFloat (unsigned int& exponent) const;
      
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
                     Arb operator * (ArbInt     const& l, Arb        const& r);
  template <class T> Arb operator * (Arb        const& l, SafeInt<T> const  r);
  template <class T> Arb operator * (SafeInt<T> const  l, Arb        const& r);
  template <class T> Arb operator * (Arb        const& l, T          const  r);
  template <class T> Arb operator * (T          const  l, Arb        const& r);
                     Arb operator / (Arb        const& l, Arb        const& r);
                     Arb operator / (Arb        const& l, ArbInt     const& r);
                     Arb operator / (ArbInt     const& l, Arb        const& r);
  template <class T> Arb operator / (Arb        const& l, SafeInt<T> const  r);
  template <class T> Arb operator / (SafeInt<T> const  l, Arb        const& r);
  template <class T> Arb operator / (Arb        const& l, T          const  r);
  template <class T> Arb operator / (T          const  l, Arb        const& r);
                     Arb operator % (Arb        const& l, Arb        const& r);
                     Arb operator % (Arb        const& l, ArbInt     const& r);
                     Arb operator % (ArbInt     const& l, Arb        const& r);
  template <class T> Arb operator % (Arb        const& l, SafeInt<T> const  r);
  template <class T> Arb operator % (SafeInt<T> const  l, Arb        const& r);
  template <class T> Arb operator % (Arb        const& l, T          const  r);
  template <class T> Arb operator % (T          const  l, Arb        const& r);
                     Arb operator + (Arb        const& l, Arb        const& r);
                     Arb operator + (Arb        const& l, ArbInt     const& r);
                     Arb operator + (ArbInt     const& l, Arb        const& r);
  template <class T> Arb operator + (Arb        const& l, SafeInt<T> const  r);
  template <class T> Arb operator + (SafeInt<T> const  l, Arb        const& r);
  template <class T> Arb operator + (Arb        const& l, T          const  r);
  template <class T> Arb operator + (T          const  l, Arb        const& r);
                     Arb operator - (Arb        const& l, Arb        const& r);
                     Arb operator - (Arb        const& l, ArbInt     const& r);
                     Arb operator - (ArbInt     const& l, Arb        const& r);
  template <class T> Arb operator - (Arb        const& l, SafeInt<T> const  r);
  template <class T> Arb operator - (SafeInt<T> const  l, Arb        const& r);
  template <class T> Arb operator - (Arb        const& l, T          const  r);
  template <class T> Arb operator - (T          const  l, Arb        const& r);
  
  // Bit shift operators.
                     Arb operator << (Arb        const& l, Arb        const& r);
                     Arb operator << (Arb        const& l, ArbInt     const& r);
                     Arb operator << (ArbInt     const& l, Arb        const& r);
  template <class T> Arb operator << (Arb        const& l, SafeInt<T> const  r);
  template <class T> Arb operator << (SafeInt<T> const  l, Arb        const& r);
  template <class T> Arb operator << (Arb        const& l, T          const  r);
  template <class T> Arb operator << (T          const  l, Arb        const& r);
                     Arb operator >> (Arb        const& l, Arb        const& r);
                     Arb operator >> (Arb        const& l, ArbInt     const& r);
                     Arb operator >> (ArbInt     const& l, Arb        const& r);
  template <class T> Arb operator >> (Arb        const& l, SafeInt<T> const  r);
  template <class T> Arb operator >> (SafeInt<T> const  l, Arb        const& r);
  template <class T> Arb operator >> (Arb        const& l, T          const  r);
  template <class T> Arb operator >> (T          const  l, Arb        const& r);
  
  // Comparison operators.
                     bool operator >  (Arb        const& l, Arb        const& r);
                     bool operator >  (Arb        const& l, ArbInt     const& r);
                     bool operator >  (ArbInt     const& l, Arb        const& r);
  template <class T> bool operator >  (Arb        const& l, SafeInt<T> const  r);
  template <class T> bool operator >  (SafeInt<T> const  l, Arb        const& r);
  template <class T> bool operator >  (Arb        const& l, T          const  r);
  template <class T> bool operator >  (T          const  l, Arb        const& r);
                     bool operator >= (Arb        const& l, Arb        const& r);
                     bool operator >= (Arb        const& l, ArbInt     const& r);
                     bool operator >= (ArbInt     const& l, Arb        const& r);
  template <class T> bool operator >= (Arb        const& l, SafeInt<T> const  r);
  template <class T> bool operator >= (SafeInt<T> const  l, Arb        const& r);
  template <class T> bool operator >= (Arb        const& l, T          const  r);
  template <class T> bool operator >= (T          const  l, Arb        const& r);
                     bool operator <  (Arb        const& l, Arb        const& r);
                     bool operator <  (Arb        const& l, ArbInt     const& r);
                     bool operator <  (ArbInt     const& l, Arb        const& r);
  template <class T> bool operator <  (Arb        const& l, SafeInt<T> const  r);
  template <class T> bool operator <  (SafeInt<T> const  l, Arb        const& r);
  template <class T> bool operator <  (Arb        const& l, T          const  r);
  template <class T> bool operator <  (T          const  l, Arb        const& r);
                     bool operator <= (Arb        const& l, Arb        const& r);
                     bool operator <= (Arb        const& l, ArbInt     const& r);
                     bool operator <= (ArbInt     const& l, Arb        const& r);
  template <class T> bool operator <= (Arb        const& l, SafeInt<T> const  r);
  template <class T> bool operator <= (SafeInt<T> const  l, Arb        const& r);
  template <class T> bool operator <= (Arb        const& l, T          const  r);
  template <class T> bool operator <= (T          const  l, Arb        const& r);
                     bool operator == (Arb        const& l, Arb        const& r);
                     bool operator == (Arb        const& l, ArbInt     const& r);
                     bool operator == (ArbInt     const& l, Arb        const& r);
  template <class T> bool operator == (Arb        const& l, SafeInt<T> const  r);
  template <class T> bool operator == (SafeInt<T> const  l, Arb        const& r);
  template <class T> bool operator == (Arb        const& l, T          const  r);
  template <class T> bool operator == (T          const  l, Arb        const& r);
                     bool operator != (Arb        const& l, Arb        const& r);
                     bool operator != (Arb        const& l, ArbInt     const& r);
                     bool operator != (ArbInt     const& l, Arb        const& r);
  template <class T> bool operator != (Arb        const& l, SafeInt<T> const  r);
  template <class T> bool operator != (SafeInt<T> const  l, Arb        const& r);
  template <class T> bool operator != (Arb        const& l, T          const  r);
  template <class T> bool operator != (T          const  l, Arb        const& r);
  
  // Arithmetic assignment operators.
                     Arb&        operator *= (Arb&        l, Arb        const& r);
                     Arb&        operator *= (Arb&        l, ArbInt     const& r);
                     ArbInt&     operator *= (ArbInt&     l, Arb        const& r);
  template <class T> Arb&        operator *= (Arb&        l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator *= (SafeInt<T>& l, Arb        const& r);
  template <class T> Arb&        operator *= (Arb&        l, T          const  r);
  template <class T> T&          operator *= (T&          l, Arb        const& r);
                     Arb&        operator /= (Arb&        l, Arb        const& r);
                     Arb&        operator /= (Arb&        l, ArbInt     const& r);
                     ArbInt&     operator /= (ArbInt&     l, Arb        const& r);
  template <class T> Arb&        operator /= (Arb&        l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator /= (SafeInt<T>& l, Arb        const& r);
  template <class T> Arb&        operator /= (Arb&        l, T          const  r);
  template <class T> T&          operator /= (T&          l, Arb        const& r);
                     Arb&        operator %= (Arb&        l, Arb        const& r);
                     Arb&        operator %= (Arb&        l, ArbInt     const& r);
                     ArbInt&     operator %= (ArbInt&     l, Arb        const& r);
  template <class T> Arb&        operator %= (Arb&        l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator %= (SafeInt<T>& l, Arb        const& r);
  template <class T> Arb&        operator %= (Arb&        l, T          const  r);
  template <class T> T&          operator %= (T&          l, Arb        const& r);
                     Arb&        operator += (Arb&        l, Arb        const& r);
                     Arb&        operator += (Arb&        l, ArbInt     const& r);
                     ArbInt&     operator += (ArbInt&     l, Arb        const& r);
  template <class T> Arb&        operator += (Arb&        l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator += (SafeInt<T>& l, Arb        const& r);
  template <class T> Arb&        operator += (Arb&        l, T          const  r);
  template <class T> T&          operator += (T&          l, Arb        const& r);
                     Arb&        operator -= (Arb&        l, Arb        const& r);
                     Arb&        operator -= (Arb&        l, ArbInt     const& r);
                     ArbInt&     operator -= (ArbInt&     l, Arb        const& r);
  template <class T> Arb&        operator -= (Arb&        l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator -= (SafeInt<T>& l, Arb        const& r);
  template <class T> Arb&        operator -= (Arb&        l, T          const  r);
  template <class T> T&          operator -= (T&          l, Arb        const& r);
  
  // Bit shift assignment operators.
                     Arb&        operator <<= (Arb&        l, Arb        const& r);
                     Arb&        operator <<= (Arb&        l, ArbInt     const& r);
                     ArbInt&     operator <<= (ArbInt&     l, Arb        const& r);
  template <class T> Arb&        operator <<= (Arb&        l, SafeInt<T> const  r);
  template <class T> SafeInt<T>& operator <<= (SafeInt<T>& l, Arb        const& r);
  template <class T> Arb&        operator <<= (Arb&        l, T          const  r);
  template <class T> T&          operator <<= (T&          l, Arb        const& r);
                     Arb&        operator >>= (Arb&        l, Arb        const& r);
                     Arb&        operator >>= (Arb&        l, ArbInt     const& r);
                     ArbInt&     operator >>= (ArbInt&     l, Arb        const& r);
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
  inline Arb Arb::operator + () const { return *this;                                                                             }
  inline Arb Arb::operator - () const { Arb retval(*this, true); retval._data->positive = !retval._data->positive; return retval; }
  
  /*
   * Casting operators.
   */
  inline Arb::operator bool               () const { return !isZero();                   }
  inline Arb::operator signed   char      () const { return Value<signed   char     >(); }
  inline Arb::operator unsigned char      () const { return Value<unsigned char     >(); }
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
                     inline Arb& Arb::operator = (Arb         const& number) { return copy(number); }
                     inline Arb& Arb::operator = (std::string const& number) { return set(number);  }
                     inline Arb& Arb::operator = (ArbInt      const& number) { return set(number);  }
  template <class T> inline Arb& Arb::operator = (T           const  number) { return set(number);  }
  
  /*
   * Get the base of this number.
   */
  inline Arb::BaseT Arb::Base () const { return _data->base; }
  
  /*
   * Get the maximum number of radix digits to output.
   */
  inline std::string::size_type Arb::MaxRadix () const { return _maxradix; }
  
  /*
   * Set the maximum number of radix digits to output.
   */
  inline Arb& Arb::MaxRadix (std::string::size_type const maxradix) { _maxradix = maxradix; return *this; }
  
  /*
   * Get the point position of a fixed-point number.
   */
  inline Arb::PointPosT Arb::PointPos () const { return _data->pointpos; }
  
  /*
   * Get whether this is a fixed-point number or not.
   */
  inline bool Arb::Fixed () const { return _data->fix; }
  
  /*
   * Get the fix source for this number.
   */
  inline Arb::FixType Arb::Fix () const { return _data->fixtype; }
  
  /*
   * Get the output format of this number.
   */
  inline Arb::OutputFormat Arb::Format () const { return _format; }
  
  /*
   * Set the output format of this number.
   */
  inline Arb& Arb::Format (OutputFormat const format) { _format = (format == FMT_DEFAULT) ? _format : format; return *this; }
  
  /*
   * Get the round method of this number.
   */
  inline Arb::RoundMethod Arb::Round () const { return _round; }
  
  /*
   * Set the round method of this number.
   */
  inline Arb& Arb::Round (RoundMethod const round) { _round = (round == ROUND_DEFAULT) ? _round : round; return *this; }
  
  /*
   * Get the fixed quotient of this number.
   */
  inline Arb Arb::FixQ () const { return Arb(_data->fixq); }
  
  /*
   * Get the value of this number.
   */
  template <class T> inline T Arb::Value () const { T retval; _Get<T, _GetNumType<T>::value>::op(retval, *this); return retval; }
  
  /*
   * Set the value of this number.
   */
  template <class T> inline Arb& Arb::Value (T const number) { return set(number); }
  
  /*
   * Set from a built-in type.
   */
  template <class T> inline Arb& Arb::set (SafeInt<T> const number) { _Set<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::set (T          const number) { _Set<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  
  /*
   * Arithmetic operator backends.
   */
  template <class T> inline Arb& Arb::op_mul (SafeInt<T> const number) { _Mul<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_mul (T          const number) { _Mul<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_div (SafeInt<T> const number) { _Div<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_div (T          const number) { _Div<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_mod (SafeInt<T> const number) { _Mod<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_mod (T          const number) { _Mod<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_add (SafeInt<T> const number) { _Add<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_add (T          const number) { _Add<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_sub (SafeInt<T> const number) { _Sub<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::op_sub (T          const number) { _Sub<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  
  /*
   * Shift left, shift right.
   */
                     inline Arb& Arb::op_shl (Arb        const& number) { Arb retval(*this, true); retval._shift(number, _DIR_L);                      return copy(retval); }
                     inline Arb& Arb::op_shl (ArbInt     const& number) { Arb retval(*this, true); retval._shift(number, _DIR_L);                      return copy(retval); }
  template <class T> inline Arb& Arb::op_shl (SafeInt<T> const  number) { Arb retval(*this, true); _ShL<T, _GetNumType<T>::value>::op(retval, number); return copy(retval); }
  template <class T> inline Arb& Arb::op_shl (T          const  number) { Arb retval(*this, true); _ShL<T, _GetNumType<T>::value>::op(retval, number); return copy(retval); }
                     inline Arb& Arb::op_shr (Arb        const& number) { Arb retval(*this, true); retval._shift(number, _DIR_R);                      return copy(retval); }
                     inline Arb& Arb::op_shr (ArbInt     const& number) { Arb retval(*this, true); retval._shift(number, _DIR_R);                      return copy(retval); }
  template <class T> inline Arb& Arb::op_shr (SafeInt<T> const  number) { Arb retval(*this, true); _ShR<T, _GetNumType<T>::value>::op(retval, number); return copy(retval); }
  template <class T> inline Arb& Arb::op_shr (T          const  number) { Arb retval(*this, true); _ShR<T, _GetNumType<T>::value>::op(retval, number); return copy(retval); }
  
  /*
   * Comparison operator backends.
   */
  template <class T> inline bool Arb::op_gt (SafeInt<T> const  number) const { return _GT<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool Arb::op_gt (T          const  number) const { return _GT<T, _GetNumType<T>::value>::op(*this, number); }
                     inline bool Arb::op_ge (Arb        const& number) const { return !op_lt(number);                                   }
                     inline bool Arb::op_ge (ArbInt     const& number) const { return !op_lt(number);                                   }
  template <class T> inline bool Arb::op_ge (SafeInt<T> const  number) const { return _GE<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool Arb::op_ge (T          const  number) const { return _GE<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool Arb::op_lt (SafeInt<T> const  number) const { return _LT<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool Arb::op_lt (T          const  number) const { return _LT<T, _GetNumType<T>::value>::op(*this, number); }
                     inline bool Arb::op_le (Arb        const& number) const { return !op_gt(number);                                   }
                     inline bool Arb::op_le (ArbInt     const& number) const { return !op_gt(number);                                   }
  template <class T> inline bool Arb::op_le (SafeInt<T> const  number) const { return _LE<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool Arb::op_le (T          const  number) const { return _LE<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool Arb::op_eq (SafeInt<T> const  number) const { return _EQ<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool Arb::op_eq (T          const  number) const { return _EQ<T, _GetNumType<T>::value>::op(*this, number); }
                     inline bool Arb::op_ne (Arb        const& number) const { return !op_eq(number);                                   }
                     inline bool Arb::op_ne (ArbInt     const& number) const { return !op_eq(number);                                   }
  template <class T> inline bool Arb::op_ne (SafeInt<T> const  number) const { return _NE<T, _GetNumType<T>::value>::op(*this, number); }
  template <class T> inline bool Arb::op_ne (T          const  number) const { return _NE<T, _GetNumType<T>::value>::op(*this, number); }
  
  /*
   * Return whether this number is an integer.
   */
  inline bool Arb::isInteger () const { return (_data->q == 1); }
  
  /*
   * Return whether this number is positive.
   */
  inline bool Arb::isPositive () const { return _data->positive || _data->p == 0; }
  inline bool Arb::isNegative () const { return !isPositive();                    }
  
  /*
   * Return whether this number is equal to zero.
   */
  inline bool Arb::isZero () const { return _data->p.isZero(); }
  
  /*
   * Return whether this number is even or odd.
   */
  inline bool Arb::isOdd  () const { return (isInteger() && _data->p.isOdd());  }
  inline bool Arb::isEven () const { return (isInteger() && _data->p.isEven()); }
  
  /*
   * Return the integer portion of this number.
   */
  inline Arb Arb::toInt () const { Arb retval(*this, true); retval._forcereduce(_DigsT(1)); return retval; }
  
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
  inline Arb Arb::abs () const { Arb retval(*this, true); retval._data->positive = true; return retval; }
  
  /*
   * Bit shift this number.
   */
  template <class T> inline Arb& Arb::_shift (SafeInt<T> const bits, _Dir const dir) { _Shift<T, _GetNumType<T>::value>::op(*this, bits, dir); return *this; }
  template <class T> inline Arb& Arb::_shift (T          const bits, _Dir const dir) { _Shift<T, _GetNumType<T>::value>::op(*this, bits, dir); return *this; }
  
  /*
   * Convert number to floating-point range and return exponent.
   */
  template <class T> ArbInt Arb::_toFloat (unsigned int& exponent) const {
    
    // Work area.
    Arb                   tmpnum;
    SafeInt<unsigned int> tmpexp;
    
    // Check for zero.
    if (isZero()) {
      exponent = 0;
      return tmpnum._data->p;
    }
    
    // Set tmpnum to *this here to avoid carrying over rounding or fixed-point
    // properties, all we are interested in here is the value.
    tmpnum.set(*this);
    
    // Convert tmpnum to the range of 1 <= tmpnum < 2, save changes in
    // exponent so that tmpnum * 2^exponent == *this.
    ArbInt bits_p = tmpnum._data->p.bitsInNumber();
    ArbInt bits_q = tmpnum._data->q.bitsInNumber();
    bool   denorm = false;
    if (bits_p < bits_q) {
      
      // Get the difference in bits.
      ArbInt bitdiff = bits_q - bits_p;
      
      // If bitdiff >= bias, try to store this number as a denormalized
      // number.
      if (bitdiff >= _FloatInfo<T>::bias) {
        
        // See if we can keep the exponent within range and still have bits of
        // result left. If not, throw an exception, caller should deal with
        // underflow.
        if (bitdiff >= _FloatInfo<T>::bias + (_FloatInfo<T>::mantissabits - 1)) {
          throw Arb::Errors::ScalarUnderflowSpecialized<T>().Number(ConstReferencePointer<Arb>(new Arb(*this))).Limit(std::numeric_limits<T>::denorm_min());
        }
        
        // We will have room, flag as denormalized.
        denorm = true;
        
      }
      
      // Shift p up until it has the same number of bits as q, then make sure
      // that 1 <= tmpnum < 2, unless this is a denormalized number. Set
      // exponent according to the number of bits shifted. If this is a
      // denormalized number, shift it the max we can with our max exponent.
      if (denorm) {
        tmpnum._data->p <<= _FloatInfo<T>::bias - 1;
        tmpexp            = 0;
      } else {
        tmpnum._data->p <<= bitdiff;
        tmpexp            = static_cast<unsigned int>(_FloatInfo<T>::bias - bitdiff);
        if (tmpnum._data->q > tmpnum._data->p) {
          if (tmpexp == 1) {
            --tmpexp;
          } else {
            tmpnum._data->p <<= 1;
            --tmpexp;
          }
        }
      }
      
    } else {
      
      // Get the difference in bits.
      ArbInt bitdiff = bits_p - bits_q;
      
      // If bitdiff > bias, we won't be able to store this number, it will
      // overflow the exponent.
      if (bitdiff > _FloatInfo<T>::bias) {
        throw Arb::Errors::ScalarOverflowSpecialized<T>().Number(ConstReferencePointer<Arb>(new Arb(*this))).Limit(std::numeric_limits<T>::max());
      }
      
      // Shift q up until it has the same number of bits as p, then make sure
      // that 1 <= tmpnum < 2. Set exponent according to the number of bits
      // shifted. If q is still > p, shift p up instead of q back down to
      // prevent the rare case of p & q have an equal number of bits to begin
      // with and then losing precision by shifting it down. Should be a very
      // rare occurance.
      tmpnum._data->q <<= bitdiff;
      tmpexp            = static_cast<unsigned int>(_FloatInfo<T>::bias + bitdiff);
      if (tmpnum._data->q > tmpnum._data->p) {
        tmpnum._data->p <<= 1;
        --tmpexp;
      }
      
    }
    
    // Convert tmpnum to x/bits in mantissa.
    tmpnum._forcereduce(ArbInt(1) << _FloatInfo<T>::mantissabits - 1);
    
    // We done. Set the exponent and return the fraction.
    exponent = tmpexp;
    return tmpnum._data->p;
    
  }
  
  /*
   * Assignment operator for _Data.
   */
  inline Arb::_Data& Arb::_Data::operator = (_Data const& data) { return copy(data); }
  
  /*
   * Determine number type.
   */
  template <class T> Arb::_NumType const Arb::_GetNumType<T>::value =
    std::numeric_limits<T>::is_specialized ? (
      std::numeric_limits<T>::is_integer ? (
        std::numeric_limits<T>::is_signed ? (
          _NUM_SINT
        ) : (
          _NUM_UINT
        )
      ) : (
        _NUM_FLPT
      )
    ) : (
      _NUM_UNKNOWN
    )
  ;
  
  /*
   * Set from an unsigned integer type.
   */
  template <class T> void Arb::_Set<T, Arb::_NUM_UINT>::op (Arb& l, SafeInt<T> const r) {
    
    // Work area.
    Arb new_num;
    
    // Carry over the old fixed-point porperties.
    new_num._data->fix      = l._data->fix;
    new_num._data->pointpos = l._data->pointpos;
    new_num._data->base     = l._data->base;
    new_num._data->fixq     = l._data->fixq;
    
    // This number is a whole number.
    new_num._data->p        = r;
    new_num._data->q        = 1;
    new_num._data->positive = true;
    
    // Reduce the fraction.
    new_num._reduce();
    
    // Move the new data into place and return.
    l._data = new_num._data;
    
  }
  template <class T> inline void Arb::_Set<T, Arb::_NUM_UINT>::op (Arb& l, T const r) { Arb::_Set<T, Arb::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Set from a signed integer type.
   */
  template <class T> void Arb::_Set<T, Arb::_NUM_SINT>::op (Arb& l, SafeInt<T> const r) {
    
    // Work area.
    Arb new_num;
    
    // Carry over the old fixed-point properties.
    new_num._data->fix      = l._data->fix;
    new_num._data->pointpos = l._data->pointpos;
    new_num._data->base     = l._data->base;
    new_num._data->fixq     = l._data->fixq;
    
    // This number is a whole number.
    new_num._data->p        = r.abs();
    new_num._data->q        = 1;
    new_num._data->positive = (r >= 0);
    
    // Reduce the fraction.
    new_num._reduce();
    
    // Move the new data into place and return.
    l._data = new_num._data;
    
  }
  template <class T> inline void Arb::_Set<T, Arb::_NUM_SINT>::op (Arb& l, T const r) { Arb::_Set<T, Arb::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Get as an unsigned integer.
   */
  template <class T> void Arb::_Get<T, Arb::_NUM_UINT>::op (SafeInt<T>& l, Arb const& r) {
    
    // If r is negative, error.
    if (r.isNegative()) {
      throw Arb::Errors::ScalarOverflowSpecialized<T>().Number(ConstReferencePointer<Arb>(new Arb(r))).Limit(0);
    }
    
    // Get the value the same way as a signed integer.
    Arb::_Get<T, Arb::_NUM_SINT>::op(l, r);
    
  }
  template <class T> inline void Arb::_Get<T, Arb::_NUM_UINT>::op (T& l, Arb const& r) { SafeInt<T> tmp; Arb::_Get<T, Arb::_NUM_UINT>::op(tmp, r); l = tmp; }
  
  /*
   * Get as a signed integer.
   */
  template <class T> void Arb::_Get<T, Arb::_NUM_SINT>::op (SafeInt<T>& l, Arb const& r) {
    
    // Make a temporary copy of r reduced to an integer.
    Arb tmp = r.toInt();
    
    // Catch and convert any errors.
    try {
      
      // If the number is positive, convert normally. If it is negative,
      // reduce the abs by 1 to avoid errors at ::min() then convert.
      if (r.isPositive()) {
        l = static_cast<T>(tmp._data->p);
      } else {
        l = -SafeInt<T>(static_cast<T>(tmp._data->p - 1)) - 1;
      }
      
    // Only error that should occur.
    } catch (ArbInt::Errors::ScalarOverflowSpecialized<T>& e) {
      throw Arb::Errors::ScalarOverflowSpecialized<T>().Number(ConstReferencePointer<Arb>(new Arb(r))).Limit(e.Limit());
    }
    
  }
  template <class T> inline void Arb::_Get<T, Arb::_NUM_SINT>::op (T& l, Arb const& r) { SafeInt<T> tmp; Arb::_Get<T, Arb::_NUM_SINT>::op(tmp, r); l = tmp; }
  
  /*
   * Multiply by an unsigned integer type.
   */
  template <class T> void Arb::_Mul<T, Arb::_NUM_UINT>::op (Arb& l, SafeInt<T> const r) {
    
    // Multiplying 0 is easy.
    if (l == 0) {
      return;
    }
    
    // Multiplying by 1 is also easy.
    if (r == 1) {
      return;
    }
    
    // Work area.
    Arb retval(l, true);
    
    // Multiply.
    retval._data->p *= r;
    
    // Reduce.
    retval._reduce();
    
    // Move the result in and return.
    l._data = retval._data;
    
  }
  template <class T> inline void Arb::_Mul<T, Arb::_NUM_UINT>::op (Arb& l, T const r) { Arb::_Mul<T, Arb::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Multiply by a signed integer type.
   */
  template <class T> void Arb::_Mul<T, Arb::_NUM_SINT>::op (Arb& l, SafeInt<T> const r) {
    
    // Work area.
    Arb retval(l, true);
    
    // Multiply by the abs.
    try {
      Arb::_Mul<T, _NUM_UINT>::op(retval, r.abs());
    } catch (typename SafeInt<T>::Errors::UnOpOverflow) {
      retval.op_mul(ArbInt(~r) + 1);
    }
    
    // Set the sign.
    retval._data->positive = (retval._data->positive == (r > 0));
    
    // Move result in and return.
    l._data = retval._data;
    
  }
  template <class T> inline void Arb::_Mul<T, Arb::_NUM_SINT>::op (Arb& l, T const r) { Arb::_Mul<T, Arb::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Multiply by a floating-point type.
   */
  template <class T> inline void Arb::_Mul<T, Arb::_NUM_FLPT>::op (Arb& l, T const r) { l.op_mul(Arb(r)); }
  
  /*
   * Divide by an unsigned integer type.
   */
  template <class T> void Arb::_Div<T, Arb::_NUM_UINT>::op (Arb& l, SafeInt<T> const r) {
    
    // Dividing 0 is easy.
    if (l == 0) {
      return;
    }
    
    // Dividing by 0 is verboten.
    if (r == 0) {
      throw Arb::Errors::DivByZeroBinary<Arb, T>().Left(l).Operator("/").Right(r);
    }
    
    // Dividing by 1 is also easy.
    if (r == 1) {
      return;
    }
    
    // Work area.
    Arb retval(l, true);
    
    // Divide.
    retval._data->q *= r;
    
    // Reduce.
    retval._reduce();
    
    // Move the result into place and return.
    l._data = retval._data;
  
  }
  template <class T> inline void Arb::_Div<T, Arb::_NUM_UINT>::op (Arb& l, T const r) { Arb::_Div<T, Arb::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Divide by a signed integer type.
   */
  template <class T> void Arb::_Div<T, Arb::_NUM_SINT>::op (Arb& l, SafeInt<T> const r) {
    
    // Work area.
    Arb retval(l, true);
    
    // Divide by the abs.
    try {
      Arb::_Div<T, _NUM_UINT>::op(retval, r.abs());
    } catch (typename SafeInt<T>::Errors::UnOpOverflow) {
      retval.op_div(ArbInt(~r) + 1);
    }
    
    // Set the sign.
    retval._data->positive = (retval._data->positive == (r > 0));
    
    // Move result in and return.
    l._data = retval._data;
    
  }
  template <class T> inline void Arb::_Div<T, Arb::_NUM_SINT>::op (Arb& l, T const r) { Arb::_Div<T, Arb::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Divide by a floating-point type.
   */
  template <class T> inline void Arb::_Div<T, Arb::_NUM_FLPT>::op (Arb& l, T const r) { l.op_div(Arb(r)); }
  
  /*
   * Modulo divide by an unsigned integer type.
   */
  template <class T> void Arb::_Mod<T, Arb::_NUM_UINT>::op (Arb& l, SafeInt<T> const r) {
    
    // Dividing 0 is easy.
    if (l == 0) {
      return;
    }
    
    // Dividing by 0 is verboten.
    if (r == 0) {
      throw Arb::Errors::DivByZeroBinary<Arb, T>().Left(l).Operator("%").Right(r);
    }
    
    // Throw an error if both numbers are not integer.
    if (!l.isInteger()) {
      throw Arb::Errors::NonIntegerBinary<Arb, T>().Left(l).Operator("%").Right(r);
    }
    
    // Work area.
    Arb retval(l, true);
    
    // Modulo divide p.
    retval._data->p %= r;
    
    // Done.
    l._data = retval._data;
    
  }
  template <class T> inline void Arb::_Mod<T, Arb::_NUM_UINT>::op (Arb& l, T const r) { Arb::_Mod<T, Arb::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Modulo divide by a signed integer type.
   */
  template <class T> inline void Arb::_Mod<T, Arb::_NUM_SINT>::op (Arb& l, SafeInt<T> const r) { Arb::_Mod<T, Arb::_NUM_UINT>::op(l, r); }
  template <class T> inline void Arb::_Mod<T, Arb::_NUM_SINT>::op (Arb& l, T const r) { Arb::_Mod<T, Arb::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Modulo divide by a floating-point type.
   */
  template <class T> inline void Arb::_Mod<T, Arb::_NUM_FLPT>::op (Arb& l, T const r) { l.op_mod(Arb(r)); }
  
  /*
   * Add an unsigned integer type.
   */
  template <class T> void Arb::_Add<T, Arb::_NUM_UINT>::op (Arb& l, SafeInt<T> const r) {
    
    // Adding to 0 is easy.
    if (l == 0) {
      l = r;
      return;
    }
    
    // Adding 0 is also easy.
    if (r == 0) {
      return;
    }
    
    // Work area.
    Arb retval(l, true);
    
    // If l is negative, subtract.
    if (retval < 0) {
      
      // Subtract the easy way if l is an integer, otherwise scale.
      if (retval.isInteger()) {
        if (r > retval._data->p) {
          retval._data->p        = r - retval._data->p;
          retval._data->positive = true;
        } else {
          retval._data->p -= r;
        }
      } else {
        ArbInt tmp(r * retval._data->q);
        if (tmp > retval._data->p) {
          retval._data->p        = tmp - retval._data->p;
          retval._data->positive = true;
        } else {
          retval._data->p -= tmp;
        }
        retval._reduce();
      }
      
    // Otherwise, add.
    } else {
      
      // Add the easy way if l is an integer, otherwise scale.
      if (retval.isInteger()) {
        retval._data->p += r;
      } else {
        retval._data->p += r * retval._data->q;
        retval._reduce();
      }
      
    }
    
    // Move the data in and return.
    l._data = retval._data;
    
  }
  template <class T> inline void Arb::_Add<T, Arb::_NUM_UINT>::op (Arb& l, T const r) { Arb::_Add<T, Arb::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Add a signed integer type.
   */
  template <class T> void Arb::_Add<T, Arb::_NUM_SINT>::op (Arb& l, SafeInt<T> const r) {
    
    // Adding to 0 is easy.
    if (l == 0) {
      l = r;
      return;
    }
    
    // Adding 0 is also easy.
    if (r == 0) {
      return;
    }
    
    // Work area.
    Arb retval(l, true);
    
    // Get the abs of r to do arithmetic with. In very rare instances, r will
    // be the min of a particular type, and experience overflow when negating.
    // Catch this error and resort to Abs addition.
    SafeInt<T> rabs;
    try {
      rabs = r.abs();
    } catch (typename SafeInt<T>::Errors::UnOpOverflow) {
      // Rethrow the error if we're not here for the reason this hack is
      // intended for.
      if (r != std::numeric_limits<T>::min()) {
        throw;
      }
      retval.op_add(Arb(r));
    }
    
    // If signs do not match, subtract.
    if ((retval < 0) != (r < 0)) {
      
      // Subtract the easy way if l is an integer, otherwise scale.
      if (retval.isInteger()) {
        if (rabs > retval._data->p) {
          retval._data->p        = rabs - retval._data->p;
          retval._data->positive = !retval._data->positive;
        } else {
          retval._data->p -= rabs;
        }
      } else {
        ArbInt tmp(rabs * retval._data->q);
        if (tmp > retval._data->p) {
          retval._data->p        = tmp - retval._data->p;
          retval._data->positive = !retval._data->positive;
        } else {
          retval._data->p -= tmp;
        }
        retval._reduce();
      }
      
    // Otherwise, add normally.
    } else {
      
      // Add the easy way if l is integer, otherwise scale.
      if (retval.isInteger()) {
        retval._data->p += rabs;
      } else {
        retval._data->p += rabs * retval._data->q;
        retval._reduce();
      }
      
    }
    
    // Move the data in and return.
    l._data = retval._data;
    
  }
  template <class T> inline void Arb::_Add<T, Arb::_NUM_SINT>::op (Arb& l, T const r) { Arb::_Add<T, Arb::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Add a floating-point type.
   */
  template <class T> inline void Arb::_Add<T, Arb::_NUM_FLPT>::op (Arb& l, T const r) { l.op_add(Arb(r)); }
  
  /*
   * Subtract an unsigned integer type.
   */
  template <class T> void Arb::_Sub<T, Arb::_NUM_UINT>::op (Arb& l, SafeInt<T> const r) {
    
    // Subtracting 0 is easy.
    if (r == 0) {
      return;
    }
    
    // Subtracting from 0 is also easy.
    if (l == 0) {
      Arb retval(l, true);
      retval = r;
      retval._data->positive = !retval._data->positive;
      l._data = retval._data;
      return;
    }
    
    // Work area.
    Arb retval(l, true);
    
    // If l is negative, this is addition.
    if (retval < 0) {
      
      // Scale if necessary and add.
      if (retval.isInteger()) {
        retval._data->p += r;
      } else {
        retval._data->p += r * retval._data->q;
        retval._reduce();
      }
    
    // Otherwise, subtract.
    } else {
      
      // Subtract.
      if (retval.isInteger()) {
        if (r > retval._data->p) {
          retval._data->p        = r - retval._data->p;
          retval._data->positive = !retval._data->positive;
        } else {
          retval._data->p -= r;
        }
      } else {
        ArbInt tmp(r * retval._data->q);
        if (tmp > retval._data->p) {
          retval._data->p        = tmp - retval._data->p;
          retval._data->positive = !retval._data->positive;
        } else {
          retval._data->p -= tmp;
        }
        retval._reduce();
      }
      
    }
    
    // Move the result in and return.
    l._data = retval._data;
    
  }
  template <class T> inline void Arb::_Sub<T, Arb::_NUM_UINT>::op (Arb& l, T const r) { Arb::_Sub<T, Arb::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Subtract a signed integer type.
   */
  template <class T> void Arb::_Sub<T, Arb::_NUM_SINT>::op (Arb& l, SafeInt<T> const r) {
    
    // Subtracting 0 is easy.
    if (r == 0) {
      return;
    }
    
    // Subtracting from 0 is also easy.
    if (l == 0) {
      Arb retval(l, true);
      retval = r;
      retval._data->positive = !retval._data->positive;
      l._data = retval._data;
      return;
    }
    
    // Work area.
    Arb retval(l, true);
    
    // Get the abs of r to do arithmetic with. In very rare instances, r will
    // be the min of a particular type, and experience overflow when negating.
    // Catch this error and resort to Abs subtraction.
    SafeInt<T> rabs;
    try {
      rabs = r.abs();
    } catch (typename SafeInt<T>::Errors::UnOpOverflow) {
      // Rethrow the error if we're not here for the reason this hack is
      // intended for.
      if (r != std::numeric_limits<T>::min()) {
        throw;
      }
      retval.op_sub(Arb(r));
    }
    
    // If signs do not match, add.
    if ((retval < 0) != (r < 0)) {
      
      // Scale if necessary and add.
      if (retval.isInteger()) {
        retval._data->p += rabs;
      } else {
        retval._data->p += rabs * retval._data->q;
        retval._reduce();
      }
      
    } else {
      
      // Subtract.
      if (retval.isInteger()) {
        if (rabs > retval._data->p) {
          retval._data->p        = rabs - retval._data->p;
          retval._data->positive = !retval._data->positive;
        } else {
          retval._data->p -= rabs;
        }
      } else {
        ArbInt tmp(rabs * retval._data->q);
        if (tmp > retval._data->p) {
          retval._data->p        = tmp - retval._data->p;
          retval._data->positive = !retval._data->positive;
        } else {
          retval._data->p -= tmp;
        }
        retval._reduce();
      }
      
    }
    
    // Move the result in and return.
    l._data = retval._data;
    
  }
  template <class T> inline void Arb::_Sub<T, Arb::_NUM_SINT>::op (Arb& l, T const r) { Arb::_Sub<T, Arb::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Subtract a floating-point type.
   */
  template <class T> inline void Arb::_Sub<T, Arb::_NUM_FLPT>::op (Arb& l, T const r) { l.op_sub(Arb(r)); }
  
  /*
   * Shift by an unsigned integer type.
   */
  template <class T> void Arb::_Shift<T, Arb::_NUM_UINT>::op (Arb& l, SafeInt<T> const r, _Dir const dir) {
    
    // Only work if necessary.
    if (!l || !r) {
      return;
    }
    
    // Shift left by left-shifting p. Shift right by left-shifting q.
    if (dir == _DIR_L) {
      l._data->p <<= r;
    } else {
      l._data->q <<= r;
    }
    
    // Reduce the fraction and we're done.
    l._reduce();
    
  }
  template <class T> inline void Arb::_Shift<T, Arb::_NUM_UINT>::op (Arb& l, T const r, _Dir const dir) { Arb::_Shift<T, Arb::_NUM_UINT>::op(l, SafeInt<T>(r), dir); }
  
  /*
   * Shift by a signed integer type.
   */
  template <class T> void Arb::_Shift<T, Arb::_NUM_SINT>::op (Arb& l, SafeInt<T> const r, _Dir const dir) {
    
    // Reverse the shift direction and r if r is negative.
    if (r < 0) {
      try {
        Arb::_Shift<T, Arb::_NUM_UINT>::op(l, -r, dir == _DIR_L ? _DIR_R : _DIR_L);
      } catch (typename SafeInt<T>::Errors::UnOpOverflow) {
        l._shift(Arb(~r) + 1, dir == _DIR_L ? _DIR_R : _DIR_L);
      }
    } else {
      Arb::_Shift<T, Arb::_NUM_UINT>::op(l, r, dir);
    }
    
  }
  template <class T> inline void Arb::_Shift<T, Arb::_NUM_SINT>::op (Arb& l, T const r, _Dir const dir) { Arb::_Shift<T, Arb::_NUM_SINT>::op(l, SafeInt<T>(r), dir); }
  
  /*
   * Shift by a floating-point type.
   */
  template <class T> inline void Arb::_Shift<T, Arb::_NUM_FLPT>::op (Arb& l, T const r, _Dir const dir) { l._shift(Arb(r), dir); }
  
  /*
   * Shift left.
   */
  template <class T> inline void Arb::_ShL<T, Arb::_NUM_UINT>::op (Arb& l, SafeInt<T> const r) { Arb::_Shift<T, Arb::_NUM_UINT>::op(l, r, _DIR_L); }
  template <class T> inline void Arb::_ShL<T, Arb::_NUM_UINT>::op (Arb& l, T          const r) { Arb::_Shift<T, Arb::_NUM_UINT>::op(l, r, _DIR_L); }
  template <class T> inline void Arb::_ShL<T, Arb::_NUM_SINT>::op (Arb& l, SafeInt<T> const r) { Arb::_Shift<T, Arb::_NUM_SINT>::op(l, r, _DIR_L); }
  template <class T> inline void Arb::_ShL<T, Arb::_NUM_SINT>::op (Arb& l, T          const r) { Arb::_Shift<T, Arb::_NUM_SINT>::op(l, r, _DIR_L); }
  template <class T> inline void Arb::_ShL<T, Arb::_NUM_FLPT>::op (Arb& l, T          const r) { Arb::_Shift<T, Arb::_NUM_FLPT>::op(l, r, _DIR_L); }
  
  /*
   * Shift right.
   */
  template <class T> inline void Arb::_ShR<T, Arb::_NUM_UINT>::op (Arb& l, SafeInt<T> const r) { Arb::_Shift<T, Arb::_NUM_UINT>::op(l, r, _DIR_R); }
  template <class T> inline void Arb::_ShR<T, Arb::_NUM_UINT>::op (Arb& l, T          const r) { Arb::_Shift<T, Arb::_NUM_UINT>::op(l, r, _DIR_R); }
  template <class T> inline void Arb::_ShR<T, Arb::_NUM_SINT>::op (Arb& l, SafeInt<T> const r) { Arb::_Shift<T, Arb::_NUM_SINT>::op(l, r, _DIR_R); }
  template <class T> inline void Arb::_ShR<T, Arb::_NUM_SINT>::op (Arb& l, T          const r) { Arb::_Shift<T, Arb::_NUM_SINT>::op(l, r, _DIR_R); }
  template <class T> inline void Arb::_ShR<T, Arb::_NUM_FLPT>::op (Arb& l, T          const r) { Arb::_Shift<T, Arb::_NUM_FLPT>::op(l, r, _DIR_R); }
  
  /*
   * Greater than an unsigned integer type.
   */
  template <class T> bool Arb::_GT<T, Arb::_NUM_UINT>::op (Arb const& l, SafeInt<T> const r) {
    
    // Check for zeros.
    if (l._data->p == 0) {
      return false;
    } else if (r == 0) {
      return l._data->positive;
    }
    
    // Check signs.
    if (!l._data->positive) {
      return false;
    }
    
    // Compare the number.
    if (l.isInteger()) {
      return (l._data->p > r);
    } else {
      return (l._data->p > r * l._data->q);
    }
    
  }
  template <class T> inline bool Arb::_GT<T, Arb::_NUM_UINT>::op (Arb const& l, T const r) { return Arb::_GT<T, Arb::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Greater than a signed integer type.
   */
  template <class T> bool Arb::_GT<T, Arb::_NUM_SINT>::op (Arb const& l, SafeInt<T> const r) {
    
    // Check for zeros.
    if (l._data->p == 0) {
      if (r == 0) {
        return false;
      } else {
        return (r < 0);
      }
    } else if (r == 0) {
      return l._data->positive;
    }
    
    // Check signs.
    if (l._data->positive) {
      if (r < 0) {
        return true;
      }
    } else {
      if (r > 0) {
        return false;
      }
    }
    
    // Compare the number.
    if (l.isInteger()) {
      if (l._data->positive) {
        return (l._data->p > r);
      } else {
        return (l._data->p < r);
      }
    } else {
      if (l._data->positive) {
        return (l._data->p > r * l._data->q);
      } else {
        return (l._data->p < r * l._data->q);
      }
    }
    
  }
  template <class T> inline bool Arb::_GT<T, Arb::_NUM_SINT>::op (Arb const& l, T const r) { return Arb::_GT<T, Arb::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Greater than a floating-point type.
   */
  template <class T> inline bool Arb::_GT<T, Arb::_NUM_FLPT>::op (Arb const& l, T const r) { return l.op_gt(Arb(r)); }
  
  /*
   * Greater than or equal to an unsigned integer type.
   */
  template <class T> inline bool Arb::_GE<T, Arb::_NUM_UINT>::op (Arb const& l, SafeInt<T> const r) { return !Arb::_LT<T, Arb::_NUM_UINT>::op(l, r); }
  template <class T> inline bool Arb::_GE<T, Arb::_NUM_UINT>::op (Arb const& l, T const r) { return Arb::_GE<T, Arb::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Greater than or equal to a signed integer type.
   */
  template <class T> inline bool Arb::_GE<T, Arb::_NUM_SINT>::op (Arb const& l, SafeInt<T> const r) { return !Arb::_LT<T, Arb::_NUM_SINT>::op(l, r); }
  template <class T> inline bool Arb::_GE<T, Arb::_NUM_SINT>::op (Arb const& l, T const r) { return Arb::_GE<T, Arb::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Greater than or equal to a floating-point type.
   */
  template <class T> inline bool Arb::_GE<T, Arb::_NUM_FLPT>::op (Arb const& l, T const r) { return l.op_ge(Arb(r)); }
  
  /*
   * Less than an unsigned integer type.
   */
  template <class T> bool Arb::_LT<T, Arb::_NUM_UINT>::op (Arb const& l, SafeInt<T> const r) {
    
    // Check for zeros.
    if (l._data->p == 0) {
      return (r > 0);
    } else if (r == 0) {
      return !l._data->positive;
    }
    
    // Check signs.
    if (!l._data->positive) {
      return true;
    }
    
    // Compare the number.
    if (l.isInteger()) {
      return (l._data->p < r);
    } else {
      return (l._data->p < r * l._data->q);
    }
    
  }
  template <class T> inline bool Arb::_LT<T, Arb::_NUM_UINT>::op (Arb const& l, T const r) { return Arb::_LT<T, Arb::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Less than a signed integer type.
   */
  template <class T> bool Arb::_LT<T, Arb::_NUM_SINT>::op (Arb const& l, SafeInt<T> const r) {
    
    // Check for zeros.
    if (l._data->p == 0) {
      if (r == 0) {
        return false;
      } else {
        return (r > 0);
      }
    } else if (r == 0) {
      return !l._data->positive;
    }
    
    // Check signs.
    if (l._data->positive) {
      if (r < 0) {
        return false;
      }
    } else {
      if (r > 0) {
        return true;
      }
    }
    
    // Compare the number.
    if (l.isInteger()) {
      if (l._data->positive) {
        return (l._data->p < r);
      } else {
        return (l._data->p > r);
      }
    } else {
      if (l._data->positive) {
        return (l._data->p < r * l._data->q);
      } else {
        return (l._data->p > r * l._data->q);
      }
    }
    
  }
  template <class T> inline bool Arb::_LT<T, Arb::_NUM_SINT>::op (Arb const& l, T const r) { return Arb::_LT<T, Arb::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Greater than a floating-point type.
   */
  template <class T> inline bool Arb::_LT<T, Arb::_NUM_FLPT>::op (Arb const& l, T const r) { return l.op_lt(Arb(r)); }
  
  /*
   * Less than or equal to an unsigned integer type.
   */
  template <class T> inline bool Arb::_LE<T, Arb::_NUM_UINT>::op (Arb const& l, SafeInt<T> const r) { return !Arb::_GT<T, Arb::_NUM_UINT>::op(l, r); }
  template <class T> inline bool Arb::_LE<T, Arb::_NUM_UINT>::op (Arb const& l, T const r) { return Arb::_LE<T, Arb::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Less than or equal to a signed integer type.
   */
  template <class T> inline bool Arb::_LE<T, Arb::_NUM_SINT>::op (Arb const& l, SafeInt<T> const r) { return !Arb::_GT<T, Arb::_NUM_SINT>::op(l, r); }
  template <class T> inline bool Arb::_LE<T, Arb::_NUM_SINT>::op (Arb const& l, T const r) { return Arb::_LE<T, Arb::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Less than or equal to a floating-point type.
   */
  template <class T> inline bool Arb::_LE<T, Arb::_NUM_FLPT>::op (Arb const& l, T const r) { return l.op_le(Arb(r)); }
  
  /*
   * Equal to an unsigned integer type.
   */
  template <class T> bool Arb::_EQ<T, Arb::_NUM_UINT>::op (Arb const& l, SafeInt<T> const r) {
    
    // Check for zeros.
    if (l._data->p == 0) {
      return (r == 0);
    } else if (r == 0) {
      return false;
    }
    
    // Neither number is zero, check signs.
    if (!l._data->positive) {
      return false;
    }
    
    // Check numbers.
    if (l.isInteger()) {
      return (l._data->p == r);
    } else {
      return (l._data->p == r * l._data->q);
    }
    
  }
  template <class T> inline bool Arb::_EQ<T, Arb::_NUM_UINT>::op (Arb const& l, T const r) { return Arb::_EQ<T, Arb::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Equal to a signed integer type.
   */
  template <class T> bool Arb::_EQ<T, Arb::_NUM_SINT>::op (Arb const& l, SafeInt<T> const r) {
    
    // Check for zeros.
    if (l._data->p == 0) {
      return (r == 0);
    } else if (r == 0) {
      return false;
    }
    
    // Neither number is zero, check signs.
    if (l._data->positive != (r > 0)) {
      return false;
    }
    
    // Check numbers.
    if (l.isInteger()) {
      return (l._data->p == r);
    } else {
      return (l._data->p == r * l._data->q);
    }
    
  }
  template <class T> inline bool Arb::_EQ<T, Arb::_NUM_SINT>::op (Arb const& l, T const r) { return Arb::_EQ<T, Arb::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Equal to a floating-point type.
   */
  template <class T> inline bool Arb::_EQ<T, Arb::_NUM_FLPT>::op (Arb const& l, T const r) { return l.op_eq(Arb(r)); }
  
  /*
   * Not equal to an unsigned integer type.
   */
  template <class T> inline bool Arb::_NE<T, Arb::_NUM_UINT>::op (Arb const& l, SafeInt<T> const r) { return !Arb::_EQ<T, Arb::_NUM_UINT>::op(l, r); }
  template <class T> inline bool Arb::_NE<T, Arb::_NUM_UINT>::op (Arb const& l, T const r) { return Arb::_NE<T, Arb::_NUM_UINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Not equal to a signed integer type.
   */
  template <class T> inline bool Arb::_NE<T, Arb::_NUM_SINT>::op (Arb const& l, SafeInt<T> const r) { return !Arb::_EQ<T, Arb::_NUM_SINT>::op(l, r); }
  template <class T> inline bool Arb::_NE<T, Arb::_NUM_SINT>::op (Arb const& l, T const r) { return Arb::_NE<T, Arb::_NUM_SINT>::op(l, SafeInt<T>(r)); }
  
  /*
   * Not equal to a floating-point type.
   */
  template <class T> inline bool Arb::_NE<T, Arb::_NUM_FLPT>::op (Arb const& l, T const r) { return l.op_ne(Arb(r)); }
  
  /***************************************************************************
   * Operators.
   ***************************************************************************/
  
  /*
   * Stream I/O operators.
   */
  inline std::ostream&       operator << (std::ostream&       l, Arb  const& r) { l << r.toString();                                  return l; }
  inline std::ostringstream& operator << (std::ostringstream& l, Arb  const& r) { l << r.toString();                                  return l; }
  inline std::istream&       operator >> (std::istream&       l, Arb&        r) { std::string input; std::cin >> input; r.set(input); return l; }
  
  /*
   * Arithmetic operators.
   */
                     inline Arb operator * (Arb        const& l, Arb        const& r) { return Arb(l).op_mul(r);    }
                     inline Arb operator * (Arb        const& l, ArbInt     const& r) { return Arb(l).op_mul(r);    }
                     inline Arb operator * (ArbInt     const& l, Arb        const& r) { return Arb(r).op_mul(l);    }
  template <class T> inline Arb operator * (Arb        const& l, SafeInt<T> const  r) { return Arb(l).op_mul(r);    }
  template <class T> inline Arb operator * (SafeInt<T> const  l, Arb        const& r) { return Arb(r).op_mul(l);    }
  template <class T> inline Arb operator * (Arb        const& l, T          const  r) { return Arb(l).op_mul(r);    }
  template <class T> inline Arb operator * (T          const  l, Arb        const& r) { return Arb(r).op_mul(l);    }
                     inline Arb operator / (Arb        const& l, Arb        const& r) { return Arb(l).op_div(r);    }
                     inline Arb operator / (Arb        const& l, ArbInt     const& r) { return Arb(l).op_div(r);    }
                     inline Arb operator / (ArbInt     const& l, Arb        const& r) { return Arb(l).op_div(r);    }
  template <class T> inline Arb operator / (Arb        const& l, SafeInt<T> const  r) { return Arb(l).op_div(r);    }
  template <class T> inline Arb operator / (SafeInt<T> const  l, Arb        const& r) { return Arb(l).op_div(r);    }
  template <class T> inline Arb operator / (Arb        const& l, T          const  r) { return Arb(l).op_div(r);    }
  template <class T> inline Arb operator / (T          const  l, Arb        const& r) { return Arb(l).op_div(r);    }
                     inline Arb operator % (Arb        const& l, Arb        const& r) { return Arb(l).op_mod(r);    }
                     inline Arb operator % (Arb        const& l, ArbInt     const& r) { return Arb(l).op_mod(r);    }
                     inline Arb operator % (ArbInt     const& l, Arb        const& r) { return Arb(l).op_mod(r);    }
  template <class T> inline Arb operator % (Arb        const& l, SafeInt<T> const  r) { return Arb(l).op_mod(r);    }
  template <class T> inline Arb operator % (SafeInt<T> const  l, Arb        const& r) { return Arb(l).op_mod(r);    }
  template <class T> inline Arb operator % (Arb        const& l, T          const  r) { return Arb(l).op_mod(r);    }
  template <class T> inline Arb operator % (T          const  l, Arb        const& r) { return Arb(l).op_mod(r);    }
                     inline Arb operator + (Arb        const& l, Arb        const& r) { return Arb(l).op_add(r);    }
                     inline Arb operator + (Arb        const& l, ArbInt     const& r) { return Arb(l).op_add(r);    }
                     inline Arb operator + (ArbInt     const& l, Arb        const& r) { return Arb(r).op_add(l);    }
  template <class T> inline Arb operator + (Arb        const& l, SafeInt<T> const  r) { return Arb(l).op_add(r);    }
  template <class T> inline Arb operator + (SafeInt<T> const  l, Arb        const& r) { return Arb(r).op_add(l);    }
  template <class T> inline Arb operator + (Arb        const& l, T          const  r) { return Arb(l).op_add(r);    }
  template <class T> inline Arb operator + (T          const  l, Arb        const& r) { return Arb(r).op_add(l);    }
                     inline Arb operator - (Arb        const& l, Arb        const& r) { return Arb(l).op_sub(r);    }
                     inline Arb operator - (Arb        const& l, ArbInt     const& r) { return Arb(l).op_sub(r);    }
                     inline Arb operator - (ArbInt     const& l, Arb        const& r) { return -(Arb(r).op_sub(l)); }
  template <class T> inline Arb operator - (Arb        const& l, SafeInt<T> const  r) { return Arb(l).op_sub(r);    }
  template <class T> inline Arb operator - (SafeInt<T> const  l, Arb        const& r) { return -(Arb(r).op_sub(l)); }
  template <class T> inline Arb operator - (Arb        const& l, T          const  r) { return Arb(l).op_sub(r);    }
  template <class T> inline Arb operator - (T          const  l, Arb        const& r) { return -(Arb(r).op_sub(l)); }
  
  /*
   * Bit shift operators.
   */
                     inline Arb operator << (Arb        const& l, Arb        const& r) { return Arb(l).op_shl(r); }
                     inline Arb operator << (Arb        const& l, ArbInt     const& r) { return Arb(l).op_shl(r); }
                     inline Arb operator << (ArbInt     const& l, Arb        const& r) { return Arb(l).op_shl(r); }
  template <class T> inline Arb operator << (Arb        const& l, SafeInt<T> const  r) { return Arb(l).op_shl(r); }
  template <class T> inline Arb operator << (SafeInt<T> const  l, Arb        const& r) { return Arb(l).op_shl(r); }
  template <class T> inline Arb operator << (Arb        const& l, T          const  r) { return Arb(l).op_shl(r); }
  template <class T> inline Arb operator << (T          const  l, Arb        const& r) { return Arb(l).op_shl(r); }
                     inline Arb operator >> (Arb        const& l, Arb        const& r) { return Arb(l).op_shr(r); }
                     inline Arb operator >> (Arb        const& l, ArbInt     const& r) { return Arb(l).op_shr(r); }
                     inline Arb operator >> (ArbInt     const& l, Arb        const& r) { return Arb(l).op_shr(r); }
  template <class T> inline Arb operator >> (Arb        const& l, SafeInt<T> const  r) { return Arb(l).op_shr(r); }
  template <class T> inline Arb operator >> (SafeInt<T> const  l, Arb        const& r) { return Arb(l).op_shr(r); }
  template <class T> inline Arb operator >> (Arb        const& l, T          const  r) { return Arb(l).op_shr(r); }
  template <class T> inline Arb operator >> (T          const  l, Arb        const& r) { return Arb(l).op_shr(r); }
  
  /*
   * Comparison operators.
   */
                     inline bool operator >  (Arb        const& l, Arb        const& r) { return  l.op_gt(r); }
                     inline bool operator >  (Arb        const& l, ArbInt     const& r) { return  l.op_gt(r); }
                     inline bool operator >  (ArbInt     const& l, Arb        const& r) { return !r.op_ge(l); }
  template <class T> inline bool operator >  (Arb        const& l, SafeInt<T> const  r) { return  l.op_gt(r); }
  template <class T> inline bool operator >  (SafeInt<T> const  l, Arb        const& r) { return !r.op_ge(l); }
  template <class T> inline bool operator >  (Arb        const& l, T          const  r) { return  l.op_gt(r); }
  template <class T> inline bool operator >  (T          const  l, Arb        const& r) { return !r.op_ge(l); }
                     inline bool operator >= (Arb        const& l, Arb        const& r) { return  l.op_ge(r); }
                     inline bool operator >= (Arb        const& l, ArbInt     const& r) { return  l.op_ge(r); }
                     inline bool operator >= (ArbInt     const& l, Arb        const& r) { return !r.op_gt(l); }
  template <class T> inline bool operator >= (Arb        const& l, SafeInt<T> const  r) { return  l.op_ge(r); }
  template <class T> inline bool operator >= (SafeInt<T> const  l, Arb        const& r) { return !r.op_gt(l); }
  template <class T> inline bool operator >= (Arb        const& l, T          const  r) { return  l.op_ge(r); }
  template <class T> inline bool operator >= (T          const  l, Arb        const& r) { return !r.op_gt(l); }
                     inline bool operator <  (Arb        const& l, Arb        const& r) { return  l.op_lt(r); }
                     inline bool operator <  (Arb        const& l, ArbInt     const& r) { return  l.op_lt(r); }
                     inline bool operator <  (ArbInt     const& l, Arb        const& r) { return !r.op_le(l); }
  template <class T> inline bool operator <  (Arb        const& l, SafeInt<T> const  r) { return  l.op_lt(r); }
  template <class T> inline bool operator <  (SafeInt<T> const  l, Arb        const& r) { return !r.op_le(l); }
  template <class T> inline bool operator <  (Arb        const& l, T          const  r) { return  l.op_lt(r); }
  template <class T> inline bool operator <  (T          const  l, Arb        const& r) { return !r.op_le(l); }
                     inline bool operator <= (Arb        const& l, Arb        const& r) { return  l.op_le(r); }
                     inline bool operator <= (Arb        const& l, ArbInt     const& r) { return  l.op_le(r); }
                     inline bool operator <= (ArbInt     const& l, Arb        const& r) { return !r.op_lt(l); }
  template <class T> inline bool operator <= (Arb        const& l, SafeInt<T> const  r) { return  l.op_le(r); }
  template <class T> inline bool operator <= (SafeInt<T> const  l, Arb        const& r) { return !r.op_lt(l); }
  template <class T> inline bool operator <= (Arb        const& l, T          const  r) { return  l.op_le(r); }
  template <class T> inline bool operator <= (T          const  l, Arb        const& r) { return !r.op_lt(l); }
                     inline bool operator == (Arb        const& l, Arb        const& r) { return  l.op_eq(r); }
                     inline bool operator == (Arb        const& l, ArbInt     const& r) { return  l.op_eq(r); }
                     inline bool operator == (ArbInt     const& l, Arb        const& r) { return  r.op_eq(l); }
  template <class T> inline bool operator == (Arb        const& l, SafeInt<T> const  r) { return  l.op_eq(r); }
  template <class T> inline bool operator == (SafeInt<T> const  l, Arb        const& r) { return  r.op_eq(l); }
  template <class T> inline bool operator == (Arb        const& l, T          const  r) { return  l.op_eq(r); }
  template <class T> inline bool operator == (T          const  l, Arb        const& r) { return  r.op_eq(l); }
                     inline bool operator != (Arb        const& l, Arb        const& r) { return  l.op_ne(r); }
                     inline bool operator != (Arb        const& l, ArbInt     const& r) { return  l.op_ne(r); }
                     inline bool operator != (ArbInt     const& l, Arb        const& r) { return  r.op_ne(l); }
  template <class T> inline bool operator != (Arb        const& l, SafeInt<T> const  r) { return  l.op_ne(r); }
  template <class T> inline bool operator != (SafeInt<T> const  l, Arb        const& r) { return  r.op_ne(l); }
  template <class T> inline bool operator != (Arb        const& l, T          const  r) { return  l.op_ne(r); }
  template <class T> inline bool operator != (T          const  l, Arb        const& r) { return  r.op_ne(l); }
  
  /*
   * Arithmetic assignment operators.
   */
                     inline Arb&        operator *= (Arb&        l, Arb        const& r) { return l.op_mul(r);                             }
                     inline Arb&        operator *= (Arb&        l, ArbInt     const& r) { return l.op_mul(r);                             }
                     inline ArbInt&     operator *= (ArbInt&     l, Arb        const& r) { l = Arb(r).op_mul(l).toArbInt(); return l;      }
  template <class T> inline Arb&        operator *= (Arb&        l, SafeInt<T> const  r) { return l.op_mul(r);                             }
  template <class T> inline SafeInt<T>& operator *= (SafeInt<T>& l, Arb        const& r) { l = static_cast<T>(Arb(r).op_mul(l)); return l; }
  template <class T> inline Arb&        operator *= (Arb&        l, T          const  r) { return l.op_mul(r);                             }
  template <class T> inline T&          operator *= (T&          l, Arb        const& r) { l = Arb(r).op_mul(l); return l;                 }
                     inline Arb&        operator /= (Arb&        l, Arb        const& r) { return l.op_div(r);                             }
                     inline Arb&        operator /= (Arb&        l, ArbInt     const& r) { return l.op_div(r);                             }
                     inline ArbInt&     operator /= (ArbInt&     l, Arb        const& r) { l = Arb(l).op_div(r).toArbInt(); return l;      }
  template <class T> inline Arb&        operator /= (Arb&        l, SafeInt<T> const  r) { return l.op_div(r);                             }
  template <class T> inline SafeInt<T>& operator /= (SafeInt<T>& l, Arb        const& r) { l = static_cast<T>(Arb(l).op_div(r)); return l; }
  template <class T> inline Arb&        operator /= (Arb&        l, T          const  r) { return l.op_div(r);                             }
  template <class T> inline T&          operator /= (T&          l, Arb        const& r) { l = Arb(l).op_div(r); return l;                 }
                     inline Arb&        operator %= (Arb&        l, Arb        const& r) { return l.op_mod(r);                             }
                     inline Arb&        operator %= (Arb&        l, ArbInt     const& r) { return l.op_mod(r);                             }
                     inline ArbInt&     operator %= (ArbInt&     l, Arb        const& r) { l = Arb(l).op_mod(r).toArbInt(); return l;      }
  template <class T> inline Arb&        operator %= (Arb&        l, SafeInt<T> const  r) { return l.op_mod(r);                             }
  template <class T> inline SafeInt<T>& operator %= (SafeInt<T>& l, Arb        const& r) { l = static_cast<T>(Arb(l).op_mod(r)); return l; }
  template <class T> inline Arb&        operator %= (Arb&        l, T          const  r) { return l.op_mod(r);                             }
  template <class T> inline T&          operator %= (T&          l, Arb        const& r) { l = Arb(l).op_mod(r); return l;                 }
                     inline Arb&        operator += (Arb&        l, Arb        const& r) { return l.op_add(r);                             }
                     inline Arb&        operator += (Arb&        l, ArbInt     const& r) { return l.op_add(r);                             }
                     inline ArbInt&     operator += (ArbInt&     l, Arb        const& r) { l = Arb(r).op_add(l).toArbInt(); return l;      }
  template <class T> inline Arb&        operator += (Arb&        l, SafeInt<T> const  r) { return l.op_add(r);                             }
  template <class T> inline SafeInt<T>& operator += (SafeInt<T>& l, Arb        const& r) { l = static_cast<T>(Arb(r).op_add(l)); return l; }
  template <class T> inline Arb&        operator += (Arb&        l, T          const  r) { return l.op_add(r);                             }
  template <class T> inline T&          operator += (T&          l, Arb        const& r) { l = Arb(r).op_add(l); return l;                 }
                     inline Arb&        operator -= (Arb&        l, Arb        const& r) { return l.op_sub(r);                             }
                     inline Arb&        operator -= (Arb&        l, ArbInt     const& r) { return l.op_sub(r);                             }
                     inline ArbInt&     operator -= (ArbInt&     l, Arb        const& r) { l = Arb(l).op_sub(r).toArbInt(); return l;      }
  template <class T> inline Arb&        operator -= (Arb&        l, SafeInt<T> const  r) { return l.op_sub(r);                             }
  template <class T> inline SafeInt<T>& operator -= (SafeInt<T>& l, Arb        const& r) { l = static_cast<T>(Arb(l).op_sub(r)); return l; }
  template <class T> inline Arb&        operator -= (Arb&        l, T          const  r) { return l.op_sub(r);                             }
  template <class T> inline T&          operator -= (T&          l, Arb        const& r) { l = Arb(l).op_sub(r); return l;                 }
  
  /*
   * Bit shift assignment operators.
   */
                     inline Arb&        operator <<= (Arb&        l, Arb        const& r) { return l.op_shl(r);                             }
                     inline Arb&        operator <<= (Arb&        l, ArbInt     const& r) { return l.op_shl(r);                             }
                     inline ArbInt&     operator <<= (ArbInt&     l, Arb        const& r) { l = Arb(l).op_shl(r).toArbInt(); return l;      }
  template <class T> inline Arb&        operator <<= (Arb&        l, SafeInt<T> const  r) { return l.op_shl(r);                             }
  template <class T> inline SafeInt<T>& operator <<= (SafeInt<T>& l, Arb        const& r) { l = static_cast<T>(Arb(l).op_shl(r)); return l; }
  template <class T> inline Arb&        operator <<= (Arb&        l, T          const  r) { return l.op_shl(r);                             }
  template <class T> inline T&          operator <<= (T&          l, Arb        const& r) { l = Arb(l).op_shl(r); return l;                 }
                     inline Arb&        operator >>= (Arb&        l, Arb        const& r) { return l.op_shr(r);                             }
                     inline Arb&        operator >>= (Arb&        l, ArbInt     const& r) { return l.op_shr(r);                             }
                     inline ArbInt&     operator >>= (ArbInt&     l, Arb        const& r) { l = Arb(l).op_shr(r).toArbInt(); return l;      }
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
