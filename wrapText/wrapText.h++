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
  #include <Exception.h++>
  #include <toString.h++>

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
          
          // Cannot wrap to zero width.
          class ZeroWidth : public Base {
            public:
              virtual ~ZeroWidth () throw() {};
              virtual char const* what () const throw() { return "Cannot wrap to zero width."; };
          };
          
          // Indent greater than wrap width.
          class IndentOverrun : public Base {
            public:
              virtual ~IndentOverrun () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Indent of " + DAC::toString(_indent) + " leaves no room for wrap width of " + DAC::toString(_width) + " characters.").c_str();
                } catch (...) {
                  return "Indent leaves no room for text. Error creating message string.";
                }
              };
              IndentOverrun& Indent (std::string::size_type const indent) { _indent = indent; return *this; };
              IndentOverrun& Width  (std::string::size_type const width ) { _width  = width ; return *this; };
              std::string::size_type Indent () const { return _indent; };
              std::string::size_type Width  () const { return _width ; };
            private:
              std::string::size_type _indent;
              std::string::size_type _width ;
          };
          
          // POI past end of string.
          class POIOverrun : public Base {
            public:
              virtual ~POIOverrun () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Point of interest position " + DAC::toString(_poi) + " overruns end of string \"" + _text + "\".").c_str();
                } catch (...) {
                  return "Point of interest overruns text. Error creating message string.";
                }
              };
              virtual POIOverrun& POI  (std::string::size_type const  poi ) { _poi  = poi ; return *this; };
              virtual POIOverrun& Text (std::string            const& text) { _text = text; return *this; };
              virtual std::string::size_type POI  () const { return _poi ; };
              virtual std::string            Text () const { return _text; };
            private:
              std::string::size_type _poi ;
              std::string            _text;
          };
          class POIShyOverrun : public POIOverrun {
            public:
              virtual ~POIShyOverrun () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Soft hyphen position " + DAC::toString(POI()) + " overruns end of string \"" + Text() + "\".").c_str();
                } catch (...) {
                  return "Soft hyphen position overruns text. Error creating message string.";
                }
              };
              virtual POIShyOverrun& POI  (std::string::size_type const  poi ) { return reinterpret_cast<POIShyOverrun&>(POIOverrun::POI (poi )); };
              virtual POIShyOverrun& Text (std::string            const& text) { return reinterpret_cast<POIShyOverrun&>(POIOverrun::Text(text)); };
              virtual std::string::size_type POI  () const { return POIOverrun::POI (); };
              virtual std::string            Text () const { return POIOverrun::Text(); };
          };
          class POINBOverrun : public POIOverrun {
            public:
              virtual ~POINBOverrun () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Non-break position " + DAC::toString(POI()) + " overruns end of string \"" + Text() + "\".").c_str();
                } catch (...) {
                  return "Non-break position overruns text. Error creating message string.";
                }
              };
              virtual POINBOverrun& POI  (std::string::size_type const  poi ) { return reinterpret_cast<POINBOverrun&>(POIOverrun::POI (poi )); };
              virtual POINBOverrun& Text (std::string            const& text) { return reinterpret_cast<POINBOverrun&>(POIOverrun::Text(text)); };
              virtual std::string::size_type POI  () const { return POIOverrun::POI (); };
              virtual std::string            Text () const { return POIOverrun::Text(); };
          };
          class POIZWSOverrun : public POIOverrun {
            public:
              virtual ~POIZWSOverrun () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Zero width space position " + DAC::toString(POI()) + " overruns end of string \"" + Text() + "\".").c_str();
                } catch (...) {
                  return "Zero width space position overruns text. Error creating message string.";
                }
              };
              virtual POIZWSOverrun& POI  (std::string::size_type const  poi ) { return reinterpret_cast<POIZWSOverrun&>(POIOverrun::POI (poi )); };
              virtual POIZWSOverrun& Text (std::string            const& text) { return reinterpret_cast<POIZWSOverrun&>(POIOverrun::Text(text)); };
              virtual std::string::size_type POI  () const { return POIOverrun::POI (); };
              virtual std::string            Text () const { return POIOverrun::Text(); };
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
      std::string operator () (std::string const* const text = 0) const;
      
      // Properties.
      wrapText& Width    (std::string::size_type const width   );
      wrapText& TabWidth (std::string::size_type const tabwidth);
      wrapText& ShyPos   (POIContainer const*    const shypos  );
      wrapText& NBPos    (POIContainer const*    const nbpos   );
      wrapText& ZWSPos   (POIContainer const*    const zwspos  );
      wrapText& Indent   (std::string::size_type const indent  );
      wrapText& Hanging  (bool                   const hanging );
      wrapText& StartCol (std::string::size_type const startcol);
      std::string::size_type Width    () const;
      std::string::size_type TabWidth () const;
      POIContainer const*    ShyPos   () const;
      POIContainer const*    NBPos    () const;
      POIContainer const*    ZWSPos   () const;
      std::string::size_type Indent   () const;
      std::string::size_type StartCol () const;
      bool                   Hanging  () const;
      
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
      
      // Processed point of interest list.
      typedef std::map<std::string::size_type, _CharType> _POIList;
      
      /***********************************************************************/
      // Data members.
      
      // Pointer to the text to wrap.
      std::string const* _textptr;
      
      // Width to wrap text to.
      std::string::size_type _width;
      
      // Number of spaces in a tab.
      std::string::size_type _tabwidth;
      
      // Pointers to lists of special character positions.
      POIContainer const* _shypos;
      POIContainer const* _nbpos ;
      POIContainer const* _zwspos;
      
      // Indent.
      std::string::size_type _indent ;
      bool                   _hanging;
      
      // Begin text at this column. Used to determine tab widths.
      std::string::size_type _startcol;
      
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************
   * wrapText
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Default constructor.
   */
  inline wrapText::wrapText () { clear(); }
  
  /*
   * Function call operator.
   */
  inline std::string wrapText::operator () (std::string const* const text) const { return wrap(text); }
  
  /*
   * Properties.
   */
  inline wrapText& wrapText::Width    (std::string::size_type const width   ) { _width    = width   ; return *this; }
  inline wrapText& wrapText::TabWidth (std::string::size_type const tabwidth) { _tabwidth = tabwidth; return *this; }
  inline wrapText& wrapText::ShyPos   (POIContainer const*    const shypos  ) { _shypos   = shypos  ; return *this; }
  inline wrapText& wrapText::NBPos    (POIContainer const*    const nbpos   ) { _nbpos    = nbpos   ; return *this; }
  inline wrapText& wrapText::ZWSPos   (POIContainer const*    const zwspos  ) { _zwspos   = zwspos  ; return *this; }
  inline wrapText& wrapText::Indent   (std::string::size_type const indent  ) { _indent   = indent  ; return *this; }
  inline wrapText& wrapText::Hanging  (bool                   const hanging ) { _hanging  = hanging ; return *this; }
  inline wrapText& wrapText::StartCol (std::string::size_type const startcol) { _startcol = startcol; return *this; }
  inline std::string::size_type        wrapText::Width    () const { return _width   ; }
  inline std::string::size_type        wrapText::TabWidth () const { return _tabwidth; }
  inline wrapText::POIContainer const* wrapText::ShyPos   () const { return _shypos  ; }
  inline wrapText::POIContainer const* wrapText::NBPos    () const { return _nbpos   ; }
  inline wrapText::POIContainer const* wrapText::ZWSPos   () const { return _zwspos  ; }
  inline std::string::size_type        wrapText::Indent   () const { return _indent  ; }
  inline bool                          wrapText::Hanging  () const { return _hanging ; }
  inline std::string::size_type        wrapText::StartCol () const { return _startcol; }
  
}

#endif

