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
  using namespace std;
  
  /***************************************************************************
   * wrapText
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Reset to just-constructed defaults.
   */
  void wrapText::clear () {
    
    _textptr = 0;
    
    _width    = 0;
    _tabwidth = 8;
    
    _shypos = 0;
    _nbpos  = 0;
    _zwspos = 0;
    
  }
  
  /*
   * Wrap text.
   */
  string wrapText::wrap (string const* const text) const {
    
    // Check for no text.
    if (text == 0 && _textptr == 0) {
      throw Errors::NullText();
    }
    
    // Cannot wrap to zero width.
    if (_width == 0) {
      throw Errors::ZeroWidth();
    }
    
    // Work area.
    string   const& work      = *(text ? text : _textptr);
    string          retval                               ;
    POIContainer    wordstart                            ;
    POIContainer    wordend                              ;
    _POIList        poi                                  ;
    
    cout << "text: " << work << endl;
    
    // Add points of interest.
    if (_shypos) {
      for (POIContainer::const_iterator i = _shypos->begin(); i != _shypos->end(); ++i) {
        poi[*i] = _CT_SHY;
      }
    }
    if (_nbpos) {
      for (POIContainer::const_iterator i = _nbpos->begin(); i != _nbpos->end(); ++i) {
        poi[*i] = _CT_NB;
      }
    }
    if (_zwspos) {
      for (POIContainer::const_iterator i = _zwspos->begin(); i != _zwspos->end(); ++i) {
        poi[*i] = _CT_ZWS;
      }
    }
    for (string::size_type pos = 0; (pos = work.find_first_of(" \t\n", pos + 1)) != string::npos; ) {
      switch (work[pos]) {
        case ' ' : poi[pos] = _CT_SPACE  ; break;
        case '\t': poi[pos] = _CT_TAB    ; break;
        case '\n': poi[pos] = _CT_NEWLINE; break;
      };
    }
    poi[work.length()] = _CT_END;
    
    // Find word boundaries.
    string::size_type start = 0;
    for (_POIList::iterator i = poi.begin(); i != poi.end(); ++i) {
      cout << "position: " << i->first << "  item: " << i->second << endl;
      switch (i->second) {
        case _CT_ZWS    :
        case _CT_SHY    :
          cout << "FUCKFARTY" << endl;
          wordstart.push_back(start       );
          wordend  .push_back(i->first - 1);
          start = i->first;
        break;
        case _CT_SPACE  :
        case _CT_TAB    :
        case _CT_NEWLINE:
          wordstart.push_back(start);
          wordend  .push_back(i->first - 1);
          start = i->first + 1;
        break;
        case _CT_END    :
          wordstart.push_back(start);
          wordend  .push_back(i->first - 1);
        break;
        case _CT_NB     :
        break;
      };
    }
    
    // Wrap.
    string::size_type linepos = 0;
    string::size_type nbwidth = 0;
    string            nbtext     ;
    string            btext      ;
    for (POIContainer::size_type word = 0; word != wordstart.size(); ++word) {
      
      // Determine the length of this block of text.
      string::size_type textwidth = wordend[word] - wordstart[word] + 1;
      
      cout << "linepos: " << linepos << "  textwidth: " << textwidth << "  nbwidth: " << nbwidth << "  type: " << poi[wordend[word] + 1] << "  text: " << work.substr(wordstart[word], textwidth) << endl;
      
      // Try to fit this block on the current line.
      if (linepos + nbwidth + textwidth + btext.length() > _width) {
        
        /*
         * HEY!
         * Need to check if *this* block of text is a soft hyphen, and if so
         * add 1 to text width for at least part of the calculation. May be
         * cleaner if there's a nice way to move the (n)btext processing above
         * the wrapping.
         */
        
        // Add break text. This assumes that btext is never more than 1
        // character. Probably will never matter, but if you make a btext that
        // is longer, have fun with this.
        if (!btext.empty()) {
          if (linepos + btext.length() > _width) {
            retval += "\n";
            linepos = 1;
          }
          retval += btext;
        }
        
        // Hard break any text longer than the requested width.
        if (textwidth > _width) {
          cout << "wordstart: " << wordstart[word] << "  wordend: " << wordend[word] << endl;
          
          // Fill each line as much as possible.
          for (string::size_type pos = wordstart[word]; pos <= wordend[word]; pos += _width) {
            cout << "pos: " << pos << endl;
            retval += "\n" + work.substr(pos, min(wordend[word] - pos + 1, _width));
          }
          
        // Text is less or equal to line length, output normally.
        } else {
          cout << "fart!" << endl;
          retval += "\n" + work.substr(wordstart[word], textwidth);
        }
        
        // Line position is one after the last character of the last group.
        linepos = (textwidth - 1) % _width + 1;
        
      // Add text to the line and increment line position.
      } else {
        retval  += nbtext + work.substr(wordstart[word], textwidth);
        linepos += nbwidth + textwidth;
      }
      
      // Determine break and non-break text and its width.
      switch (poi[wordend[word] + 1]) {
        
        // These will not appear in the list, only here to avoid compiler
        // warnings.
        case _CT_END:
        case _CT_NB:
        break;
        
        // Zero-width space.
        case _CT_ZWS:
          cout << "FARTY!" << endl;
          btext .clear();
          nbtext.clear();
          nbwidth = 0;
        break;
        
        // Soft hyphen.
        case _CT_SHY:
          btext = "-";
          nbtext.clear();
          nbwidth = 0;
        break;
        
        // Space.
        case _CT_SPACE:
          btext.clear();
          nbtext  = " ";
          nbwidth =   1;
        break;
        
        // Tab. Yes, that width calculation is correct.
        case _CT_TAB:
          btext.clear();
          nbtext = "\t";
          nbwidth = _tabwidth + 1 - (linepos % _tabwidth + 1);
        break;
        
        // Newline. 
        case _CT_NEWLINE:
          btext.clear();
          nbtext  = '\n';
          nbwidth =    0;
          linepos =    0;
        break;
        
      };
      
    }
    
    // Done.
    return retval;
    
  }
  
  /*
  * Determine if a block of text should wrap.
  */
  /*
  bool wrapText::_shouldWrap (string::size_type const linepos  ,
                              string::size_type const textwidth ) {
    
    
    
  }
  */

}

