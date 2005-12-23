/*****************************************************************************
 * GetOptCXX
 *****************************************************************************
 * This is a C++ wrapper class around getopt and getoptlong.
 * 
 * The interface with getopt is just one nasty hack after another, primarily
 * in error conditions. Since I want to pick up any quirks of getopt, we're
 * pretty much stuck with the nasty hacks. I believe I've caught at least 98%
 * of the error conditions and will report them correctly.
 *****************************************************************************/

// Include guard.
#if !defined(GETOPTCXX_u3p2dg82u3pd8u28u278b)
  #define GETOPTCXX_u3p2dg82u3pd8u28u278b

// Standard includes.
#include <string>
#include <vector>

// System includes.
#include <cxx-general/Exception.hxx>
#include <cxx-general/ReferencePointer.hxx>
#include <cxx-general/toString.hxx>

// Namespace wrapping.
namespace DAC {
  
  /***************************************************************************
   * GetOptCXX
   ***************************************************************************
   * This is the class.
   ***************************************************************************/
  class GetOptCXX {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Errors.
      
      // Forward declarations for errors.
      class Option;
      
      class Errors {
        
        public:
          
          // All GetOptCXX errors are based on this.
          class Base : public Exception {
            public:
              virtual ~Base () throw() {};
              virtual char const* what () const throw() { return "Undefined error in GetOptCXX."; };
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
                  return ("Option " + (_option->isLong() ? ("--" + _option->Long()) : ("-" + _option->Short())) + " is missing a required argument.").c_str();
                } catch (...) {
                  return "Option is missing a required argument. Error creating message string.";
                }
              };
              MissingArg& Option (GetOptCXX::Option const* const option) throw() { _option = option; return *this; };
              GetOptCXX::Option const* Option () const throw() { return _option; };
            private:
              GetOptCXX::Option const* _option;
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
      GetOptCXX ();
      
      // Copy constructor.
      GetOptCXX (GetOptCXX const& source, bool const deep = false);
      
      // argc & argv constructor.
      GetOptCXX (int const argc, char const* const* const argv);
      
      // Valid options constructor.
      GetOptCXX (Options const& options);
      
      // Assignment operator.
      GetOptCXX& operator = (GetOptCXX const& right) throw();
      
      // Properties.
      GetOptCXX& CheckLong  (bool checklong );
      GetOptCXX& CheckShort (bool checkshort);
      GetOptCXX& POSIXCheck (bool posixcheck);
      bool CheckLong  () const;
      bool CheckShort () const;
      bool POSIXCheck () const;
      
      // Reset to just-constructed state.
      void clear ();
      
      // Copy a given GetOptCXX.
      void copy (GetOptCXX const& source, bool const deep = false) throw();
      
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
      GetOptCXX& set_cmdArgs (int const argc, char const* const* const argv);
      
      // Set the valid options. This will overwrite any previous options.
      GetOptCXX& set_options (Options const& options);
      
      // Add a valid option.
      GetOptCXX& add_option (Option const& option);
      
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
  inline GetOptCXX::GetOptCXX () { clear(); }
  
  // Copy constructor.
  inline GetOptCXX::GetOptCXX (GetOptCXX const& source, bool const deep) { copy(source, deep); }
  
  // Assignment operator.
  inline GetOptCXX& GetOptCXX::operator = (GetOptCXX const& source) throw () { copy(source); return *this; }
  
  // Properties.
  inline GetOptCXX& GetOptCXX::CheckLong (bool checklong ) {
    if (checklong != _data->checklong ) {
      GetOptCXX tmp(*this, true);
      tmp._data->modified  = true;
      tmp._data->checklong = checklong;
      _data = tmp._data;
    }
    return *this;
  }
  inline GetOptCXX& GetOptCXX::CheckShort (bool checkshort) {
    if (checkshort != _data->checkshort) {
      GetOptCXX tmp(*this, true);
      tmp._data->modified   = true      ;
      tmp._data->checkshort = checkshort;
      _data = tmp._data;
    }
    return *this;
  }
  inline GetOptCXX& GetOptCXX::POSIXCheck (bool posixcheck) {
    if (posixcheck != _data->posixcheck) {
      GetOptCXX tmp(*this, true);
      tmp._data->modified   = true      ;
      tmp._data->posixcheck = posixcheck;
      _data = tmp._data;
    }
    return *this;
  }
  inline bool GetOptCXX::CheckLong  () const { return _data->checklong ; }
  inline bool GetOptCXX::CheckShort () const { return _data->checkshort; }
  inline bool GetOptCXX::POSIXCheck () const { return _data->posixcheck; }
  
  // See if an option has been defined.
  inline bool GetOptCXX::isDefined (char        const  sopt) const { return _find_option(sopt); }
  inline bool GetOptCXX::isDefined (std::string const& lopt) const { return _find_option(lopt); }
  
