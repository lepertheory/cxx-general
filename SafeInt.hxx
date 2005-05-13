/*****************************************************************************
 * SafeInt.hxx
 *****************************************************************************
 * Safe integer class.
 *****************************************************************************/

// Include guard.
#if !defined(SAFEINT_3k54kbuihub7hbh0)
  #define SAFEINT_3k54kbuihub7hbh0

// STL includes.
  #include <string>

// Internal includes.
  #include "Exception.hxx"
  
// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************
   * SafeInt utilities.
   ***************************************************************************/
  namespace SafeIntUtil {
  
    /*************************************************************************/
    // Enums.
    
    // Relationship types.
    enum RelType {
      SE_SE, SE_UE, SS_SL, SS_UL, SL_SS, SL_US,
      UE_UE, UE_SE, US_SL, US_UL, UL_SS, UL_US
    };
    
  }
    
  /***************************************************************************
   * SafeInt
   ***************************************************************************
   * This is an overflow safe replacement for any integer type.
   ***************************************************************************/
  template <class T> class SafeInt {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Function members.
      
      // Default constructor.
      explicit SafeInt (T const value = 0);
      
      // Copy constructor.
      SafeInt (SafeInt<T> const& value);
      
      // Conversion constructor.
      template <class U> explicit SafeInt (U          const  value);
      template <class U> explicit SafeInt (SafeInt<U> const& value);
      
      // Increment / decrement operators.
      SafeInt& operator ++ ();
      SafeInt  operator ++ (int);
      SafeInt& operator -- ();
      SafeInt  operator -- (int);
      
      // Sign operators.
      SafeInt operator + () const;
      SafeInt operator - () const;
      
      // Not operator.
      bool operator ! () const;
      
      // Bitwise compliment.
      SafeInt operator ~ () const;
      
      // Assignment operator.
                         SafeInt& operator = (SafeInt<T> const& value);
                         SafeInt& operator = (T          const  value);
      template <class U> SafeInt& operator = (SafeInt<U> const& value);
      template <class U> SafeInt& operator = (U          const  value);
      
      // Arithmetic assignment operators.
                         SafeInt& operator *= (SafeInt<T> const& value);
                         SafeInt& operator *= (T          const  value);
      template <class U> SafeInt& operator *= (SafeInt<U> const& value);
      template <class U> SafeInt& operator *= (U          const  value);
                         SafeInt& operator /= (SafeInt<T> const& value);
                         SafeInt& operator /= (T          const  value);
      template <class U> SafeInt& operator /= (SafeInt<U> const& value);
      template <class U> SafeInt& operator /= (U          const  value);
                         SafeInt& operator %= (SafeInt<T> const& value);
                         SafeInt& operator %= (T          const  value);
      template <class U> SafeInt& operator %= (SafeInt<U> const& value);
      template <class U> SafeInt& operator %= (U          const  value);
                         SafeInt& operator += (SafeInt<T> const& value);
                         SafeInt& operator += (T          const  value);
      template <class U> SafeInt& operator += (SafeInt<U> const& value);
      template <class U> SafeInt& operator += (U          const  value);
                         SafeInt& operator -= (SafeInt<T> const& value);
                         SafeInt& operator -= (T          const  value);
      template <class U> SafeInt& operator -= (SafeInt<U> const& value);
      template <class U> SafeInt& operator -= (U          const  value);
      
      // Bit shift assignment operators.
                         SafeInt& operator <<= (SafeInt<T> const& value);
                         SafeInt& operator <<= (T          const  value);
      template <class U> SafeInt& operator <<= (SafeInt<U> const& value);
      template <class U> SafeInt& operator <<= (U          const  value);
                         SafeInt& operator >>= (SafeInt<T> const& value);
                         SafeInt& operator >>= (T          const  value);
      template <class U> SafeInt& operator >>= (SafeInt<U> const& value);
      template <class U> SafeInt& operator >>= (U          const  value);
      
      // Bitwise assignment operators.
                         SafeInt& operator &= (SafeInt<T> const& value);
                         SafeInt& operator &= (T          const  value);
      template <class U> SafeInt& operator &= (SafeInt<U> const& value);
      template <class U> SafeInt& operator &= (U          const  value);
                         SafeInt& operator |= (SafeInt<T> const& value);
                         SafeInt& operator |= (T          const  value);
      template <class U> SafeInt& operator |= (SafeInt<U> const& value);
      template <class U> SafeInt& operator |= (U          const  value);
                         SafeInt& operator ^= (SafeInt<T> const& value);
                         SafeInt& operator ^= (T          const  value);
      template <class U> SafeInt& operator ^= (SafeInt<U> const& value);
      template <class U> SafeInt& operator ^= (U          const  value);
      
      // Arithmetic operator backends.
                         SafeInt& op_mul (SafeInt<T> const& value);
                         SafeInt& op_mul (T          const  value);
      template <class U> SafeInt& op_mul (SafeInt<U> const& value);
      template <class U> SafeInt& op_mul (U          const  value);
                         SafeInt& op_div (SafeInt<T> const& value);
                         SafeInt& op_div (T          const  value);
      template <class U> SafeInt& op_div (SafeInt<U> const& value);
      template <class U> SafeInt& op_div (U          const  value);
                         SafeInt& op_mod (SafeInt<T> const& value);
                         SafeInt& op_mod (T          const  value);
      template <class U> SafeInt& op_mod (SafeInt<U> const& value);
      template <class U> SafeInt& op_mod (U          const  value);
                         SafeInt& op_add (SafeInt<T> const& value);
                         SafeInt& op_add (T          const  value);
      template <class U> SafeInt& op_add (SafeInt<U> const& value);
      template <class U> SafeInt& op_add (U          const  value);
                         SafeInt& op_sub (SafeInt<T> const& value);
                         SafeInt& op_sub (T          const  value);
      template <class U> SafeInt& op_sub (SafeInt<U> const& value);
      template <class U> SafeInt& op_sub (U          const  value);
      
      // Bit shift operator backends.
                         SafeInt& op_shl (SafeInt<T> const& value);
                         SafeInt& op_shl (T          const  value);
      template <class U> SafeInt& op_shl (SafeInt<U> const& value);
      template <class U> SafeInt& op_shl (U          const  value);
                         SafeInt& op_shr (SafeInt<T> const& value);
                         SafeInt& op_shr (T          const  value);
      template <class U> SafeInt& op_shr (SafeInt<U> const& value);
      template <class U> SafeInt& op_shr (U          const  value);
      
      // Comparison operator backends.
                         bool op_gt (SafeInt<T> const& value) const;
                         bool op_gt (T          const  value) const;
      template <class U> bool op_gt (SafeInt<U> const& value) const;
      template <class U> bool op_gt (U          const  value) const;
                         bool op_ge (SafeInt<T> const& value) const;
                         bool op_ge (T          const  value) const;
      template <class U> bool op_ge (SafeInt<U> const& value) const;
      template <class U> bool op_ge (U          const  value) const;
                         bool op_lt (SafeInt<T> const& value) const;
                         bool op_lt (T          const  value) const;
      template <class U> bool op_lt (SafeInt<U> const& value) const;
      template <class U> bool op_lt (U          const  value) const;
                         bool op_le (SafeInt<T> const& value) const;
                         bool op_le (T          const  value) const;
      template <class U> bool op_le (SafeInt<U> const& value) const;
      template <class U> bool op_le (U          const  value) const;
                         bool op_eq (SafeInt<T> const& value) const;
                         bool op_eq (T          const  value) const;
      template <class U> bool op_eq (SafeInt<U> const& value) const;
      template <class U> bool op_eq (U          const  value) const;
                         bool op_ne (SafeInt<T> const& value) const;
                         bool op_ne (T          const  value) const;
      template <class U> bool op_ne (SafeInt<U> const& value) const;
      template <class U> bool op_ne (U          const  value) const;
      
      // Bitwise operator backends.
                         SafeInt& op_bit_and (SafeInt<T> const& value);
                         SafeInt& op_bit_and (T          const  value);
      template <class U> SafeInt& op_bit_and (SafeInt<U> const& value);
      template <class U> SafeInt& op_bit_and (U          const  value);
                         SafeInt& op_bit_ior (SafeInt<T> const& value);
                         SafeInt& op_bit_ior (T          const  value);
      template <class U> SafeInt& op_bit_ior (SafeInt<U> const& value);
      template <class U> SafeInt& op_bit_ior (U          const  value);
                         SafeInt& op_bit_xor (SafeInt<T> const& value);
                         SafeInt& op_bit_xor (T          const  value);
      template <class U> SafeInt& op_bit_xor (SafeInt<U> const& value);
      template <class U> SafeInt& op_bit_xor (U          const  value);
                         SafeInt& op_bit_cpm ();
      
      // Logical operator backends.
                         bool op_log_and (SafeInt<T> const& value) const;
                         bool op_log_and (T          const  value) const;
      template <class U> bool op_log_and (SafeInt<U> const& value) const;
      template <class U> bool op_log_and (U          const  value) const;
      
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Nested classes.
      
      // Determine the relationship between two types.
      template <class U> class _Relationship {
        public:
          const static SafeIntUtil::RelType value;
        private:
          static SafeIntUtil::RelType s_checkRelationship ();
      };
      
      // Safely cast.
      template <class U, SafeIntUtil::RelType> class _SafeCaster;
      template <class U> class _SafeCaster<U, SafeIntUtil::SE_SE> { public: static T cast (U const value); };
      template <class U> class _SafeCaster<U, SafeIntUtil::SE_UE> { public: static T cast (U const value); };
      template <class U> class _SafeCaster<U, SafeIntUtil::SS_SL> { public: static T cast (U const value); };
      template <class U> class _SafeCaster<U, SafeIntUtil::SS_UL> { public: static T cast (U const value); };
      template <class U> class _SafeCaster<U, SafeIntUtil::SL_SS> { public: static T cast (U const value); };
      template <class U> class _SafeCaster<U, SafeIntUtil::SL_US> { public: static T cast (U const value); };
      template <class U> class _SafeCaster<U, SafeIntUtil::UE_UE> { public: static T cast (U const value); };
      template <class U> class _SafeCaster<U, SafeIntUtil::UE_SE> { public: static T cast (U const value); };
      template <class U> class _SafeCaster<U, SafeIntUtil::US_SL> { public: static T cast (U const value); };
      template <class U> class _SafeCaster<U, SafeIntUtil::US_UL> { public: static T cast (U const value); };
      template <class U> class _SafeCaster<U, SafeIntUtil::UL_SS> { public: static T cast (U const value); };
      template <class U> class _SafeCaster<U, SafeIntUtil::UL_US> { public: static T cast (U const value); };
      
      /***********************************************************************/
      // Data members.
      
      // This is the number all this trouble is for.
      T _value;
      
  };
  
  /***************************************************************************
   * SafeInt Exceptions.
   ***************************************************************************/
  namespace SafeIntErrors {
    
    // All SafeInt errors are based off of this.
    class Base : public Exception {};
    
    // Overflow.
    class Overflow : public Base {
      public:
        virtual char const* what () const throw();
        virtual ~Overflow () throw();
      private:
        std::string _message;
    };
    
    // Overflow resulting from a cast.
    class CastOverflow : public Overflow {
      public:
        template <class T, class U> CastOverflow (T const num, U const limit) throw();
    };
    
  }
  
}

