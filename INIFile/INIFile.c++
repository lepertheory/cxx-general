/*****************************************************************************
 * INIFile
 *****************************************************************************
 * Blah blah blah.
 *****************************************************************************/

// Standard includes.
#include <string>
#include <vector>
#include <fstream>

// Internal includes.
#include <ValReader.h++>
#include <tokenize.h++>
#include <AutoArray.h++>
#include <trim.h++>
#include <demangle.h++>

// Class include.
#include "INIFile.h++"

// Import namespaces.
using namespace std;

// Namespace wrapping.
namespace DAC {
  
  /***************************************************************************
   * Class INIFile.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Default constructor.
   */
  INIFile::INIFile () {
    
    // Initialize.
    clear();
    
  }
  
  /*
   * Copy constructor.
   */
  INIFile::INIFile (INIFile const& inifile) {
    
    // Copy the given data. No need to init, copy does everything.
    copy(inifile);
    
  }
  
  /*
   * Filename constructor.
   */
  INIFile::INIFile (string const& filename) {
    
    // Initialize.
    clear();
    
    // Set the filename.
    _filename = filename;
    
  }
  
  /*
   * Provide access to sections.
   */
  INIFile::Section INIFile::operator [] (string const& section) {
    
    // Read INI file if necessary.
    if (!_wasread) {
      read();
    }
    
    // Call const version.
    return (*const_cast<INIFile const*>(this))[section];
    
  } 
  INIFile::Section INIFile::operator [] (string const& section) const {
    
    // Ensure that the INI file has been read.
    if (!_wasread) {
      throw Errors::NotRead();
    }
    
    // Ensure that this section is defined.
    if (!section_defined(section)) {
      throw Errors::SectionUndefined();
    }
    
    // Return a Section object pointing to this section.
    return Section(section, _sections.find(section)->second);
    
  }
  
  /*
   * Reset to just-constructed state.
   */
  void INIFile::clear () {
    
    // Non-throw.
    _wasread = false;
    
    _filename.clear();
    _sections.clear();
    
  }
  
  /*
   * Copy a given INIFile.
   */
  void INIFile::copy (INIFile const& inifile) {
    
    // Non-throw.
    _wasread = inifile._wasread;
    
    _filename = inifile._filename;
    _sections = inifile._sections;
    
  }
  
  /*
   * Get a list of sections in this INI file.
   */
  INIFile::SectionListPT INIFile::get_sections () {
    
    // Read data if needed.
    if (!_wasread) {
      read();
    }
    
    // Call const function.
    return const_cast<INIFile const*>(this)->get_sections();

  }
  INIFile::SectionListPT INIFile::get_sections () const {
    
    // Make sure data has been read.
    if (!_wasread) {
      throw Errors::NotRead();
    }
    
    // Work area.
    SectionListPT retval(new SectionListT);
    
    // Push section names into the section list.
    for (_SectionT::const_iterator i = _sections.begin(); i != _sections.end(); ++i) {
      retval->push_back(i->first);
    }
    
    // Done.
    return retval;
    
  }
  
  /*
   * Read the INI file.
   */
  void INIFile::read () {
    
    // Make sure that the filename is set.
    if (_filename.empty()) {
      throw Errors::FileNotSet();
    }
    
    // Work area.
    AutoArray<char> buffer     ;
    vector<string>  lines      ;
    _SectionT       newsections;
    
    // Do not allow file errors to propagate.
    try {
      
      // Open the INI file.
      ifstream file(_filename.c_str());
      if (!file) {
        throw Errors::FileNoOpen();
      }
      
      // Get the size of the file.
      streampos filesize = 0;
      file.seekg(0, ios::end);
      filesize = file.tellg();
      file.seekg(0, ios::beg);
      
      // Read the entire file into a buffer.
      buffer = new char[filesize + static_cast<streampos>(1)];
      file.read(buffer.get(), filesize);
      file.close();
      
    // Error during file operation.
    } catch (exception& e) {
      string errtype;
      throw Errors::FileUnexpectedError().Type(demangle(errtype, e)).Message(e.what());
    } catch (...) {
      throw Errors::FileUnexpectedError().Type("-Unknown-").Message("-Unknown-");
    }
    
    // Split each line.
    tokenize(buffer.get(), lines, "\n");
    
    // Process each line.
    string section;
    for (vector<string>::iterator line = lines.begin(); line != lines.end(); ++line) {
      
      // Trim whitespace.
      trim(*line);
      
      // Skip blank lines and comments.
      if (line->empty() || (*line)[0] == ';') {
        continue;
      }
      
      // Work area.
      string::size_type valuesep = string::npos;
      
      // Check if this is the beginning of a section.
      if ((*line)[0] == '[' && (*line)[line->size() - 1] == ']') {
        
        // Make sure the section is not blank.
        if (line->size() == 2) {
          throw Errors::SectionBlank();
        }
        
        // Get the section name.
        section = line->substr(1, line->size() - 2);
        
        // Make sure this section does not already exist.
        if (newsections.count(section)) {
          throw Errors::SectionMultiDefine().Section(section);
        }
        
        // Create the new section.
        newsections[section];
        
      // Check for a key=value.
      } else if ((valuesep = line->find('=')) != string::npos) {
        
        // Make sure that we have a key.
        if (valuesep == 0) {
          throw Errors::ValueNoKey();
        }
        
        // Work area.
        string key  ;
        string value;
        
        // Get the key, trim whitespace from end.
        key = line->substr(0, min(line->find_last_not_of(" \t", valuesep - 1) + 1, valuesep));
        
        // Make sure that we have an active section.
        if (section.empty()) {
          throw Errors::KeyNoSection().Key(key);
        }
        
        // Make sure this key does not already exist in this section.
        if (newsections[section].count(key)) {
          throw Errors::KeyMultiDefine().Section(section).Key(key);
        }
        
        // Get the value, trim whitespace from beginning.
        if (valuesep != line->size() - 1) {
          value = line->substr(line->find_first_not_of(" \t", valuesep + 1));
        }
        
        // Add the key.
        newsections[section].insert(_KeyT::value_type(key, value));
        
      // Parse error.
      } else {
        throw Errors::ParseError().Line(line - lines.begin() + 1);
      }
      
    }
    
    // Swap in new sections.
    _sections.swap(newsections);
    
    // File was successfully read.
    _wasread = true;
    
  }
  
  /*
   * Reset file read status.
   */
  void INIFile::reset () {
    
    // Turn off wasread flag.
    _wasread = false;
    
    // Clear data.
    _sections.clear();
    
  }
  
  /***************************************************************************
   * Class INIFile::Section.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Return a key's value.
   */
  ValReader INIFile::Section::operator [] (string const& key) const {
    
    // Make sure the key is defined.
    if (!key_defined(key)) {
      throw Errors::KeyUndefined();
    }
    
    // Return the value.
    return _keys.find(key)->second;
    
  }
  
  /*
   * Get a list of keys in this section.
   */
  INIFile::KeyListPT INIFile::Section::get_keys () const {
    
    // Work area.
    KeyListPT retval(new KeyListT);
    
    // Push key names into the key list.
    for (_KeyT::const_iterator i = _keys.begin(); i != _keys.end(); ++i) {
      retval->push_back(i->first);
    }
    
    // Done.
    return retval;
    
  }
  
}

