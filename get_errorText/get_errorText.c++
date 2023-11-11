/*****************************************************************************
 * get_errorText.c++
 *****************************************************************************
 * Returns error text from an errno.
 *****************************************************************************/

// Standard includes.
#include <string>
#include <cstring>

// Namespace wrapping.
namespace DAC {
  
  /***************************************************************************/
  // Functions.
  
  /*
   * Get the error message for a particular errno.
   */
  std::string& get_errorText (std::string& buffer, int errnum) {
    
#if defined(GET_ERRORTEXT_REENTRANT)
    // Work area.
    char  buf[1024];
  #if (_POSIX_C_SOURCE >= 200112L) && !_GNU_SOURCE
    int   retval;
  #else
    char* retval;
  #endif
    
    // Try to get the error string.
    retval = strerror_r(errnum, buf, sizeof(buf));
    
    // I don't trust this function for a second.
    buf[sizeof(buf) - 1] = '\0';
    
  #if (_POSIX_C_SOURCE >= 200112L) && !_GNU_SOURCE
    // SUSv3 version.
    if (!retval) {
      buffer = buf;
    } else {
      switch (retval) {
        case EINVAL: buffer = "Error number is invalid."                                         ; break;
        case ERANGE: buffer = "Buffer too small for error text."                                 ; break;
        default    : buffer = "Unexpected error " + to_string(retval) + " retrieving error text."; break;
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
#else
    // Get the error string.
    buffer = strerror(errnum);
#endif
    
    // Done.
    return buffer;
    
  }
  
}

