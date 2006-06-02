/*****************************************************************************
 * logBase.h++
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
  
  /***************************************************************************/
  // Functions.
  
  // Get the logarithm of a number in a given base.
  template <class NumT, class BaseT> long double logBase (NumT const& number, BaseT const& base) throw();
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************/
  // Functions.
  
  /*
   * Find the logarithm of a number in a given base.
   */
  template <class NumT, class BaseT> inline long double logBase (NumT const& number, BaseT const& base) throw() {
    return std::log(static_cast<long double>(number)) / std::log(static_cast<long double>(base));
  }
  
}

// End include guard.
#endif
