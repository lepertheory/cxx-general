/*********************************************************************************************************************************
 * Getopt - A C++ wrapper for getopt_long.
 *********************************************************************************************************************************
 * Exception safety:
 * __________________________________________________________________________________________________________
 *              Options     ()                                                                     | Nothrow
 *              Options     (int const, char const* const*)                                        | Basic
 *              Options     (int const, char const* const*, passopt const*, int const, bool const) | Basic
 *              Options     (Options const&)                                                       | Strong
 *  Options&    operator=   (Options const&)                                                       | Strong
 *  char const* operator[]  (int const)                                                            | Strong
 *  void        clear       ()                                                                     | Nothrow
 *  void        setCmdLine  (int const, char const* const*)                                        | Strong
 *  void        addOpt      (char const, char const*, argreq const)                                | Strong
 *  void        addOpt      (char const, argreq const)                                             | Strong
 *  void        addOpt      (char const*, argreq const)                                            | Strong
 *  void        addOpt      (passopt const)                                                        | Strong
 *  void        addOpt      (passopt const*, int const)                                            | Strong
 *  char const* getName     ()                                                                     | Strong
 *  bool        isDef       (char const)                                                           | Nothrow
 *  bool        isDef       (char const*)                                                          | Nothrow
 *  void        checkOpts   ()                                                                     | Strong
 *  int         cmdLineArgs ()                                                                     | Strong
 *  bool        isSet       (char const)                                                           | Strong
 *  bool        isSet       (char const*)                                                          | Strong
 *  int         numSet      (char const)                                                           | Strong
 *  int         numSet      (char const*)                                                          | Strong
 *  int         numArgs     ()                                                                     | Strong
 *  int         numArgs     (char const)                                                           | Strong
 *  int         numArgs     (char const*)                                                          | Strong
 *  int         argNum      ()                                                                     | Strong
 *  int         argNum      (char const)                                                           | Strong
 *  int         argNum      (char const*)                                                          | Strong
 *  char const* getArg      ()                                                                     | Strong
 *  char const* getArg      (char const)                                                           | Strong
 *  char const* getArg      (char const*)                                                          | Strong
 *  void        resetArgs   ()                                                                     | Nothrow
 *  void        resetArgs   (char const)                                                           | Strong
 *  void        resetArgs   (char const*)                                                          | Strong
 * ------------------------------------------------------------------------------------------------+--------
 *  void        allocMem    (int const, char const* const*)                                        | Strong
 *  void        copyIn      (Options const&)                                                       | Strong
 *********************************************************************************************************************************/

#if !defined(GETOPT_k45ky3890ky23)
#  define GETOPT_k45ky3890ky23
  
#  include <string>
#  include <vector>
#  include <map>
#  include <memory>
  
