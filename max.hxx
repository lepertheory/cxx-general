/*****************************************************************************
 * max.hxx
 *****************************************************************************
 * Returns the larger of two values.
 *****************************************************************************/

// Include guard.
#if !defined(MAX_jfiwnva8s829vr92v)
  #define MAX_jfiwnva8s829vr92v
  
  // Contain in namespace.
  namespace DAC {
    
    /***************************************************************************/
    // Return the larger of two values. Will work for any type that defines a >
    // operator.
    template <class T> inline T max (T const left, T const right) {
      
      if (left > right) {
        return left;
      } else {
        return right;
      }
      
    };
    /***************************************************************************/
    
  };
  
// End include guard.
#endif
