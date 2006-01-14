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
          
          // Cannot wrap to zero width.
          class ZeroWidth : public Base {
            public:
              virtual ~ZeroWidth () throw() {};
              virtual char const* what () const throw() { return "Cannot wrap to zero width."; };
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
      std::string::size_type Width    () const;
      std::string::size_type TabWidth () const;
      POIContainer const*    ShyPos   () const;
      POIContainer const*    NBPos    () const;
      POIContainer const*    ZWSPos   () const;
      
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
      
      // Width to wrap text to.
      std::string::size_type _width;
      
      // Number of spaces in a tab.
      std::string::size_type _tabwidth;
      
      // Pointers to lists of special character positions.
      POIContainer const* _shypos;
      POIContainer const* _nbpos ;
      POIContainer const* _zwspos;
      
      /***********************************************************************/
      // Static function members.
      
      // Decide if a given block of text should be wrapped.
      /*
      static bool _shouldWrap (std::string::size_type const linepos  ,
                               std::string::size_type const textwidth );
      */
      
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
  inline std::string::size_type        wrapText::Width    () const { return _width   ; }
  inline std::string::size_type        wrapText::TabWidth () const { return _tabwidth; }
  inline wrapText::POIContainer const* wrapText::ShyPos   () const { return _shypos  ; }
  inline wrapText::POIContainer const* wrapText::NBPos    () const { return _nbpos   ; }
  inline wrapText::POIContainer const* wrapText::ZWSPos   () const { return _zwspos  ; }
  
  /***************************************************************************
   * wrapText::_POI
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.

  /*
   * Default constructor.
   */
  /*
  inline wrapText::_POI::_POI () :
    _pos (std::string::npos),
    _type(_CT_END          )
  {};
  */
  
}

#endif

