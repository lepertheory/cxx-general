/*****************************************************************************
 * get_errorText.h++
 *****************************************************************************
 * Returns error text from an errno.
 *****************************************************************************/

// Include guard.
#if !defined(GETERRORTEXT_43uk97dud3)
  #define GETERRORTEXT_43uk97dud3

// Standard includes.
  #include <string>
  #include <cstring>
  #include <cerrno>

// System includes.
  #include <AutoArray.h++>

// Namespace wrapping.
namespace DAC {
  
  // Declaration.
  std::string& get_errorText (std::string& buffer, int errnum);
  
  // Definition.
  inline std::string& getErrorText (std::string& buffer, int errnum) {
    
    // Work area.
    // TODO: Find a way to get the maximum error message length. The current
    // value should be fine, but I don't like this.
    char buf[1024];
    int  retval   (0);
    
    // Try to get the error string. Retval may be a char* or it may be an int,
    // we have no way of knowing.
    retval = reinterpret_cast<int>(strerror_r(errnum, buf, sizeof(buf)));
    
    // I don't trust this function for a second.
    buf[sizeof(buf) - 1] = '\0';
    
  #if _XOPEN_SOURCE >= 600
    // SUSv3 version.
    if (!retval) {
      buffer = buf;
    } else {
      switch (retval) {
        case EINVAL: buffer = "Error number is invalid."                                        ; break;
        case ERANGE: buffer = "Buffer too small for error text."                                ; break;
        default    : buffer = "Unexpected error " + toString(retval) + " retrieving error text."; break;
      };
    }
  #else
    // GNU version.
    if (retval == buf) {
      buffer = buf;
    } else {
      buffer = retval;
    }
    return buffer;
  #endif
    
    // Done.
    return buffer;
    
  }
  
}

#endif