  // See if an option has been set.
  inline bool GetOptCXX::isSet (char        const  sopt) const { return numSet(sopt); }
  inline bool GetOptCXX::isSet (std::string const& lopt) const { return numSet(lopt); }
  
  // See how many times an option has been set.
  inline size_t GetOptCXX::numSet (char        const  sopt) const { return _scan_option(sopt)->numset; }
  inline size_t GetOptCXX::numSet (std::string const& lopt) const { return _scan_option(lopt)->numset; }
  
  // See how many arguments an option or the command-line has.
  inline size_t GetOptCXX::numArgs (                       ) const { if (_data->modified) { scan(); } return _data->arguments.size(); }
  inline size_t GetOptCXX::numArgs (char        const  sopt) const { return _scan_option(sopt)->args.size(); }
  inline size_t GetOptCXX::numArgs (std::string const& lopt) const { return _scan_option(lopt)->args.size(); }
  
  // Get arguments.
  inline std::vector<std::string> GetOptCXX::getArgs (                       ) const { if (_data->modified) { scan(); } return _data->arguments; }
  inline std::vector<std::string> GetOptCXX::getArgs (char        const  sopt) const { return _scan_option(sopt)->args; }
  inline std::vector<std::string> GetOptCXX::getArgs (std::string const& lopt) const { return _scan_option(lopt)->args; }
  
  // Get ordered command-line arguments.
  inline std::vector<std::string> GetOptCXX::getOrdered () const { if (_data->modified) { scan(); } return _data->ordered; }
  
  // _Data default constructor.
  inline GetOptCXX::_Data::_Data () { clear(); }
  
  // _Data copy constructor.
  inline GetOptCXX::_Data::_Data (_Data const& source) { copy(source); }
  
  // _Data assignment operator.
  inline GetOptCXX::_Data& GetOptCXX::_Data::operator = (_Data const& source) { return copy(source); }
  
  // Option constructor.
  inline GetOptCXX::Option::Option (char const sopt, std::string const& lopt, ArgReq const argreq, bool const optreq) :
    _sopt(sopt), _lopt(lopt), _argreq(argreq), _optreq(optreq), _isset_sopt(true), _isset_lopt(true)
  {}
  inline GetOptCXX::Option::Option (char const sopt, ArgReq const argreq, bool const optreq) :
    _sopt(sopt), _argreq(argreq), _optreq(optreq), _isset_sopt(true), _isset_lopt(false)
  {}
  inline GetOptCXX::Option::Option (std::string const& lopt, ArgReq const argreq, bool const optreq) :
    _lopt(lopt), _argreq(argreq), _optreq(optreq), _isset_sopt(false), _isset_lopt(true)
  {}
  
  // _Option constructor.
  inline GetOptCXX::_Option::_Option (char const sopt, std::string const& lopt, ArgReq const argreq, bool const optreq) :
    Option(sopt, lopt, argreq, optreq), numset(0)
  {}
  inline GetOptCXX::_Option::_Option (char const sopt, ArgReq const argreq, bool const optreq) :
    Option(sopt, argreq, optreq), numset(0)
  {}
  inline GetOptCXX::_Option::_Option (std::string const& lopt, ArgReq const argreq, bool const optreq) :
    Option(lopt, argreq, optreq), numset(0)
  {}
  inline GetOptCXX::_Option::_Option (Option const& option) :
    Option(option), numset(0)
  {}
  
  // Option properties.
  inline GetOptCXX::Option& GetOptCXX::Option::Short          (char        const  sopt  ) { _sopt   = sopt  ; _isset_sopt = sopt != '\0' ; return *this; }
  inline GetOptCXX::Option& GetOptCXX::Option::Long           (std::string const& lopt  ) { _lopt   = lopt  ; _isset_lopt = !lopt.empty(); return *this; }
  inline GetOptCXX::Option& GetOptCXX::Option::ArgRequirement (ArgReq      const  argreq) { _argreq = argreq;                              return *this; }
  inline GetOptCXX::Option& GetOptCXX::Option::Required       (bool        const  optreq) { _optreq = optreq;                              return *this; }
  inline char              GetOptCXX::Option::Short          () const { return _sopt      ; }
  inline std::string       GetOptCXX::Option::Long           () const { return _lopt      ; }
  inline GetOptCXX::ArgReq GetOptCXX::Option::ArgRequirement () const { return _argreq    ; }
  inline bool              GetOptCXX::Option::Required       () const { return _optreq    ; }
  inline bool              GetOptCXX::Option::isShort        () const { return _isset_sopt; }
  inline bool              GetOptCXX::Option::isLong         () const { return _isset_lopt; }
}

#endif

