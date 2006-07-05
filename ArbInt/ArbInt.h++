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
                         ArbInt& set (ArbInt      const& number);
                         ArbInt& set (UArbInt     const& number);
                         ArbInt& set (std::string const& number);
      template <class T> ArbInt& set (SafeInt<T>  const  number);
      template <class T> ArbInt& set (T           const  number);
      
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
      
      // Raise this number to a power.
                         ArbInt pow (ArbInt  const& exp);
                         ArbInt pow (UArbInt const& exp);
      template <class T> ArbInt pow (T       const  exp);
      
    /*
     * Private members.
     */
    private:
      
      
      
  };
  
}

// End include guard.
#endif
