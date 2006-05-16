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
    // TODO: Find a way to get the maximum error message length. The current
    // value should be fine, but I don't like this.
    char  buf[1024];
  #if _XOPEN_SOURCE >= 600
    int   retval;
  #else
    char* retval;
  #endif
    
    // Try to get the error string.
    retval = strerror_r(errnum, buf, sizeof(buf));
    
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

