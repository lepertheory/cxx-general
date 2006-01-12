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

// System includes.
  #include <cxx-general/Exception.h++>
  #include <cxx-general/toString.h++>

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************
   * wrapText
   ***************************************************************************
   * Function object that wraps a text string.
   ***************************************************************************/
  class wrapText {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Errors.
      class Errors {
        public:
          
          // All wrapText errors are based off of this.
          class Base : public Exception {
            public:
              virtual ~Base () throw() {};
              virtual char const* what () const throw() { return "Unknown error in wrapText()."; };
          };
          
          // Unmatched control character.
          class UnmatchedControlChar : public ns_wrapText::Errors::Base {
            public:
              virtual ~UnmatchedControlChar () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Unmatched control character at position" + DAC::toString(_pos) + " in text \"" + _text + "\".").c_str();
                } catch (...) {
                  return "Unmatched control character. Error creating message string.";
                }
              };
              UnmatchedControlChar& Position (std::string::size_type const  pos ) { _pos  = pos ; return *this; };
              UnmatchedControlChar& Text     (std::string            const& text) { _text = text; return *this; };
              std::string::size_type Position () const { return _pos ; };
              std::string            Text     () const { return _text; };
            private:
              std::string::size_type _pos ;
              std::string            _text;
          };
          
          // Unrecognized control character.
          class UnrecognizedControlChar : public ns_wrapText::Errors::Base {
            public:
              virtual ~UnrecognizedControlChar : () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Unrecognized control character at position" + DAC::toString(_pos) + " in text \"" _text + "\".").c_str();
                } catch (...) {
                  return "Unrecognized control character. Error creating message string.";
                }
              };
              UnrecognizedControlChar& Position (std::string::size_type const  pos ) { _pos  = pos ; return *this; };
              UnrecognizedControlChar& Text     (std::string            const& text) { _text = text; return *this; };
              std::string::size_type Position () const { return _pos ; };
              std::string            Text     () const { return _text; };
            private:
              std::string::size_type _pos ;
              std::string            _text;
          };
          
        // Instantiation of this class is not allowed.
        private:
          Errors ();
          Errors (Errors const&);
          Errors& operator = (Errors const&);
      };
        
  };
  
  /*
  // wrapText utils.
  namespace ns_wrapText {
    
    / ************************************************************************* /
    // Types.
    
    // Types of characters at given positions.
    enum _CharType {
      _CT_SPACE  ,
      _CT_TAB    ,
      _CT_CC     ,
      _CT_ZWSP   ,
      _CT_NBSP   ,
      _CT_SHY    ,
      _CT_NEWLINE,
      _CT_END
    };
    
    // Info about positions in the string.
    class _Position {
      public:
        Position (std::string::size_type const new_pos  = 0
                  CharType               const new_type = CT_END) :
          pos (new_pos ),
          type(new_type)
        {};
        std::string::size_type pos ;
        CharType               type;
    };
    
    // Points of interest.
    typedef std::vector<Position> _PosArrT;
    
    / *
    // Info about positions in the target string.
    class Position {
      public:
        Position(std::string::size_type const newprepos    = 0,
                 std::string::size_type const newpostpos   = 0,
                 std::string::size_type const newlinepos   = 0,
                 std::string::size_type const newtextlen   = 0,
                 std::string::size_type const newfulllen   = 0,
                 std::string::size_type const newgrouplen  = 0,
                 size_t                 const newgroupsize = 0,
                 CharType               const newtype      = CT_END) :
          prepos   (newprepos   ),
          postpos  (newpostpos  ),
          linepos  (newlinepos  ),
          textlen  (newtextlen  ),
          fulllen  (newfulllen  ),
          grouplen (newgrouplen ),
          groupsize(newgroupsize),
          type     (newtype     )
        {};
        std::string::size_type prepos   ;
        std::string::size_type postpos  ;
        std::string::size_type linepos  ;
        std::string::size_type textlen  ;
        std::string::size_type fulllen  ;
        std::string::size_type grouplen ;
        size_t                 groupsize;
        CharType               type     ;
    };
    
    // Noteworthy positions.
    typedef std::vector<Position> PosArrT;
    * /
    
    // Errors.
    class Errors {
    };
    
  }
  */
  
}

#endif

