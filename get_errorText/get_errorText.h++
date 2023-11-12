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

// Namespace wrapping.
namespace DAC {
  
  /***************************************************************************/
  // Functions.
  
  // Get the error message for a particular errno.
  std::string& get_errorText (std::string& buffer, int errnum);
  
}

#endif

