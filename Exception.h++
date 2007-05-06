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
      Exception ();
      
      // Descructor.
      ~Exception () throw();
      
      // Get the cause of this error.
      virtual char const* what () const throw();
      
      // Get the type of this error.
      std::string      & type (std::string& buffer) const;
      std::string const& type (                   ) const;
      
      // Reset to just-constructed state.
      virtual Exception& clear ();
      
      // Buffer a temporary string. This is a general enough task that it
      // shouldn't even necessarily be in this class, however it is so
      // frequently needed in conjunction with exceptions (and so rarely
      // needed in other places) that it just makes sense.
      char const* buffer_message (std::string& message) const;
      
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
  
  inline Exception::Exception () : std::exception() {}
  
  inline Exception::~Exception () throw() { /* Da nada. */ }
  
  inline char const* Exception::what () const throw() { return "Undefined error."; }
  
  inline std::string      & Exception::type(std::string& buffer) const { return demangle(buffer, *this); }
  inline std::string const& Exception::type(                   ) const { return type(_strbuf);           }
  
  inline Exception& Exception::clear() { return *this; }
  
  /*
   * Buffer a temporary string.
   */
  inline char const* Exception::buffer_message (std::string& message) const {
    
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
