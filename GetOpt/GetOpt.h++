/*****************************************************************************
 * GetOpt
 *****************************************************************************
 * This is a C++ wrapper class around getopt and getoptlong.
 * 
 * The interface with getopt is just one nasty hack after another, primarily
 * in error conditions. Since I want to pick up any quirks of getopt, we're
 * pretty much stuck with the nasty hacks. I believe I've caught at least 98%
 * of the error conditions and will report them correctly.
 *****************************************************************************/

// Include guard.
#if !defined(GETOPT_u3p2dg82u3pd8u28u278b)
  #define GETOPT_u3p2dg82u3pd8u28u278b

// Standard includes.
  #include <string>
  #include <vector>

// System includes.
  #include <cxx-general/Exception.h++>
  #include <cxx-general/ReferencePointer.h++>
  #include <cxx-general/toString.h++>

// Namespace wrapping.
namespace DAC {
  
  /***************************************************************************
   * GetOpt
   ***************************************************************************
   * This is the class.
   ***************************************************************************/
  class GetOpt {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Errors.
      class Errors {
        
        public:
          
          // All GetOpt errors are based on this.
          class Base : public Exception {
            public:
              virtual ~Base () throw() {};
              virtual char const* what () const throw() { return "Undefined error in GetOpt."; };
          };
          
