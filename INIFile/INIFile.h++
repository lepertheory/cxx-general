/*****************************************************************************
 * INIFile
 *****************************************************************************
 * Facilitates reading from an INI file.
 *****************************************************************************/

// Include guard.
#if !defined(INIFILE_d2u8fxu01)
  #define INIFILE_d2u8fxu01

// Standard includes.
  #include <string>
  #include <map>
  #include <iterator>
  #include <vector>

// System includes.
  #include <cxx-general/ReferencePointer.h++>
  #include <cxx-general/Exception.h++>

// Namespace wrapping.
namespace DAC {
  
  /***************************************************************************
   * INIFile
   ***************************************************************************/
  class INIFile {
    
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Data types.
      
      // Key type..
      typedef std::map<std::string, std::string> _KeyT;
      
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Data types.
      
      // Lists of sections and keys.
      typedef std::vector<std::string> SectionListT;
      typedef std::vector<std::string> KeyListT    ;
      
      // Pointers to 
      typedef ReferencePointer<SectionListT> SectionListPT;
      typedef ReferencePointer<KeyListT    > KeyListPT;
      
      class Section {
        public:
          
      };
      
      /***********************************************************************/
      // Errors.
      class Errors {
        public:
          
          // All INIFile errors are based off on this.
          class Base : public Exception {
            public:
              virtual ~Base () throw() {};
              virtual char const* what () const throw() { return "Undefined error in INIFile."; };
          };
          
          // Filename was not specified.
          class FileNotSet : public Base {
            public:
              virtual ~FileNotSet () throw() {};
              virtual char const* what () const throw() { return "Cannot perform requested operation, filename is not set."; };
          };
          
