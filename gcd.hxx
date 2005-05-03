/*****************************************************************************
 * gcd.hxx
 *****************************************************************************
 * Finds the greatest common divisor of two given numbers.
 *****************************************************************************/

// Include guard.
#if !defined(GCD_23pbuk78ud34jdux9)
  #define GCD_23pbuk78ud34jdux9
  
  // Contain in namespace.
  namespace DAC {
    
    // Declaration
    template <class T> T gcd (T const& n1, T const& n2);
    
    // Definition
    template <class T> T gcd (T const& n1, T const& n2) {
      
      // Return value.
      T retval;
      
      // Work area. tmp_n1 must be > tmp_n2.
      T tmp_n1(n1);
      T tmp_n2(n2);
      if (tmp_n1 < tmp_n2) {
        swap(tmp_n1, tmp_n2);
      }
      
      // Euclid's algorithm.
      while (tmp_n2 != 0) {
        T tmp  = tmp_n2;
        tmp_n2 = tmp_n1 % tmp_n2;
        tmp_n1 = tmp;
      }
      
      // We done, return the result.
      return tmp_n1;
      
    }
    
  };
  
#endif
