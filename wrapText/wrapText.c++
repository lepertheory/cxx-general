/*****************************************************************************
 * wrapText
 *****************************************************************************
 * Wrap text to a given width.
 *****************************************************************************/

// Module include.
#include "wrapText.h++"

// Standard includes.
#include <string>

// Namespace wrapper.
namespace DAC {
  
  // Namespaces to use.
  using namespace std        ;
  using namespace ns_wrapText;
  
  /***************************************************************************/
  // Functions.

  /*
   * Wrap text, yo.
   */
  string wrapText (string const& text, size_t const width) {
    
    // Control character and points of interest. If you change CC, make sure
    // to change the first character of POI.
    static char        const CC  = '%';
    static char const* const POI = "% \t\n";
    
    // Work area.
    _PosArrT positions;
    string   retval   ;
    
    // Find and mark all control characters.
    for (std::string::size_type pos = 0; (pos = text.find_first_of(POI, pos)) != std::string::npos; ) {
      
      // Process this POI.
      _Position newpos(pos, CT_END);
      switch (text[pos]) {
        
        // Control character.
        case CC:
          
          // Unmatched control characters are not allowed.
          if (text.length() <= pos + 1) {
            throw Errors::UnmatchedControlChar().Position(pos + 1).Text(text);
          }
          
          // Process our control character.
          switch (text[pos + 1]) {
            case CC : newpos.type = _CT_CC  ; break;
            case 'z': newpos.type = _CT_ZWSP; break;
            case 'n': newpos.type = _CT_NBSP; break;
            case 'h': newpos.type = _CT_SHY ; break;
            default: {
              throw Errors::UnrecognizedControlChar().Position(pos + 1).Text(text);
            }
          };
          positions.push_back(newpos);
          
        break;
        
        // Normal characters.
        case ' ' : newpos.type = _CC_SPACE  ; break;
        case '\t': newpos.type = _CC_TAB    ; break;
        case '\n': newpos.type = _CC_NEWLINE; break;
        
      };
      positions.push_back(newpos);
      
    }
    positions.push_back(text.length(), _CT_END);
    
    // Wrap the text.
    std::string            segment    ;
    std::string::size_type lastpos = 0;
    std::string::size_type linepos = 0;
    for (_PosArrT::iterator i = positions.begin(); i != positions.end(); ++i) {
      
      // Add the text only to this segment.
      segment += text.substr(lastpos, i->pos - lastpos);
      
      // Wrap character.
      switch (i->type) {
        case _CT_SPACE: {
          if (linepos + segment.length() > width) {
            retval += wrap_text;
          } else {
            retval      += nowrap_text + segment;
            linepos     += segment.length() + nowrap_text.length();
            wrap_text    = "" ;
            nowrap_text  = " ";
          }
        }
      };
      
    }
    
    // Done.
    return retval;
    
    /*
    // Work area.
    PosArrT positions;
    string  retval   ;
    
    // Find all control characters and whitespace.
    std::string::size_type pos    = 0;
    std::string::size_type oldpos = 0;
    do {
      
      // Find the next point of interest.
      pos = text.find_first_of("% \t\n");
      
      // Get just the text length.
      std::string::size_type textlen = (pos == std::string::npos ? text.length() : pos) - oldpos;
      
      // Get this character's post-processing position.
      std::string::size_type postpos = (positions.empty() ? 0 : positions.back().postpos + positions.back().size) + textlen;
      
      // Get the previous character's line position.
      std::string::size_type lastlinepos = positions.empty() ? 0 : positions.back().linepos;
      
      // Get the previous full text length.
      std::string::size_type lasttextlen = positions.empty() ? 0 : positions.back().grouplen;
      
      // Get the potential starting character.
      std::string::size_type startchr = lastlinepos + lasttextlen;
      
      // Get whether or not to join the previous group.
      bool joingroup = !positions.empty() && positions.back().type == CT_CT;
      
      // Interrupt processing at end.
      if (pos == string::npos) {
        positions.push_back(Position(pos, postpos, , CT_END));
        break;
      }
      
      // Process the character.
      switch (text[pos]) {
        
        // Control character.
        case '%': {

          // Make sure we have a matching control character.
          if (pos + 1 >= text.size()) {
            throw Errors::UnmatchedControlChar().Position(pos + 1).Text(text);
          }
          
          // Only recognized control character so far.
          if (text[pos + 1] != 'h') {
            throw Errors::UnrecognizedControlChar().Position(pos + 1).Text(text);
          }
          
          // Set position info. Wrap if necessary.
          if (startchr + textlen + 1 > width) {
            positions.push_back(
              Position(
                pos,
                postpos,
                startchr + textlen,
                textlen,
                textlen + 1,
                (joingroup ? lasttextlen + textlen : 0) + textlen + 1,
                joingroup ? positions.back().groupsize + 1 : 0,
                CT_CT
              )
            );
          } else {
            if (joingroup) {
              PosArrT::iterator i = positions.end() - (positions.back().groupsize + 1);
              if (i->linepos != 0) {
                positions.insert(i, Position(
                  i->prepos,
                  i->postpos,
                  i->linepos,
                  0, 0, 0, 0,
                  CT_CR
                ));
                i = positions.end() - (positions.back().groupsize + 1);
                i->linepos = 0;
                for (PosArrT::iterator i = positions.end() - positions.back().groupsize; i != positions.end() ++i) {
                  i->linepos = (i - 1)->linepos + (i - 1)->fulllen;
                }
              }
              if (positions.back().linepos + positions.back().fulllen + textlen > width) {
                positions.push_back(
                  Position(
                    pos,
                    
            }
          }
          
        } break;
        
        // Space.
        case ' ': {
          positions.push_back(Position(pos, positions.back().postpos + positions.back().size + (pos - oldpos), 1, CT_SHY));
        } break;
        
        // Tab.
        case ' ': {
        } break;
        
        // Newline. {
        case '\n': {
        } break;
        
      };
      
    // Search until string not found.
    };
    
    // Done.
    return retval;
    */
    
  }
  
}

