/*****************************************************************************
 * abs.hxx
 *****************************************************************************
 * Returns the absolute value of a number.
 *****************************************************************************/

// Include guard.
#if !defined(ABS_2897ydeuD7AD87ud8a)
  #define ABS_2897ydeuD7AD87ud8a
  
  // Contain in namespace.
  namespace DAC {
  
    /*************************************************************************/
    // Return the absolute value for a number. Works for any type that defines
    // a numeric_limits specialization and the - unary operator.
    template <class T> inline T abs (T const& number) {
      
      if (number < 0) {
        return -number;
      } else {
        return number;
      }
      
    }
    /*************************************************************************/
    
  };
    
// End include guard.
#endif
