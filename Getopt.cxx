#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdio>

#include <getopt.h>

#include "getoptcxx.hxx"
#include "AutoArray.hxx"

using namespace std;
using namespace AutoArrayClass;

namespace getoptcxx {
  
  /**************************************************************************/
  Options::Options () throw() {
    
    clear();
    
  }
  
  /**************************************************************************/
  Options::Options (int const argc, char const* const* argv) {
    
    clear();
    
    setCmdLine(argc, argv);
    
  }
  
  /**************************************************************************/
  Options::Options (int const argc, char const* const* argv, passopt const* opts, int const numopts, bool const checknow) {
    
    clear();
    
    setCmdLine(argc, argv);
    
    addOpt(opts, numopts);
    
    if (checknow) {
      checkOpts();
    }
    
  }
  
  /**************************************************************************/
  Options::Options (Options const& copy) {
    
    copyIn(copy);
    
  }
  
  /**************************************************************************/
  Options& Options::operator= (Options const& copy) {
    
    copyIn(copy);
    
    return *this;
    
  }
  
  /**************************************************************************/
  char const* Options::operator[] (int const argnum) const {
    
    if (_modified) {
      throw Errors::NotChecked();
    }
    
    if (argnum >= cmdLineArgs()) {
      throw Errors::OutOfRange();
    }
    
    return _ordered[argnum].c_str();
    
  }
  
  /**************************************************************************/
  void Options::clear () throw() {
    
    _argc = 0;
    _argv.reset();
    
    _modified   = true;
    _numopts    = 0;
    _argnum     = 0;
    _cmdlineset = false;
    
    _shopts.clear();
    _lopts.clear();
    _shoptsr.clear();
    _loptsr.clear();
    _opts.clear();
    _args.clear();
    _ordered.clear();
    
  }
  
  /**************************************************************************/
  void Options::setCmdLine (int const argc, char const* const* argv) {
    
    allocMem(argc, argv);
    
    _modified   = true;
    _cmdlineset = true;
    
  }
  
  /**************************************************************************/
  void Options::addOpt (char const shopt, char const* lopt, argreq const arg) {
    
    if ((shopt != '\0') || (lopt != "")) {
      
      vector<_opt> tmp_opts        = _opts;
      map<int, char>   tmp_shopts  = _shopts;
      map<char, int>   tmp_shoptsr = _shoptsr;
      map<int, string> tmp_lopts   = _lopts;
      map<string, int> tmp_loptsr  = _loptsr;
      
      _opt newopt;
      newopt.opt     = _numopts;
      newopt.numset  = 0;
      newopt.require = arg;
      newopt.argnum  = 0;
      tmp_opts.push_back(newopt);
      
      if (shopt != '\x00') {
        if (isDef(shopt)) {
          throw Errors::AlreadyDefined(string("Option '") + charToString(shopt) + string("' is already defined."));
        } else {
          tmp_shopts[_numopts] = shopt;
          tmp_shoptsr[shopt]   = _numopts;
        }
      }
      if (lopt[0] != '\x00') {
        if (isDef(lopt)) {
          throw Errors::AlreadyDefined(string("Option '") + string(lopt) + string("' is already defined."));
        } else {
          tmp_lopts[_numopts] = lopt;
          tmp_loptsr[lopt]    = _numopts;
        }
      }
      
      swap(tmp_opts,    _opts);
      swap(tmp_shopts,  _shopts);
      swap(tmp_shoptsr, _shoptsr);
      swap(tmp_lopts,   _lopts);
      swap(tmp_loptsr,  _loptsr);
      
      _numopts++;
      
      _modified = true;
      
    }
    
  }
  
  /**************************************************************************/
  void Options::addOpt (passopt const* opts, int const numopts) {
    
    bool             tmp_modified = _modified;
    int              tmp_numopts  = _numopts;
    vector<_opt>     tmp_opts     = _opts;
    map<int, char>   tmp_shopts   = _shopts;
    map<char, int>   tmp_shoptsr  = _shoptsr;
    map<int, string> tmp_lopts    = _lopts;
    map<string, int> tmp_loptsr   = _loptsr;
    
    try {
      for (int i = 0; i != numopts; ++i) {
        addOpt(opts[i].shopt, opts[i].lopt, opts[i].require);
      }
    }
    
    catch (...) {
      _modified = tmp_modified;
      _numopts  = tmp_numopts;
      swap(tmp_opts,    _opts);
      swap(tmp_shopts,  _shopts);
      swap(tmp_shoptsr, _shoptsr);
      swap(tmp_lopts,   _lopts);
      swap(tmp_loptsr,  _loptsr);
      throw;
    }
    
  }
  
