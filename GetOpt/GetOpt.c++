/*****************************************************************************
 * GetOpt
 *****************************************************************************
 * This is a C++ wrapper class around the getopt and getoptlong functions.
 *****************************************************************************/

// STL includes.
#include <string.h>
#include <algorithm>
#include <map>

// System includes.
#include <unistd.h>
#include <getopt.h>
#include <cxx-general/AutoArray.h++>
#include <cxx-general/wrapText/wrapText.h++>
#include <cxx-general/toString.h++>

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
   * Get arguments.
   */
  template <> std::string GetOpt::getArg<std::string> (size_t const argnum) const {
    if (_data->modified) {
      scan();
    }
    if (argnum >= _data->arguments.size()) {
      throw Errors::ArgOOBCmdLine().ArgNum(argnum).Size(_data->arguments.size());
    }
    return _data->arguments[argnum];
  }
  template <> std::string GetOpt::getArg<std::string> (char const sopt, size_t const argnum) const {
    _ArgList const& work = _scan_option(sopt)->args;
    if (argnum >= work.size()) {
      throw Errors::ArgOOBShort().ArgNum(argnum).Size(work.size()).Option(sopt);
    }
    return work[argnum];
  }
  template <> std::string GetOpt::getArg<std::string> (std::string const& lopt, size_t const argnum) const {
    _ArgList const& work = _scan_option(lopt)->args;
    if (argnum >= work.size()) {
      throw Errors::ArgOOBLong().ArgNum(argnum).Size(work.size()).Option(lopt);
    }
    return work[argnum];
  }
  
  /*
   * Get all arguments as a list.
   */
  //template <> 
  
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
    wrapText::POIContainer shypos;
    wrapText::POIContainer nbpos ;
    wrapText::POIContainer zwspos;
    string work = _procText(_data->description, _data->programname, shypos, nbpos, zwspos);
    retval += "\n"
            + wrapText().Width (_data->helpwidth)
                        .ShyPos(&shypos         )
                        .NBPos (&nbpos          )
                        .ZWSPos(&zwspos         ).wrap(&work)
            + "\n";
    
    // Output options if there are any.
    if (!_data->options.empty()) {
      
      // Constants.
      static string::size_type const PREOPT  = 2; // "  "
      static string::size_type const SHOPT   = 2; // "-a"
      static string::size_type const SH_LSEP = 2; // ", "
      static string::size_type const LSEP    = 2; // "  "
      static string::size_type const LPRE    = 2; // "--"
      static string::size_type const ARGFIX  = 1; // "=" or " "
      static string::size_type const ARGOPT  = 2; // "[]"
      
      // Work area.
      bool has_short = false;
      bool has_long  = false;
      string::size_type overhead  = 0;
      string::size_type max_width = 0;
      string::size_type real_max  = 0;
      string::size_type loptwidth = 0;
      string::size_type optwidth  = 0;
      map<_Options::size_type, string::size_type> widths;
      
      // Blank line first.
      retval += "\n";
      
      // Gather option data.
      for (_Options::iterator i = _data->options.begin(); i != _data->options.end(); ++i) {
        if (i->isShort()) {
          has_short = true;
        }
        if (i->isLong()) {
          has_long = true;
          std::string::size_type width = i->Long().length();
          if (i->ArgRequirement()) {
            width += ARGFIX + i->ArgName().length();
            if (i->ArgRequirement() == ARG_OPTIONAL) {
              width += ARGOPT;
            }
          }
          cout << "width: " << width << endl;
          widths[i - _data->options.begin()] = width;
          max_width = max(max_width, width);
        }
      }
      overhead = PREOPT + (has_short ? SHOPT + (has_long ? SH_LSEP : 0) : 0) + (has_long ? LPRE + LSEP : 0);
      real_max = overhead + max_width;
      
      // Get the top 80% width of options for the target width. This could
      // theoretically overflow, but not before chewing up sizeof(_Option) / 4
      // gigs of RAM first. Also, I don't see any particular harm in this
      // overflowing, other than screwed up widths.
      {
        
        // Hard limits on sizes.
        string::size_type hardmin = max(overhead, _data->helpwidth     / 5);
        string::size_type hardmax = max(overhead, _data->helpwidth * 2 / 5);
        
        // See if we don't need to do any squeezing.
        if (real_max <= hardmin) {
          loptwidth = max_width;
          optwidth  = real_max ;
        } else {
          
          // Different processing if there are very few options.
          if (widths.size() > 2) {
            
            // Get the top 80% of widths.
            loptwidth = widths[widths.size() * 4 / 5];
            optwidth  = loptwidth + overhead;
            
            cout << "loptwidth: " << loptwidth << "  optwidth: " << optwidth << "  overhead: " << overhead << endl;
            
            // Clamp to hard limits.
            if (optwidth > hardmax) {
              optwidth  = hardmax;
              loptwidth = optwidth - overhead;
            } else if (optwidth < hardmin) {
              optwidth  = hardmin;
              loptwidth = optwidth - overhead;
            }
            
          // Just clamp max width to hard limits.
          } else if (!widths.empty()) {
            
            // Take max width and clamp it.
            loptwidth = max_width;
            optwidth  = max_width + overhead;
            if (optwidth > hardmax) {
              optwidth  = hardmax;
              loptwidth = optwidth - overhead;
            } else if (optwidth < hardmin) {
              optwidth  = hardmin;
              loptwidth = optwidth - overhead;
            }
            
          // No long options were defined.
          } else {
            
            // Short option width.
            loptwidth = 0;
            optwidth  = overhead;
            real_max  = overhead;
            
          }
          
        }
          
      }
      
      // Create wrapping function object.
      wrapText wrap;
      wrap.Width(_data->helpwidth - optwidth).Indent(2).Hanging(true);
      
      // Output options.
      for (_Options::iterator i = _data->options.begin(); i != _data->options.end(); ++i) {
        
        // Option indent.
        retval += "  ";
        
        // Short option.
        if (has_short) {
          if (i->isShort()) {
            retval += "-" + DAC::toStringChr(i->Short());
            if (i->isLong()) {
              retval += ", ";
            } else {
              retval += "  ";
            }
          } else {
            retval += "    ";
          }
        }
        
        // Long option.
        if (has_long) {
          if (i->isLong()) {
            retval += "--" + i->Long();
            if (i->ArgRequirement()) {
              retval += "=";
              if (i->ArgRequirement() == ARG_OPTIONAL) {
                retval += "[" + i->ArgName() + "]";
              } else {
                retval += i->ArgName();
              }
            }
            cout << "loptwidth: " << loptwidth << endl;
            if (widths[_data->options.begin() - i] > loptwidth) {
              retval += "\n" + string(optwidth, ' ');
            } else {
              if (widths[_data->options.begin() - i] < loptwidth) {
                cout << "fart!" << endl;
                retval += string(loptwidth - widths[_data->options.begin() - i], ' ');
              }
              retval += "  ";
            }
          } else {
            retval += string(LPRE + loptwidth + LSEP, ' ');
          }
        } else {
          retval += " ";
        }
        
        // Description.
        {
          work    = _procText(i->Help(), i->ArgName(), shypos, nbpos, zwspos);
          work    = wrap.ShyPos(&shypos).NBPos(&nbpos).ZWSPos(&zwspos).wrap(&work);
          cout << "optwidth: " << optwidth << endl;
          retval += _hanging_indent(work, optwidth);
        }
        
        // Newline at end of option.
        retval += "\n";
        
      }
      
    }
    
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
  
  /*
   * Process text for passing to wrapText.
   */
  string GetOpt::_procText (string const& text, string const& replace,
                            wrapText::POIContainer& shy,
                            wrapText::POIContainer& nb ,
                            wrapText::POIContainer& zws ) {
    
    // Work area.
    string retval;
    
    // Clear return values.
    shy.clear();
    nb .clear();
    zws.clear();
    
    // Process special characters in text.
    string::size_type pos    = 0;
    string::size_type oldpos = 0;
    for (;;) {
      
      // Find the next escape character.
      pos = text.find('%', oldpos);
      
      // No unmatched escape.
      if (pos + 1 > text.length()) {
        throw Errors::UnmatchedEscape().Position(pos + 1).Text(text);
      }
      
      // End processing if we are at the end of the string. Grab the rest of
      // the text.
      if (pos == string::npos) {
        if (oldpos < text.length()) {
          retval += text.substr(oldpos);
        }
        break;
      }
      
      // Push text up to this point into the return if there is text to push.
      if (pos > oldpos) {
        retval += text.substr(oldpos, pos - oldpos);
      }
      
      // Process option.
      switch (text[pos + 1]) {
        case '%': retval += "%"; break;
        case 'h': shy.push_back(retval.length()); break;
        case 'n': nb .push_back(retval.length()); break;
        case 'z': zws.push_back(retval.length()); break;
        case 's': retval += replace; break;
        default: {
          throw Errors::UnknownEscape().Position(pos + 1).Text(text);
        }
      };
      
      // Set the next position.
      oldpos = pos + 2;
      
    }
    
    // Done.
    return retval;
    
  }
  
  /*
   * Convert a string to bool.
   */
  bool GetOpt::_toBool (string const& text) {
    
    // An empty string is false.
    if (text.empty()) {
      return false;
    }
    
    // Work area.
    string work(_uppercase(text));
    
    // Check for negative text.
    if (work == "F" || work == "N" || work == "FALSE" || work == "NO" || work == "OFF") {
      return false;
    }
    
    // Check for positive text.
    if (work == "T" || work == "Y" || work == "TRUE" || work == "YES" || work == "ON") {
      return true;
    }
    
    // Check for 0 numeric. If text can't be converted to number, any old text
    // is true, just like any old number other than zero.
    try {
      return Arb(work);
    } catch (Arb::Errors::Base&) {
      return true;
    }
    
  }
  
  /*
   * Apply a hanging indent.
   */
  string GetOpt::_hanging_indent (string const& text, string::size_type const indent) {
    
    // Work area.
    string retval;
    
    string::size_type oldpos = 0;
    string::size_type pos    = 0;
    for (; (pos = text.find('\n', oldpos)) != string::npos; ) {
      if (oldpos) {
        retval += string(indent, ' ');
      }
      retval += text.substr(oldpos, pos - oldpos + 1);
      oldpos  = pos + 1;
    }
    if (oldpos) {
      retval += string(indent, ' ');
    }
    retval += text.substr(oldpos);
    
    // Done.
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

