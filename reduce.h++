/*****************************************************************************
 * reduce.h++
 *****************************************************************************
 * Reduce a fraction.
 *****************************************************************************/

// Internal includes.
#include "cxx-general/gcd.h++"

// Include guard.
#if !defined(REDUCE_ykbf73k3ub834b)
  #define REDUCE_ykbf73k3ub834b

// Contain in namepsace.
namespace DAC {
  
  // Reduce a given fraction.
  template <class T> void reduce (T& p, T& q) {
    
    // Get the greatest common divisor.
    T tmp = gcd(p, q);
    
    // Reduce.
    p /= tmp;
    q /= tmp;
    
  }
  
}

#endif