  /**************************************************************************/
  char const* Options::getName () const {
    
    if (!_cmdlineset) {
      throw Errors::NoCmdLine();
    }
    
    return _argv[0].get();
    
  }
  
  /**************************************************************************/
  void Options::checkOpts () {
    
    typedef vector<_opt>::iterator voi;
    
    int               optnum        = 0;
    string            short_options = ":";
    AutoArray<option> long_options;
    
    int tmp_argnum = _argnum;
    
    vector<_opt>   tmp_opts    = _opts;
    vector<string> tmp_ordered = _ordered;
    vector<string> tmp_args    = _args;
    
    int                          tmp_argc = _argc;
    AutoArray< AutoArray<char> > tmp_argv;
    
    if (!_cmdlineset) {
      throw Errors::NoCmdLine();
    }
    
    tmp_argv = new AutoArray<char>[tmp_argc];
    for (int i = 0; i != tmp_argc; ++i) {
      tmp_argv[i] = new char[strlen(_argv[i].get()) + 1];
      strncpy(tmp_argv[i].get(), _argv[i].get(), strlen(_argv[i].get()) + 1);
    }
    long_options = new option[_lopts.size() + 1];
    
    tmp_argnum = 0;
    for (voi i = tmp_opts.begin(); i != tmp_opts.end(); ++i) {
      i->numset = 0;
      i->arguments.clear();
      i->argnum = 0;
    }
    
    for (voi i = tmp_opts.begin(); i != tmp_opts.end(); ++i) {
      if (_shopts[i->opt] != '\0') {
        short_options += _shopts[i->opt];
        switch (i->require) {
          case ARG_REQ:
            short_options += ":";
            break;
          case ARG_OPT:
            short_options += "::";
            break;case ARG_NONE:
            break;
        }
      }
      if (_lopts[i->opt] != "") {
        long_options[optnum].name = _lopts[i->opt].c_str();
        switch (i->require) {
          case ARG_NONE: long_options[optnum].has_arg = no_argument;       break;
          case ARG_OPT:  long_options[optnum].has_arg = optional_argument; break;
          case ARG_REQ:  long_options[optnum].has_arg = required_argument; break;
        };
        long_options[optnum].has_arg = i->require;
        long_options[optnum].flag    = 0;
        long_options[optnum].val     = 0;
        optnum++;
      }
    }
    long_options[optnum].name    = 0;
    long_options[optnum].has_arg = 0;
    long_options[optnum].flag    = 0;
    long_options[optnum].val     = 0;
    
    optind = 0;
    
    AutoArray<char*> tmp_tmp_argv(new char*[tmp_argc]);
    for (int i = 0; i != tmp_argc; ++i) {
      tmp_tmp_argv[i] = tmp_argv[i].get();
    }
    
    for (;;) {
      
      int opt     = 0;
      int ind     = 0;
      int longopt = 0;
      
      if ((opt = getopt_long(tmp_argc, tmp_tmp_argv.get(), short_options.c_str(), long_options.get(), &longopt)) == -1) {
        break;
      }
      
      switch (opt) {
        case 0:
          {
            if (static_cast<unsigned int>(longopt) >= _lopts.size()) {
              throw Errors::LibError(string("getopt_long returned a reference to invalid long option index '") + intToString(longopt) + string("'."));
            }
            tmp_ordered.push_back(string("--") + long_options[longopt].name);
            ind = _loptsr[long_options[longopt].name];
            ++tmp_opts[ind].numset;
            if (optarg) {
              tmp_opts[ind].arguments.push_back(optarg);
            }
            break;
          }
        case ':':
          {
            string errorstr;
            errorstr = "Option '";
            if (optopt) {
              errorstr += '-' + charToString(optopt);
            } else {
              if (optind > tmp_argc) {
                throw Errors::LibError(string("getopt_long variable optiond refers to a nonexistant option index '") + intToString(optind) + string("'."));
              }
              errorstr += tmp_tmp_argv[optind - 1];
            }
            errorstr += "' missing a required argument.";
            throw Errors::MissingArg(errorstr);
            break;
          }
        case '?':
          {
            string errorstr;
            errorstr = "Option '";
            if (optopt) {
              errorstr += '-' + charToString(optopt);
            } else {
              if (optind > tmp_argc) {
                throw Errors::LibError(string("getopt_long variable optiond refers to a nonexistant option index '") + intToString(optind) + string("'."));
              }
              errorstr += tmp_tmp_argv[optind - 1];
            }
            errorstr += "' is not a valid option.";
            throw Errors::Undefined(errorstr);
            break;
          }
        default:
          {
            if (_shoptsr.find(opt) == _shoptsr.end()) {
              throw Errors::Undefined(string("getopt_long returned undefined option '") + charToString(opt) + string("'."));
            }
            tmp_ordered.push_back(string("-") + static_cast<char>(opt));
            ind = _shoptsr[opt];
            ++tmp_opts[ind].numset;
            if (optarg) {
              tmp_opts[ind].arguments.push_back(optarg);
            }
            break;
          }
      }
      
    }
    
    while (optind < tmp_argc) {
      tmp_args.push_back(tmp_tmp_argv[optind++]);
    }
      
    _argnum = tmp_argnum;
    
    swap(tmp_opts,    _opts);
    swap(tmp_ordered, _ordered);
    swap(tmp_args,    _args);
    
    _modified = false;
    
  }
  
