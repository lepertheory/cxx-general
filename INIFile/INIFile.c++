// Standard includes.
#include <string>

// Internal includes.
#include "cxx-general/POSIXFile/POSIXFile.h++"

// Class include.
#include "cxx-general/INIFile/INIFile.h++"

// Import namespaces.
using namespace std;

// Namespace wrapping.
namespace DAC {
  
  /***************************************************************************
   * Class INIFile.
   ***************************************************************************/
  
  // Default constructor.
  INIFile::INIFile () {
    
    // Initialize.
    clear();
    
  }
  
  // Copy constructor.
  INIFile::INIFile (INIFile const& inifile) {
    
    // Copy the given data. No need to init, copy does everything.
    copy(inifile);
    
  }
  
  // Filename constructor.
  INIFile::INIFile (string const& filename) {
    
    // Initialize.
    clear();
    
    // Set the filename.
    _data->filename = filename;
    
  }
  
  // Set the name of the INI file.
  INIFile& INIFile::Filename (string const& filename) {
    
    // Create a new INIFile with the given filename.
    INIFile retval(filename);
    
    // Swap in the new data and return.
    _data = retval._data;
    return *this;
    
  }
  
  // Reset to just-constructed state.
  void INIFile::clear () {
    
    // Create new data.
    _data = new _Data;
    
  }
  
  // Copy a given INIFile.
  void INIFile::copy (INIFile const& inifile) {
    
    // Copy.
    _data = inifile._data;
    
  }
  
  // Deep copy, do not wait for COW.
  void INIFile::deepcopy (INIFile const& inifile) {
    
    // Create new data.
    _data = new _Data(*(inifile._data));
    
  }
  
  // Read the INI file.
  void INIFile::read () {
    
    // Make sure that the filename is set.
    if (_data->filename.empty()) {
      throw Errors::FileNotSet();
    }
    
    // Work area.
    POSIXFile file(_data->filename);
    
    // Make sure we will be able to read the file.
    if (!file.is_exist()) {
      throw Errors::FileNonExist().Filename(_data->filename);
    }
    if (!file.is_file()) {
      throw Errors::FileNonFile().Filename(_data->filename);
    }
    if (!file.is_readable()) {
      throw Errors::FileNonReadable().Filename(_data->filename);
    }
    
    // Work area.
    _DataPT    retval(new _Data(*_data));
    _SectionT& newsections = retval->sections;
    
    // Process each line.
    string line   ;
    string section;
    for (file.OpenMode(POSIXFile::OM_READ).open(); !file.eof_line(); ) {
      
      // Read a line and trim whitespace.
      line = file.read_line(true);
      
      // Skip blank lines and comments.
      if (line.empty() || line[0] == ';') {
        continue;
      }
      
      // Work area.
      string::size_type valuesep = string::npos;
      
      // Check if this is the beginning of a section.
      if (line[0] == '[' && line[line.size() - 1] == ']') {
        
        // Make sure the section is not blank.
        if (line.size() == 2) {
          throw Errors::SectionBlank();
        }
        
        // Get the section name.
        section = line.substr(1, line.size() - 2);
        
        // Make sure this section does not already exist.
        if (newsections.count(section)) {
          throw Errors::SectionMultiDefine().Section(section);
        }
        
        // Create the new section.
        newsections[section];
        
      // Check for a key=value.
      } else if ((valuesep = line.find('=')) != string::npos) {
        
        // Make sure that we have a key.
        if (valuesep == 0) {
          throw Errors::ValueNoKey();
        }
        
        // Work area.
        string key  ;
        string value;
        
        // Get the key, trim whitespace from end.
        key = line.substr(0, min(line.find_last_not_of(" \t", valuesep - 1) + 1, valuesep));
        
        // Make sure that we have an active section.
        if (section.empty()) {
          throw Errors::KeyNoSection().Key(key);
        }
        
        // Make sure this key does not already exist in this section.
        if (newsections[section].count(key)) {
          throw Errors::KeyMultiDefine().Section(section).Key(key);
        }
        
        // Get the value, trim whitespace from beginning.
        if (valuesep != line.size() - 1) {
          value = line.substr(line.find_first_not_of(" \t", valuesep + 1));
        }
        
        // Add the key.
        newsections[section].insert(_KeyT::value_type(key, value));
        
      // Parse error.
      } else {
        throw Errors::ParseError().Line(line);
      }
      
    }
    file.close();
    
    // Move the new data into place and return.
    _data = retval;
    
  }
  
  /***************************************************************************
   * Class INIFile::_Data.
   ***************************************************************************/
  
  // Default constructor.
  INIFile::_Data::_Data () {
    
    // Init.
    clear();
    
  }
  
  // Copy constructor.
  INIFile::_Data::_Data (_Data const& data) {
    
    // Copy the given data. No need to init, copy does everything.
    copy(data);
    
  }
  
  // Reset to just constructed state.
  void INIFile::_Data::clear () {
    
    // Clear.
    filename.clear();
    sections.clear();
    
  }
  
  // Copy a given _Data object.
  void INIFile::_Data::copy (_Data const& data) {
    
    filename = data.filename;
    sections = data.sections;
    
  }
  
}

