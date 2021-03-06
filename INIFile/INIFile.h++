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
  #include <ReferencePointer.h++>
  #include <Exception.h++>
  #include <ValReader.h++>

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
      
      // Key type.
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
      
      // Pointers to lists of sections and keys.
      typedef ReferencePointer<SectionListT> SectionListPT;
      typedef ReferencePointer<KeyListT    > KeyListPT;
      
      /***********************************************************************
       * Section
       ***********************************************************************
       * Provides access to INI file sections.
       ***********************************************************************/
      class Section {
        
        /*
         * Public members.
         */
        public:
          
          /*******************************************************************/
          // Function members.
          
          // Constructs with a reference to a section.
          Section (std::string const& section, _KeyT const& keylist) :
            _section(section),
            _keys   (keylist)
          {};
          
          // Provides access to key values.
          ValReader operator [] (std::string const& key) const;
          
          // Determine if a particular key is defined.
          bool key_defined (std::string const& key) const;
          
          // Get a list of keys defined in this section.
          KeyListPT get_keys () const;
          
        /*
         * Private members.
         */
        private:
          
          /*******************************************************************/
          // Data members.
          
          // The name of the section we are representing.
          std::string _section;
          
          // Reference to the keys in this section.
          _KeyT const& _keys;
          
      };
      
      /***********************************************************************/
      // Errors.
      class Errors {
        public:
          
          // All INIFile errors are based off on this.
          class Base : public Exception { public: virtual char const* what () const throw() { return "Undefined error in INIFile."; }; };
          
          // INI file has not been read.
          class NotRead : public Base { public: virtual char const* what () const throw() { return "INI file has not yet been read."; }; };
          
          // Filename was not specified.
          class FileNotSet : public Base { public: virtual char const* what () const throw() { return "Cannot perform requested operation, filename is not set."; }; };
          
          // File could not be opened.
          class FileNoOpen : public Base { public: virtual char const* what () const throw() { return "INI file could not be opened."; } };
          
          // Unexpected file error.
          class FileUnexpectedError : public Base {
            public:
              virtual ~FileUnexpectedError () throw() {};
              virtual char const* what () const throw() {
                try {
                  std::string tmpmsg("Unexpected " + _type + " error reading INI file: " + _message);
                  return Exception::buffer_message(tmpmsg);
                } catch (...) {
                  return "Unexpected error reading INI file. Error creating message string.";
                }
              };
              FileUnexpectedError& Type    (std::string const& errtype) { _type    = errtype; return *this; };
              FileUnexpectedError& Message (std::string const& message) { _message = message; return *this; };
              std::string Type    () const { return _type   ; };
              std::string Message () const { return _message; };
            private:
              std::string _type   ;
              std::string _message;
          };
          
          // Multiple definition of a section.
          class SectionMultiDefine : public Base {
            public:
              virtual ~SectionMultiDefine () throw() {};
              virtual char const* what () const throw() {
                try {
                  std::string tmpmsg("Multiple definitions of section \"" + _section + "\".");
                  return Exception::buffer_message(tmpmsg);
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
          class SectionUndefined : public Base { public: virtual char const* what () const throw() { return "Section is undefined."; }; };
          
          // Section title is blank.
          class SectionBlank : public Base { public: virtual char const* what () const throw() { return "Section title is blank."; }; };
          
          // Value without a key.
          class ValueNoKey : public Base { public: virtual char const* what () const throw() { return "Value specified without a key."; }; };
          
          // Key without a section.
          class KeyNoSection : public Base {
            public:
              virtual ~KeyNoSection () throw() {};
              virtual char const* what () const throw() {
                try {
                  std::string tmpmsg("Key \"" + _key + "\" was specified without a section.");
                  return Exception::buffer_message(tmpmsg);
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
                  std::string tmpmsg("Multiple definitions of key \"" + _key + "\" within section \"" + _section + "\".");
                  return Exception::buffer_message(tmpmsg);
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
          
          // Key is undefined.
          class KeyUndefined : public Base { public: virtual char const* what () const throw() { return "Key is undefined."; }; };
          
          // Unable to parse line.
          class ParseError : public Base {
            public:
              virtual ~ParseError () throw() {};
              virtual char const* what () const throw() {
                try {
                  std::string tmpmsg("Could not parse line " + to_string(_line) + ".");
                  return Exception::buffer_message(tmpmsg);
                } catch (...) {
                  return "Could not parse line. Error creating message string.";
                }
              };
              ParseError& Line (size_t const line) { _line = line; return *this; };
              size_t Line () const { return _line; };
            private:
              size_t _line;
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
      
      // Provide access to section keys.
      Section operator [] (std::string const& section)     ;
      Section operator [] (std::string const& section) const;
      
      // Properties.
      INIFile& Filename (std::string const& filename);
      std::string Filename () const;
      
      // Reset to just-constructed state.
      void clear ();
      
      // Copy a given INIFile object.
      void copy (INIFile const& inifile);
      
      // Determine if a section is defined in this INI file.
      bool section_defined (std::string const& section)      ;
      bool section_defined (std::string const& section) const;
      
      // Determine if a key is defined in a given section.
      bool key_defined (std::string const& section, std::string const& key)      ;
      bool key_defined (std::string const& section, std::string const& key) const;
      
      // Get a list of sections in this INI file.
      SectionListPT get_sections ()      ;
      SectionListPT get_sections () const;
      
      // Get a list of keys in a given section.
      KeyListPT get_keys (std::string const& section)      ;
      KeyListPT get_keys (std::string const& section) const;
      
      // Read the INI file. Not necessary to call this manually unless you
      // want the file read immedately. A good reason would be to catch any
      // errors that are thrown at a predictable time.
      void read ();
      
      // Reset file read status. Will make new calls re-read the file.
      void reset ();
      
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Data types.
      
      // Section type.
      typedef std::map<std::string, _KeyT> _SectionT;
      
      /***********************************************************************/
      // Data members.
      
      // The name of the INI file.
      std::string _filename;
      
      // Sections.
      _SectionT _sections;
      
      // Track whether INI file was read.
      bool _wasread;
      
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************
   * Class INIFile.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Assignment operator.
   */
  inline INIFile& INIFile::operator = (INIFile const& inifile) { copy(inifile); return *this; }
  
  /*
   * Determine if a section is defined in this INI file.
   */
  inline bool INIFile::section_defined (std::string const& section) {
    if (!_wasread) {
      read();
    }
    return const_cast<INIFile const*>(this)->section_defined(section);
  }
  inline bool INIFile::section_defined (std::string const& section) const {
    if (!_wasread) {
      throw Errors::NotRead();
    }
    return _sections.count(section);
  }
  
  /*
   * Determine if a key is defined in a given section.
   */
  inline bool INIFile::key_defined (std::string const& section, std::string const& key) {
    if (!_wasread) {
      read();
    }
    return const_cast<INIFile const*>(this)->key_defined(section, key);
  }
  inline bool INIFile::key_defined (std::string const& section, std::string const& key) const {
    if (!_wasread) {
      throw Errors::NotRead();
    }
    if (!section_defined(section)) {
      return false;
    }
    return (*this)[section].key_defined(key);
  }
  
  /*
   * Get a list of keys in a given section.
   */
  inline INIFile::KeyListPT INIFile::get_keys (std::string const& section) {
    return (*this)[section].get_keys();
  }
  inline INIFile::KeyListPT INIFile::get_keys (std::string const& section) const {
    return (*this)[section].get_keys();
  }
  
  /*
   * Properties.
   */
  inline std::string INIFile::Filename (                           ) const { return _filename;                            }
  inline INIFile&    INIFile::Filename (std::string const& filename)       { clear(); _filename = filename; return *this; }
  
  /***************************************************************************
   * Class INIFile::Section.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Verify that a key is defined.
   */
  inline bool INIFile::Section::key_defined (std::string const& key) const { return _keys.count(key); }
  
}

#endif