  /**************************************************************************/
  int Options::cmdLineArgs () const {
    
    if (_modified) {
      throw Errors::NotChecked();
    }
    
    return _ordered.size();
    
  }
  
  /**************************************************************************/
  int Options::numSet (char const shopt) const {
    
    if (_modified) {
      throw Errors::NotChecked();
    }
    
    if (!isDef(shopt)) {
      throw Errors::Undefined(string("Option '") + charToString(shopt) + string("' is undefined."));
    }
    
    return _opts[_shoptsr.find(shopt)->second].numset;
    
  }
  
  /**************************************************************************/
  int Options::numSet (char const* lopt) const {
    
    if (_modified) {
      throw Errors::NotChecked();
    }
    
    if (!isDef(lopt)) {
      throw Errors::Undefined(string("Option '") + string(lopt) + string("' is undefined."));
    }
    
    return _opts[_loptsr.find(lopt)->second].numset;
    
  }
  
  /**************************************************************************/
  int Options::numArgs () const {
    
    if (_modified) {
      throw Errors::NotChecked();
    }
    
    return _args.size();
    
  }
  
  /**************************************************************************/
  int Options::numArgs (char const shopt) const {
    
    if (_modified) {
      throw Errors::NotChecked();
    }
    
    if (!isSet(shopt)) {
      throw Errors::NotSet(string("Option '") + charToString(shopt) + string("' is not set."));
    }
    
    return _opts[_shoptsr.find(shopt)->second].arguments.size();
    
  }
  
  /**************************************************************************/
  int Options::numArgs (char const* lopt) const {
    
    if (_modified) {
      throw Errors::NotChecked();
    }
    
    if (!isSet(lopt)) {
      throw Errors::NotSet(string("Option '") + string(lopt) + string("' is not set."));
    }
    
    return _opts[_loptsr.find(lopt)->second].arguments.size();
    
  }
  
  /**************************************************************************/
  int Options::argNum () const {
    
    if (_modified) {
      throw Errors::NotChecked();
    }
    
    return _argnum;
    
  }
  
  /**************************************************************************/
  int Options::argNum (char const shopt) const {
    
    if (_modified) {
      throw Errors::NotChecked();
    }
    
    if (!isSet(shopt)) {
      throw Errors::NotSet(string("Option '") + charToString(shopt) + string("' is not set."));
    }
    
    return _opts[_shoptsr.find(shopt)->second].argnum;
    
  }
  
  /**************************************************************************/
  int Options::argNum (char const* lopt) const {
    
    if (_modified) {
      throw Errors::NotChecked();
    }
    
    if (!isSet(lopt)) {
      throw Errors::NotSet(string("Option '") + string(lopt) + string("' is not set."));
    }
    
    return _opts[_loptsr.find(lopt)->second].argnum;
    
  }
  
