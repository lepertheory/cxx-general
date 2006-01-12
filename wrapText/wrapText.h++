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
  #include <vector>
  #include <map>

// System includes.
  #include <cxx-general/Exception.h++>

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************
   * wrapText
   ***************************************************************************
   * Function object to wrap text to a given width.
   ***************************************************************************/
  class wrapText {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Types.
      
      // Points of interest.
      typedef std::string::size_type POI;
      
      // Container of points of interest.
      typedef std::vector<POI> POIContainer;
      
      /***********************************************************************/
      // Errors.
      class Errors {
        public:
          
          // All errors are based off this.
          class Base : public Exception {
            public:
              virtual ~Base () throw() {};
              virtual char const* what () const throw() { return "Unknown error in wrapText()."; };
          };
          
          // No pointer to text to wrap was provided.
          class NullText : public Base {
            public:
              virtual ~NullText () throw() {};
              virtual char const* what () const throw() { return "No pointer to text to wrap was provided."; };
          };
          
        // No instances of this class are allowed.
        private:
          Errors ();
          Errors (Errors const&);
          Errors& operator = (Errors const&);
        
      };
      
      /***********************************************************************/
      // Function members.
      
      // Default constructor.
      wrapText ();
      
      // Reset to just constructed defaults.
      void clear ();
      
      // Function call operator.
      std::string operator () const;
      
      // Wrap text.
      std::string wrap (std::string const* const text = 0) const;
      
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Types.
      
      // Type of special character.
      enum _CharType {
        _CT_SHY    ,
        _CT_NB     ,
        _CT_ZWS    ,
        _CT_END    ,
        _CT_SPACE  ,
        _CT_TAB    ,
        _CT_NEWLINE
      };
      
      /***********************************************************************
       * _POI
       ***********************************************************************
       * Point of interest.
       ***********************************************************************/
      /*
      class _POI {
        
        / *
         * Public members.
         * /
        public:
          
          / ******************************************************************* /
          // Data members.
          
          // Position.
          std::string::size_type pos;
          
          // Character type.
          _CharType type;
          
          / ******************************************************************* /
          // Function members.
          
          // Default constructor.
          _POI ();
        
      };
      */
      
      // Processed point of interest list.
      //typedef std::map<std::string::size_type, _POI> _POIList;
      typedef std::map<std::string::size_type, _CharType> _POIList;
      
      /***********************************************************************/
      // Data members.
      
      // Pointer to the text to wrap.
      std::string const* _textptr;
      
      // Pointers to lists of special character positions.
      POIContainer const* _shypos;
      POIContainer const* _nbspos;
      POIContainer const* _zwspos;
      
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************
   * wrapText::_POI
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.

  /*
   * Default constructor.
   */
  inline wrapText::_POI () :
    _pos (std::string::npos),
    _type(_CT_END          )
  {};
  
}

#endif