/*****************************************************************************
 * Global operators.
 *****************************************************************************/

// Stream I/O operators.
template <class T> std::ostream& operator << (std::ostream& l, DAC::SafeInt<T> const& r);
template <class T> std::istream& operator >> (std::istream& l, DAC::SafeInt<T>&       r);

// Arithmetic operators.
template <class T>          DAC::SafeInt<T> operator * (DAC::SafeInt<T> const& l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator * (DAC::SafeInt<T> const& l, T               const  r);
template <class T>          DAC::SafeInt<T> operator * (T               const  l, DAC::SafeInt<T> const& r);
template <class T, class U> DAC::SafeInt<T> operator * (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator * (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator * (U               const  l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator / (DAC::SafeInt<T> const& l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator / (DAC::SafeInt<T> const& l, T               const  r);
template <class T>          DAC::SafeInt<T> operator / (T               const  l, DAC::SafeInt<T> const& r);
template <class T, class U> DAC::SafeInt<T> operator / (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator / (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator / (U               const  l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator % (DAC::SafeInt<T> const& l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator % (DAC::SafeInt<T> const& l, T               const  r);
template <class T>          DAC::SafeInt<T> operator % (T               const  l, DAC::SafeInt<T> const& r);
template <class T, class U> DAC::SafeInt<T> operator % (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator % (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator % (U               const  l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator + (DAC::SafeInt<T> const& l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator + (DAC::SafeInt<T> const& l, T               const  r);
template <class T>          DAC::SafeInt<T> operator + (T               const  l, DAC::SafeInt<T> const& r);
template <class T, class U> DAC::SafeInt<T> operator + (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator + (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator + (U               const  l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator - (DAC::SafeInt<T> const& l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator - (DAC::SafeInt<T> const& l, T               const  r);
template <class T>          DAC::SafeInt<T> operator - (T               const  l, DAC::SafeInt<T> const& r);
template <class T, class U> DAC::SafeInt<T> operator - (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator - (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator - (U               const  l, DAC::SafeInt<T> const& r);

// Bit shift operators.
template <class T>          DAC::SafeInt<T> operator << (DAC::SafeInt<T> const& l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator << (DAC::SafeInt<T> const& l, T               const  r);
template <class T>          DAC::SafeInt<T> operator << (T               const  l, DAC::SafeInt<T> const& r);
template <class T, class U> DAC::SafeInt<T> operator << (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator << (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator << (U               const  l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator >> (DAC::SafeInt<T> const& l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator >> (DAC::SafeInt<T> const& l, T               const  r);
template <class T>          DAC::SafeInt<T> operator >> (T               const  l, DAC::SafeInt<T> const& r);
template <class T, class U> DAC::SafeInt<T> operator >> (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator >> (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator >> (U               const  l, DAC::SafeInt<T> const& r);

// Comparison operators.
template <class T>          bool operator >  (DAC::SafeInt<T> const& l, DAC::SafeInt<T> const& r);
template <class T>          bool operator >  (DAC::SafeInt<T> const& l, T               const  r);
template <class T>          bool operator >  (T               const  l, DAC::SafeInt<T> const& r);
template <class T, class U> bool operator >  (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> bool operator >  (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> bool operator >  (U               const  l, DAC::SafeInt<T> const& r);
template <class T>          bool operator >= (DAC::SafeInt<T> const& l, DAC::SafeInt<T> const& r);
template <class T>          bool operator >= (DAC::SafeInt<T> const& l, T               const  r);
template <class T>          bool operator >= (T               const  l, DAC::SafeInt<T> const& r);
template <class T, class U> bool operator >= (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> bool operator >= (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> bool operator >= (U               const  l, DAC::SafeInt<T> const& r);
template <class T>          bool operator <  (DAC::SafeInt<T> const& l, DAC::SafeInt<T> const& r);
template <class T>          bool operator <  (DAC::SafeInt<T> const& l, T               const  r);
template <class T>          bool operator <  (T               const  l, DAC::SafeInt<T> const& r);
template <class T, class U> bool operator <  (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> bool operator <  (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> bool operator <  (U               const  l, DAC::SafeInt<T> const& r);
template <class T>          bool operator <= (DAC::SafeInt<T> const& l, DAC::SafeInt<T> const& r);
template <class T>          bool operator <= (DAC::SafeInt<T> const& l, T               const  r);
template <class T>          bool operator <= (T               const  l, DAC::SafeInt<T> const& r);
template <class T, class U> bool operator <= (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> bool operator <= (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> bool operator <= (U               const  l, DAC::SafeInt<T> const& r);
template <class T>          bool operator == (DAC::SafeInt<T> const& l, DAC::SafeInt<T> const& r);
template <class T>          bool operator == (DAC::SafeInt<T> const& l, T               const  r);
template <class T>          bool operator == (T               const  l, DAC::SafeInt<T> const& r);
template <class T, class U> bool operator == (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> bool operator == (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> bool operator == (U               const  l, DAC::SafeInt<T> const& r);
template <class T>          bool operator != (DAC::SafeInt<T> const& l, DAC::SafeInt<T> const& r);
template <class T>          bool operator != (DAC::SafeInt<T> const& l, T               const  r);
template <class T>          bool operator != (T               const  l, DAC::SafeInt<T> const& r);
template <class T, class U> bool operator != (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> bool operator != (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> bool operator != (U               const  l, DAC::SafeInt<T> const& r);

// Bitwise operators.
template <class T>          DAC::SafeInt<T> operator & (DAC::SafeInt<T> const& l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator & (DAC::SafeInt<T> const& l, T               const  r);
template <class T>          DAC::SafeInt<T> operator & (T               const  l, DAC::SafeInt<T> const& r);
template <class T, class U> DAC::SafeInt<T> operator & (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator & (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator & (U               const  l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator | (DAC::SafeInt<T> const& l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator | (DAC::SafeInt<T> const& l, T               const  r);
template <class T>          DAC::SafeInt<T> operator | (T               const  l, DAC::SafeInt<T> const& r);
template <class T, class U> DAC::SafeInt<T> operator | (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator | (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator | (U               const  l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator ^ (DAC::SafeInt<T> const& l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator ^ (DAC::SafeInt<T> const& l, T               const  r);
template <class T>          DAC::SafeInt<T> operator ^ (T               const  l, DAC::SafeInt<T> const& r);
template <class T, class U> DAC::SafeInt<T> operator ^ (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator ^ (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator ^ (U               const  l, DAC::SafeInt<T> const& r);

// Logical operators.
template <class T>          DAC::SafeInt<T> operator && (DAC::SafeInt<T> const& l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator && (DAC::SafeInt<T> const& l, T               const  r);
template <class T>          DAC::SafeInt<T> operator && (T               const  l, DAC::SafeInt<T> const& r);
template <class T, class U> DAC::SafeInt<T> operator && (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator && (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator && (U               const  l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator || (DAC::SafeInt<T> const& l, DAC::SafeInt<T> const& r);
template <class T>          DAC::SafeInt<T> operator || (DAC::SafeInt<T> const& l, T               const  r);
template <class T>          DAC::SafeInt<T> operator || (T               const  l, DAC::SafeInt<T> const& r);
template <class T, class U> DAC::SafeInt<T> operator || (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator || (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator || (U               const  l, DAC::SafeInt<T> const& r);

/*****************************************************************************
 * Inline and template definitions.
 *****************************************************************************/

namespace DAC {
  
  /***************************************************************************
   * Class SafeInt.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  // Default constructor.
  template <class T> inline SafeInt<T>::SafeInt (T const value) { _value = value; }
  
  // Copy constructor.
  template <class T> inline SafeInt<T>::SafeInt (SafeInt<T> const& value) { _value = value._value; }
  
  // Conversion constructor.
  template <class T> template <class U> inline SafeInt<T>::SafeInt (U          const  value) { _value = _SafeCaster<U, _Relationship<U>::value>::cast(value);        }
  template <class T> template <class U> inline SafeInt<T>::SafeInt (SafeInt<U> const& value) { _value = _SafeCaster<U, _Relationship<U>::value>::cast(value._value); }
  
  // Increment / decrement operators.
  template <class T> inline SafeInt<T>& SafeInt<T>::operator ++ ()    { return op_add(1);                                   }
  template <class T> inline SafeInt<T>  SafeInt<T>::operator ++ (int) { SafeInt<T> retval(*this); op_add(1); return retval; }
  template <class T> inline SafeInt<T>& SafeInt<T>::operator -- ()    { return op_sub(1);                                   }
  template <class T> inline SafeInt<T>  SafeInt<T>::operator -- (int) { SafeInt<T> retval(*this); op_sub(1); return retval; }
  
  // Sign operators.
  template <class T> inline SafeInt<T> SafeInt<T>::operator + () const { return *this;      }
  template <class T> inline SafeInt<T> SafeInt<T>::operator - () const { return *this * -1; }
  
  // Not operator.
  template <class T> inline bool SafeInt<T>::operator ! () const { return !_value; }
  
  // Bitwise compliment.
  template <class T> inline SafeInt<T> SafeInt<T>::operator ~ () const { return SafeInt<T>(*this).op_bit_cpm(); }
  
  // Assignment operator.
  template <class T>                    inline SafeInt<T>& SafeInt<T>::operator = (SafeInt<T> const& value) { _value = value._value;                                                                              return *this; }
  template <class T>                    inline SafeInt<T>& SafeInt<T>::operator = (T          const  value) { _value = value;                                                                                     return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator = (SafeInt<U> const& value) { _value = _SafeCaster<U, _Relationship<U>::value>::cast(value._value); return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator = (U          const  value) { _value = _SafeCaster<U, _Relationship<U>::value>::cast(value);        return *this; }
  
  /*************************************************************************
   * Class SafeInt::_SafeCaster.
   *************************************************************************/
  
  // Cast.
  template <class T> template <class U> inline T SafeInt<T>::_SafeCaster<U, SafeIntUtil::SE_SE>::cast (U const value) { return static_cast<T>(value); }
  template <class T> template <class U> inline T SafeInt<T>::_SafeCaster<U, SafeIntUtil::SE_UE>::cast (U const value) { return static_cast<T>(value); }
  template <class T> template <class U> inline T SafeInt<T>::_SafeCaster<U, SafeIntUtil::SS_SL>::cast (U const value) {
    if (value < static_cast<U>(std::numeric_limits<T>::min())) {
      throw SafeIntErrors::CastOverflow(value, std::numeric_limits<T>::min());
    }
    if (value > static_cast<U>(std::numeric_limits<T>::max())) {
      throw SafeIntErrors::CastOverflow(value, std::numeric_limits<T>::max());
    }
    return static_cast<T>(value);
  }
  template <class T> template <class U> inline T SafeInt<T>::_SafeCaster<U, SafeIntUtil::SS_UL>::cast (U const value) {
    if (value > static_cast<U>(std::numeric_limits<T>::max())) {
      throw SafeIntErrors::CastOverflow(value, std::numeric_limits<T>::max());
    }
    return static_cast<T>(value);
  }
  template <class T> template <class U> inline T SafeInt<T>::_SafeCaster<U, SafeIntUtil::SL_SS>::cast (U const value) { return static_cast<T>(value); }
  template <class T> template <class U> inline T SafeInt<T>::_SafeCaster<U, SafeIntUtil::SL_US>::cast (U const value) { return static_cast<T>(value); }
  template <class T> template <class U> inline T SafeInt<T>::_SafeCaster<U, SafeIntUtil::UE_UE>::cast (U const value) { return static_cast<T>(value); }
  template <class T> template <class U> inline T SafeInt<T>::_SafeCaster<U, SafeIntUtil::UE_SE>::cast (U const value) {
    if (value < 0) {
      throw SafeIntErrors::CastOverflow(value, 0);
    }
    return static_cast<T>(value);
  }
  template <class T> template <class U> inline T SafeInt<T>::_SafeCaster<U, SafeIntUtil::US_SL>::cast (U const value) {
    if (value < 0) {
      throw SafeIntErrors::CastOverflow(value, 0);
    }
    if (value > static_cast<U>(std::numeric_limits<T>::max())) {
      throw SafeIntErrors::CastOverflow(value, std::numeric_limits<T>::max());
    }
    return static_cast<T>(value);
  }
  template <class T> template <class U> inline T SafeInt<T>::_SafeCaster<U, SafeIntUtil::UL_SS>::cast (U const value) {
    if (value < 0) {
      throw SafeIntErrors::CastOverflow(value, 0);
    }
    return static_cast<T>(value);
  }
  template <class T> template <class U> inline T SafeInt<T>::_SafeCaster<U, SafeIntUtil::UL_US>::cast (U const value) { return static_cast<T>(value); }
  
  /*************************************************************************
   * Class SafeInt::_Relationship.
   *************************************************************************/
  
  /*************************************************************************/
  // Static data members.
  
  // Relationship between T and U.
  template <class T> template <class U> const SafeIntUtil::RelType SafeInt<T>::_Relationship<U>::value(s_checkRelationship());
  
  /*************************************************************************/
  // Static function members.
  
  // Find the size / sign relationship between type T and type U.
  template <class T> template <class U> SafeIntUtil::RelType SafeInt<T>::_Relationship<U>::s_checkRelationship () {
    
    // Check the size relationship.
    if (std::numeric_limits<T>::digits > std::numeric_limits<U>::digits) {
      if (std::numeric_limits<T>::is_signed) {
        if (std::numeric_limits<U>::is_signed) {
          return SafeIntUtil::SL_SS;
        } else {
          return SafeIntUtil::SL_US;
        }
      } else {
        if (std::numeric_limits<U>::is_signed) {
          return SafeIntUtil::UL_SS;
        } else {
          return SafeIntUtil::UL_US;
        }
      }
    } else if (std::numeric_limits<T>::digits < std::numeric_limits<U>::digits) {
      if (std::numeric_limits<T>::is_signed) {
        if (std::numeric_limits<U>::is_signed) {
          return SafeIntUtil::SS_SL;
        } else {
          return SafeIntUtil::SS_UL;
        }
      } else {
        if (std::numeric_limits<U>::is_signed) {
          return SafeIntUtil::US_SL;
        } else {
          return SafeIntUtil::US_UL;
        }
      }
    } else {
      if (std::numeric_limits<T>::is_signed) {
        if (std::numeric_limits<U>::is_signed) {
          return SafeIntUtil::SE_SE;
        } else {
          return SafeIntUtil::SE_UE;
        }
      } else {
        if (std::numeric_limits<U>::is_signed) {
          return SafeIntUtil::UE_SE;
        } else {
          return SafeIntUtil::UE_UE;
        }
      }
    }
    
  }
  
  /***************************************************************************
   * SafeInt Errors.
   ***************************************************************************/
  
  namespace SafeIntErrors {
    
    inline char const* Overflow::what () const throw() {
      try {
        if (!_message.empty()) {
          return _message.c_str();
        } else {
          return "Overflow.";
        }
      } catch (...) {
        return "Error returning message string.";
      }
    }
    inline Overflow::~Overflow () throw() {}
    
    template <class T, class U> CastOverflow::CastOverflow (T const num, U const limit) throw () {
      try {
        _message = toString(num) + " overflows type limit of " + toString(limit) + " in attempted cast.";
      } catch (...) {
        _message.clear();
      }
    }
  }
  
}

#endif