  /**************************************************************************/
  char const* Options::getArg () {
    
    if (_modified) {
      throw Errors::NotChecked();
    }
    
    if (_argnum >= numArgs()) {
      _argnum = 0;
      return 0;
    }
    
    return _args[_argnum++].c_str();
    
  }
  
  /**************************************************************************/
  char const* Options::getArg (char const shopt) {
    
    if (_modified) {
      throw Errors::NotChecked();
    }
    
    if (!isSet(shopt)) {
      throw Errors::NotSet(string("Option '") + charToString(shopt) + string("' is not set."));
    }
    
    if (_opts[_shoptsr[shopt]].argnum >= numArgs(shopt)) {
      _opts[_shoptsr[shopt]].argnum = 0;
      return 0;
    }
    
    return _opts[_shoptsr[shopt]].arguments[_opts[_shoptsr[shopt]].argnum++].c_str();
    
  }
  
  /**************************************************************************/
  char const* Options::getArg (char const* lopt) {
    
    if (_modified) {
      throw Errors::NotChecked();
    }
    
    if (!isSet(lopt)) {
      throw Errors::NotSet(string("Option '") + string(lopt) + string("' is not set."));
    }
    
    if (_opts[_loptsr[lopt]].argnum >= numArgs(lopt)) {
      _opts[_loptsr[lopt]].argnum = 0;
      return 0;
    }
    
    return _opts[_loptsr[lopt]].arguments[_opts[_loptsr[lopt]].argnum++].c_str();
    
  }
  
  /**************************************************************************/
  void Options::resetArgs (char const shopt) {
    
    if (_modified) {
      throw Errors::NotChecked();
    }
    
    if (!isSet(shopt)) {
      throw Errors::NotSet(string("Option '") + charToString(shopt) + string("' is not set."));
    }
    
    _opts[_shoptsr[shopt]].argnum = 0;
    
  }
  
  /**************************************************************************/
  void Options::resetArgs (char const* lopt) {
    
    if (_modified) {
      throw Errors::NotChecked();
    }
    
    if (!isSet(lopt)) {
      throw Errors::NotSet(string("Option '") + string(lopt) + string("' is not set."));
    }
    
    _opts[_loptsr[lopt]].argnum = 0;
    
  }
  
  /**************************************************************************/
  void Options::allocMem (int const argc, char const* const* argv) {
    
    AutoArray< AutoArray<char> > tmp_argv;
    
    tmp_argv = new AutoArray<char>[argc];
    for (int i = 0; i != argc; ++i) {
      int arglength = strlen(argv[i]);
      tmp_argv[i] = new char[arglength + 1];
      strncpy(tmp_argv[i].get(), argv[i], arglength + 1);
    }
    
    _argv = tmp_argv;
    _argc = argc;
    
  }
  
  /**************************************************************************/
  void Options::copyIn (Options const& copy) {
    
    vector<_opt>     tmp_opts    = copy._opts;
    map<int, char>   tmp_shopts  = copy._shopts;
    map<char, int>   tmp_shoptsr = copy._shoptsr;
    map<int, string> tmp_lopts   = copy._lopts;
    map<string, int> tmp_loptsr  = copy._loptsr;
    vector<string>   tmp_args    = copy._args;
    vector<string>   tmp_ordered = copy._ordered;
    
    AutoArray<char*> tmp_argv(new char*[copy._argc]);
    for (int i = 0; i != copy._argc; ++i) {
      tmp_argv[i] = copy._argv[i].get();
    }
    
    allocMem(copy._argc, tmp_argv.get());
    
    swap(tmp_opts,    _opts);
    swap(tmp_shopts,  _shopts);
    swap(tmp_shoptsr, _shoptsr);
    swap(tmp_lopts,   _lopts);
    swap(tmp_loptsr,  _loptsr);
    swap(tmp_args,    _args);
    swap(tmp_ordered, _ordered);
    
    _modified = copy._modified;
    _numopts  = copy._numopts;
    
    _argnum = copy._argnum;
    
    _cmdlineset = copy._cmdlineset;
    
  }
  
};
