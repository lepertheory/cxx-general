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
  
  // Internal includes.
  #include "ReferencePointer.hxx"
  
  // Forward declarations.
  namespace DAC {
    class Exception;
  }
  
  // Stream output operator.
  std::ostream& operator << (std::ostream& left, DAC::Exception const& right);
  
  // Contain wrapping.
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
        virtual const char*        what () const throw();
        virtual std::string const& Text () const throw();
        
        // Reset to just-constructed state.
        virtual Exception& clear () throw();
      
      /***********************************************************************
       * Protected members.
       ***********************************************************************/
      protected:
        
        char _text[80];
      
    };
    
    /*************************************************************************
     * Inline and template definitions.
     *************************************************************************/
    
    /*************************************************************************/
    // Constructor.
    inline Exception::Exception () throw() {
      clear();
    }
    /*************************************************************************/
    
    /*************************************************************************/
    // Destructor.
    inline Exception::~Exception () throw() {
      // Nothing.
    }
    /*************************************************************************/
    
    /*************************************************************************/
    // Describe the problem.
    inline char const* Exception::what () const throw() {
      try {
        return Text().c_str();
      } catch (...) {
        return 0;
      }
    }
    /*************************************************************************/
    
    /*************************************************************************/
    // Accessors.
    inline std::string Exception::Text () const throw() {
      try {
        return std::string(&text);
      } catch (...) {
        return std::string();
      }
    }
    /*************************************************************************/
    
    /*************************************************************************/
    // Reset to just-constructed state.
    inline Exception& Exception::clear() throw() {
      _text[0] = '\0';
      return *this;
    }
    /*************************************************************************/
    
  };

  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  inline std::ostream& operator << (std::ostream& left, DAC::Exception const& right) { left << right.Text(); return left; }
  
// End include guard.
#endif