          // Undefined option was referenced.
          class UndefinedOpt : public Base {
            public:
              virtual ~UndefinedOpt () throw() {};
              virtual char const* what () const throw() { return "Undefined option was referenced."; };
          };
          class UndefinedShort : public UndefinedOpt {
            public:
              virtual ~UndefinedShort () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Undefined short option \"" + DAC::toStringChr(_option) + "\" was referenced.").c_str();
                } catch (...) {
                  return "Undefined short option was referenced. Error creating message string.";
                }
              };
              UndefinedShort& Option (char const option) throw() { _option = option; return *this; }
              char const Option () const throw() { return _option; }
            private:
              char _option;
          };
          class UndefinedLong : public UndefinedOpt {
            public:
              virtual ~UndefinedLong () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Undefined long option \"" + _option + "\" was referenced.").c_str();
                } catch (...) {
                  return "Undefined long option was referenced. Error creating message string.";
                }
              };
              UndefinedLong& Option (std::string const& option) throw() { _option = option; return *this; }
              std::string Option () const throw() { return _option; }
            private:
              std::string _option;
          };
          
          // Attempt to add a duplicate option.
          class DuplicateOption : public Base {
            public:
              virtual ~DuplicateOption () throw() {};
              virtual char const* what () const throw() { return "Attempted to add a duplicate option."; }
          };
          class DuplicateShortOption : public DuplicateOption {
            public:
              virtual ~DuplicateShortOption () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Attempted to add a duplicate short option, \"" + DAC::toStringChr(_option) + "\".").c_str();
                } catch (...) {
                  return "Attempted to add a duplicate short option. Error creating message string.";
                }
              };
              DuplicateShortOption& Option (char const option) throw() { _option = option; return *this; }
              char const Option () const throw() { return _option; };
            private:
              char _option;
          };
          class DuplicateLongOption : public DuplicateOption {
            public:
              virtual ~DuplicateLongOption () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Attempted to add a duplicate long option, \"" + _option + "\".").c_str();
                } catch (...) {
                  return "Attempted to add a duplicate long option. Error creating message string.";
                }
              };
              DuplicateLongOption& Option (std::string const& option) throw() { _option = option; return *this; }
              std::string Option () const throw() { return _option; }
            private:
              std::string _option;
          };
          
          // User error.
          class UserError : public Base {
            public:
              virtual ~UserError () throw() {};
              virtual char const* what () const throw() { return "Unspecified user error in option processing."; };
          };
          
          // User specified an invalid option.
          class InvalidOption : public UserError {
            public:
              virtual ~InvalidOption () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Invalid option passed: " + _option + ".").c_str();
                } catch (...) {
                  return "Invalid option passed. Error creating message string.";
                }
              };
              InvalidOption& Option (std::string const& option) throw () { _option = option; return *this; }
              std::string Option () const throw() { return _option; };
            private:
              std::string _option;
          };
          
          // Option missing a required argument.
          class MissingArg : public UserError {
            public:
              virtual ~MissingArg () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Option " + _option + " is missing a required argument.").c_str();
                } catch (...) {
                  return "Option is missing a required argument. Error creating message string.";
                }
              };
              MissingArg& Option (std::string const& option) throw() { _option = option; return *this; };
              std::string const Option () const throw() { return _option; };
            private:
              std::string _option;
          };
        
        // No instances of this class allowed.
        private:
          Errors ();
          Errors (Errors const&);
          Errors& operator = (Errors const&);
        
      };
      
      /***********************************************************************/
      // Data types.
      
      // Whether an option argument is required.
      enum ArgReq { ARG_NONE, ARG_OPTIONAL, ARG_REQUIRED };
      
      // Describes an option.
      class Option {
        public:
          Option (char const sopt, std::string const& lopt, ArgReq const argreq, bool const optreq);
          Option (char const sopt,                          ArgReq const argreq, bool const optreq);
          Option (                 std::string const& lopt, ArgReq const argreq, bool const optreq);
          Option& Short          (char        const  sopt  );
          Option& Long           (std::string const& lopt  );
          Option& ArgRequirement (ArgReq      const  argreq);
          Option& Required       (bool        const  optreq);
          char        Short          () const;
          std::string Long           () const;
          ArgReq      ArgRequirement () const;
          bool        Required       () const;
          bool        isShort        () const;
          bool        isLong         () const;
        private:
          char        _sopt;
          std::string _lopt;
          ArgReq      _argreq;
          bool        _optreq;
          bool        _isset_sopt;
          bool        _isset_lopt;
      };
      
      // List of options.
      typedef std::vector<Option> Options;
      
      /***********************************************************************/
      // Function members.
      
      // Default constructor.
      GetOpt ();
      
      // Copy constructor.
      GetOpt (GetOpt const& source, bool const deep = false);
      
      // argc & argv constructor.
      GetOpt (int const argc, char const* const* const argv);
      
      // Valid options constructor.
      GetOpt (Options const& options);
      
      // Assignment operator.
      GetOpt& operator = (GetOpt const& right) throw();
      
      // Properties.
      GetOpt& CheckLong  (bool checklong );
      GetOpt& CheckShort (bool checkshort);
      GetOpt& POSIXCheck (bool posixcheck);
      bool CheckLong  () const;
      bool CheckShort () const;
      bool POSIXCheck () const;
      
      // Reset to just-constructed state.
      void clear ();
      
      // Copy a given GetOpt.
      void copy (GetOpt const& source, bool const deep = false) throw();
      
      // See if a particular option has been defined.
      bool isDefined (char        const  sopt) const;
      bool isDefined (std::string const& lopt) const;
      
      // See if a particular option is set.
      bool isSet (char        const  sopt) const;
      bool isSet (std::string const& lopt) const;
      
      // See how many times an option has been set.
      size_t numSet (char        const  sopt) const;
      size_t numSet (std::string const& lopt) const;
      
      // See how many arguments we have. First form is for non-option args.
      size_t numArgs (                       ) const;
      size_t numArgs (char        const  sopt) const;
      size_t numArgs (std::string const& lopt) const;
      
      // Get the arguments. First form is for non-option args.
      std::vector<std::string> getArgs (                       ) const;
      std::vector<std::string> getArgs (char        const  sopt) const;
      std::vector<std::string> getArgs (std::string const& lopt) const;
      
      // Get command-line arguments in order.
      std::vector<std::string> getOrdered () const;
      
      // Set the command-line arguments.
      GetOpt& set_cmdArgs (int const argc, char const* const* const argv);
      
      // Set the valid options. This will overwrite any previous options.
      GetOpt& set_options (Options const& options);
      
      // Add a valid option.
      GetOpt& add_option (Option const& option);
      
      // Scan the options. You will probably never use this function.
      void scan () const;
    
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Data types.
      
      // List of arguments.
      typedef std::vector<std::string> _ArgList;
      
      // Internal options structure, with status.
      class _Option : public Option {
        public:
          _Option (char const sopt, std::string const& lopt, ArgReq const argreq, bool const optreq);
          _Option (char const sopt,                          ArgReq const argreq, bool const optreq);
          _Option (                 std::string const& lopt, ArgReq const argreq, bool const optreq);
          _Option (Option const& option);
          int to_getoptArgReq () const;
          size_t   numset;
          _ArgList args;
      };
      
      // List of internal options.
      typedef std::vector<_Option> _Options;
      
      // This is the data store type, wrapped in one class for easy COW.
      class _Data {
        public:
          _ArgList argv;
          _Options options;
          _ArgList arguments;
          _ArgList ordered;
          bool     modified;
          size_t   numlong;
          size_t   numshort;
          bool     checklong;
          bool     checkshort;
          bool     posixcheck;
          _Data ();
          _Data (_Data const& source);
          _Data& operator = (_Data const& right);
          _Data& clear ();
          _Data& copy (_Data const& source);
      };
      
      // Reference pointer to class data.
      typedef ReferencePointer<_Data> _DataPT;
      
      /***********************************************************************/
      // Data members.
      
      // All data is in here.
      _DataPT _data;
      
      /***********************************************************************/
      // Function members.
      
      // Add a valid option.
      void _add_option (Option const& option);
      
      // Find a given option.
      _Option* _find_option (char        const  sopt) const;
      _Option* _find_option (std::string const& lopt) const;
      
      // Find a partial match for a long option. If a perfect match is found,
      // stop looking. If an ambiguous match is found, return null pointer.
      _Option* _find_part_option (std::string const& lopt) const;
      
      // Find a given option, scan args if needed.
      _Option* _scan_option (char        const  sopt) const;
      _Option* _scan_option (std::string const& lopt) const;
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  // Default constructor.
  inline GetOpt::GetOpt () { clear(); }
  
  // Copy constructor.
  inline GetOpt::GetOpt (GetOpt const& source, bool const deep) { copy(source, deep); }
  
  // Assignment operator.
  inline GetOpt& GetOpt::operator = (GetOpt const& source) throw () { copy(source); return *this; }
  
  // Properties.
  inline GetOpt& GetOpt::CheckLong (bool checklong ) {
    if (checklong != _data->checklong ) {
      GetOpt tmp(*this, true);
      tmp._data->modified  = true;
      tmp._data->checklong = checklong;
      _data = tmp._data;
    }
    return *this;
  }
  inline GetOpt& GetOpt::CheckShort (bool checkshort) {
    if (checkshort != _data->checkshort) {
      GetOpt tmp(*this, true);
      tmp._data->modified   = true      ;
      tmp._data->checkshort = checkshort;
      _data = tmp._data;
    }
    return *this;
  }
  inline GetOpt& GetOpt::POSIXCheck (bool posixcheck) {
    if (posixcheck != _data->posixcheck) {
      GetOpt tmp(*this, true);
      tmp._data->modified   = true      ;
      tmp._data->posixcheck = posixcheck;
      _data = tmp._data;
    }
    return *this;
  }
  inline bool GetOpt::CheckLong  () const { return _data->checklong ; }
  inline bool GetOpt::CheckShort () const { return _data->checkshort; }
  inline bool GetOpt::POSIXCheck () const { return _data->posixcheck; }
  
  // See if an option has been defined.
  inline bool GetOpt::isDefined (char        const  sopt) const { return _find_option(sopt); }
  inline bool GetOpt::isDefined (std::string const& lopt) const { return _find_option(lopt); }
  
  // See if an option has been set.
  inline bool GetOpt::isSet (char        const  sopt) const { return numSet(sopt); }
  inline bool GetOpt::isSet (std::string const& lopt) const { return numSet(lopt); }
  
  // See how many times an option has been set.
  inline size_t GetOpt::numSet (char        const  sopt) const { return _scan_option(sopt)->numset; }
  inline size_t GetOpt::numSet (std::string const& lopt) const { return _scan_option(lopt)->numset; }
  
  // See how many arguments an option or the command-line has.
  inline size_t GetOpt::numArgs (                       ) const { if (_data->modified) { scan(); } return _data->arguments.size(); }
  inline size_t GetOpt::numArgs (char        const  sopt) const { return _scan_option(sopt)->args.size(); }
  inline size_t GetOpt::numArgs (std::string const& lopt) const { return _scan_option(lopt)->args.size(); }
  
  // Get arguments.
  inline std::vector<std::string> GetOpt::getArgs (                       ) const { if (_data->modified) { scan(); } return _data->arguments; }
  inline std::vector<std::string> GetOpt::getArgs (char        const  sopt) const { return _scan_option(sopt)->args; }
  inline std::vector<std::string> GetOpt::getArgs (std::string const& lopt) const { return _scan_option(lopt)->args; }
  
  // Get ordered command-line arguments.
  inline std::vector<std::string> GetOpt::getOrdered () const { if (_data->modified) { scan(); } return _data->ordered; }
  
  // _Data default constructor.
  inline GetOpt::_Data::_Data () { clear(); }
  
  // _Data copy constructor.
  inline GetOpt::_Data::_Data (_Data const& source) { copy(source); }
  
  // _Data assignment operator.
  inline GetOpt::_Data& GetOpt::_Data::operator = (_Data const& source) { return copy(source); }
  
  // Option constructor.
  inline GetOpt::Option::Option (char const sopt, std::string const& lopt, ArgReq const argreq, bool const optreq) :
    _sopt(sopt), _lopt(lopt), _argreq(argreq), _optreq(optreq), _isset_sopt(true), _isset_lopt(true)
  {}
  inline GetOpt::Option::Option (char const sopt, ArgReq const argreq, bool const optreq) :
    _sopt(sopt), _argreq(argreq), _optreq(optreq), _isset_sopt(true), _isset_lopt(false)
  {}
  inline GetOpt::Option::Option (std::string const& lopt, ArgReq const argreq, bool const optreq) :
    _lopt(lopt), _argreq(argreq), _optreq(optreq), _isset_sopt(false), _isset_lopt(true)
  {}
  
  // _Option constructor.
  inline GetOpt::_Option::_Option (char const sopt, std::string const& lopt, ArgReq const argreq, bool const optreq) :
    Option(sopt, lopt, argreq, optreq), numset(0)
  {}
  inline GetOpt::_Option::_Option (char const sopt, ArgReq const argreq, bool const optreq) :
    Option(sopt, argreq, optreq), numset(0)
  {}
  inline GetOpt::_Option::_Option (std::string const& lopt, ArgReq const argreq, bool const optreq) :
    Option(lopt, argreq, optreq), numset(0)
  {}
  inline GetOpt::_Option::_Option (Option const& option) :
    Option(option), numset(0)
  {}
  
  // Option properties.
  inline GetOpt::Option& GetOpt::Option::Short          (char        const  sopt  ) { _sopt   = sopt  ; _isset_sopt = sopt != '\0' ; return *this; }
  inline GetOpt::Option& GetOpt::Option::Long           (std::string const& lopt  ) { _lopt   = lopt  ; _isset_lopt = !lopt.empty(); return *this; }
  inline GetOpt::Option& GetOpt::Option::ArgRequirement (ArgReq      const  argreq) { _argreq = argreq;                              return *this; }
  inline GetOpt::Option& GetOpt::Option::Required       (bool        const  optreq) { _optreq = optreq;                              return *this; }
  inline char            GetOpt::Option::Short          () const { return _sopt      ; }
  inline std::string     GetOpt::Option::Long           () const { return _lopt      ; }
  inline GetOpt::ArgReq  GetOpt::Option::ArgRequirement () const { return _argreq    ; }
  inline bool            GetOpt::Option::Required       () const { return _optreq    ; }
  inline bool            GetOpt::Option::isShort        () const { return _isset_sopt; }
  inline bool            GetOpt::Option::isLong         () const { return _isset_lopt; }
}

#endif
