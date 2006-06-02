/*****************************************************************************
 * demangle.h++
 *****************************************************************************
 * Returns a demangled version of the given type.
 *****************************************************************************/

// Include guard.
#if !defined(DEMANGLE_487ib9k837549y8274k9y823du)
  #define DEMANGLE_487ib9k837549y8274k9y823du

// STL includes.
  #include <string>

// Compiler includes.
  #include <typeinfo>
  #if defined(CC_GCC)
    #include <cxxabi.h>
  #endif

// Namespace wrapping.
namespace DAC {
  
  /***************************************************************************/
  // Functions.
  
  // Get the type of an object.
  template <class T> std::string& demangle (std::string& buffer, T const& thing = T()) throw();
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************/
  // Functions.
  
  /*
   * Get the type of an object.
   */
  template <class T> std::string& demangle (std::string& buffer, T const& thing) throw() {
    
    // Shut up the compiler.
    if (&thing) {};
    
  #if defined(CC_CL)
    // typeid operator throws an exception if there is a failure.
    try {
      buffer = typeid(thing).name();
    } catch (...) {
      buffer = "Unable to demangle type name.";
    }
  #elif defined(CC_GCC)
    // Work area.
    int   status   (0);
    char* demangled(0);
    
    // Demangle the name. If an error is encountered, set the name to the
    // mangled version.
    if (!(demangled = abi::__cxa_demangle(typeid(thing).name(), 0, 0, &status))) {
      buffer = typeid(thing).name();
    } else if (status != 0) {
      buffer = typeid(thing).name();
    } else {
      buffer = demangled;
    }
    
    // If demangled is not a null pointer, free it and set to null.
    if (demangled != 0) {
      free(demangled);
      demangled = 0;
    }
  #else
    // Unknown compiler.
    buffer = "Unable to determine compiler, cannot demangle.";
  #endif
    
    // We done, return.
    return buffer;
    
  }
  
}

#endif
