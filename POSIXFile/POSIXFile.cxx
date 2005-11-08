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

// Class include.
#include "POSIXFile.hxx"

// Namespaces used.
using namespace std;

// Namespace wrapper.
namespace DAC {
  
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
    
    _cachevalid = false;
    
    _stat.st_dev     = 0;
    _stat.st_ino     = 0;
    _stat.st_mode    = 0;
    _stat.st_nlink   = 0;
    _stat.st_uid     = 0;
    _stat.st_gid     = 0;
    _stat.st_rdev    = 0;
    _stat.st_size    = 0;
    _stat.st_blksize = 0;
    _stat.st_blocks  = 0;
    _stat.st_atime   = 0;
    _stat.st_mtime   = 0;
    _stat.st_ctime   = 0;
    
    _filename.clear();
    
  }
  
  // Copy a given File. Does not copy the file itself.
  void POSIXFile::copy (POSIXFile const& source) {
    
    // Prepare for the new file if necessary.
    if (!_filename.empty() && _filename != source._filename) {
      clear();
    }
    
    // Copy.
    _filename = source._filename;
    
  }
  
  // Update the cache.
  void POSIXFile::_update_cache () const {
    
    // Call correct stat() or fstat() if the file is already open.
    
    
  }
  
}

