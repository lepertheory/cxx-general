/*****************************************************************************
 * Exception.h++
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
  #include <demangle.h++>

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
      std::string      & type (std::string& buffer) const throw();
      std::string const& type (                   ) const throw();
      
      // Reset to just-constructed state.
      virtual Exception& clear () throw();
      
      // Buffer a temporary string.
      char const* buffer_message (std::string& message) const throw();
      
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Data members.
      
      // Buffer for constructed error messages.
      mutable std::string _buffer;
      
      // Buffer for other returned strings.
      mutable std::string _strbuf;
    
  };
  
/*****************************************************************************
 * Operators.
 *****************************************************************************/
}

// Stream output operator.
std::ostream& operator << (std::ostream& left, DAC::Exception const& right);

namespace DAC {
  
  /***************************************************************************
  * Inline and template definitions.
  ****************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  inline Exception::Exception () throw() { clear(); }
  
  inline Exception::~Exception () throw() { /* Nothing. */ }
  
  inline char const* Exception::what () const throw() { return "Undefined error."; }
  
  inline std::string      & Exception::type(std::string& buffer) const throw() { return demangle(buffer, *this); }
  inline std::string const& Exception::type(                   ) const throw() { return type(_strbuf);           }
  
  inline Exception& Exception::clear() throw() { return *this; }
  
  /*
   * Buffer a temporary string.
   */
  inline char const* Exception::buffer_message (std::string& message) const throw() {
    
    // Swap the contents of the message with the buffer.
    _buffer.swap(message);
    
    // Return a pointer to the char array.
    return _buffer.c_str();
    
  }
  
}

// Stream output operator.
inline std::ostream& operator << (std::ostream& left, DAC::Exception const& right) { left << right.what(); return left; }

// End include guard.
#endif
