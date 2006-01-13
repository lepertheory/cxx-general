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
    
    _width = 0;
    
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
    
    // Work area.
    string   const& work      = *(text ? text : _textptr);
    string          retval                               ;
    POIContainer    wordstart                            ;
    POIContainer    wordend                              ;
    _POIList        poi                                  ;
    
    // Add points of interest.
    if (_shypos) {
      for (POIContainer::const_iterator i = _shypos->begin(); i != _shypos->end(); ++i) {
        poi[i - _shypos->begin()] = _CT_SHY;
      }
    }
    if (_nbpos) {
      for (POIContainer::const_iterator i = _nbpos->begin(); i != _nbpos->end(); ++i) {
        poi[i - _nbpos->begin()] = _CT_NB;
      }
    }
    if (_zwspos) {
      for (POIContainer::const_iterator i = _zwspos->begin(); i != _zwspos->end(); ++i) {
        poi[i - _zwspos->begin()] = _CT_ZWS;
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
      switch (i->second) {
        case _CT_ZWS    :
        case _CT_SHY    :
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
    string            nbtext     ;
    string            btext      ;
    for (POIContainer::size_type word = 0; word != wordstart.size(); ++word) {
      
      /* HAY DAVE
       * The soft hypen doesn't matter. Just test for the length of this block
       * plus 1 unless the next block is a zero-width line break.
       */
      /*
      // Determine the wrapped and non-wrapped widths of this text block.
      string::size_type wrapwidth   = 0;
      string::size_type nowrapwidth = 0;
      if (wordend[word] - wordstart[word]) {
        switch (poi.lower_bound(wordstart[word])->second) {
          case _CT_SHY
          case _CT_NB
          case _CT_ZWS
          case _CT_END
          case _CT_SPACE
          case _CT_TAB
          case _CT_NEWLINE
      */
      
      // Determine the length of this block of text.
      string::size_type textwidth = wordend[word] - wordstart[word] + 1;
      
      cout << "linepos: " << linepos << "  textwidth: " << textwidth << endl;
      
      // Try to fit this block on the current line.
      switch (poi[wordend[word] + 1]) {
        case _CT_END:
        case _CT_SHY:
        case _CT_NB:
        case _CT_ZWS:
        case _CT_SPACE:
          if (linepos + nbtext.length() + textwidth > _width) {
            retval += btext + "\n";
            if (textwidth > _width) {
              for (string::size_type pos = wordstart[word]; pos + _width <= wordend[word]; pos += _width) {
                retval += work.substr(pos, min(wordend[word] - pos, _width - 1));
              }
              linepos = textwidth % _width;
            } else {
              retval += work.substr(wordstart[word], textwidth);
              linepos = textwidth;
            }
          } else {
            retval += nbtext + work.substr(wordstart[word], textwidth);
            linepos += nbtext.length() + textwidth;
          }
          btext.clear();
          nbtext = " ";
        break;
        case _CT_TAB:
        case _CT_NEWLINE:
        break;
      };
      
    }
    
    // Done.
    return retval;
    
  }
  
}

