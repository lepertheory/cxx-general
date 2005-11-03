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

// System includes.
  #include <cxx-general/demangle.hxx>

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************
   * Exception
   ***************************************************************************
   * Base class for all exceptions thrown.
   ***************************************************************************/
  class Exception : public std::exception {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Function members.
      
      // Constructor.
      Exception () throw();
      
      // Destructor.
      virtual ~Exception () throw();
      
      // Get the cause of this error.
      virtual char const* what () const throw();
      
      // Get the type of this error.
      std::string type () const throw();
      
      // Reset to just-constructed state.
      virtual Exception& clear () throw();
    
  };
  
}

/*****************************************************************************
 * Global operators.
 *****************************************************************************/

// Stream output operator.
std::ostream& operator << (std::ostream& left, DAC::Exception const& right);
  
/*****************************************************************************
 * Inline and template definitions.
 *****************************************************************************/

namespace DAC {
  
  inline Exception::Exception () throw() { clear(); }
  
  inline Exception::~Exception () throw() { /* Nothing. */ }
  
  inline char const* Exception::what () const throw() { return "Undefined error."; }
  
  inline std::string Exception::type() const throw() { return demangle(*this); }
  
  inline Exception& Exception::clear() throw() { return *this; }
  
}

/***************************************************************************
 * Inline and template definitions.
 ***************************************************************************/

// Stream output operator.
inline std::ostream& operator << (std::ostream& left, DAC::Exception const& right) { left << right.what(); return left; }

// End include guard.
#endif
