/*****************************************************************************
 * lcm.h++
 *****************************************************************************
 * Finds the least common multiple of two given numbers.
 *****************************************************************************/

// Include guard.
#if !defined(LCM_28ky54089bgyxkykg8)
  #define LCM_28ky54089bgyxkykg8

// System includes.
  #include <cxx-general/gcd.h++>

// Contain in namespace.
namespace DAC {
  
  // Declaration.
  template <class T> T lcm (T const& n1, T const& n2);
  
  // Definition.
  template <class T> T lcm (T const& n1, T const& n2) {
    
    // Work area.
    T tmp_n1(n1);
    T tmp_n2(n2);
    if (tmp_n1 < tmp_n2) {
      swap(tmp_n1, tmp_n2);
    }
    
    // LCM is (n1 * n2) / gcd(n1, n2). n1 is always the largest of the two,
    // so save the multiplication by n1 until the end when we have the
    // smallest number possible. This will prevent overflow if possible.
    return (tmp_n2 / gcd(tmp_n1, tmp_n2)) * tmp_n1;
    
  }
  
};

#endif
