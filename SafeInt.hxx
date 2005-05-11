/*****************************************************************************
 * SafeInt.hxx
 *****************************************************************************
 * Safe integer class.
 *****************************************************************************/

// Include guard.
#if !defined(SAFEINT_3k54kbuihub7hbh0)
  #define SAFEINT_3k54kbuihub7hbh0

// STL includes.
  #include "Exception.hxx"
  
// Namespace wrapper.
namespace DAC {
  
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
      template <class U> explicit SafeInt (U const value);
      
      // Increment / decrement operators.
      SafeInt& operator ++ ();
      SafeInt  operator ++ (int);
      SafeInt& operator -- ();
      SafeInt  operator -- (int);
      
      
    
  };
  
}

#endif
