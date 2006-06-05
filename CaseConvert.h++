/*****************************************************************************
 * CaseConvert
 *****************************************************************************
 * Convert text to upper or lower case.
 *****************************************************************************/

// Include guard.
#if !defined(CASECONVERT_894ukb89)
  #define CASECONVERT_894ukb89

// Standard includes.
#include <cctype>
#include <string>

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************
   * Declarations.
   ***************************************************************************/
  
  /***************************************************************************/
  // Functions.
  
  // Convert a string to uppercase.
  std::string& uppercase (std::string& text);
  
  // Convert a string to lowercase.
  std::string& lowercase (std::string& text);
  
  // Convert a string.
  std::string& convert_string (std::string& text, char (*char_conversion) (char const));
  
  // Convert a single character to uppercase.
  char char_uppercase (char const character);
  
  // Convert a single character to lowercase.
  char char_lowercase (char const character);
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /*
   * Convert a string to uppercase.
   */
  inline std::string& uppercase (std::string& text) { return convert_string(text, char_uppercase); }
  
  /*
   * Convert a string to lowercase.
   */
  inline std::string& lowercase (std::string& text) { return convert_string(text, char_lowercase); }
  
  /*
   * Convert a string.
   */
  inline std::string& convert_string (std::string& text, char (*char_conversion) (char const)) {
    
    // Apply the conversion character by character.
    for (std::string::iterator i = text.begin(); i != text.end(); ++i) {
      *i = char_conversion(*i);
    }
    
    // Done.
    return text;
    
  }
  
  /*
   * Convert a single character to uppercase.
   */
  inline char char_uppercase (char const character) { return std::toupper(character); }
  
  /*
   * Convert a single character to lowercase.
   */
  inline char char_lowercase (char const character) { return std::tolower(character); }
  
}

// End include guard.
#endif

