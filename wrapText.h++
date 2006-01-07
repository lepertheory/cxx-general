/*****************************************************************************
 * wrapText
 *****************************************************************************
 * Wraps text to a given length.
 *****************************************************************************/

// Include guard.
#if !defined(WRAPTEXT_81pjibu0d)
  #define WRAPTEXT_81pjibu0d

// Standard includes.
  #include <string>

// System includes.
  #include <cxx-general/Exception.h++>

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************/
  // Errors.
  namespace wrapText {
    class Errors {
      public:
        
        // All wrapText errors are based off of this.
        class Base : public Exception {
          public:
            virtual ~Base () throw() {};
            virtual char const* what () const throw() { return "Unknown error in wrapText()."; };
        }
        
      // Instantiation of this class is not allowed.
      private:
        Errors ();
        Errors (Errors const&);
        Errors& operator = (Errors const&);
        
    };
  }
  
  /***************************************************************************/
  // Functions.

  // Wrap text.
  std::string wrapText (std::string const& text, size_t const width);
  

  
}

#endif

