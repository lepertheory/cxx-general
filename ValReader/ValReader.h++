/*****************************************************************************
 * ValReader
 *****************************************************************************
 * Facilitates reading a string value as a variety of types.
 *****************************************************************************/

// Include guard.
#if !defined(VALREADER_1j08gehbb)
  #define VALREADER_1j08gehbb

// Standard includes.
  #include <string>

// System includes.
  #include <demangle.h++>
  #include <ArbInt.h++>
  #include <Arb.h++>
  #include <Exception.h++>

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************
   * ValReader
   ***************************************************************************
   * This is the class.
   ***************************************************************************/
  class ValReader {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Errors.
      class Errors {
        public:
          
          // All errors are based off this.
          class Base : public Exception {
            public:
              virtual ~Base () throw() {};
              virtual char const* what () const throw() {
                return "Unspecified error in ValReader.";
              };
          };
          
          // Error converting number.
          class BadNum : public Base {
            public:
              virtual ~BadNum () throw() {};
              virtual char const* what () const throw() {
                try {
                  std::string tmpmsg("Could not convert value to \"" + _type + "\", lower-level error message is \"" + _errtext + "\"");
                  return Exception::buffer_message(tmpmsg);
                } catch (...) {
                  return "Could not convert value to requested type. Error creating message string.";
                }
              };
              BadNum& Type    (std::string const& totype ) throw() { _type    = totype ; return *this; };
              BadNum& ErrText (std::string const& errtext) throw() { _errtext = errtext; return *this; };
              std::string Type    () const throw() { return _type   ; };
              std::string ErrText () const throw() { return _errtext; };
            private:
              std::string _type   ;
              std::string _errtext;
          };
          
        // Instantiation of this class is not allowed.
        private:
          Errors ();
          Errors (Errors const&);
          Errors& operator = (Errors const&);
          
      };
      
      /***********************************************************************/
      // Function members.
      
      // Constructor.
      ValReader (std::string const& value = _BLANK);
      
      // Get unmodified value. Synonym for to_string()
      operator std::string () const;
      
      // Get value as a string. Returns unmodified value.
      std::string to_string () const;
      
      // Get value as a boolean.
      bool to_boolean () const;
      
      // Get value as an integer.
      template <class T> T to_integer () const;
      
      // Get value as a floating-point number.
      template <class T> T to_float () const;
    
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Constants.
      
      // Blank string, for default constructor.
      static std::string const _BLANK;
      
      /***********************************************************************/
      // Data members.
      
      // Reference to the value we are converting.
      std::string const& _value;
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************
   * ValReader
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Constructor.
   */
  inline ValReader::ValReader (std::string const& value) : _value(value) {}
  
  /*
   * Get unmodified value. Synonym for to_string().
   */
  inline ValReader::operator std::string () const { return _value; }
  
  /*
   * Get unmodified value.
   */
  inline std::string ValReader::to_string () const { return _value; }
  
  /*
   * Get value as an integer.
   */
  template <class T> T ValReader::to_integer () const {
    
    // Work area.
    T retval;
    
    // Wrap in try blocks to catch any failure of conversion and throw an
    // an error that hides implementation details. Rely on ArbInt to do the
    // conversion.
    try {
      retval = ArbInt(_value);
    } catch (ArbInt::Errors::Base& e) {
      throw Errors::BadNum().Type(demangle(retval)).ErrText(e.what());
    }
    
    // Conversion was successful, return.
    return retval;
    
  }
  
  /*
   * Get value as a floating-point number.
   */
  template <class T> T ValReader::to_float () const {
    
    // Work area.
    T retval;
    
    // Blah blah, read the function above.
    try {
      retval = Arb(_value);
    } catch (Arb::Errors::Base& e) {
      throw Errors::BadNum().Type(demangle(retval)).ErrText(e.what());
    }
    
    // Conversion was successful, return.
    return retval;
    
  }
  
}

// End include guard.
#endif

