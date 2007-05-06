/*****************************************************************************
 * abs.h++
 *****************************************************************************
 * I should not have to do this.
 *****************************************************************************/

// Include guard.
#if !defined(ABS_9u54ku2kbuk4u3kbd89)
  #define ABS_9u54ku2kbuk4u3kbd89
  
// STL includes.
  #include <limits>

// Contain in namespace.
namespace DAC {
  
  /***************************************************************************/
  // Functions.
  
  // Get absolute value of a number.
  template <class T> T abs (T const value);
  
  // Additional namespace, hide utility functions.
  namespace absUtils {
    template <class T, bool>  class AbsSplit;
    template <class T> class AbsSplit<T, false> { public: static T op (T const value); };
    template <class T> class AbsSplit<T, true > { public: static T op (T const value); };
  }
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Get absolute value of a number.
   */
  template <class T> inline T abs (T const value) {
    return absUtils::AbsSplit<T, std::numeric_limits<T>::is_signed>::op(value);
  }
  
  /*
   * Get the abs without multiplying by -1, should compile down to a no-op for
   * unsigned values.
   */
  namespace absUtils {
    template <class T> inline T AbsSplit<T, false>::op (T const value) { return value; }
    template <class T> inline T AbsSplit<T, true >::op (T const value) {
      return (value > 0) ? value : ~value + 1;
    }
  }
  
}

#endif
