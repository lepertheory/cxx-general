/*****************************************************************************
 * xorswap.hxx
 *****************************************************************************
 * Swap two numbers by repeated XORing.
 *****************************************************************************/

// Include guard.
#if !defined(XORSWAP_234bk89p7ikx48ibo987pik938)
  #define XORSWAP_234bk89p7ikx48ibo987pik938
  
  /***************************************************************************/
  // Swap two numbers by XORing.
  template <class T> inline void xorSwap (T& left, T& right) {
    
    // Make sure that the two numbers are not the same, if they are, there's
    // no need to swap them.
    if (left != right) {
      
      // This is the XOR swap. You should know this.
      left  ^= right;
      right ^= left;
      left  ^= right;
      
    }
    
  }
  /***************************************************************************/
  
#endif