          // Error reading INI file.
          class FileError : public Base {
            public:
              virtual ~FileError () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Unspecified error reading INI file \"" + _filename + "\".").c_str();
                } catch (...) {
                  return "Unspecified error reading INI file. Error creating message string.";
                }
              };
              virtual FileError& Filename (std::string const& filename) { _filename = filename; return *this; };
              std::string Filename () const { return _filename; };
            private:
              std::string _filename;
          };
          class FileNonExist : public FileError {
            public:
              virtual ~FileNonExist () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("INI file \"" + FileError::Filename() + "\" does not exist.").c_str();
                } catch (...) {
                  return "INI file does not exist. Error creating message string.";
                }
              };
              virtual FileNonExist& Filename (std::string const& filename) { FileError::Filename(filename); return *this; };
          };
          class FileNonFile : public FileError {
            public:
              virtual ~FileNonFile () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("INI file \"" + FileError::Filename() + "\" is not a file.").c_str();
                } catch (...) {
                  return "INI file is not a file. Error creating message string.";
                }
              };
              virtual FileNonFile& Filename (std::string const& filename) { FileError::Filename(filename); return *this; };
          };
          class FileNonReadable : public FileError {
            public:
              virtual ~FileNonReadable () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("INI file \"" + FileError::Filename() + "\" is not readable.").c_str();
                } catch (...) {
                  return "INI file is not readable. Error creating message string.";
                }
              };
              virtual FileNonReadable& Filename (std::string const& filename) { FileError::Filename(filename); return *this; };
          };
          class FileUnexpectedError : public FileError {
            public:
              virtual ~FileUnexpectedError () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Unexpected error reading \"" + FileError::Filename() + "\". Error message was \"" + _errmsg + "\".").c_str();
                } catch (...) {
                  return "Unexpected error reading INI file. Error creating message string.";
                }
              };
              virtual FileUnexpectedError& Filename (std::string const& filename) { FileError::Filename(filename); return *this; };
              FileUnexpectedError& ErrMsg (std::string const& errmsg) { _errmsg = errmsg; return *this; };
              std::string ErrMsg () const { return _errmsg; };
            private:
              std::string _errmsg;
          };
          
          // Multiple definition of a section.
          class SectionMultiDefine : public Base {
            public:
              virtual ~SectionMultiDefine () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Multiple definitions of section \"" + _section + "\".").c_str();
                } catch (...) {
                  return "Multiple definitions of a section. Error creating message string.";
                }
              };
              SectionMultiDefine& Section (std::string const& section) { _section = section; return *this; };
              std::string Section () const { return _section; };
            private:
              std::string _section;
          };
          
          // Section is undefined.
          class SectionUndefined : public Base {
            public:
              virtual ~SectionUndefined () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Section \"" + _section + "\" is undefined.").c_str();
                } catch (...) {
                  return "Section is undefined. Error creating message string.";
                }
              };
              SectionUndefined& Section (std::string const& section) { _section = section; return *this; };
              std::string Section () const { return _section; }
            private:
              std::string _section;
          };
          
          // Section title is blank.
          class SectionBlank : public Base {
            public:
              virtual ~SectionBlank () throw() {};
              virtual char const* what () const throw() { return "Section title is blank."; };
          };
          
          // Value without a key.
          class ValueNoKey : public Base {
            public:
              virtual ~ValueNoKey () throw() {};
              virtual char const* what () const throw() { return "Value specified without a key."; };
          };
          
          // Key without a section.
          class KeyNoSection : public Base {
            public:
              virtual ~KeyNoSection () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Key \"" + _key + "\" was specified without a section.").c_str();
                } catch (...) {
                  return "Key was specified without a section. Error creating message string.";
                }
              };
              KeyNoSection& Key (std::string const& key) { _key = key; return *this; };
              std::string Key () const { return _key; };
            private:
              std::string _key;
          };
          
          // Multiple definitions of a single key in a section.
          class KeyMultiDefine : public Base {
            public:
              virtual ~KeyMultiDefine () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Multiple definitions of key \"" + _key + "\" within section \"" + _section + "\".").c_str();
                } catch (...) {
                  return "Multiple definitions of a key within a section. Error creating message string.";
                }
              };
              KeyMultiDefine& Section (std::string const& section) { _section = section; return *this; };
              KeyMultiDefine& Key     (std::string const& key    ) { _key     = key    ; return *this; };
              std::string Section () const { return _section; };
              std::string Key     () const { return _key    ; };
            private:
              std::string _section;
              std::string _key    ;
          };
          
          // Unable to parse line.
          class ParseError : public Base {
            public:
              virtual ~ParseError () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Could not parse line: \"" + _line + "\".").c_str();
                } catch (...) {
                  return "Could not parse line. Error creating message string.";
                }
              };
              ParseError& Line (std::string const& line) { _line = line; return *this; };
              std::string Line () const { return _line; };
            private:
              std::string _line;
          };
        
        // No instantiation of this class is allowed.
        private:
          Errors ();
          Errors (Errors const&);
          Errors& operator = (Errors const&);
        
      };
      
      /***********************************************************************/
      // Function members.
      
      // Default constructor.
      INIFile ();
      
      // Copy constructor.
      INIFile (INIFile const& inifile);
      
      // Filename constructor.
      INIFile (std::string const& filename);
      
      // Assignment operator.
      INIFile& operator = (INIFile const& inifile);
      
      // Provide access to sections.
      Section operator [] (std::string const& section) const;
      
      // Properties.
      INIFile& Filename (std::string const& filename);
      std::string Filename () const;
      
      // Reset to just-constructed state.
      void clear ();
      
      // Copy a given INIFile object.
      void copy (INIFile const& inifile);
      
      // Do a deep copy, no COW.
      void deepcopy (INIFile const& inifile);
      
      // Determine if a section is defined in this INI file.
      bool section_defined (std::string const& section) const;
      
      // Get a list of sections in this INI file.
      SectionListPT get_sections () const;
      
      // Get a list of keys in a given section.
      KeyListPT get_keys (std::string const& section) const;
      
      // Read the INI file. Not necessary to call this manually unless you
      // want the file read immedately. A good reason would be to catch any
      // errors that are thrown at a predictable time.
      void read ();
      
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Data types.
      
      // Section type.
      typedef std::map<std::string, _KeyT> _SectionT;
      
      /***********************************************************************
       * _Data
       ***********************************************************************
       * Holds all copy-on-write data for this object.
       ***********************************************************************/
      class _Data {
        
        /*
         * Public members.
         */
        public:
          
          /*******************************************************************/
          // Data members.
          
          // The name of the INI file.
          std::string filename;
          
          // Sections.
          _SectionT sections;
          
          /*******************************************************************/
          // Function members.
          
          // Default constructor.
          _Data ();
          
          // Copy constructor.
          _Data (_Data const& data);
          
          // Assignment operator.
          _Data& operator = (_Data const& data);
          
          // Reset to just-constructed state.
          void clear ();
          
          // Copy a given _Data object.
          void copy (_Data const& data);
        
      };
      
      // Reference-counted pointer to _Data.
      typedef ReferencePointer<_Data> _DataPT;
      
      /***********************************************************************/
      // Data members.
      
      _DataPT _data;
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  // Assignment operator.
  inline INIFile& INIFile::operator = (INIFile const& inifile) { copy(inifile); return *this; }
  
  // Determine if a section is defined in this INI file.
  inline bool INIFile::section_defined (std::string const& section) const { return _data->sections.count(section); }
  
  // Get a list of sections in this INI file.
  inline INIFile::SectionListPT INIFile::get_sections () const {
    SectionListPT retval(new SectionListT);
    for (_SectionT::const_iterator i = _data->sections.begin(); i != _data->sections.end(); ++i) {
      retval->push_back(i->first);
    }
    return retval;
  }
  
  // Get a list of keys in a given section.
  inline INIFile::KeyListPT INIFile::get_keys (std::string const& section) const {
    if (!section_defined(section)) {
      throw Errors::SectionUndefined().Section(section);
    }
    KeyListPT retval(new KeyListT);
    for (_KeyT::const_iterator i = _data->sections[section].begin(); i != _data->sections[section].end(); ++i) {
      retval->push_back(i->first);
    }
    return retval;
  }
  
  // Properties.
  inline std::string INIFile::Filename () const { return _data->filename; }
  
}

#endif

