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
        virtual const char* what () const throw();
        
        // Reset to just-constructed state.
        virtual Exception& clear () throw();
      
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
      return "Undefined error.";
    }
    
    // Reset to just-constructed state.
    inline Exception& Exception::clear() throw() {
      return *this;
    }
    
  };

  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  inline std::ostream& operator << (std::ostream& left, DAC::Exception const& right) { left << right.what(); return left; }
  
// End include guard.
#endif
