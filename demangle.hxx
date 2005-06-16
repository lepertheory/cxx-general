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
  #if defined(PLAT_WIN32)
    #include <typeinfo.h>
  #elif defined(PLAT_POSIX)
    #include <cxxabi.h>
  #endif

// Namespace wrapping.
namespace DAC {
  
  // Prototype.
  template <class T> std::string demangle (T const& thing);
  
  // Implementation.
  template <class T> std::string demangle (T const& thing) {
    
    // Return value.
    std::string retval;
  
  #if defined(PLAT_WIN32)
    // Win32 version.
    
    // typeid operator throws an exception if there is a failure.
    try {
      retval = typeid(thing).name();
    } catch (...) {
      retval = "Unable to demangle type name.";
    }
    
  #elif defined(PLAT_POSIX)
    // Posix version.
    
    // Work area.
    int   status    = 0;
    char* demangled = 0;
    
    // Demangle the name. If an error is encountered, set the name to the
    // mangled version.
    if (!(demangled = abi::__cxa_demangle(typeid(thing).name(), 0, 0, &status))) {
      retval = typeid(thing).name();
    } else if (status != 0) {
      retval = typeid(thing).name();
    } else {
      retval = demangled;
    }
    
    // If demangled is not a null pointer, free it and set to null.
    if (demangled != 0) {
      free(demangled);
      demangled = 0;
    }
  #endif
    
    // We done, return.
    return retval;
    
  }
  
}

#endif
