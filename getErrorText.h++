/*****************************************************************************
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
  #include <cxx-general/AutoArray.h++>

// Namespace wrapping.
namespace DAC {
  
  // Declaration.
  std::string getErrorText (int errnum);
  
  // Definition.
  inline std::string getErrorText (int errnum) {
    
    // Constants.
    static unsigned int const MINBUF =
  #if defined(GETERRORTEXT_MINLEN)
      GETERRORTEXT_MINLEN
  #elif defined(GETERRORTEXT_MAXLEN)
      GETERRORTEXT_MAXLEN / 4
  #else
      128
  #endif
    ;
    static unsigned int const MAXBUF =
  #if defined(GETERRORTEXT_MAXLEN)
      GETERRORTEXT_MAXLEN
  #elif defined(GETERRORTEXT_MINLEN)
      GETERRORTEXT_MINLEN * 4
  #else
      512
  #endif
    ;
    
    // Try to get the error string, start with a small buffer and double it
    // if not large enough.
    for (unsigned int bufsize = MINBUF; bufsize <= MAXBUF; bufsize *= 2) {
      
      // Try to get the error string. Retval may be a char* or it may be an
      // int, we have no way of knowing. This is why #define sucks. Hopefully
      // int and pointer are the same size on your system, Also not a big fan
      // of the GNU version of this. Silent truncation can bite my ass. 
      AutoArray<char> buf(new char[bufsize]);
      int retval = reinterpret_cast<int>(strerror_r(errnum, buf.get(), bufsize));
      
      // Success with the SUSv3 version.
      if (!retval) {
        return buf.get();
      }
      
      // Successs with the GNU version.
      if (retval != -1) {
        return reinterpret_cast<char*>(retval);
      }
      
      // Unsuccessful. If because error code is invalid, return that info.
      if (errno == EINVAL) {
        return "Error number is invalid.";
      }
      
    }
    
    // Error text is larger than max buffer size.
    return "Error text is larger than max buffer size.";
    
  }
  
}

#endif

