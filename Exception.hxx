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
        ~Exception () throw();
        
        // Accessors.
        virtual std::exception const* Previous ()                                     throw();
        virtual Exception&            Previous (std::exception const* previous)       throw();
        virtual std::string const&    Text     ()                               const throw();
        virtual Exception&            Text     (std::string const& text)              throw();
        
        // Reset to just-constructed state.
        virtual Exception& clear () throw();
        
        // Describe the problem.
        virtual char const* what () const throw();
        
      /***********************************************************************
       * Protected members.
       ***********************************************************************/
      protected:
        
        std::exception const* _previous;
        std::string           _text;
      
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
    // Accessors.
    inline std::exception const* Exception::Previous () throw() {
      return _previous;
    }
    inline Exception& Exception::Previous (std::exception const* previous) throw() {
      _previous = previous;
      return *this;
    }
    inline std::string const& Exception::Text () const throw() {
      return _text;
    }
    inline Exception& Exception::Text (std::string const& text) throw() {
      try {
        _text = text;
      } catch (...) {
      }
      return *this;
    }
    /*************************************************************************/
    
    /*************************************************************************/
    // Reset to just-constructed state.
    inline Exception& Exception::clear() throw() {
      
      _text.clear();
      _previous = 0;
      
      return *this;
      
    }
    /*************************************************************************/
    
    /*************************************************************************/
    // Describe the problem.
    inline char const* Exception::what () const throw() {
      
      try {
        return _text.c_str();
      } catch (...) {
        return 0;
      }
      
    }
    /*************************************************************************/
    
  };

  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  inline std::ostream& operator << (std::ostream& left, DAC::Exception const& right) { left << right.Text(); return left; }
  
// End include guard.
#endif
