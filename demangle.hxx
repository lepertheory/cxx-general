/*****************************************************************************
 * demangle.hxx
 *****************************************************************************
 * Returns a demangled version of the given type.
 *****************************************************************************/

// Include guard.
#if !defined(DEMANGLE_487ib9k837549y8274k9y823du)
  #define DEMANGLE_487ib9k837549y8274k9y823du
  
  // STL includes.
  #include <string>
  
  // Compiler includes.
  #include <cxxabi.h>
  
  // Namespace wrapping.
  namespace DAC {
    
    // Prototype.
    template <class T> std::string demangle (T const& thing);
    
    // Implementation.
    template <class T> std::string demangle (T const& thing) {
      
      int   status    = 0;
      char* demangled = 0;
      
      std::string retval;
      
      if (!(demangled = abi::__cxa_demangle(typeid(thing).name(), 0, 0, &status))) {
        retval = typeid(thing).name();
      } else if (status != 0) {
        retval = typeid(thing).name();
      } else {
        retval = demangled;
        free(demangled);
      }
      
      return retval;
      
    }
    
  };
  
#endif