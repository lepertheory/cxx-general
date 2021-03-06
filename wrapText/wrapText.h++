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
  #include <to_string.h++>

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
          class Base : public Exception { public: virtual char const* what () const throw() { return "Unknown error in wrapText()."; }; };
          
          // No pointer to text to wrap was provided.
          class NullText : public Base { public: virtual char const* what () const throw() { return "No pointer to text to wrap was provided."; }; };
          
          // Cannot wrap to zero width.
          class ZeroWidth : public Base { public: virtual char const* what () const throw() { return "Cannot wrap to zero width."; }; };
          
          // Indent greater than wrap width.
          class IndentOverrun : public Base { public: virtual char const* what () const throw() { return "Indent leaves no room for text."; }; };
          
          // POI past end of string.
          class POIOverrun : public Base {
            public:
              virtual ~POIOverrun () throw() {};
              virtual char const* what () const throw() {
                try {
                  std::string tmpmsg("Point of interest position " + DAC::to_string(_poi) + " overruns end of string.");
                  return Exception::buffer_message(tmpmsg);
                } catch (...) {
                  return "Point of interest overruns text. Error creating message string.";
                }
              };
              virtual POIOverrun& POI (std::string::size_type const poi ) { _poi = poi; return *this; };
              virtual std::string::size_type POI  () const { return _poi  ; };
            private:
              std::string::size_type _poi;
          };
          class POIShyOverrun : public POIOverrun {
            public:
              virtual ~POIShyOverrun () throw() {};
              virtual char const* what () const throw() {
                try {
                  std::string tmpmsg("Soft hyphen position " + DAC::to_string(POI()) + " overruns end of string.");
                  return Exception::buffer_message(tmpmsg);
                } catch (...) {
                  return "Soft hyphen position overruns text. Error creating message string.";
                }
              };
              virtual POIShyOverrun& POI (std::string::size_type const poi ) { POIOverrun::POI(poi); return *this; };
              virtual std::string::size_type POI () const { return POIOverrun::POI(); };
          };
          class POINBOverrun : public POIOverrun {
            public:
              virtual ~POINBOverrun () throw() {};
              virtual char const* what () const throw() {
                try {
                  std::string tmpmsg("Non-break position " + DAC::to_string(POI()) + " overruns end of string.");
                  return Exception::buffer_message(tmpmsg);
                } catch (...) {
                  return "Non-break position overruns text. Error creating message string.";
                }
              };
              virtual POINBOverrun& POI (std::string::size_type const  poi) { POIOverrun::POI(poi); return *this; };
              virtual std::string::size_type POI () const { return POIOverrun::POI(); };
          };
          class POIZWSOverrun : public POIOverrun {
            public:
              virtual ~POIZWSOverrun () throw() {};
              virtual char const* what () const throw() {
                try {
                  std::string tmpmsg("Zero width space position " + DAC::to_string(POI()) + " overruns end of string.");
                  return Exception::buffer_message(tmpmsg);
                } catch (...) {
                  return "Zero width space position overruns text. Error creating message string.";
                }
              };
              virtual POIZWSOverrun& POI (std::string::size_type const poi) { POIOverrun::POI(poi); return *this; };
              virtual std::string::size_type POI () const { return POIOverrun::POI(); };
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
      std::string      & operator () (std::string& buffer, std::string const* const text = 0) const;
      std::string const& operator () (                     std::string const* const text = 0) const;
      
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
      std::string      & wrap (std::string& buffer, std::string const* const text = 0) const;
      std::string const& wrap (                     std::string const* const text = 0) const;
      
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
      
      // String buffer.
      mutable std::string _strbuf;
      
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
  inline std::string      & wrapText::operator () (std::string& buffer, std::string const* const text) const { return wrap(buffer, text); }
  inline std::string const& wrapText::operator () (                     std::string const* const text) const { return wrap(        text); }
  
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
  
  /*
   * Wrap text with our own buffer.
   */
  inline std::string const& wrapText::wrap (std::string const* const text) const { return wrap(_strbuf, text); }
  
}

#endif

