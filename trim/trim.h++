/*****************************************************************************
 * trim.h++
 *****************************************************************************
 * Interface for string trimming functions.
 *****************************************************************************/

// Include guard.
#if !defined(TRIM_8hu489mu498mu)
  #define TRIM_8hu489mu498mu

// Standard includes.
  #include <string>

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************/
  // Functions.
  
  // Trim whitespace from a string.
  std::string& ltrim (std::string& text, std::string const& whitespace = " \t\n");
  std::string& rtrim (std::string& text, std::string const& whitespace = " \t\n");
  std::string& trim  (std::string& text, std::string const& whitespace = " \t\n");
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************/
  // Functions.
  
  /*
   * Trim whitespace from beginning and end of a string.
   */
  inline std::string& trim (std::string& text, std::string const& whitespace) {
    rtrim(text, whitespace);
    ltrim(text, whitespace);
    return text;
  }
  
}

// End include guard.
#endif

