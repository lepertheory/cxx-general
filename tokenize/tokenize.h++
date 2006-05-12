/*****************************************************************************
 * tokenize.h++
 *****************************************************************************
 * Interface for function tokenize().
 *****************************************************************************/

// Include guard.
#if !defined(TOKENIZE_9h84uh890u4)
  #define TOKENIZE_9h84uh890u4

// Standard includes.
#include <string>
#include <vector>

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************/
  // Functions.
  
  // Tokenize a string.
  void tokenize (std::string const& strtotok, std::vector<std::string>& retval, std::string const& delim = " \t\n");
  
}

// End include guard.
#endif

