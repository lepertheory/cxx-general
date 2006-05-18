/*****************************************************************************
 * trim.c++
 *****************************************************************************
 * Implementation for string trimming functions.
 *****************************************************************************/

// Standard includes.
#include <string>

// Module includes.
#include "trim.h++"

// Namespaces used.
using namespace std;

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************/
  // Functions.
  
  /*
   * Trim whitespace from the beginning of a string.
   */
  string& ltrim (string& text, string const& whitespace) {
    
    // Don't work on an empty string.
    if (text.empty()) {
      return text;
    }
    
    // Find first non-whitespace character.
    string::size_type nowsbegin(text.find_first_not_of(whitespace));
    
    // If first character is non-whitespace, return exactly what we were sent.
    if (nowsbegin == 0) {
      return text;
    }
    
    // If there is no non-whitespace, return an empty string.
    if (nowsbegin == string::npos) {
      text.clear();
      return text;
    }
    
    // Erase whitespace and return. Do not add 1 when converting offset to
    // length because we want to preserve the character found.
    text.erase(0, nowsbegin);
    return text;
    
  }
  
  /*
   * Trim whitespace from the end of a string.
   */
  string& rtrim (string& text, string const& whitespace) {
    
    // Don't work on an empty string.
    if (text.empty()) {
      return text;
    }
    
    // Find last non-whitespace character.
    string::size_type nowsend(text.find_last_not_of(whitespace));
    
    // If last character is non-whitespace, return exactly what we were sent.
    if (nowsend == text.size() - 1) {
      return text;
    }
    
    // If there is no non-whitespace, return an empty string.
    if (nowsend == string::npos) {
      text.clear();
      return text;
    }
    
    // Erase whitespace and return.
    text.erase(nowsend + 1);
    return text;
    
  }
  
}