#  include "AutoArray.hxx"
#  include "Exception.hxx"
  
  // It's contained.
  namespace DAC {
    
    // String building.
    std::string charToString (char const from);
    std::string intToString  (int const  from);
    
    // The class itself.
    class Options {
      
      public:
        
        // Errors
        namespace Errors {
          
          class Main : public Exception { public: Main (std::string const& text) : Exception(text) {}; };
          
          class NotChecked : public Exception { public: NotChecked (std::string const& text = "You must call member function checkOpts().") : Main(text) {}; };
          class NoCmdLine  : public Exception { public: NoCmdLine  (std::string const& text = "The command line has not been set.")         : Main(text) {}; };
          class OptSet     : public Exception { public: OptSet     (std::string const& text)                                                : Main(text) {}; };
          class OptCheck   : public Exception { public: OptCheck   (std::string const& text)                                                : Main(text) {}; };
          class LibError   : public Exception { public: LibError   (std::string const& text)                                                : Main(text) {}; };
          class OutOfRange : public Exception { public: OutOfRange (std::string const& text = "Element does not exist.")                    : Main(text) {}; };
          
          class AlreadyDefined : public OptSet { public: AlreadyDefined (std::string const& text) : OptSet(text) {}; };
          
          class Undefined  : public OptCheck { public: Undefined  (std::string const& text) : OptCheck(text) {}; };
          class NotSet     : public OptCheck { public: NotSet     (std::string const& text) : OptCheck(text) {}; };
          class MissingArg : public OptCheck { public: MissingArg (std::string const& text) : OptCheck(text) {}; };
          
        };
        
        // Argument requirements for options.
        enum argreq { ARG_NONE, ARG_OPT, ARG_REQ };
        
        // Structure to enable passing options as a single *.
        struct passopt {
          char   shopt;
          char*  lopt;
          argreq require;
        };
        
        // Default constructor.
        Options () throw();
        
        // Constructor.
        Options (int const argc, char const* const* argv);
        
        // One shot constructor.
        Options (int const argc, char const* const* argv, passopt const* opts, int const numopts, bool const checknow = true);
        
        // Copy constructor.
        Options (Options const& copy);
        
        // Assignment operator.
        Options& operator= (Options const& copy);
        
        // Array access operator.
        char const* operator[] (int const argnum) const;
        
        // Reset to just-constructed state.
        void clear () throw();
        
        // Set the command line.
        void setCmdLine (int const argc, char const* const* argv);
        
        // Add options.
        void addOpt (char const  shopt, char const* lopt, argreq const arg = ARG_NONE);
        void addOpt (char const  shopt, argreq const arg = ARG_NONE);
        void addOpt (char const* lopt,  argreq const arg = ARG_NONE);
        void addOpt (passopt const opt);
        
        // Add an array of options.
        void addOpt (passopt const* opts, int const numopts);
        
        // Get the name the program was executed with.
        char const* getName () const;
        
        // Check if an option has been defined.
        bool isDef (char const  shopt) const throw();
        bool isDef (char const* lopt)  const throw();
        
        // Check the options. You don't need to call this unless you want the
        // library calls to happen at a certain time.
        void checkOpts ();
        
        // Get the number of argumnts set on the command-line.
        int cmdLineArgs () const;
        
        // Check if an option has been set, or how many times it was set.
        bool isSet  (char const  shopt) const;
        bool isSet  (char const* lopt)  const;
        int  numSet (char const  shopt) const;
        int  numSet (char const* lopt)  const;
        
        // Get the number of arguments for either the command line or for an
        // option.
        int numArgs ()                  const;
        int numArgs (char const  shopt) const;
        int numArgs (char const* lopt)  const;
        
        // Which argument number are we on now?
        int argNum ()                  const;
        int argNum (char const  shopt) const;
        int argNum (char const* lopt)  const;
        
        // Get the next argument. Returns a null pointer at the end of the
        // argument list.
        char const* getArg ();
        char const* getArg (char const  shopt);
        char const* getArg (char const* lopt);
        
        // Start checking arguments at the beginning again.
        void resetArgs ()                  throw();
        void resetArgs (char const  shopt);
        void resetArgs (char const* lopt);
      
      private:
        
        int                                                          _argc;
        AutoArrayClass::AutoArray< AutoArrayClass::AutoArray<char> > _argv;
        bool                                                         _modified;
        int                                                          _numopts;
        int                                                          _argnum;
        bool                                                         _cmdlineset;
        
        struct _opt {
          int                      opt;
          int                      numset;
          argreq                   require;
          std::vector<std::string> arguments;
          int                      argnum;
        };
        
        std::map<int, char>        _shopts;
        std::map<int, std::string> _lopts;
        std::map<char, int>        _shoptsr;
        std::map<std::string, int> _loptsr;
        std::vector<_opt>          _opts;
        std::vector<std::string>   _args;
        std::vector<std::string>   _ordered;
        
        void allocMem (int const argc, char const* const* argv);
        
        void copyIn (Options const& copy);
        
    };
    
    inline std::string charToString (char const from) {
      char buffer[2];
      snprintf(&buffer[0], (sizeof buffer) - 1, "%c", from);
      return buffer;
    }
    
    inline std::string intToString (int const from) {
      char buffer[21];
      snprintf(&buffer[0], (sizeof buffer) - 1, "%i", from);
      return buffer;
    }
    
    inline void Options::addOpt (char const  shopt, argreq const arg) { addOpt(shopt,     "",       arg);         }
    inline void Options::addOpt (char const* lopt,  argreq const arg) { addOpt('\x00',    lopt,     arg);         }
    inline void Options::addOpt (passopt const opt)                   { addOpt(opt.shopt, opt.lopt, opt.require); }
    
    inline bool Options::isDef (char const  shopt) const throw() { return (_shoptsr.find(shopt) != _shoptsr.end()); }
    inline bool Options::isDef (char const* lopt)  const throw() { return (_loptsr.find(lopt)   != _loptsr.end());  }
    
    inline bool Options::isSet (char const* shopt) const { return (numSet(shopt) > 0); }
    inline bool Options::isSet (char const  lopt)  const { return (numSet(lopt)  > 0); }
    
    inline void Options::resetArgs () throw() { _argnum = 0; };
    
  };
  
#endif
