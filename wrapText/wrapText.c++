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
    
    _indent  =     0;
    _hanging = false;
    
    _startcol = 0;
    
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
    
    // Do not allow indent wider than wrap width - 1.
    if (_indent >= _width) {
      throw Errors::IndentOverrun().Indent(_indent).Width(_width);
    }
    
    // Work area.
    string   const& work      = *(text ? text : _textptr);
    string          retval                               ;
    POIContainer    wordstart                            ;
    POIContainer    wordend                              ;
    _POIList        poi                                  ;
    
    // Add points of interest. Passed POI's are done second so that they
    // overwrite any that are automatically found.
    for (string::size_type pos = 0; (pos = work.find_first_of(" \t\n", pos + 1)) != string::npos; ) {
      switch (work[pos]) {
        case ' ' : poi[pos] = _CT_SPACE  ; break;
        case '\t': poi[pos] = _CT_TAB    ; break;
        case '\n': poi[pos] = _CT_NEWLINE; break;
      };
    }
    if (_shypos) {
      for (POIContainer::const_iterator i = _shypos->begin(); i != _shypos->end(); ++i) {
        if (*i >= work.length()) {
          throw Errors::POIShyOverrun().POI(*i).Text(work);
        }
        poi[*i] = _CT_SHY;
      }
    }
    if (_nbpos) {
      for (POIContainer::const_iterator i = _nbpos->begin(); i != _nbpos->end(); ++i) {
        if (*i >= work.length()) {
          throw Errors::POINBOverrun().POI(*i).Text(work);
        }
        poi[*i] = _CT_NB;
      }
    }
    if (_zwspos) {
      for (POIContainer::const_iterator i = _zwspos->begin(); i != _zwspos->end(); ++i) {
        if (*i >= work.length()) {
          throw Errors::POIZWSOverrun().POI(*i).Text(work);
        }
        poi[*i] = _CT_ZWS;
      }
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
    
    // Wrap. Do normal indent if called for.
    string::size_type linepos      = 0;
    string::size_type nbwidth      = 0;
    string::size_type next_nbwidth = 0;
    string nbtext     ;
    string next_nbtext;
    string btext      ;
    string next_btext ;
    if (!_hanging && _indent) {
      retval  += string(_indent, ' ');
      linepos  = _indent;
    }
    for (POIContainer::size_type word = 0; word != wordstart.size(); ++word) {
      
      // Determine the length of this block of text.
      string::size_type textwidth = wordend[word] - wordstart[word] + 1;
      
      // Set the current break and non-break text.
      nbwidth = next_nbwidth  ;
      nbtext.swap(next_nbtext);
      btext .swap(next_btext );
      
      // Calculate width of non-break text.
      if (nbtext == "\t") {
        nbwidth = _tabwidth + 1 - ((_startcol + linepos) % _tabwidth + 1);
      } else if (nbtext == "\n") {
        linepos = 0;
        nbwidth = 0;
      } else {
        nbwidth = nbtext.length();
      }
      
      // Determine break and non-break text for the next block.
      switch (poi[wordend[word] + 1]) {
        
        // These will not appear in the list, only here to avoid compiler
        // warnings.
        case _CT_NB:
        case _CT_END:
        break;
        
        // Zero-width space.
        case _CT_ZWS:
          next_btext .clear();
          next_nbtext.clear();
        break;
        
        // Soft hyphen.
        case _CT_SHY:
          next_btext = "-";
          next_nbtext.clear();
        break;
        
        // Space.
        case _CT_SPACE:
          next_btext.clear();
          next_nbtext  = " ";
        break;
        
        // Tab. Yes, that width calculation is correct.
        case _CT_TAB:
          next_btext.clear();
          next_nbtext = "\t";
        break;
        
        // Newline. 
        case _CT_NEWLINE:
          next_btext.clear();
          next_nbtext  = '\n';
        break;
        
      };
      
      // Try to fit this block on the current line.
      if (linepos + nbwidth + textwidth + next_btext.length() > _width) {
        
        // Add break text. This assumes that btext is never more than 1
        // character. Probably will never matter, and everything except for
        // this one little part should be OK, but if you make a btext that
        // is longer, have fun with this.
        if (!btext.empty()) {
          if (linepos + btext.length() > _width) {
            retval += "\n";
            if (_indent && _hanging) {
              retval += string(_indent, ' ');
              linepos = _indent;
            }
            linepos += 1;
          }
          retval += btext;
        }
        
        // Hard break any text longer than the requested width.
        if (textwidth > _width - (_hanging ? _indent : 0)) {
          
          // Fill each line as much as possible.
          if (_hanging && _indent) {
            for (string::size_type pos = wordstart[word]; pos <= wordend[word]; pos += _width - _hanging) {
              retval += "\n" + string(_indent, ' ') + work.substr(pos, min(wordend[word] - pos + 1, _width - _indent));
            }
          } else {
            for (string::size_type pos = wordstart[word]; pos <= wordend[word]; pos += _width) {
              retval += "\n" + work.substr(pos, min(wordend[word] - pos + 1, _width));
            }
          }
          
        // Text is less or equal to line length, output normally.
        } else {
          if (_hanging && _indent) {
            retval += "\n" + string(_indent, ' ') + work.substr(wordstart[word], textwidth);
          } else {
            retval += "\n" + work.substr(wordstart[word], textwidth);
          }
        }
        
        // Line position is one after the last character of the last group.
        linepos = _indent + (textwidth - 1) % (_width - (_hanging ? _indent : 0)) + 1;
        
      // Add text to the line and increment line position.
      } else {
        retval  += nbtext + work.substr(wordstart[word], textwidth);
        linepos += nbwidth + textwidth;
      }
      
    }
    
    // Done.
    return retval;
    
  }
  
}

