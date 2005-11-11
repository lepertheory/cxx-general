/*****************************************************************************
 * POSIXFile.cxx
 *****************************************************************************
 * Implementation of class POSIXFile.
 *****************************************************************************/

// STL includes.
#include <string>

// System includes.
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <cxx-general/toString.hxx>

// Class include.
#include "POSIXFile.hxx"

// Namespaces used.
using namespace std;

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************/
  // Constants.
  
  // Directory separator.
  char const POSIXFile::DIR_SEP = '/';
  
  /***************************************************************************/
  // Function members.
  
  // Constructor names the file.
  POSIXFile::POSIXFile (string const& filename) {
    
    // Do standard init.
    clear();
    
    // Set the filename.
    Filename(filename);
    
  }
  
  // Reset to just-constructed defaults.
  void POSIXFile::clear () {
    
    // Get a new _data.
    _data = new _Data;
    
  }
  
  // Copy a given POSIXFile. Does not copy the file itself.
  void POSIXFile::copy (POSIXFile const& source) {
    
    // Prepare for the new file if necessary.
    if (!_data->filename.empty() && _data->filename != source._data->filename) {
      clear();
    }
    
    // Work area.
    _data = source._data;
    
  }
  
  // Get the file part of the filename.
  string POSIXFile::basename () const {
    
    // Work area.
    string::size_type pos;
    string::size_type sz       = _data->filename.size();
    string::size_type lastchar = _data->filename.find_last_not_of(DIR_SEP);
    
    // If filename is less than two characters (can't have a separator &
    // filename in one character), return filename. May be an empty string.
    if (sz < 2) {
      return _data->filename;
    }
    
    // If the filename does not contain a separator before the last component,
    // return the string minus any trailing separators. 1 + lastchar to
    // convert from an offset to a length.
    if ((pos = _data->filename.rfind(DIR_SEP, lastchar)) == string::npos) {
      return _data->filename.substr(0, 1 + lastchar);
    }
    
    // If we have gotten this far and not found a non-separator character, we
    // were passed a string of only separators, which can only be root. Return
    // dir separator.
    if (lastchar == string::npos) {
      return DAC::toStringChr(DIR_SEP);
    }
    
    // Return the filename starting one character after the last separator,
    // and ending at the last character that is not a separator. Subtract pos
    // from offset for a length, do not + 1 because subtracting two offsets
    // gives us a length.
    return _data->filename.substr(pos + 1, lastchar - pos);
    
  }
  
  // Update the cache.
  void POSIXFile::_update_cache () const {
    
    // Call correct stat() or fstat() if the file is already open.
    if (_data->fd != 0) {
    } else {
      if (stat(_data->filename.c_str(), &_data->stat)) {
        switch (errno) {
          case EACCES:
            throw Errors::AccessDenied().Operation("stat").Filename(_data->filename);
          case ELOOP:
            throw Errors::SymlinkLoop().Operation("stat").Filename(_data->filename);
          case ENAMETOOLONG:
            throw Errors::NameTooLong().Filename(_data->filename);
          case ENOENT:
            throw Errors::PathNonExist().Filename(_data->filename);
          case ENOMEM:
            throw Errors::OutOfMemory();
          case ENOTDIR:
            throw Errors::NotDirectory().Filename(_data->filename);
          default:
            throw Errors::Unexpected().Errno(errno);
        };
      }
    }
    
    // Cache has been successfully updated.
    _data->cache_valid = true;
    
  }
  
  // Clear _data.
  void POSIXFile::_Data::clear () {
    
    cache_valid = false;
    
    fd = 0;
    
    stat.st_dev     = 0;
    stat.st_ino     = 0;
    stat.st_mode    = 0;
    stat.st_nlink   = 0;
    stat.st_uid     = 0;
    stat.st_gid     = 0;
    stat.st_rdev    = 0;
    stat.st_size    = 0;
    stat.st_blksize = 0;
    stat.st_blocks  = 0;
    stat.st_atime   = 0;
    stat.st_mtime   = 0;
    stat.st_ctime   = 0;
    
    filename.clear();
    
  }
  
  // Copy _data.
  void POSIXFile::_Data::copy (_Data const& source) {
    
    // Remove any info from the old data.
    clear();
    
    // New filename.
    filename = source.filename;
    
    // Open with the same mode as the previous file.
    
    
  }
  
}

