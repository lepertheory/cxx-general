/*****************************************************************************
 * GetOpt
 *****************************************************************************
 * This is a C++ wrapper class around the getopt and getoptlong functions.
 *****************************************************************************/

// Standard includes.
#include <string.h>

// System includes.
#include <unistd.h>
#include <getopt.h>
#include <cxx-general/AutoArray.h++>

// Class includes.
#include "GetOpt.h++"

// Namespaces to use.
using namespace std;

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************
   * GetOpt
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  /*
   * argc & argv constructor.
   */
  GetOpt::GetOpt (int const argc, char const* const* const argv) {
    
    // Normal initialization.
    clear();
    
    // Set the arguments.
    set_cmdArgs(argc, argv);
    
  }
  
  /*
   * Valid options constructor.
   */
  GetOpt::GetOpt (Options const& options) {
    
    // Normal initialization.
    clear();
    
    // Set the valid options.
    set_options(options);
    
  }
  
  /*
   * Reset to just-constructed state.
   */
  void GetOpt::clear () {
    
    // Just create a new _data and let the old one drop away. ReferencePointer
    // will take care of the rest.
    _data = new _Data;
    
  }
  
  /*
   * Copy a given GetOpt. Can be called in place of clear().
   */
  void GetOpt::copy (GetOpt const& source, bool const deep) throw() {
    
    // If deep copy, then copy the structure, otherwise just make another
    // reference to it.
    if (deep) {
      _data = new _Data(*source._data);
    } else {
      _data = source._data;
    }
    
  }
  
  /*
   * Set the command-line arguments.
   */
  GetOpt& GetOpt::set_cmdArgs (int const argc, char const* const* const argv) {
    
    // Work area.
    GetOpt tmp(*this, true);
    
    // Clear the old argument array.
    tmp._data->modified = true;
    tmp._data->argv.clear();
    
    // Load the new argument array.
    tmp._data->argv.reserve(argc);
    for (int i = 0; i != argc; ++i) {
      tmp._data->argv.push_back(argv[i]);
    }
    
    // Swap in new data and return self.
    _data = tmp._data;
    return *this;
    
  }
  
  /*
   * Set the valid options.
   */
  GetOpt& GetOpt::set_options (Options const& options) {
    
    // Work area.
    GetOpt tmp(*this, true);
    
    // Clear the old option list.
    tmp._data->modified = true;
    tmp._data->options.clear();
    
    // Add each option passed.
    tmp._data->options.reserve(options.size());
    for (Options::const_iterator i = options.begin(); i != options.end(); ++i) {
      tmp._add_option(*i);
    }
    
    // Swap in new data and return self.
    _data = tmp._data;
    return *this;
    
  }
  
  /*
   * Add an option.
   */
  GetOpt& GetOpt::add_option (Option const& option) {
    
    // Work area.
    GetOpt tmp(*this, true);
    
    // Add the option.
    tmp._add_option(option);
    
    // Swap in new data and return.
    _data = tmp._data;
    return *this;
    
  }
  
  /*
   * Get the short help for this program.
   */
  string GetOpt::getShortHelp () const {
    
    // Not much to it.
    return "Usage: " + _data->programname + " " + _data->usage + "\n";
    
  }
  
  /*
   * Get the help screen for these options.
   */
  string GetOpt::getHelp () const {
    
    // Work area.
    string retval;
    
    // All start with this.
    retval = getShortHelp();
    
    // Blank line, then description. Wrap description to the terminal length.
    // Break at whitespace and soft hyphens.
    retval += "\n" + _wrap_text(_data->description, _data->helpwidth) + "\n";
    
    // Done.
    return retval;
    
  }
  
  /*
   * Scan the options.
   */
  void GetOpt::scan () const {
    
    // Work area.
    int                      argc = 0;
    AutoArray<char *>        argv;
    string                   sopts;
    AutoArray<struct option> lopts(new struct option[_data->numlong + 1]);
    _Options                 tmp_options(_data->options);
    _ArgList                 tmp_arguments;
    _ArgList                 tmp_ordered;
    
    // Clear option arguments and set counts.
    for (_Options::iterator i = tmp_options.begin(); i != tmp_options.end(); ++i) {
      i->numset = 0;
      i->args.clear();
    }
    
    // Get the total size of the argument list so that we can manage memory in
    // one shot.
    size_t argsize = 0;
    for (_ArgList::iterator i = _data->argv.begin(); i != _data->argv.end(); ++i) {
      argsize += i->length() + 1;
    }
    
    // Create an argument list for getopt to scan. Put all args in one array,
    // easier to manage memory that way.
    argc = _data->argv.size();
    argv = new char*[argc];
    AutoArray<char> argv_string(new char[argsize]);
    size_t curpos = 0;
    for (int i = 0; i != argc; ++i) {
      argv[i] = argv_string.get() + curpos;
      strncpy(argv[i], _data->argv[i].c_str(), _data->argv[i].length() + 1);
      curpos += _data->argv[i].length() + 1;
    }
    
    // If the user requests, enable fully POSIX compliant option parsing, i.e.
    // stop parsing at the first non-option. Otherwise, preserve the order of
    // options, including non-option arguments.
    sopts = _data->posixcheck ? "+" : "-";
    
    // Notify of missing option argument with a ':'.
    sopts += ":";
    
    // Create the option lists.
    size_t numlong = 0;
    for (_Options::iterator i = _data->options.begin(); i != _data->options.end(); ++i) {
      if (_data->checkshort && i->isShort()) {
        sopts += i->Short();
        switch (i->ArgRequirement()) {
          case ARG_NONE    :                break;
          case ARG_OPTIONAL: sopts += "::"; break;
          case ARG_REQUIRED: sopts += ":" ; break;
        }
      }
      if (_data->checklong && i->isLong()) {
        lopts[numlong].name    = i->Long().c_str();
        lopts[numlong].has_arg = i->to_getoptArgReq();
        lopts[numlong].flag    = 0;
        lopts[numlong].val     = 0;
        ++numlong;
      }
    }
    if (_data->checklong) {
      lopts[numlong].name    = 0;
      lopts[numlong].has_arg = 0;
      lopts[numlong].flag    = 0;
      lopts[numlong].val     = 0;
    }
    
    // Turn off getopt's error messages.
    opterr = 0;
    
    // Even if getopt is not called we will use this to process arguments,
    // initialize it to the first arg.
    optind = 1;
    
    // Check options.
    int longindex = 0;
    if (_data->checklong || _data->checkshort) {
      for (;;) {
        
        // Make the call.
        int opt = 0;
        if (_data->checklong && _data->checkshort) {
          opt = getopt_long(argc, argv.get(), sopts.c_str(), lopts.get(), &longindex);
        } else if (_data->checklong) {
          opt = getopt_long_only(argc, argv.get(), sopts.c_str(), lopts.get(), &longindex);
        } else {
          opt = getopt(argc, argv.get(), sopts.c_str());
        }
        
        // Exit at the end of options.
        if (opt == -1) {
          break;
        }
        
        // Add non-option argument.
        if (opt == 1) {
          tmp_arguments.push_back(optarg);
          tmp_ordered  .push_back(optarg);
          continue;
        }
        
        // Unrecognized option.
        if (static_cast<char>(opt) == '?') {
          throw Errors::InvalidOption().Option(optopt != 0 ? std::string("-") + static_cast<char>(optopt) : argv[optind - 1]);
        }
        
        // Option is missing a required argument.
        if (static_cast<char>(opt) == ':') {
          if (optopt == 0) {
            throw Errors::MissingArg().Option("--" + _find_part_option(argv[optind - 1] + 2     )->Long ());
          } else {
            throw Errors::MissingArg().Option("-"  + _find_option     (static_cast<char>(optopt))->Short());
          }
        }
        
        // Option.
        for (_Options::iterator i = tmp_options.begin(); i != tmp_options.end(); ++i) {
          if ((opt == 0 && i->Long() == lopts[longindex].name) || (opt != 0 && i->Short() == static_cast<char>(opt))) {
            ++i->numset;
            string tmp;
            if (opt == 0) {
              tmp += "--";
              tmp += lopts[longindex].name;
            } else {
              tmp += "-";
              tmp += static_cast<char>(opt);
            }
            if (optarg != 0) {
              i->args.push_back(optarg);
              if (opt == 0) {
                tmp += "=";
              }
              tmp += optarg;
            }
            tmp_ordered.push_back(tmp);
            break;
          }
        }
        
      }
    }
    
    // Any unprocessed arguments are non-option arguments.
    for (int i = optind; i != argc; ++i) {
      tmp_arguments.push_back(argv[i]);
      tmp_ordered  .push_back(argv[i]);
    }
    
    // Move our work into place.
    _data->options  .swap(tmp_options  );
    _data->arguments.swap(tmp_arguments);
    _data->ordered  .swap(tmp_ordered  );
    _data->modified = false;
    
  }
  
  /*
   * Add a valid option.
   */
  void GetOpt::_add_option (Option const& option) {
    
    // Verify that the given option does not exist.
    if (option.isShort() && isDefined(option.Short())) {
      throw Errors::DuplicateShortOption().Option(option.Short());
    }
    if (option.isLong() && isDefined(option.Long())) {
      throw Errors::DuplicateLongOption().Option(option.Long());
    }
    
    // Add the option.
    _data->modified = true;
    _data->options.push_back(option);
    if (option.isShort()) { ++_data->numshort; }
    if (option.isLong ()) { ++_data->numlong ; }
    
  }
  
  /*
   * Find the requested option.
   */
  GetOpt::_Option* GetOpt::_find_option (char const sopt) const {
    
    // Just iterate through the options and return.
    for (_Options::iterator i = _data->options.begin(); i != _data->options.end(); ++i) {
      if (i->Short() == sopt) {
        return &*i;
      }
    }
    
    // Option was not found.
    return 0;
    
  }
  GetOpt::_Option* GetOpt::_find_option (string const& lopt) const {
    
    // Iterate through the options and return.
    for (_Options::iterator i = _data->options.begin(); i != _data->options.end(); ++i) {
      if (i->isLong() && i->Long() == lopt) {
        return &*i;
      }
    }
    
    // Option was not found.
    return 0;
    
  }
  
  /*
   * Find a partial option.
   */
  GetOpt::_Option* GetOpt::_find_part_option (string const& lopt) const {
    
    // Work area.
    _Options::iterator retval    = _data->options.end();
    bool               ambiguous = false;
    
    // Iterate through options, return on perfect match, otherwise make sure
    // this is not an ambiguous match.
    for (_Options::iterator i = _data->options.begin(); i != _data->options.end(); ++i) {
      if (i->isLong()) {
        if (i->Long() == lopt) {
          return &*i;
        }
        if (i->Long().substr(0, lopt.length()) == lopt) {
          if (retval == _data->options.end()) {
            retval = i;
          } else {
            ambiguous = true;
          }
        }
      }
    }
    
    // If we found a non-ambiguous match, return it.
    if (!ambiguous && retval != _data->options.end()) {
      return &*retval;
    }
    
    // Failure.
    return 0;
    
  }
  
  /*
   * Find an option, scan if needed.
   */
  GetOpt::_Option* GetOpt::_scan_option (char const sopt) const {
    
    // If the data structure was modified, we need to re-scan. Would probably
    // be better to only scan if we are checking for a valid option, but then
    // we would have to search the option list twice, and we're going to have
    // to do this eventually anyway.
    if (_data->modified) {
      scan();
    }
    
    // Find the option.
    _Option* retval = _find_option(sopt);
    
    // Throw if the option is not defined.
    if (!retval) {
      throw Errors::UndefinedShort().Option(sopt);
    }
    
    // Return the result.
    return retval;
    
  }
  GetOpt::_Option* GetOpt::_scan_option (string const& lopt) const {
    
    // If the data structure was modified, we need to re-scan.
    if (_data->modified) {
      scan();
    }
    
    // Find the option.
    _Option* retval = _find_option(lopt);
    
    // Throw if the option is not defined.
    if (!retval) {
      throw Errors::UndefinedLong().Option(lopt);
    }
    
    // Return the result.
    return retval;
    
  }
  
  /***************************************************************************/
  // Static function members.

  /*
   * Wrap text to the given width.
   */
  string GetOpt::_wrap_text (string const& text, size_t const width) {
    
    // Static data.
    static char const* const BREAK = "% \t";
    
    // Work area.
    string retval;
    
    cout << "text: \"" << text << "\"\n"
         << "width: " << width << "\n";
    // Step through text looking for breaking characters. %h is a soft hyphen.
    string::size_type pos      = 0;
    string::size_type oldpos   = 0;
    string::size_type savepos  = 0;
    string::size_type curwidth = 0;
    string            ws;
    for (pos = text.find_first_of(BREAK); ; pos = text.find_first_of(BREAK, oldpos)) {
      
      cout << "pos: " << pos << endl;
      // Handle control character.
      if (pos != string::npos && text[pos] == '%') {
        
        // Make sure we have more characters. If not, we have an error.
        if (text.length() <= pos + 1) {
          throw Errors::UnmatchedControlChar().Position(pos + 1).Text(text);
        }
        
        // Blah.
        switch (text[pos + 1]) {
          
          // Literal '%'.
          case '%': {
            
          } break;
          
          // Unrecognized control char.
          default: {
            throw Errors::UnmatchedControlChar().Position(pos + 1).Text(text);
          }
          
        };
        
      }
      
      // Set pos to proper place.
      if (pos == string::npos) {
        pos = text.length() - 1;
      } else {
        --pos;
      }
      
      // Get the width of this block of text.
      string::size_type blockwidth = pos - oldpos + 1;
      string::size_type wslen      = ws.empty() ? 0 : (ws == " " ? 1 : 8 - (curwidth % 8));
      
      // Try to fit the current block of text on this line.
      cout << "oldpos1: " << oldpos << "  pos1: " << pos << "  blockwidth1: " << blockwidth << "  curwidth1: " << curwidth << endl;
      if (curwidth + wslen + blockwidth < width) {
        
        // Add last whitespace plus text.
        retval   += ws + text.substr(oldpos, blockwidth);
        curwidth += wslen + blockwidth;
        
      } else {
        
        // Move text down to its own line, split there if necessary.
        retval += "\n";
        while (blockwidth >= width) {
          retval     += text.substr(oldpos, width - 1) + "\n";
          oldpos     += width - 1;
          blockwidth  = pos - oldpos + 1;
        }
        retval   += text.substr(oldpos, blockwidth);
        curwidth  = blockwidth;
        
      }
      
      // End processing if at end of string.
      if (pos == text.size() - 1) {
        break;
      }
      
      // Update whitespace and position.
      ws     = text.substr(pos + 1, 1);
      oldpos = pos + 2;
      
      cout << "retval: " << retval << endl;
      
    }
    
    // Done, return.
    return retval;
    
  }
  
  /***************************************************************************
   * GetOpt::_Data
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Reset to just-constructed state.
   */
  GetOpt::_Data& GetOpt::_Data::clear () {
    
    // Non-throwing ops.
    modified = true;
    argv     .clear();
    options  .clear();
    arguments.clear();
    ordered  .clear();
    numlong    =  0;
    numshort   =  0;
    helpwidth  = 80;
    checklong  = true;
    checkshort = true;
    posixcheck = false;
    programname.clear();
    usage      .clear();
    description.clear();
    postinfo   .clear();
    bugaddress .clear();
    
    // We done.
    return *this;
    
  }
  
  /*
   * Copy a given _Data.
   */
  GetOpt::_Data& GetOpt::_Data::copy (_Data const& source) {
    
    // Create deep copies of vectors. Do this in temp in case of any errors.
    _ArgList tmp_argv     (source.argv     );
    _Options tmp_options  (source.options  );
    _ArgList tmp_arguments(source.arguments);
    _ArgList tmp_ordered  (source.ordered  );
    
    // Non-throwing ops.
    argv     .swap(tmp_argv     );
    options  .swap(tmp_options  );
    arguments.swap(tmp_arguments);
    ordered  .swap(tmp_ordered  );
    numlong     = source.numlong    ;
    numshort    = source.numshort   ;
    checklong   = source.checklong  ;
    checkshort  = source.checkshort ;
    posixcheck  = source.posixcheck ;
    modified    = source.modified   ;
    programname = source.programname;
    usage       = source.usage      ;
    description = source.description;
    postinfo    = source.postinfo   ;
    bugaddress  = source.bugaddress ;
    helpwidth   = source.helpwidth  ;
    
    // We done.
    return *this;
    
  }
  
  /*
   * Translate to getopt's argument requirement.
   */
  int GetOpt::_Option::to_getoptArgReq () const {
    switch (ArgRequirement()) {
      case ARG_NONE    : return no_argument      ;
      case ARG_OPTIONAL: return optional_argument;
      case ARG_REQUIRED: return required_argument;
    }
    return -1;
  }
  
}

