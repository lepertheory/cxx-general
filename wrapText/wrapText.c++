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
    string::size_type nbwidth = 0;
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
      
      // Get the width of the non-breaking text.
      /*
      string::size_type nbwidth = 0;
      string::size_type tabs    = 0;
      for (string::size_type pos = 0; (pos = nbtext.find('\t', pos)) != string::npos; ++pos) { ++tabs; }
      if (tabs) {
        nbwidth = linepos % _tabwidth + _tabwidth * (tabs - 1);
      } else {
        nbwidth = nbtext.length();
      }
      */
      
      cout << "linepos: " << linepos << "  textwidth: " << textwidth << "  nbwidth: " << nbwidth << "  text: " << work.substr(wordstart[word], textwidth) << endl;
      
      // Try to fit this block on the current line.
      if (linepos + nbwidth + textwidth > _width) {
        retval += btext;
        if (textwidth > _width) {
          cout << "wordstart: " << wordstart[word] << "  wordend: " << wordend[word] << endl;
          for (string::size_type pos = wordstart[word]; pos <= wordend[word]; pos += _width) {
            cout << "pos: " << pos << endl;
            retval += "\n" + work.substr(pos, min(wordend[word] - pos + 1, _width));
          }
        } else {
          retval += "\n" + work.substr(wordstart[word], textwidth);
        }
        linepos = (textwidth - 1) % _width + 1;
      } else {
        retval  += nbtext + work.substr(wordstart[word], textwidth);
        linepos += nbwidth + textwidth;
      }
      
      switch (poi[wordend[word] + 1]) {
        case _CT_END:
        case _CT_NB:
        case _CT_ZWS:
        break;
        case _CT_SHY:
          btext = "-";
          nbtext.clear();
          nbwidth = 0;
        break;
        case _CT_SPACE:
          btext.clear();
          nbtext  = " ";
          nbwidth =   1;
        break;
        case _CT_TAB:
          btext.clear();
          nbtext = "\t";
          nbwidth = _tabwidth + 1 - linepos % _tabwidth;
        break;
        case _CT_NEWLINE:
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

