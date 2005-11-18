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
  
  // Permissions mask.
  mode_t const POSIXFile::PERM_MASK = S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO;
  
  /***************************************************************************/
  // Function members.
  
  // Constructor names the file.
  POSIXFile::POSIXFile (string const& filename) {
    
    // Do standard init.
    clear();
    
    // Set the filename.
    Filename(filename);
    
  }
  
  // Set the mode.
  POSIXFile& POSIXFile::Mode (mode_t const new_mode, bool const cache) {
    
    // Make sure the stat cache is updated.
    _check_cache(cache);
    
    // Update the mode of the file.
    _cache_valid = false;
    if (_fd && fchmod(_fd, new_mode) || chmod(_filename.c_str(), new_mode)) {
      switch (errno) {
        case EACCES      : throw Errors::AccessDenied ().Operation("chmod").Filename(_filename);
        case EIO         : throw Errors::IOError      ().Operation("chmod").Filename(_filename);
        case EBADF       : throw Errors::BadDescriptor().Operation("chmod").Descriptor(_fd);
        case ELOOP       : throw Errors::SymlinkLoop  ().Operation("chmod").Filename(_filename);
        case ENAMETOOLONG: throw Errors::NameTooLong  ().Filename(_filename);
        case ENOENT      : throw Errors::PathNonExist ().Filename(_filename);
        case ENOMEM      : throw Errors::OutOfMemory  ();
        case ENOTDIR     : throw Errors::NotDirectory ().Filename(_filename);
        case EPERM       : throw Errors::NoPermission ().Operation("chmod").Filename(_filename);
        case EROFS       : throw Errors::ReadOnlyFS   ().Operation("chmod").Filename(_filename);
        default          : throw Errors::Unexpected   ().Errno(errno);
      };
    }
    
    // Set the new mode in the cache.
    _stat.st_mode = _stat.st_mode & PERM_MASK | new_mode;
    _cache_valid  = true;
    
    // Done.
    return *this;
    
  }
  
  // Reset to just-constructed defaults.
  void POSIXFile::clear () {
    
    _cache_valid = false;
    
    _fd     = 0;
    _curpos = 0;
    
    _filename.clear();
    
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
    
    _flags = O_APPEND | O_CREAT;
    
  }
  
  // Copy.
  void POSIXFile::copy (POSIXFile const& source) {
    
    // Remove any info from the old data.
    clear();
    
    // New filename.
    _filename = source._filename;
    
    // Open flags.
    _flags = source._flags;
    
    // Transfer the current position. Will be overwritten unless saved before
    // open and restored after.
    _curpos = source._curpos;
    
  }
  
  // Open the file.
  void POSIXFile::open () {
    
    // Make sure the file is not already open.
    if (_fd) {
      throw Errors::AlreadyOpen().Filename(_filename);
    }
    
    // Open the file.
    if ((_fd = ::open(_filename.c_str(), _flags)) == -1) {
      switch (errno) {
        case EACCES      : throw Errors::AccessDenied ().Operation("open").Filename(_filename);
        case EEXIST      : throw Errors::FileExists   ().Operation("open").Filename(_filename);
        case EISDIR      : throw Errors::IsDirectory  ().Filename(_filename);
        case ELOOP       : throw Errors::SymlinkLoop  ().Operation("open").Filename(_filename);
        case EMFILE      : throw Errors::ProcMaxFiles ();
        case ENAMETOOLONG: throw Errors::NameTooLong  ().Filename(_filename);
        case ENFILE      : throw Errors::SysMaxFiles  ();
        case ENOENT      : throw Errors::PathNonExist ().Filename(_filename);
        case ENOMEM      : throw Errors::OutOfMemory  ();
        case ENOSPC      : throw Errors::NoSpace      ();
        case ENOTDIR     : throw Errors::NotDirectory ().Filename(_filename);
        case EOVERFLOW   : throw Errors::FileOverflow ().Filename(_filename);
        case EPERM       : throw Errors::NoPermission ().Operation("open").Filename(_filename);
        case EROFS       : throw Errors::ReadOnlyFS   ().Operation("open").Filename(_filename);
        case ETXTBSY     : throw Errors::ExecuteWrite ().Filename(_filename);
        default          : throw Errors::Unexpected   ().Errno(errno);
      };
    }
    
    // Reset the current file position.
    if (_flags & O_APPEND) {
      _curpos = size();
    } else {
      _curpos = 0;
    }
    
  }
  
  // Close the file.
  void POSIXFile::close () {
    
    // Make sure the file is open.
    if (!_fd) {
      throw Errors::NotOpen();
    }
    
    // Close the file.
    if (_fd.set(0)) {
      switch (errno) {
        case EBADF: throw Errors::BadDescriptor().Operation("close");
        case EINTR: throw Errors::Interrupted  ().Operation("close");
        case EIO  : throw Errors::IOError      ().Operation("close").Filename(_filename);
        default   : throw Errors::Unexpected   ().Errno(errno);
      };
    }
    
  }
  
  // Get the file part of the filename.
  string POSIXFile::basename () const {
    
    // Work area.
    string::size_type pos;
    string::size_type sz       = _filename.size();
    string::size_type lastchar = _filename.find_last_not_of(DIR_SEP);
    
    // If filename is less than two characters (can't have a separator &
    // filename in one character), return filename. May be an empty string.
    if (sz < 2) {
      return _filename;
    }
    
    // If the filename does not contain a separator before the last component,
    // return the string minus any trailing separators. 1 + lastchar to
    // convert from an offset to a length.
    if ((pos = _filename.rfind(DIR_SEP, lastchar)) == string::npos) {
      return _filename.substr(0, 1 + lastchar);
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
    return _filename.substr(pos + 1, lastchar - pos);
    
  }
  
  // Update the cache.
  void POSIXFile::_update_cache () const {
    
    // Call correct stat() or fstat() if the file is already open.
    if (_fd && fstat(_fd, &_stat) || stat(_filename.c_str(), &_stat)) {
      switch (errno) {
        case EACCES      : throw Errors::AccessDenied ().Operation("stat").Filename(_filename);
        case EBADF       : throw Errors::BadDescriptor().Operation("stat").Descriptor(_fd);
        case ELOOP       : throw Errors::SymlinkLoop  ().Operation("stat").Filename(_filename);
        case ENAMETOOLONG: throw Errors::NameTooLong  ().Filename(_filename);
        case ENOENT      : throw Errors::PathNonExist ().Filename(_filename);
        case ENOMEM      : throw Errors::OutOfMemory  ();
        case ENOTDIR     : throw Errors::NotDirectory ().Filename(_filename);
        default          : throw Errors::Unexpected   ().Errno(errno);
      };
    }
    
    // Cache has been successfully updated.
    _cache_valid = true;
    
  }
  
  // Seek to a particular offset.
  void POSIXFile::seek (off_t const offset, SeekMode const whence) {
    
    // Make sure the file is open.
    if (!_fd) {
      throw Errors::NotOpen();
    }
    
    // Seek.
    _seek_update_pos(offset, whence);
    
  }
  
  // Read the entire file as a string.
  string POSIXFile::get_file () {
    
    // Work area.
    string         retval;
    auto_ptr<char> buf   (new char[size()]);
    
    // Reserve space for the file contents.
    retval.reserve(size());
    
    // Read the file.
    _read(reinterpret_cast<void*>(buf.get()), size(), false);
    
    // Copy the buffer into the string.
    retval.assign(buf.get(), size());
    
    // Done.
    return retval;
    
  }
  
  // Read from the file.
  ssize_t POSIXFile::_read (void* const buf, off_t const bufsize, bool const updatepos) {
    
    // Work area.
    ssize_t retval = 0;
    
    // Get the current file status.
    bool  fileopen = _fd;
    off_t tmppos   = _curpos;
    
    // Try block to ensure that any changes are undone.
    try {
      
      // If the file is not open, open it.
      if (!fileopen) {
        open();
      }
      
      // Seek to the beginning of the file.
      // FIXME: Duh. Generic read, not entire file, we don't want to seek to beginning.
      if (updatepos) {
        _seek_update_pos(0);
      } else {
        _seek(0);
      }
      
      // Read the file.
      if ((retval = read(_fd, buf, bufsize)) == -1) {
        switch (errno) {
          case EAGAIN: throw Errors::TryAgain     ();
          case EBADF : throw Errors::BadDescriptor().Operation("read");
          case EINTR : throw Errors::Interrupted  ().Operation("read");
          case EINVAL: throw Errors::Invalid      ().Operation("read").Filename(_filename);
          case EIO   : throw Errors::IOError      ().Operation("read").Filename(_filename);
          case EISDIR: throw Errors::IsDirectory  ().Filename(_filename);
          default    : throw Errors::Unexpected   ().Errno(errno);
        };
      }
      
      // Check for end of file.
      if (!retval) {
        throw Errors::EoF().Operation("read");
      }
      
    // Restore the previous state.
    } catch (...) {
      if (!fileopen) {
        try { _fd = 0; } catch (...) {}
      }
      _curpos = tmppos;
      throw;
    }
    
    // Close the file if it was not previously opened.
    if (!fileopen) {
      _fd = 0;
    }
    
    // Update / restore the current position.
    if (updatepos) {
      _curpos += retval;
    } else {
      _curpos = tmppos;
    }
    
    // Return the number of bytes read.
    return retval;
    
  }
  
  // Seek to a particular offset.
  void POSIXFile::_seek (off_t const offset, SeekMode const whence) {
    
    // Do the seek.
    if (lseek(_fd, offset, whence) == -1) {
      switch (errno) {
        case EBADF : throw Errors::BadDescriptor().Operation("lseek");
        case ESPIPE: throw Errors::CannotSeek   ().Filename(_filename);
        default    : throw Errors::Unexpected   ().Errno(errno);
      };
    }
    
  }
  
}

