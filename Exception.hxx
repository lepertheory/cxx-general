/*****************************************************************************
 * Exception.hxx
 *****************************************************************************
 * Base class for all exceptions thrown by me, DAC.
 *****************************************************************************/

// Include guard.
#if !defined(EXCEPTION_tubu848xo9i)
  #define EXCEPTION_tubu848xo9i
  
  // STL includes
  #include <exception>
  #include <string>
  #include <iostream>
  
  // Forward declarations.
  namespace DAC {
    class Exception;
  }
  
  // Stream output operator.
  std::ostream& operator << (std::ostream& left, DAC::Exception const& right);
  
  // Namespace wrapping.
  namespace DAC {
    
    /*************************************************************************
     * Exception
     *************************************************************************
     * Base class for all exceptions thrown.
     *************************************************************************/
    class Exception : public std::exception {
      
      /***********************************************************************
       * Public members.
       ***********************************************************************/
      public:
        
        // Constructor.
        Exception () throw();
        
        // Destructor.
        virtual ~Exception () throw();
        
        // Get the cause of this error.
        virtual const char*        what ()                        const throw();
        virtual std::string const& Text ()                        const throw();
        virtual Exception&         Text (std::string const& text)       throw();
        
        // Reset to just-constructed state.
        virtual Exception& clear () throw();
      
      /***********************************************************************
       * Protected members.
       ***********************************************************************/
      protected:
        
        const size_t _TEXTSIZE = 80;
        
        char _text[_TEXTSIZE];
      
    };
    
    /*************************************************************************
     * Inline and template definitions.
     *************************************************************************/
    
    // Constructor.
    inline Exception::Exception () throw() {
      clear();
    }
    
    // Destructor.
    inline Exception::~Exception () throw() {
      // Nothing.
    }
    
    // Describe the problem.
    inline char const* Exception::what () const throw() {
      try {
        return Text().c_str();
      } catch (...) {
        return 0;
      }
    }
    
    // Accessors.
    inline std::string Exception::Text () const throw() {
      try {
        return std::string(&text);
      } catch (...) {
        return std::string();
      }
    }
    inline Exception& Exception::Text (std::string const& text) throw() {
      strncpy(&_text, text.c_str(), _TEXTSIZE - 1);
      _text[_TEXTSIZE - 1] = '\0';
    }
    
    // Reset to just-constructed state.
    inline Exception& Exception::clear() throw() {
      _text[0] = '\0';
      return *this;
    }
    
  };

  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  inline std::ostream& operator << (std::ostream& left, DAC::Exception const& right) { left << right.Text(); return left; }
  
// End include guard.
#endif
