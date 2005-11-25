/*****************************************************************************
 * abs.hxx
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
  
  // Additional namespace, hide utility functions.
  namespace absUtils {
    
    template <class T, bool>  class AbsSplit;
    template <class T> class AbsSplit<T, false> { public: static T op (T const value); };
    template <class T> class AbsSplit<T, true>  { public: static T op (T const value); };
    
  }
  
  // Declaration.
  template <class T> T abs (T const value);
  
  // Definition.
  template <class T> inline T abs (T const value) {
    return absUtils::AbsSplit<T, std::numeric_limits<T>::is_signed>::op(value);
  }
  
  namespace absUtils {
    
    template <class T> inline T AbsSplit<T, false>::op (T const value) { return value; }
    template <class T> inline T AbsSplit<T, true> ::op (T const value) {
      return (value > 0) ? value : ~value + 1;
    }
    
  }
  
}

#endif
