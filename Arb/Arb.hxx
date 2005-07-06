/*****************************************************************************
 * Arb.hxx
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

// Internal includes.
  #include "SafeInt.hxx"
  #include "ReferencePointer.hxx"
  #include "toString.hxx"
  #include "ArbInt.hxx"
  #include "abs.hxx"

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
        ROUND_UP,
        ROUND_DOWN,
        ROUND_TOWARD_ZERO,
        ROUND_FROM_ZERO,
        ROUND_DEFAULT
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
      
      // Assignment operator.
                         Arb& operator = (Arb         const& number);
                         Arb& operator = (std::string const& number);
                         Arb& operator = (ArbInt      const& number);
      template <class T> Arb& operator = (T           const  number);
      
      // Accessors.
                         Arb& Base     (BaseT                  const base);
                         Arb& MaxRadix (std::string::size_type const maxradix);
                         Arb& PointPos (PointPosT              const pointpos);
                         Arb& Fixed    (bool                   const fixed);
                         Arb& Format   (OutputFormat           const format);
                         Arb& Round    (RoundMethod            const round);
      template <class T> Arb& Value    (T                      const number);
                         BaseT                  Base     () const;
                         std::string::size_type MaxRadix () const;
                         PointPosT              PointPos () const;
                         bool                   Fixed    () const;
                         OutputFormat           Format   () const;
                         RoundMethod            Round    () const;
      template <class T> T                      Value    () const;
      
      // Reset to just-constructed defaults.
      Arb& clear ();
      
      // Copy another number.
      Arb& copy (Arb const& number, bool const copynow = false);
      
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
      Arb ceil     () const;
      Arb floor    () const;
      Arb truncate () const;
      Arb toInt    () const;
      
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
      
      // Number types.
      enum _NumType { _NUM_UINT, _NUM_SINT, _NUM_FLPT };
      
      // Determine number type.
      template <class T> class _GetNumType { public: static _NumType const value; }
      
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
      
      // Reduce the number to its most compact representation.
      Arb& _reduce      ();
      Arb& _forcereduce (_DigsT const& q);
      
      // Normalize this number to another number.
      Arb& _normalize (Arb& number);
      
      // Check if this number is a whole number (x/1).
      bool _isWhole () const;
      
  };
  
  /***************************************************************************
   * Errors.
   ***************************************************************************/
   
  // All Arb errors are based off of this.
  class Base : public Exception {
    public:
      virtual char const* what () const throw();
      virtual ~Base () throw();
  };
  
  // Bad format.
  class BadFormat : public Base {
    public:
      virtual char const* what () const throw();
      BadFormat& Problem  (char const*                        const  problem)  throw();
      BadFormat& Position (std::string::size_type             const  position) throw();
      BadFormat& Number   (ConstReferencePointer<std::string> const& number)   throw();
      char const*                        Problem  () const throw();
      std::string::size_type             Position () const throw();
      ConstReferencePointer<std::string> Number   () const throw();
    private:
      char const*                        _problem;
      std::string::size_type             _position;
      ConstReferencePointer<std::string> _number;
  };
  
  // Divide by zero.
  class DivByZero : public Base {
    public:
      virtual char const* what () const throw();
  };
  template <class T, class U> class DivByZeroBinary : public DivByZero {
    public:
      virtual char const* what () const throw();
      DivByZeroBinary& Left     (T           const l)  throw();
      DivByZeroBinary& Operator (char const* const op) throw();
      DivByZeroBinary& Right    (U           const r)  throw();
      T           Left     () const throw();
      char const* Operator () const throw();
      U           Right    () const throw();
    private:
      T           _l;
      char const* _op;
      U           _r;
  };
  
  // Operation results in a complex number.
  class Complex : public Base {
    public:
      virtual char const* what () const throw();
  };
  class ComplexRoot : public Complex {
    public:
      virtual char const* what () const throw();
      ComplexRoot& Number (Arb const& number) throw();
      ComplexRoot& Root   (Arb const& root)   throw();
      Arb Number () const throw();
      Arb Root   () const throw();
    private:
      Arb _number;
      Arb _root;
  };
  
  // Integer-only operation attempted on a non-integer.
  class NonInteger : public Base {
    public:
      virtual char const* what () const throw();
  };
  template <class T, class U> class NonIntegerBinary : public NonInteger {
    public:
      virtual char const* what () const throw();
      NonIntegerBinary& Left     (T           const l)  throw();
      NonIntegerBinary& Operator (char const* const op) throw();
      NonIntegerBinary& Right    (U           const r)  throw();
      T           Left     () const throw();
      char const* Operator () const throw();
      U           Right    () const throw();
    private:
      T           _l;
      char const* _op;
      U           _r;
  };
  
  // Attempt to stuff Arb into a type that will not fit.
  class ScalarOverflow : public Base {
    public:
      virtual char const* what () const throw();
  };
  template <class T> class ScalarOverflowSpecialized : public ScalarOverflow {
    public:
      virtual char const* what () const throw();
      ScalarOverflowSpecialized& Number (Arb const& number) throw();
      ScalarOverflowSpecialized& Limit  (T   const  limit)  throw();
      Arb Number () const throw();
      T   Limit  () const throw();
    private:
      Arb _number;
      T   _limit;
  };
  
  /*************************************************************************
   * Operators.
   *************************************************************************/
  
  // Stream I/O operators.
  std::ostream&       operator << (std::ostream&       l, DAC::Arb  const& r);
  std::ostringstream& operator << (std::ostringstream& l, DAC::Arb  const& r);
  std::istream&       operator >> (std::istream&       l, DAC::Arb&        r);
  
  // Arithmetic operators.
                     DAC::Arb operator * (DAC::Arb        const& l, DAC::Arb        const& r);
                     DAC::Arb operator * (DAC::Arb        const& l, DAC::ArbInt     const& r);
                     DAC::Arb operator * (DAC::ArbInt     const& l, DAC::Arb        const& r);
  template <class T> DAC::Arb operator * (DAC::Arb        const& l, DAC::SafeInt<T> const  r);
  template <class T> DAC::Arb operator * (DAC::SafeInt<T> const  l, DAC::Arb        const& r);
  template <class T> DAC::Arb operator * (DAC::Arb        const& l, T               const  r);
  template <class T> DAC::Arb operator * (T               const  l, DAC::Arb        const& r);
                     DAC::Arb operator / (DAC::Arb        const& l, DAC::Arb        const& r);
                     DAC::Arb operator / (DAC::Arb        const& l, DAC::ArbInt     const& r);
                     DAC::Arb operator / (DAC::ArbInt     const& l, DAC::Arb        const& r);
  template <class T> DAC::Arb operator / (DAC::Arb        const& l, DAC::SafeInt<T> const  r);
  template <class T> DAC::Arb operator / (DAC::SafeInt<T> const  l, DAC::Arb        const& r);
  template <class T> DAC::Arb operator / (DAC::Arb        const& l, T               const  r);
  template <class T> DAC::Arb operator / (T               const  l, DAC::Arb        const& r);
                     DAC::Arb operator % (DAC::Arb        const& l, DAC::Arb        const& r);
                     DAC::Arb operator % (DAC::Arb        const& l, DAC::ArbInt     const& r);
                     DAC::Arb operator % (DAC::ArbInt     const& l, DAC::Arb        const& r);
  template <class T> DAC::Arb operator % (DAC::Arb        const& l, DAC::SafeInt<T> const  r);
  template <class T> DAC::Arb operator % (DAC::SafeInt<T> const  l, DAC::Arb        const& r);
  template <class T> DAC::Arb operator % (DAC::Arb        const& l, T               const  r);
  template <class T> DAC::Arb operator % (T               const  l, DAC::Arb        const& r);
                     DAC::Arb operator + (DAC::Arb        const& l, DAC::Arb        const& r);
                     DAC::Arb operator + (DAC::Arb        const& l, DAC::ArbInt     const& r);
                     DAC::Arb operator + (DAC::ArbInt     const& l, DAC::Arb        const& r);
  template <class T> DAC::Arb operator + (DAC::Arb        const& l, DAC::SafeInt<T> const  r);
  template <class T> DAC::Arb operator + (DAC::SafeInt<T> const  l, DAC::Arb        const& r);
  template <class T> DAC::Arb operator + (DAC::Arb        const& l, T               const  r);
  template <class T> DAC::Arb operator + (T               const  l, DAC::Arb        const& r);
                     DAC::Arb operator - (DAC::Arb        const& l, DAC::Arb        const& r);
                     DAC::Arb operator - (DAC::Arb        const& l, DAC::ArbInt     const& r);
                     DAC::Arb operator - (DAC::ArbInt     const& l, DAC::Arb        const& r);
  template <class T> DAC::Arb operator - (DAC::Arb        const& l, DAC::SafeInt<T> const  r);
  template <class T> DAC::Arb operator - (DAC::SafeInt<T> const  l, DAC::Arb        const& r);
  template <class T> DAC::Arb operator - (DAC::Arb        const& l, T               const  r);
  template <class T> DAC::Arb operator - (T               const  l, DAC::Arb        const& r);
  
  // Comparison operators.
                     bool operator >  (DAC::Arb        const& l, DAC::Arb        const& r);
                     bool operator >  (DAC::Arb        const& l, DAC::ArbInt     const& r);
                     bool operator >  (DAC::ArbInt     const& l, DAC::Arb        const& r);
  template <class T> bool operator >  (DAC::Arb        const& l, DAC::SafeInt<T> const  r);
  template <class T> bool operator >  (DAC::SafeInt<T> const  l, DAC::Arb        const& r);
  template <class T> bool operator >  (DAC::Arb        const& l, T               const  r);
  template <class T> bool operator >  (T               const  l, DAC::Arb        const& r);
                     bool operator >= (DAC::Arb        const& l, DAC::Arb        const& r);
                     bool operator >= (DAC::Arb        const& l, DAC::ArbInt     const& r);
                     bool operator >= (DAC::ArbInt     const& l, DAC::Arb        const& r);
  template <class T> bool operator >= (DAC::Arb        const& l, DAC::SafeInt<T> const  r);
  template <class T> bool operator >= (DAC::SafeInt<T> const  l, DAC::Arb        const& r);
  template <class T> bool operator >= (DAC::Arb        const& l, T               const  r);
  template <class T> bool operator >= (T               const  l, DAC::Arb        const& r);
                     bool operator <  (DAC::Arb        const& l, DAC::Arb        const& r);
                     bool operator <  (DAC::Arb        const& l, DAC::ArbInt     const& r);
                     bool operator <  (DAC::ArbInt     const& l, DAC::Arb        const& r);
  template <class T> bool operator <  (DAC::Arb        const& l, DAC::SafeInt<T> const  r);
  template <class T> bool operator <  (DAC::SafeInt<T> const  l, DAC::Arb        const& r);
  template <class T> bool operator <  (DAC::Arb        const& l, T               const  r);
  template <class T> bool operator <  (T               const  l, DAC::Arb        const& r);
                     bool operator <= (DAC::Arb        const& l, DAC::Arb        const& r);
                     bool operator <= (DAC::Arb        const& l, DAC::ArbInt     const& r);
                     bool operator <= (DAC::ArbInt     const& l, DAC::Arb        const& r);
  template <class T> bool operator <= (DAC::Arb        const& l, DAC::SafeInt<T> const  r);
  template <class T> bool operator <= (DAC::SafeInt<T> const  l, DAC::Arb        const& r);
  template <class T> bool operator <= (DAC::Arb        const& l, T               const  r);
  template <class T> bool operator <= (T               const  l, DAC::Arb        const& r);
                     bool operator == (DAC::Arb        const& l, DAC::Arb        const& r);
                     bool operator == (DAC::Arb        const& l, DAC::ArbInt     const& r);
                     bool operator == (DAC::ArbInt     const& l, DAC::Arb        const& r);
  template <class T> bool operator == (DAC::Arb        const& l, DAC::SafeInt<T> const  r);
  template <class T> bool operator == (DAC::SafeInt<T> const  l, DAC::Arb        const& r);
  template <class T> bool operator == (DAC::Arb        const& l, T               const  r);
  template <class T> bool operator == (T               const  l, DAC::Arb        const& r);
                     bool operator != (DAC::Arb        const& l, DAC::Arb        const& r);
                     bool operator != (DAC::Arb        const& l, DAC::ArbInt     const& r);
                     bool operator != (DAC::ArbInt     const& l, DAC::Arb        const& r);
  template <class T> bool operator != (DAC::Arb        const& l, DAC::SafeInt<T> const  r);
  template <class T> bool operator != (DAC::SafeInt<T> const  l, DAC::Arb        const& r);
  template <class T> bool operator != (DAC::Arb        const& l, T               const  r);
  template <class T> bool operator != (T               const  l, DAC::Arb        const& r);
  
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
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
   
  /***************************************************************************/
  // Function members.
  
  // Conversion constructor.
  template <class T> inline Arb::Arb (T const number) {
    
    // Construct fully.
    _init();
    
    // Set the number.
    set(number);
    
  }
  
  // Increment / decrement operators.
  inline Arb& Arb::operator ++ ()    { return op_add(1);                            }
  inline Arb  Arb::operator ++ (int) { Arb retval(*this); op_add(1); return retval; }
  inline Arb& Arb::operator -- ()    { return op_sub(1);                            }
  inline Arb  Arb::operator -- (int) { Arb retval(*this); op_sub(1); return retval; }
  
  // Unary sign operators.
  inline Arb Arb::operator + () const { return *this;                                                                             }
  inline Arb Arb::operator - () const { Arb retval(*this, true); retval._data->positive = !retval._data->positive; return retval; }
  
  // Casting operators.
  inline Arb::operator bool               () const { return !isZero();                   }
  inline Arb::operator signed   char      () const { return Value<signed   char     >(); }
  inline Arb::operator unsigned char      () const { return Value<unsigned char     >(); }
  inline Arb::operator signed   short int () const { return Value<signed   short int>(); }
  inline Arb::operator unsigned short int () const { return Value<unsigned short int>(); }
  inline Arb::operator signed   int       () const { return Value<signed   int      >(); }
  inline Arb::operator unsigned int       () const { return Value<unsigned int      >(); }
  inline Arb::operator signed   long int  () const { return Value<signed   long int >(); }
  inline Arb::operator unsigned long int  () const { return Value<unsigned long int >(); }
  
  // Assignment operator.
                     inline Arb& Arb::operator = (Arb         const& number) { return copy(number); }
                     inline Arb& Arb::operator = (std::string const& number) { return set(number);  }
  template <class T> inline Arb& Arb::operator = (T           const  number) { return set(number);  }
  
  // Get the base of this number.
  inline Arb::BaseT Arb::Base () const { return _data->base; }
  
  // Get the maximum number of radix digits to output.
  inline std::string::size_type Arb::MaxRadix () const { return _maxradix; }
  
  // Set the maximum number of radix digits to output.
  inline Arb& Arb::MaxRadix (std::string::size_type const maxradix) { _maxradix = maxradix; return *this; }
  
  // Get the point position of a fixed-point number.
  inline Arb::PointPosT Arb::PointPos () const { return _data->pointpos; }
  
  // Get whether this is a fixed-point number or not.
  inline bool Arb::Fixed () const { return _data->fix; }
  
  // Get the output format of this number.
  inline Arb::OutputFormat Arb::Format () const { return _format; }
  
  // Set the output format of this number.
  inline Arb& Arb::Format (OutputFormat const format) { _format = (format == FMT_DEFAULT) ? _format : format; return *this; }
  
  // Get the round method of this number.
  inline Arb::RoundMethod Arb::Round () const { return _round; }
  
  // Set the round method of this number.
  inline Arb& Arb::Round (RoundMethod const round) { _round = (round == ROUND_DEFAULT) ? _round : round; return *this; }
  
  // Get the value of this number.
  template <class T> inline T Arb::Value () const { T retval; _Get<T, _GetNumType<T>::value>::op(retval, *this); return retval; }
  
  // Set the value of this number.
  template <class T> inline Arb& Arb::Value (T const number) { return set(number); }
  
  // Set from a built-in type.
  template <class T> inline Arb& Arb::set (SafeInt<T> const number) { _Set<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  template <class T> inline Arb& Arb::set (T          const number) { _Set<T, _GetNumType<T>::value>::op(*this, number); return *this; }
  
  /*
  // Multiply by an integral type
  template <class T> Arb& Arb::op_mul (SafeInt<T> const& number) {
    
    // Multiplying 0 is easy.
    if (isZero()) {
      return *this;
    }
    
    // Multiplying by 0 is also easy.
    if (number == 0) {
      
      // Work area.
      Arb retval(*this, true);
      
      // Set to zero.
      retval._data->p = 0;
      retval._data->q = 1;
      
      // Copy in result and return.
      _data = retval._data;
      return *this;
      
    }
    
    // Multiplying by 1.
    if (number == 1) {
      return *this;
    }
    
    // Multiplying by -1.
    if (number == -1) {
      
      // Work area.
      Arb retval(*this, true);
      
      // Reverse the sign.
      retval._data->positive = !_data->positive;
      
      // Copy in result and return.
      _data = retval._data;
      return *this;
      
    }
    
    // Multiply the easy way if this is an integer.
    if (std::numeric_limits<T>::is_integer) {
      
      // Work area.
      Arb retval(*this, true);
      
      // Multiply.
      retval._data->p *= DAC::abs(number.Value());
      
      // Set the sign.
      retval._data->positive = (_data->positive == (number > 0));
      
      // Reduce.
      retval._reduce();
      
      // Move in the result and return.
      _data = retval._data;
      return *this;
      
    // Otherwise do it the hard way.
    } else {
      // FIXME!!!
      //return op_mul(Arb(number));
    }
    
  }
  template <>        inline Arb& Arb::op_mul (bool               const number) { return op_mul(SafeInt<bool                  >(number)); }
  template <>        inline Arb& Arb::op_mul (signed   char      const number) { return op_mul(SafeInt<signed   char         >(number)); }
  template <>        inline Arb& Arb::op_mul (unsigned char      const number) { return op_mul(SafeInt<unsigned char         >(number)); }
  template <>        inline Arb& Arb::op_mul (signed   short int const number) { return op_mul(SafeInt<signed   short int    >(number)); }
  template <>        inline Arb& Arb::op_mul (unsigned short int const number) { return op_mul(SafeInt<unsigned short int    >(number)); }
  template <>        inline Arb& Arb::op_mul (signed   int       const number) { return op_mul(SafeInt<signed   int          >(number)); }
  template <>        inline Arb& Arb::op_mul (unsigned int       const number) { return op_mul(SafeInt<unsigned int          >(number)); }
  template <>        inline Arb& Arb::op_mul (signed   long int  const number) { return op_mul(SafeInt<signed   long int     >(number)); }
  template <>        inline Arb& Arb::op_mul (unsigned long int  const number) { return op_mul(SafeInt<unsigned long int     >(number)); }
  template <class T> inline Arb& Arb::op_mul (T                  const number) { return op_mul(Arb(number));                             }
  
  // Divide by an integral type.
  template <class T> Arb& Arb::op_div (SafeInt<T> const& number) {
    
    // Dividing 0 is easy.
    if (isZero()) {
      return *this;
    }
    
    // Dividing by 0 is verboten.
    if (number == 0) {
      throw ArbErrors::DivByZero();
    }
    
    // Dividing by 1.
    if (number == 1) {
      return *this;
    }
    
    // Dividing by -1.
    if (number == -1) {
      
      // Work area.
      Arb retval(*this, true);
      
      // Flip the sign.
      retval._data->positive = !_data->positive;
      
      // Move in the result and return.
      _data = retval._data;
      return *this;
      
    }
    
    // Divide the easy way if this is an integer.
    if (std::numeric_limits<T>::is_integer) {
      
      // Work area.
      Arb retval(*this, true);
      
      // Divide.
      retval._data->q *= DAC::abs(number.Value());
      
      // Set the sign.
      retval._data->positive = (_data->positive == (number > 0));
      
      // Reduce.
      retval._reduce();
      
      // Move the result in and return.
      _data = retval._data;
      return *this;
      
    // Otherwise do it the hard way.
    } else {
      return op_div(Arb(number));
    }
    
  }
  template <>        inline Arb& Arb::op_div (bool               const number) { return op_div(SafeInt<bool                  >(number)); }
  template <>        inline Arb& Arb::op_div (signed   char      const number) { return op_div(SafeInt<signed   char         >(number)); }
  template <>        inline Arb& Arb::op_div (unsigned char      const number) { return op_div(SafeInt<unsigned char         >(number)); }
  template <>        inline Arb& Arb::op_div (signed   short int const number) { return op_div(SafeInt<signed   short int    >(number)); }
  template <>        inline Arb& Arb::op_div (unsigned short int const number) { return op_div(SafeInt<unsigned short int    >(number)); }
  template <>        inline Arb& Arb::op_div (signed   int       const number) { return op_div(SafeInt<signed   int          >(number)); }
  template <>        inline Arb& Arb::op_div (unsigned int       const number) { return op_div(SafeInt<unsigned int          >(number)); }
  template <>        inline Arb& Arb::op_div (signed   long int  const number) { return op_div(SafeInt<signed   long int     >(number)); }
  template <>        inline Arb& Arb::op_div (unsigned long int  const number) { return op_div(SafeInt<unsigned long int     >(number)); }
  template <class T> inline Arb& Arb::op_div (T                  const number) { return op_div(Arb(number));                             }
  
  // Modulo division by an integral type.
  template <class T> Arb& Arb::op_mod (SafeInt<T> const& number) {
    
    // Dividing 0 is easy.
    if (isZero()) {
      return *this;
    }
    
    // Dividing by 0 is verboten.
    if (number == 0) {
      throw ArbErrors::DivByZero();
    }
    
    // Throw an error if both numbers are not integer.
    if (!std::numeric_limits<T>::is_integer || !isInteger()) {
      throw ArbErrors::NonInteger();
    }
    
    // Work area.
    Arb retval(*this, true);
    
    // Modulo divide p.
    retval._data->p %= number;
    
    // We done.
    _data = retval._data;
    return *this;
    
  }
  template <>        inline Arb& Arb::op_mod (bool               const number) { return op_mod(SafeInt<bool                  >(number)); }
  template <>        inline Arb& Arb::op_mod (signed   char      const number) { return op_mod(SafeInt<signed   char         >(number)); }
  template <>        inline Arb& Arb::op_mod (unsigned char      const number) { return op_mod(SafeInt<unsigned char         >(number)); }
  template <>        inline Arb& Arb::op_mod (signed   short int const number) { return op_mod(SafeInt<signed   short int    >(number)); }
  template <>        inline Arb& Arb::op_mod (unsigned short int const number) { return op_mod(SafeInt<unsigned short int    >(number)); }
  template <>        inline Arb& Arb::op_mod (signed   int       const number) { return op_mod(SafeInt<signed   int          >(number)); }
  template <>        inline Arb& Arb::op_mod (unsigned int       const number) { return op_mod(SafeInt<unsigned int          >(number)); }
  template <>        inline Arb& Arb::op_mod (signed   long int  const number) { return op_mod(SafeInt<signed   long int     >(number)); }
  template <>        inline Arb& Arb::op_mod (unsigned long int  const number) { return op_mod(SafeInt<unsigned long int     >(number)); }
  template <class T> inline Arb& Arb::op_mod (T                  const number) { return op_mod(Arb(number));                             }
  
  // Addition of an integral type.
  template <class T> Arb& Arb::op_add (SafeInt<T> const& number) {
    
    // Adding to 0 is easy.
    if (isZero()) {
      return set(number);
    }
    
    // Adding 0 is easy.
    if (number == 0) {
      return *this;
    }
    
    // If adding an opposite sign, subtract the opposite.
    if (_data->positive != (number > 0)) {
      return op_sub(-number);
    }
    
    // Add the easy way if this is an integer.
    if (std::numeric_limits<T>::is_integer) {
      
      // Work area.
      Arb retval(*this, true);
      
      // Add the very easy way if this is an integer, otherwise scale.
      if (retval.isInteger()) {
        retval._data->p += SafeInt<T>(DAC::abs(number.Value()));
      } else {
        retval._data->p += SafeInt<T>(DAC::abs(number.Value())) * retval._data->q;
      }
      
      // Reduce.
      retval._reduce();
      
      // Move the result in and return.
      _data = retval._data;
      return *this;
      
    // Otherwise add the hard way.
    } else {
      //FIXME!!!
      //return op_add(Arb(number));
    }
    
  }
  template <>        inline Arb& Arb::op_add (bool               const number) { return op_add(SafeInt<bool                  >(number)); }
  template <>        inline Arb& Arb::op_add (signed   char      const number) { return op_add(SafeInt<signed   char         >(number)); }
  template <>        inline Arb& Arb::op_add (unsigned char      const number) { return op_add(SafeInt<unsigned char         >(number)); }
  template <>        inline Arb& Arb::op_add (signed   short int const number) { return op_add(SafeInt<signed   short int    >(number)); }
  template <>        inline Arb& Arb::op_add (unsigned short int const number) { return op_add(SafeInt<unsigned short int    >(number)); }
  template <>        inline Arb& Arb::op_add (signed   int       const number) { return op_add(SafeInt<signed   int          >(number)); }
  template <>        inline Arb& Arb::op_add (unsigned int       const number) { return op_add(SafeInt<unsigned int          >(number)); }
  template <>        inline Arb& Arb::op_add (signed   long int  const number) { return op_add(SafeInt<signed   long int     >(number)); }
  template <>        inline Arb& Arb::op_add (unsigned long int  const number) { return op_add(SafeInt<unsigned long int     >(number)); }
  template <class T> inline Arb& Arb::op_add (T                  const number) { return op_add(Arb(number));                             }
  
  // Subtraction of an integral type.
  template <class T> Arb& Arb::op_sub (SafeInt<T> const& number) {
    
    // Subtracting from 0 is easy.
    if (isZero()) {
      Arb retval(*this);
      retval.set(number);
      retval._data->positive = !retval._data->positive;
      _data = retval._data;
      return *this;
    }
    
    // Subtracting 0 is easy.
    if (number == 0) {
      return *this;
    }
    
    // If subtracting an opposite sign, add the opposite.
    if (_data->positive != (number > 0)) {
      return op_add(-number);
    }
    
    // Subtract the easy way if this is an integer.
    if (std::numeric_limits<T>::is_integer) {
      
      // Work area.
      Arb retval(*this, true);
      
      // Subtract the very easy way if this is an integer.
      SafeInt<T> anum = SafeInt<T>(DAC::abs(number.Value()));
      if (retval.isInteger()) {
        if (anum > retval._data->p) {
          retval._data->positive = !retval._data->positive;
          retval._data->p        = anum - retval._data->p;
        } else {
          retval._data->p -= anum;
        }
        
      // Otherwise do it the slightly harder way.
      } else {
        ArbInt ainum(anum * retval._data->q);
        if (ainum > retval._data->p) {
          retval._data->positive = !retval._data->positive;
          retval._data->p        = ainum - retval._data->p;
        } else {
          retval._data->p -= ainum;
        }
      }
      
      // Reduce.
      retval._reduce();
      
      // Move the result in and return.
      _data = retval._data;
      return *this;
      
    // Subtract the hard way.
    } else {
      //FIXME!!!
      //return op_sub(Arb(number));
    }
    
  }
  template <>        inline Arb& Arb::op_sub (bool               const number) { return op_sub(SafeInt<bool                  >(number)); }
  template <>        inline Arb& Arb::op_sub (signed   char      const number) { return op_sub(SafeInt<signed   char         >(number)); }
  template <>        inline Arb& Arb::op_sub (unsigned char      const number) { return op_sub(SafeInt<unsigned char         >(number)); }
  template <>        inline Arb& Arb::op_sub (signed   short int const number) { return op_sub(SafeInt<signed   short int    >(number)); }
  template <>        inline Arb& Arb::op_sub (unsigned short int const number) { return op_sub(SafeInt<unsigned short int    >(number)); }
  template <>        inline Arb& Arb::op_sub (signed   int       const number) { return op_sub(SafeInt<signed   int          >(number)); }
  template <>        inline Arb& Arb::op_sub (unsigned int       const number) { return op_sub(SafeInt<unsigned int          >(number)); }
  template <>        inline Arb& Arb::op_sub (signed   long int  const number) { return op_sub(SafeInt<signed   long int     >(number)); }
  template <>        inline Arb& Arb::op_sub (unsigned long int  const number) { return op_sub(SafeInt<unsigned long int     >(number)); }
  template <class T> inline Arb& Arb::op_sub (T                  const number) { return op_sub(Arb(number));                             }
  
  // Greater than an integral type.
  template <class T> bool Arb::op_gt (SafeInt<T> const& number) const {
    
    // Check for zeros.
    if (_data->p.isZero()) {
      if (number == 0) {
        return false;
      } else {
        return (number < 0);
      }
    } else if (number == 0) {
      return _data->positive;
    }
    
    // Check signs.
    if (_data->positive && (number < 0)) {
      return true;
    } else if (!_data->positive && (number > 0)) {
      return false;
    }
    
    // Compare the number.
    if (isInteger()) {
      if (_data->positive) {
        return (_data->p > number);
      } else {
        return (_data->p < number);
      }
    } else {
      ArbInt atmp(number * _data->q);
      if (_data->positive) {
        return (_data->p > atmp);
      } else {
        return (_data->p < atmp);
      }
    }
    
  }
  template <>        inline bool Arb::op_gt (bool               const number) const { return op_gt(SafeInt<bool                  >(number)); }
  template <>        inline bool Arb::op_gt (signed   char      const number) const { return op_gt(SafeInt<signed   char         >(number)); }
  template <>        inline bool Arb::op_gt (unsigned char      const number) const { return op_gt(SafeInt<unsigned char         >(number)); }
  template <>        inline bool Arb::op_gt (signed   short int const number) const { return op_gt(SafeInt<signed   short int    >(number)); }
  template <>        inline bool Arb::op_gt (unsigned short int const number) const { return op_gt(SafeInt<unsigned short int    >(number)); }
  template <>        inline bool Arb::op_gt (signed   int       const number) const { return op_gt(SafeInt<signed   int          >(number)); }
  template <>        inline bool Arb::op_gt (unsigned int       const number) const { return op_gt(SafeInt<unsigned int          >(number)); }
  template <>        inline bool Arb::op_gt (signed   long int  const number) const { return op_gt(SafeInt<signed   long int     >(number)); }
  template <>        inline bool Arb::op_gt (unsigned long int  const number) const { return op_gt(SafeInt<unsigned long int     >(number)); }
  template <class T> inline bool Arb::op_gt (T                  const number) const { return op_gt(Arb(number));                             }
  
  // Less than an integral type.
  template <class T> bool Arb::op_lt (SafeInt<T> const& number) const {
    
    // Check for zeros.
    if (_data->p.isZero()) {
      if (number == 0) {
        return false;
      } else {
        return (number > 0);
      }
    } else if (number == 0) {
      return !_data->positive;
    }
    
    // Check signs.
    if (_data->positive && (number < 0)) {
      return false;
    } else if (!_data->positive && (number > 0)) {
      return true;
    }
    
    // Compare the number.
    if (isInteger()) {
      if (_data->positive) {
        return (_data->p < number);
      } else {
        return (_data->p > number);
      }
    } else {
      ArbInt atmp(number * _data->q);
      if (_data->positive) {
        return (_data->p < atmp);
      } else {
        return (_data->p > atmp);
      }
    }
    
  }
  template <>        inline bool Arb::op_lt (bool               const number) const { return op_lt(SafeInt<bool                  >(number)); }
  template <>        inline bool Arb::op_lt (signed   char      const number) const { return op_lt(SafeInt<signed   char         >(number)); }
  template <>        inline bool Arb::op_lt (unsigned char      const number) const { return op_lt(SafeInt<unsigned char         >(number)); }
  template <>        inline bool Arb::op_lt (signed   short int const number) const { return op_lt(SafeInt<signed   short int    >(number)); }
  template <>        inline bool Arb::op_lt (unsigned short int const number) const { return op_lt(SafeInt<unsigned short int    >(number)); }
  template <>        inline bool Arb::op_lt (signed   int       const number) const { return op_lt(SafeInt<signed   int          >(number)); }
  template <>        inline bool Arb::op_lt (unsigned int       const number) const { return op_lt(SafeInt<unsigned int          >(number)); }
  template <>        inline bool Arb::op_lt (signed   long int  const number) const { return op_lt(SafeInt<signed   long int     >(number)); }
  template <>        inline bool Arb::op_lt (unsigned long int  const number) const { return op_lt(SafeInt<unsigned long int     >(number)); }
  template <class T> inline bool Arb::op_lt (T                  const number) const { return op_lt(Arb(number));                             }
  
  // Equal to an integral type.
  template <class T> bool Arb::op_eq (SafeInt<T> const& number) const {
    
    // Check for zeros.
    if (_data->p.isZero()) {
      return (number == 0);
    } else if (number == 0) {
      return false;
    }
    
    // Neither number is zero, check signs.
    if (_data->positive != (number > 0)) {
      return false;
    }
    
    // Check numbers.
    if (isInteger()) {
      return (_data->p == number);
    } else {
      return (_data->p == (number * _data->q));
    }
    
  }
  template <>        inline bool Arb::op_eq (bool               const number) const { return op_eq(SafeInt<bool                  >(number)); }
  template <>        inline bool Arb::op_eq (signed   char      const number) const { return op_eq(SafeInt<signed   char         >(number)); }
  template <>        inline bool Arb::op_eq (unsigned char      const number) const { return op_eq(SafeInt<unsigned char         >(number)); }
  template <>        inline bool Arb::op_eq (signed   short int const number) const { return op_eq(SafeInt<signed   short int    >(number)); }
  template <>        inline bool Arb::op_eq (unsigned short int const number) const { return op_eq(SafeInt<unsigned short int    >(number)); }
  template <>        inline bool Arb::op_eq (signed   int       const number) const { return op_eq(SafeInt<signed   int          >(number)); }
  template <>        inline bool Arb::op_eq (unsigned int       const number) const { return op_eq(SafeInt<unsigned int          >(number)); }
  template <>        inline bool Arb::op_eq (signed   long int  const number) const { return op_eq(SafeInt<signed   long int     >(number)); }
  template <>        inline bool Arb::op_eq (unsigned long int  const number) const { return op_eq(SafeInt<unsigned long int     >(number)); }
  template <class T> inline bool Arb::op_eq (T                  const number) const { return op_eq(Arb(number));                             }
  
  // Comparison operator backends.
                     inline bool Arb::op_ge (Arb        const& number) const { return !op_lt(number); }
  template <class T> inline bool Arb::op_ge (SafeInt<T> const& number) const { return !op_lt(number); }
  template <class T> inline bool Arb::op_ge (T          const  number) const { return !op_lt(number); }
                     inline bool Arb::op_le (Arb        const& number) const { return !op_gt(number); }
  template <class T> inline bool Arb::op_le (SafeInt<T> const& number) const { return !op_gt(number); }
  template <class T> inline bool Arb::op_le (T          const  number) const { return !op_gt(number); }
                     inline bool Arb::op_ne (Arb        const& number) const { return !op_eq(number); }
  template <class T> inline bool Arb::op_ne (SafeInt<T> const& number) const { return !op_eq(number); }
  template <class T> inline bool Arb::op_ne (T          const  number) const { return !op_eq(number); }
  */
  
  // Return whether this number is an integer.
  inline bool Arb::isInteger () const { return (_data->q == 1); }
  
  // Return whether this number is positive.
  inline bool Arb::isPositive () const { return _data->positive || _data->p == 0; }
  inline bool Arb::isNegative () const { return !isPositive();                    }
  
  // Return whether this number is equal to zero.
  inline bool Arb::isZero () const { return _data->p.isZero(); }
  
  // Return whether this number is even or odd.
  inline bool Arb::isOdd  () const { return (isInteger() && _data->p.isOdd());  }
  inline bool Arb::isEven () const { return (isInteger() && _data->p.isEven()); }
  
  // Return the integer portion of this number.
  inline Arb Arb::toInt () const { Arb retval(*this, true); retval._forcereduce(_DigsT(1)); return retval; }
  
  // Raise this number to a given power.
  template <class T> inline Arb Arb::pow (T const exp) const { return pow(Arb(exp)); }
  
  // Get a given root of this number.
  template <class T> inline Arb Arb::root (T const n) const { return root(Arb(n)); }
  
  // Return the absolute value of this number.
  inline Arb Arb::abs () const { Arb retval(*this, true); retval._data->positive = true; return retval; }
  
  // Check if this number is a whole number (x/1).
  inline bool Arb::_isWhole () const { return _data->q == 1; }
  
  // Set from an unsigned integer type.
  template <class T> void Arb::_Set<T, Arb::_NUM_UINT>::op (Arb& l, SafeInt<T> const r) {
    
    // Work area.
    Arb new_num;
    
    // Carry over the old fixed-point porperties.
    new_num._data->fix      = l._data->fix;
    new_num._data->pointpos = l._data->pointpos;
    new_num._data->base     = l._data->base;
    new_num._data->fixq     = l._data->fixq;
    
    // This number is a whole number.
    new_num._data->p        = number;
    new_num._data->q        = 1;
    new_num._data->positive = true;
    
    // Reduce the fraction.
    new_num._reduce();
    
    // Move the new data into place and return.
    l._data = new_num._data;
    
  }
  template <class T> inline void Arb::_Set<T, Arb::_NUM_UINT>::op (Arb& l, T const r) { Arb::_Set<T, Arb::_NUM_UINT>(l, SafeInt<T>(r)); }
  
  // Set from a signed integer type.
  template <class T> void Arb::_Set<T, Arb::_NUM_SINT>::op (Arb& l, SafeInt<T> const r) {
    
    // Work area.
    Arb new_num;
    
    // Carry over the old fixed-point properties.
    new_num._data->fix      = l._data->fix;
    new_num._data->pointpos = l._data->pointpos;
    new_num._data->base     = l._data->base;
    new_num._data->fixq     = l._data->fixq;
    
    // This number is a whole number.
    new_num._data->p        = std::abs(number);
    neW_num._data->q        = 1;
    new_num._data->positive = (number >= 0);
    
    // Reduce the fraction.
    new_num._reduce();
    
    // Move the new data into place and return.
    l._data = new_num._data;
    
  }
  template <class T> inline void Arb::_Set<T, Arb::_NUM_SINT>::op (Arb& l, T const r) { Arb::_Set<T, Arb::_NUM_SINT>(l, SafeInt<T>(r)); }
  
  // Set from a floating-point type.
  template <class T> inline void Arb::_Set<T, Arb::_NUM_FLPT>::op (Arb& l, T const r) {
    
    // Set from a string. Not very heroic, but for now, it works.
    l.set(DAC::toString(r));
    
  }
  
  // Get as an unsigned integer.
  template <class T> void Arb::_Get<T, Arb::_NUM_UINT>::op (SafeInt<T>& l, Arb const& r) {
    
    // If l is negative, error.
    if (l.isNegative()) {
      throw ArbErrors::ScalarOverflowSpecialized<T>().Number(r).Limit(0);
    }
    
    // Get the value the same way as a signed integer.
    Arb::_Get<T, Arb::_NUM_SINT>::op(l, r);
    
  }
  template <class T> inline void Arb::_Get<T, Arb::_NUM_UINT>::op (T& l, Arb const& r) { SafeInt<T> tmp; Arb::_Get<T, Arb::_NUM_UINT>(tmp, r); l = tmp; }
  
  // Get as a signed integer.
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
        l = -SafeInt<T>(static_cast<T>(tmp_data->p - 1)) - 1;
      }
      
    // Only error that should occur.
    } catch (ArbIntErrors::ScalarOverflowSpecialized<T>& e) {
      throw ArbErrors::ScalarOverflowSpecialized<T>().Number(r).Limit(e.Limit());
    }
    
  }
  template <class T> inline void Arb::_Get<T, Arb::_NUM_SINT>::op (T& l, Arb const& r) { SafeInt<T> tmp; Arb::_Get<T, Arb::_NUM_SINT>(tmp, r); l = tmp; }
  
  // Get as a floating-point type.
  template <class T> void Arb::_Get<T, Arb::_NUM_FLPT>::op (T& l, Arb const& r) {
    
    
    
  }
  
  /***************************************************************************
   * Errors.
   ***************************************************************************/
  namespace ArbErrors {
    
    inline BadFormat& BadFormat::Problem  (char const*                        const  problem)  throw() { _problem  = problem;  return *this; }
    inline BadFormat& BadFormat::Position (std::string::size_type             const  position) throw() { _position = position; return *this; }
    inline BadFormat& BadFormat::Number   (ConstReferencePointer<std::string> const& number)   throw() { _number   = number;   return *this; }
    inline char const*                        BadFormat::Problem  () const throw () { return _problem;  }
    inline std::string::size_type             BadFormat::Position () const throw () { return _position; }
    inline ConstReferencePointer<std::string> BadFormat::Number   () const throw () { return _number;   }
    
    template <class T, class U> char const* DivByZeroBinary<T, U>::what () const throw() {
      try {
        return (toString(_l) + " " + std::string(_op) + " " + toString(_r) + ": Divide by zero.").c_str();
      } catch (...) {
        return "Divide by zero. Error creating message string.";
      }
    }
    template <class T, class U> inline DivByZeroBinary<T, U>& DivByZeroBinary<T, U>::Left     (T           const l)  throw() { _l  = l;  return *this; }
    template <class T, class U> inline DivByZeroBinary<T, U>& DivByZeroBinary<T, U>::Operator (char const* const op) throw() { _op = op; return *this; }
    template <class T, class U> inline DivByZeroBinary<T, U>& DivByZeroBinary<T, U>::Right    (T           const r)  throw() { _r  = r;  return *this; }
    template <class T, class U> inline T           DivByZeroBinary<T, U>::Left     () const throw() { return _l;  }
    template <class T, class U> inline char const* DivByZeroBinary<T, U>::Operator () const throw() { return _op; }
    template <class T, class U> inline U           DivByZeroBinary<T, U>::Right    () const throw() { return _r;  }
    
    inline ComplexRoot& ComplexRoot::Number (Arb const& number) throw() { _number = number; return *this; }
    inline ComplexRoot& ComplexRoot::Root   (Arb const& root)   throw() { _root   = root;   return *this; }
    inline Arb ComplexRoot::Number () const throw() { return _number; }
    inline Arb ComplexRoot::Root   () const throw() { return _root;   }
    
    template <class T, class U> char const* NonIntegerBinary::what () const throw() {
      try {
        return (toString(_l) + " " + std::string(op) + " " + toString(_r) + ": Integer operation attempted on a non-integer.").c_str();
      } catch (...) {
        return "Integer operation attempted on a non-integer. Error creating message string.";
      }
    }
    template <class T, class U> NonIntegerBinary& NonIntegerBinary::Left     (T           const l)  throw() { _l  = l;  return *this; }
    template <class T, class U> NonIntegerBinary& NonIntegerBinary::Operator (char const* const op) throw() { _op = op; return *this; }
    template <class T, class U> NonIntegerBinary& NonIntegerBinary::Right    (U           const r)  throw() { _r  = r;  return *this; }
    template <class T, class U> T           NonIntegerBinary::Left     () const throw() { return _l;  }
    template <class T, class U> char const* NonIntegerBinary::Operator () const throw() { return _op; }
    template <class T, class U> U           NonIntegerBinary::Right    () const throw() { return _r;  }
    
    template <class T> char const* ScalarOverflowSpecialized::what () const throw() {
      try {
        return (_number.toString() + ": Overflows requested scalar type's limit of " + DAC::toString(_limit) + ".").c_str();
      } catch (...) {
        return "Arb overflows requested scalar type. Error creating message string.";
      }
    }
    template <class T> inline ScalarOverflowSpecialized<T>& ScalarOverflowSpecialized<T>::Number (Arb const& number) throw() { _number = number; return *this; }
    template <class T> inline ScalarOverflowSpecialized<T>& ScalarOverflowSpecialized<T>::Limit  (T   const  limit)  throw() { _limit  = limit;  return *this; }
    template <class T> inline Arb ScalarOverflowSpecialized<T>::Number () const throw() { return _number; }
    template <class T> inline T   ScalarOverflowSpecialized<T>::Limit  () const throw() { return _limit;  }
    
  }
  
  /***************************************************************************
   * Operators.
   ***************************************************************************/
  
  // Stream I/O operators.
  inline std::ostream&       operator << (std::ostream&       l, Arb  const& r) { l << r.toString();                                  return l; }
  inline std::ostringstream& operator << (std::ostringstream& l, Arb  const& r) { l << r.toString();                                  return l; }
  inline std::istream&       operator >> (std::istream&       l, Arb&        r) { std::string input; std::cin >> input; r.set(input); return l; }
  
  // Arithmetic operators.
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
  
  // Comparison operators.
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
  
  // Arithmetic assignment operators.
                     inline Arb&        operator *= (Arb&        l, Arb        const& r) { return l.op_mul(r); }
                     inline Arb&        operator *= (Arb&        l, ArbInt     const& r) { return l.op_mul(r); }
                     inline ArbInt&     operator *= (ArbInt&     l, Arb        const& r) { l = Arb(r).op_mul(l). }
  template <class T> inline Arb&        operator *= (Arb&        l, SafeInt<T> const  r) { }
  template <class T> inline SafeInt<T>& operator *= (SafeInt<T>& l, Arb        const& r) { }
  template <class T> inline Arb&        operator *= (Arb&        l, T          const  r) { }
  template <class T> inline T&          operator *= (T&          l, Arb        const& r) { }
                     inline Arb&        operator /= (Arb&        l, Arb        const& r) { }
                     inline Arb&        operator /= (Arb&        l, ArbInt     const& r) { }
                     inline ArbInt&     operator /= (ArbInt&     l, Arb        const& r) { }
  template <class T> inline Arb&        operator /= (Arb&        l, SafeInt<T> const  r) { }
  template <class T> inline SafeInt<T>& operator /= (SafeInt<T>& l, Arb        const& r) { }
  template <class T> inline Arb&        operator /= (Arb&        l, T          const  r) { }
  template <class T> inline T&          operator /= (T&          l, Arb        const& r) { }
                     inline Arb&        operator %= (Arb&        l, Arb        const& r) { }
                     inline Arb&        operator %= (Arb&        l, ArbInt     const& r) { }
                     inline ArbInt&     operator %= (ArbInt&     l, Arb        const& r) { }
  template <class T> inline Arb&        operator %= (Arb&        l, SafeInt<T> const  r) { }
  template <class T> inline SafeInt<T>& operator %= (SafeInt<T>& l, Arb        const& r) { }
  template <class T> inline Arb&        operator %= (Arb&        l, T          const  r) { }
  template <class T> inline T&          operator %= (T&          l, Arb        const& r) { }
                     inline Arb&        operator += (Arb&        l, Arb        const& r) { }
                     inline Arb&        operator += (Arb&        l, ArbInt     const& r) { }
                     inline ArbInt&     operator += (ArbInt&     l, Arb        const& r) { }
  template <class T> inline Arb&        operator += (Arb&        l, SafeInt<T> const  r) { }
  template <class T> inline SafeInt<T>& operator += (SafeInt<T>& l, Arb        const& r) { }
  template <class T> inline Arb&        operator += (Arb&        l, T          const  r) { }
  template <class T> inline T&          operator += (T&          l, Arb        const& r) { }
                     inline Arb&        operator -= (Arb&        l, Arb        const& r) { }
                     inline Arb&        operator -= (Arb&        l, ArbInt     const& r) { }
                     inline ArbInt&     operator -= (ArbInt&     l, Arb        const& r) { }
  template <class T> inline Arb&        operator -= (Arb&        l, SafeInt<T> const  r) { }
  template <class T> inline SafeInt<T>& operator -= (SafeInt<T>& l, Arb        const& r) { }
  template <class T> inline Arb&        operator -= (Arb&        l, T          const  r) { }
  template <class T> inline T&          operator -= (T&          l, Arb        const& r) { }
  
}

/*****************************************************************************
 * Extensions to std::
 *****************************************************************************/
namespace std {
  
  DAC::Arb ceil  (DAC::Arb const& x);
  DAC::Arb floor (DAC::Arb const& x);
  
}

#endif
