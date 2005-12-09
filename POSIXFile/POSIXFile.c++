/*****************************************************************************
 * POSIXFile.c++
 *****************************************************************************
 * Implementation of class POSIXFile.
 *****************************************************************************/

// STL includes.
#include <string>

// System includes.
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#include <pwd.h>
#include <cxx-general/toString.h++>
#include <cxx-general/AutoArray.h++>

// Class include.
#include "POSIXFile.h++"

// Namespaces used.
using namespace std;

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************/
  // Constants.
  
  // Directory separator.
  char const POSIXFile::DIR_SEP = '/';
  
  // Do not change UID/GID.
  uid_t const POSIXFile::UID_NOCHANGE = static_cast<uid_t>(-1);
  gid_t const POSIXFile::GID_NOCHANGE = static_cast<uid_t>(-1);
  
  // Permissions mask.
  mode_t const POSIXFile::_PERM_MASK = S_ISUID | S_ISGID | S_ISVTX | S_IRWXU | S_IRWXG | S_IRWXO;
  
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
      throw Errors::IsOpen().Filename(_filename);
    }
    
    // Open the file.
    if ((_fd = ::open(_filename.c_str(), _flags)) == -1) {
      s_throwSysCallError(errno, "open", _filename);
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
      throw Errors::NotOpen().Filename(_filename).Operation("close");
    }
    
    // Close the file.
    if (_fd.set(0)) {
      s_throwSysCallError(errno, "close", _filename);
    }
    
  }
  
  // Create a hard link.
  void POSIXFile::link (string const& filename) {
    
    // Update the cache.
    _update_cache();
    
    // Link.
    _cache_valid = false;
    if (::link(_filename.c_str(), filename.c_str())) {
      s_throwSysCallError(errno, "link", "\"" + _filename + "\" \"" + filename + "\"");
    }
    
    // Update the link count to the file in the cache.
    _stat.st_nlink += 1;
    _cache_valid = true;
    
  }
  
  // Delete the file.
  void POSIXFile::unlink () {
    
    // Make sure we do not have the file open.
    if (_fd) {
      throw Errors::IsOpen().Filename(_filename);
    }
    
    // Invalidate the cache.
    _cache_valid = false;
    
    // Unlink.
    if (::unlink(_filename.c_str())) {
      s_throwSysCallError(errno, "unlink", _filename);
    }
    
  }
  
  // Change the mode.
  void POSIXFile::chmod (mode_t const new_mode) {
    
    // Make sure the stat cache is updated.
    _update_cache();
    
    // Update the mode of the file.
    _cache_valid = false;
    if (_fd && fchmod(_fd, new_mode & _PERM_MASK) || ::chmod(_filename.c_str(), new_mode & _PERM_MASK)) {
      s_throwSysCallError(errno, "chmod", _filename);
    }
    
    // Set the new mode in the cache.
    _stat.st_mode = _stat.st_mode & ~_PERM_MASK | (new_mode & _PERM_MASK);
    _cache_valid  = true;
    
  }
  
  // Change file owner/group.
  void POSIXFile::chown (uid_t const owner, gid_t const group) {
    
    // Don't waste time if there's nothing to do.
    if (owner == UID_NOCHANGE && group == GID_NOCHANGE) {
      return;
    }
    
    // Invalidate the cache.
    _cache_valid = false;
    
    // Call the correct chown whether the file is open or closed.
    if (_fd && fchown(_fd, owner, group) || ::chown(_filename.c_str(), owner, group)) {
      s_throwSysCallError(errno, "chown", _filename);
    }
    
  }
  
  // Get the file part of the filename.
  string POSIXFile::basename () const {
    
    // Work area.
    string::size_type pos;
    string::size_type sz       = _filename.size();
    string::size_type lastchar = _filename.find_last_not_of(DIR_SEP);
    
    // If the filename is less than two characters, return the filename; since
    // we can't have a separator & filename in one character filename must
    // either be only a filename, only a directory separator--in which case it
    // points to the root dir--or an empty string.
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

  // Get the directory part of the filename.
  string POSIXFile::dirname () const {
    
    // Work area.
    string::size_type lastsep = _filename.rfind(DIR_SEP, _filename.find_last_not_of(DIR_SEP));
    string::size_type lastchr = string::npos;
    
    // If the filename is blank, return that.
    if (_filename.empty()) {
      return string();
    }
    
    // If the filename does not contain a separator character, return current
    // directory.
    if (lastsep == string::npos) {
      return string(".");
    }
    
    // Get the last directory name character.
    lastchr = _filename.find_last_not_of(DIR_SEP, lastsep);
    
    // If filename consists of only separator characters, return that.
    if (lastchr == string::npos) {
      return DAC::toStringChr(DIR_SEP);
    }
    
    // Return the filename up to the start of the last separator block.
    // lastchr + 1 because we are converting from an offset to a length.
    // Compress redundant directory separators.
    return _filename.substr(0, lastchr + 1);
    
  }
  
  // Convert to an absolute path. Home directory interpretation is not
  // supported.
  string POSIXFile::expand_path () const {
    
    // Constants.
    static string const CUR_DIR (DAC::toStringChr(DIR_SEP) + "." );
    static string const PREV_DIR(DAC::toStringChr(DIR_SEP) + "..");
    static string::size_type const CUR_DIR_LEN (CUR_DIR .length());
    static string::size_type const PREV_DIR_LEN(PREV_DIR.length());
    
    // Work area.
    string retval;
    
    // If the filename is blank, return that.
    if (_filename.empty()) {
      return _filename;
    }
    
    // If the filename does not begin with a directory separator, prefix the
    // cwd.
    if (_filename[0] != DIR_SEP) {
      retval += getCWD() + DAC::toStringChr(DIR_SEP);
    }
    
    // Append the filename.
    retval += _filename;
    
    // Compress any redundant directory separators.
    s_compress_dirSep(retval);
    
    // Compress any current directories.
    for (string::size_type pos = 0; pos = retval.find(CUR_DIR, pos), pos != string::npos;) {
      if (retval.length() > pos + CUR_DIR_LEN) {
        if (retval[pos + CUR_DIR_LEN] == DIR_SEP) {
          retval.erase(pos, CUR_DIR_LEN);
        } else {
          pos += CUR_DIR_LEN;
        }
      } else {
        retval.resize(pos);
      }
    }
    
    // Process any previous directories.
    for (string::size_type pos = 0; pos = retval.find(PREV_DIR, pos), pos != string::npos;) {
      cout << "retval: " << retval << "  pos: " << pos << "\n";
      if (retval.length() > pos + PREV_DIR_LEN) {
        if (retval[pos + PREV_DIR_LEN] == DIR_SEP) {
          if (pos == 0) {
            retval.erase(0, PREV_DIR_LEN);
          } else {
            string::size_type prevdirstart = retval.rfind(DIR_SEP, pos - 1);
            retval.erase(prevdirstart + 1, pos + PREV_DIR_LEN - prevdirstart);
            pos = prevdirstart;
          }
        } else {
          pos += PREV_DIR_LEN;
        }
      } else {
        if (pos == 0) {
          retval = DAC::toStringChr(DIR_SEP);
        } else {
          retval.resize(retval.rfind(DIR_SEP, pos - 1) + 1);
        }
      }
    }
    
    // We done.
    return retval;
    
  }
  
  // Seek to a particular offset.
  void POSIXFile::seek (off_t const offset, SeekMode const whence) {
    
    // Make sure the file is open.
    if (!_fd) {
      throw Errors::NotOpen().Filename(_filename).Operation("seek");
    }
    
    // Seek.
    _seek_update_pos(offset, whence);
    
  }
  
  // Read the entire file as a string.
  string POSIXFile::get_file () {
    
    // Work area.
    string          retval;
    AutoArray<char> buf;
    
    // Make sure we're not trying to read from an empty file.
    if (size() == 0) {
      return retval;
    }
    
    // Reserve space for the file contents.
    retval.reserve(size());
    buf = new char[size()];
    
    // Get the current file open status.
    bool fileopen = _fd;
    
    // Try block to ensure that any changes are undone.
    try {
      
      // If the file is not open, open it.
      if (!fileopen) {
        open();
      }
      
      // Read the entire file.
      _seek(0);
      _read(reinterpret_cast<void*>(buf.get()), size());
      
    // Restore the previous state and rethrow the error.
    } catch (...) {
      if (!fileopen) {
        try { _fd = 0; } catch (...) {}
      }
      throw;
    }
    
    // Close the file if it was not already open.
    if (!fileopen) {
      close();
    }
    
    // Copy the buffer into the string.
    retval.assign(buf.get(), size());
    
    // Done.
    return retval;
    
  }
  
  // Get the current working directory.
  string POSIXFile::getCWD () {
    
    // Hard min/maximum of any path length.
    static size_t const MIN_PATH =   256;
    static size_t const MAX_PATH = 32768;
    
    // Work area.
    AutoArray<char> buf;
    
    // Double the buffer size with each iteration.
    for (size_t bufsize = MIN_PATH; bufsize <= MAX_PATH; bufsize *= 2) {
      
      // Allocate buffer.
      buf = new char[bufsize];
      
      // Make the system call, break if we got the current path.
      if (getcwd(buf.get(), bufsize)) {
        break;
      }
      
      // Throw an error if we got any error but the buffer being too small.
      if (errno != ERANGE) {
        s_throwSysCallError(errno, "getcwd", ".");
      }
      
    }
    
    // Return the current working directory.
    return buf.get();
    
  }
  
  // Update the cache.
  void POSIXFile::_update_cache () const {
    
    // Call correct stat() or fstat() if the file is already open.
    if (_fd && fstat(_fd, &_stat) || stat(_filename.c_str(), &_stat)) {
      s_throwSysCallError(errno, "stat", _filename);
    }
    
    // Cache has been successfully updated.
    _cache_valid = true;
    
  }
  
  // Read from the file.
  ssize_t POSIXFile::_read (void* const buf, off_t const bufsize) {
    
    // Work area.
    ssize_t retval = 0;
    
    // Read the file.
    if ((retval = read(_fd, buf, bufsize)) == -1) {
      s_throwSysCallError(errno, "read", _filename);
    }
    
    // Check for end of file.
    if (!retval) {
      throw Errors::EoF().Operation("read");
    }
    
    // Return the number of bytes read.
    return retval;
    
  }
  
  // Seek to a particular offset.
  void POSIXFile::_seek (off_t const offset, SeekMode const whence) {
    
    // Do the seek.
    if (lseek(_fd, offset, whence) == -1) {
      s_throwSysCallError(errno, "lseek", _filename);
    }
    
  }
  
  // Handle and throw a system call error.
  void POSIXFile::s_throwSysCallError (int const errnum, std::string const& syscall, std::string const& filename) {
    
    // Throw the appropriate error.
    switch (errnum) {
      case EACCES      : throw dynamic_cast<Errors::AccessDenied &>(Errors::AccessDenied ().Errno(errnum).SysCall(syscall).Filename(filename));
      case EAGAIN      : throw dynamic_cast<Errors::TryAgain     &>(Errors::TryAgain     ().Errno(errnum).SysCall(syscall).Filename(filename));
      case EBADF       : throw dynamic_cast<Errors::BadDescriptor&>(Errors::BadDescriptor().Errno(errnum).SysCall(syscall).Filename(filename));
      case EEXIST      : throw dynamic_cast<Errors::FileExists   &>(Errors::FileExists   ().Errno(errnum).SysCall(syscall).Filename(filename));
      case EINTR       : throw dynamic_cast<Errors::Interrupted  &>(Errors::Interrupted  ().Errno(errnum).SysCall(syscall).Filename(filename));
      case EINVAL      : throw dynamic_cast<Errors::Invalid      &>(Errors::Invalid      ().Errno(errnum).SysCall(syscall).Filename(filename));
      case EIO         : throw dynamic_cast<Errors::IOError      &>(Errors::IOError      ().Errno(errnum).SysCall(syscall).Filename(filename));
      case EISDIR      : throw dynamic_cast<Errors::IsDirectory  &>(Errors::IsDirectory  ().Errno(errnum).SysCall(syscall).Filename(filename));
      case ELOOP       : throw dynamic_cast<Errors::SymlinkLoop  &>(Errors::SymlinkLoop  ().Errno(errnum).SysCall(syscall).Filename(filename));
      case EMFILE      : throw dynamic_cast<Errors::ProcMaxFiles &>(Errors::ProcMaxFiles ().Errno(errnum).SysCall(syscall).Filename(filename));
      case ENAMETOOLONG: throw dynamic_cast<Errors::NameTooLong  &>(Errors::NameTooLong  ().Errno(errnum).SysCall(syscall).Filename(filename));
      case ENFILE      : throw dynamic_cast<Errors::SysMaxFiles  &>(Errors::SysMaxFiles  ().Errno(errnum).SysCall(syscall).Filename(filename));
      case ENOENT      : throw dynamic_cast<Errors::PathNonExist &>(Errors::PathNonExist ().Errno(errnum).SysCall(syscall).Filename(filename));
      case ENOMEM      : throw dynamic_cast<Errors::OutOfMemory  &>(Errors::OutOfMemory  ().Errno(errnum).SysCall(syscall).Filename(filename));
      case ENOSPC      : throw dynamic_cast<Errors::NoSpace      &>(Errors::NoSpace      ().Errno(errnum).SysCall(syscall).Filename(filename));
      case ENOTDIR     : throw dynamic_cast<Errors::NotDirectory &>(Errors::NotDirectory ().Errno(errnum).SysCall(syscall).Filename(filename));
      case EOVERFLOW   : throw dynamic_cast<Errors::FileOverflow &>(Errors::FileOverflow ().Errno(errnum).SysCall(syscall).Filename(filename));
      case EPERM       : throw dynamic_cast<Errors::NotPermitted &>(Errors::NotPermitted ().Errno(errnum).SysCall(syscall).Filename(filename));
      case ESPIPE      : throw dynamic_cast<Errors::CannotSeek   &>(Errors::CannotSeek   ().Errno(errnum).SysCall(syscall).Filename(filename));
      case EROFS       : throw dynamic_cast<Errors::ReadOnlyFS   &>(Errors::ReadOnlyFS   ().Errno(errnum).SysCall(syscall).Filename(filename));
      case ETXTBSY     : throw dynamic_cast<Errors::FileBusy     &>(Errors::FileBusy     ().Errno(errnum).SysCall(syscall).Filename(filename));
      default          : throw dynamic_cast<Errors::Unexpected   &>(Errors::Unexpected   ().Errno(errnum).SysCall(syscall).Filename(filename));
    };
    
  }
  
  // Compress redundant directory separators.
  void POSIXFile::s_compress_dirSep (string& path) {
    
    // Work area.
    string::size_type ds_start = path.find(DIR_SEP);
    string::size_type ds_end   = 0;
    
    // Loop through directory separator groups.
    while (ds_start != string::npos) {
      
      // Get the end of the current directory separator group. End if there
      // are no more non-separator characters in this string. Truncate extra
      // directory separator characters. Resize to ds_start + 1 because
      // ds_start is an offset, and we need a length.
      if ((ds_end = path.find_first_not_of(DIR_SEP, ds_start)) == string::npos) {
        path.resize(ds_start + 1);
        break;
      }
      
      // Decrease ds_end because we searched on the first non-separator
      // character, this should point to the last separator.
      --ds_end;
      
      // Erase any directory separator characters in this group beyond the
      // first one.
      if (ds_end != ds_start) {
        path.erase(ds_start + 1, ds_end - ds_start);
      }
      
      // Find the start of  the next directory separator group.
      ds_start = path.find(DIR_SEP, ds_start + 1);
      
    }
    
  }
  
}

