/*****************************************************************************
 * logBase.hxx
 *****************************************************************************
 * Get the logarithm of a number in a given base.
 *****************************************************************************/

// Include guard.
#if !defined(LOGBASE_i9o8heux978do)
  #define LOGBASE_i9o8heux978do
  
  // STL includes.
  #include <cmath>
  
  // Contain in namespace.
  namespace DAC {
    
    /*************************************************************************/
    // Get the logarithm of a number in a given base.
    template <class NumT, class BaseT> NumT logBase (NumT const& number, BaseT const& base) {
      
      return (std::log(number) / std::log(base));
      
    }
    /*************************************************************************/
    
  };
  
// End include guard.
#endif
