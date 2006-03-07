/*****************************************************************************
 * POSIXFile.c++
 *****************************************************************************
 * Implementation of class POSIXFile.
 *****************************************************************************/

// STL includes.
#include <string>
#include <vector>

// System includes.
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>
#include <cerrno>
#include <pwd.h>
#include <toString.h++>
#include <AutoArray.h++>
#include <SafeInt.h++>
#include <fcntl.h>

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
  
  // Hard min/maximum of any path length.
  size_t const POSIXFile::_MIN_PATH =   256;
  size_t const POSIXFile::_MAX_PATH = 32768;
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Reset to just-constructed defaults.
   */
  void POSIXFile::clear () {
    
    _cache_valid = false;
    
    _fd     = -1;
    _curpos =  0;
    
    _filename.clear();
    _recordsep = "\n";
    _recordnum = 0;
    
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
    
    _flags      = O_APPEND | O_CREAT                                       ;
    _openmode   = OM_READ                                                  ;
    _createmode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    
    _eof      = true;
    _eof_line = true;
    
  }
  
  /*
   * Copy.
   */
  void POSIXFile::copy (POSIXFile const& source) {
    
    // Remove any info from the old data.
    clear();
    
    // New filename.
    _filename = source._filename;
    
    // Record separator.
    _recordsep = source._recordsep;
    
    // Current record number.
    _recordnum = source._recordnum;
    
    // Open flags.
    _flags      = source._flags     ;
    _openmode   = source._openmode  ;
    _createmode = source._createmode;
    
    // Transfer the current position. Will be overwritten unless saved before
    // open and restored after.
    _curpos = source._curpos;
    
    // End of file.
    _eof      = source._eof     ;
    _eof_line = source._eof_line;
    
  }
  
  /*
   * Open the file.
   */
  void POSIXFile::open () {
    
    // Make sure that a filename has been set.
    if (_filename.empty()) {
      throw Errors::NoFile();
    }
    
    // Open the file.
    if ((_fd = ::open(_filename.c_str(), _openmode | _flags, _createmode)) == -1) {
      s_throwSysCallError(errno, "open");
    }
    
    // Reset the current file position.
    _eof = false;
    if (_flags & O_APPEND) {
      _curpos = size();
    } else {
      _curpos = 0;
    }
    
  }
  
  /*
   * Close the file.
   */
  void POSIXFile::close () {
    
    // Make sure the file is open.
    if (!is_open()) {
      return;
    }
    
    // Close the file.
    if (_fd.set(-1)) {
      s_throwSysCallError(errno, "close");
    }
    
  }
  
  /*
   * Lock the file.
   */
  bool POSIXFile::lock (bool const shared, bool const wait, off_t const start, off_t const len) {
    
    // Make sure the file is open.
    if (!is_open()) {
      open();
    }
    
    // Lock. Return false if the file is already locked and this is a
    // non-blocking lock.
    struct flock lockinfo = { shared ? F_RDLCK : F_WRLCK, SEEK_SET, start, len, 0 };
    if (fcntl(_fd, wait ? F_SETLKW : F_SETLK, &lockinfo)) {
      if (wait && errno == EACCES || errno == EAGAIN) {
        return false;
      }
      s_throwSysCallError(errno, "fcntl");
    }
    
    // Success.
    return true;
    
  }
  
  /*
   * Unlock the file.
   */
  void POSIXFile::unlock (off_t const start, off_t const len) {
    
    // Don't attempt to unlock if the file is not open.
    if (!is_open()) {
      return;
    }
    
    // Unlock.
    struct flock lockinfo = { F_UNLCK, SEEK_SET, start, len, 0 };
    if (fcntl(_fd, F_SETLK, &lockinfo)) {
      s_throwSysCallError(errno, "fcntl");
    }
    
  }
  
  /*
   * See if the file is locked.
   */
  bool POSIXFile::is_locked (bool const shared, off_t const start, off_t const len) {
    
    // Make sure the file is open.
    if (!is_open()) {
      open();
    }
    
    // Check for the lock.
    struct flock lockinfo = { shared ? F_RDLCK : F_WRLCK, SEEK_SET, start, len, 0 };
    if (fcntl(_fd, F_GETLK, &lockinfo)) {
      s_throwSysCallError(errno, "fcntl");
    }
    
    // Return whether file is locked.
    return lockinfo.l_type != F_UNLCK;
    
  }
  
  /*
   * Change the mode.
   */
  void POSIXFile::chmod (mode_t const new_mode) {
    
    // Make sure the file is open.
    if (!is_open()) {
      open();
    }
    
    // Make sure the stat cache is updated.
    _update_cache();
    
    // Update the mode of the file.
    _cache_valid = false;
    if (fchmod(_fd, new_mode & _PERM_MASK)) {
      s_throwSysCallError(errno, "chmod");
    }
    
    // Set the new mode in the cache.
    _stat.st_mode = _stat.st_mode & ~_PERM_MASK | (new_mode & _PERM_MASK);
    _cache_valid  = true;
    
  }
  
  /*
   * Change file owner/group.
   */
  void POSIXFile::chown (uid_t const owner, gid_t const group) {
    
    // Don't waste time if there's nothing to do.
    if (owner == UID_NOCHANGE && group == GID_NOCHANGE) {
      return;
    }
    
    // Make sure the file is open.
    if (!is_open()) {
      open();
    }
    
    // Invalidate the cache.
    _cache_valid = false;
    
    // Call the correct chown whether the file is open or closed.
    if (fchown(_fd, owner, group)) {
      s_throwSysCallError(errno, "chown");
    }
    
  }
  
  /*
   * Truncate or expand to an exact length.
   */
  void POSIXFile::truncate (off_t const length) {
    
    // Make sure the file is open.
    if (!is_open()) {
      open();
    }
    
    // Invalidate the cache.
    _cache_valid = false;
    
    // Truncate. Temporary length because the truncate prototype does not name
    // length as const.
    off_t tmplen(length);
    if (ftruncate(_fd, tmplen)) {
      s_throwSysCallError(errno, "truncate");
    }
    
  }
  
  /*
   * Read a particular number of bytes.
   */
  string POSIXFile::read (size_t const bytes) {
    
    // Make sure the file is open.
    if (!is_open()) {
      open();
    }
    
    // Work area.
    AutoArray<char> buf       (new char[bytes]);
    ssize_t         bytes_read                 ;
    
    // Read the requested number of bytes and update the current file position.
    bytes_read = _read(reinterpret_cast<void*>(buf.get()), bytes);
    
    // Convert buffer to a string and return.
    return string(buf.get(), bytes_read);
    
  }
  
  /*
   * Read a single line.
   */
  string POSIXFile::read_line (bool const trim) {
    
    // Work area.
    static string            buf                   ;
    static string::size_type next_sep(string::npos);
    string retval;
    
    // Find the end of this record.
    next_sep = buf.find(_recordsep);
    while (next_sep == string::npos && !eof()) {
      string tmp = read(blockSize());
      buf      += tmp;
      next_sep  = buf.find(_recordsep, buf.length() - tmp.length());
    }
    _eof_line = false;
    
    // Create return value and trim buffer down to size. Turn on eof_line if
    // there are no more lines available in the file.
    retval = buf.substr(0, next_sep);
    if ((next_sep == string::npos || next_sep == buf.length() - 1) && eof()) {
      next_sep = string::npos;
      buf.clear();
      _eof_line = true;
    } else {
      buf.erase(0, next_sep + 1);
    }
    
    // Trim whitespace if requested. Ugly as hell, but whatever, it works and
    // it's contained.
    if (trim) {
      string::size_type nowsbegin = 0;
      string::size_type nowsend   = 0;
      nowsbegin = retval.find_first_not_of(" \t");
      if (nowsbegin == string::npos) {
        retval.clear();
      } else {
        if (nowsbegin != 0) {
          retval.erase(0, nowsbegin);
        }
        nowsend = retval.find_last_not_of(" \t");
        if (nowsend != retval.size() - 1) {
          retval.erase(nowsend + 1);
        }
      }
    }
    
    // Done.
    return retval;
    
  }
  
  /*
   * Read the entire file as a vector of lines.
   */
  vector<string>& POSIXFile::read_all_lines (vector<string>& buffer) {
    
    // Work area.
    vector<string> retval;
    
    // Make sure we're not trying to read from an empty file.
    if (size() == 0) {
      buffer.swap(retval);
      return buffer;
    }
    
    // Fill the vector.
    _seek(0);
    while (!eof_line()) {
      retval.push_back(read_line());
    }
    
    // Done. Copy the result into place and return a reference for syntatic
    // sugar.
    buffer.swap(retval);
    return buffer;
    
  }
  
  /*
   * Read the entire file as a string.
   */
  string POSIXFile::read_file () {
    
    // Work area.
    string          retval    ;
    AutoArray<char> buf       ;
    ssize_t         bytes_read;
    
    // Make sure we're not trying to read from an empty file.
    if (size() == 0) {
      return retval;
    }
    
    // Reserve space for the file contents.
    buf = new char[size()];
    
    // Read the entire file.
    _seek(0);
    bytes_read = _read(reinterpret_cast<void*>(buf.get()), size());
    
    // Copy the buffer into the string.
    retval.assign(buf.get(), bytes_read);
    
    // Done.
    return retval;
    
  }
  
  /*
   * Write.
   */
  ssize_t POSIXFile::write (void const* const data, size_t const bytes) {
    
    // Make sure the file is open.
    if (!is_open()) {
      open();
    }
    
    // Work area.
    ssize_t retval = 0;
    
    // Write the data.
    if ((retval = ::write(_fd, data, bytes)) == -1) {
      s_throwSysCallError(errno, "write", _filename);
    }
    
    // Update the current position and invalidate cache for file size.
    _curpos      += retval;
    _cache_valid  = false;
    
    // Check for end of file.
    _eof = _curpos >= size();
    
    // Return the number of bytes written.
    return retval;
    
  }
  
  /*
   * Create a hard link.
   */
  void POSIXFile::link (string const& src, string const& dest) {
    
    // Link.
    if (::link(src.c_str(), dest.c_str())) {
      s_throwSysCallError(errno, "link");
    }
    
  }
  
  /*
   * Create a symbolic link.
   */
  void POSIXFile::symlink (string const& src, string const& dest) {
    
    // Create symlink.
    if (::symlink(src.c_str(), dest.c_str())) {
      s_throwSysCallError(errno, "symlink");
    }
    
  }
  
  /*
   * Rename the file.
   */
  void POSIXFile::rename (string const& src, string const& dest) {
    
    // Rename.
    if (::rename(src.c_str(), dest.c_str())) {
      s_throwSysCallError(errno, "rename");
    }
    
  }
  
  /*
   * Delete the file.
   */
  void POSIXFile::unlink (string const& filename) {
    
    // Unlink.
    if (::unlink(filename.c_str())) {
      s_throwSysCallError(errno, "unlink");
    }
    
  }
  
  /*
   * Change the mode.
   */
  void POSIXFile::chmod (string const& filename, mode_t const new_mode) {
    
    // Set the mode.
    if (chmod(filename.c_str(), new_mode & _PERM_MASK)) {
      s_throwSysCallError(errno, "chmod");
    }
    
  }
  
  /*
   * Change owner/group.
   */
  void POSIXFile::chown (string const& filename, uid_t const owner, gid_t const group) {
    
    // Don't waste time if there's nothing to do.
    if (owner == UID_NOCHANGE && group == GID_NOCHANGE) {
      return;
    }
    
    // Change.
    if (chown(filename.c_str(), owner, group)) {
      s_throwSysCallError(errno, "chown");
    }
    
  }
  
  /*
   * Truncate or expand to an exact length.
   */
  void POSIXFile::truncate (string const& filename, off_t const length) {
    
    // Truncate. Temporary length because the truncate prototype dose not name
    // length as const.
    off_t tmplen(length);
    if (truncate(filename.c_str(), tmplen)) {
      s_throwSysCallError(errno, "truncate");
    }
    
  }
  
  /*
   * Get the file part of the filename.
   */
  string POSIXFile::basename (string const& filename) {
    
    // Work area.
    string::size_type pos;
    string::size_type sz       = filename.size();
    string::size_type lastchar = filename.find_last_not_of(DIR_SEP);
    
    // If the filename is less than two characters, return the filename; since
    // we can't have a separator & filename in one character filename must
    // either be only a filename, only a directory separator--in which case it
    // points to the root dir--or an empty string.
    if (sz < 2) {
      return filename;
    }
    
    // If the filename does not contain a separator before the last component,
    // return the string minus any trailing separators. 1 + lastchar to
    // convert from an offset to a length.
    if ((pos = filename.rfind(DIR_SEP, lastchar)) == string::npos) {
      return filename.substr(0, 1 + lastchar);
    }
    
    // If we have gotten this far and not found a non-separator character, we
    // were passed a string of only separators, which can only be root. Return
    // dir separator.
    if (lastchar == string::npos) {
      return toStringChr(DIR_SEP);
    }
    
    // Return the filename starting one character after the last separator,
    // and ending at the last character that is not a separator. Subtract pos
    // from offset for a length, do not + 1 because subtracting two offsets
    // gives us a length.
    return filename.substr(pos + 1, lastchar - pos);
    
  }
  
  /*
   * Get the directory part of the filename.
   */
  string POSIXFile::dirname (string const& filename) {
    
    // Work area.
    string::size_type lastsep = filename.rfind(DIR_SEP, filename.find_last_not_of(DIR_SEP));
    string::size_type lastchr = string::npos;
    
    // If the filename is blank, return that.
    if (filename.empty()) {
      return string();
    }
    
    // If the filename does not contain a separator character, return current
    // directory.
    if (lastsep == string::npos) {
      return string(".");
    }
    
    // Get the last directory name character.
    lastchr = filename.find_last_not_of(DIR_SEP, lastsep);
    
    // If filename consists of only separator characters, return that.
    if (lastchr == string::npos) {
      return toStringChr(DIR_SEP);
    }
    
    // Return the filename up to the start of the last separator block.
    // lastchr + 1 because we are converting from an offset to a length.
    // Compress redundant directory separators.
    return filename.substr(0, lastchr + 1);
    
  }
  
  /*
   * Convert to an absolute path. Home directory interpretation is not
   * supported.
   */
  string POSIXFile::expand_path (string const& filename) const {
    
    // Constants.
    static string const CUR_DIR (toStringChr(DIR_SEP) + "." );
    static string const PREV_DIR(toStringChr(DIR_SEP) + "..");
    static string::size_type const CUR_DIR_LEN (CUR_DIR .length());
    static string::size_type const PREV_DIR_LEN(PREV_DIR.length());
    
    // Work area.
    string retval;
    
    // If the filename is blank, return that.
    if (filename.empty()) {
      return filename;
    }
    
    // If the filename does not begin with a directory separator, prefix the
    // cwd.
    if (filename[0] != DIR_SEP) {
      retval += getCWD() + toStringChr(DIR_SEP);
    }
    
    // Append the filename.
    retval += filename;
    
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
  
  /*
   * Read the target of a symbolic link.
   */
  string POSIXFile::readlink (string const& filename) {
    
    // Work area. SafeInt for the signed/unsigned comparison.
    AutoArray<char> buf;
    SafeInt<int>    chars;
    
    // Double the buffer size with each iteration.
    for (size_t bufsize = _MIN_PATH; bufsize <= _MAX_PATH; bufsize *= 2) {
      
      // Allocate buffer.
      buf = new char[bufsize];
      
      // Make the system call, break if successful. bufsize - 1 because
      // readlink() does not null terminate.
      if ((chars = ::readlink(filename.c_str(), buf.get(), bufsize - 1)) == -1) {
        s_throwSysCallError(errno, "readlink");
      }
      
      // Make sure that readlink did not fill the buffer. Will waste a call if
      // the length of the path is exactly as long as buf, but there is no
      // way to tell if there was truncation otherwise. Signed/unsigned
      // comparison is the reason chars is a SafeInt.
      if (chars < bufsize) {
        buf[chars] = '\0';
        break;
      }
      
    }
    
    // Return the link target.
    return buf.get();
    
  }
  
  /*
   * Get the current working directory.
   */
  string POSIXFile::getCWD () {
    
    // Work area.
    AutoArray<char> buf;
    
    // Double the buffer size with each iteration.
    for (size_t bufsize = _MIN_PATH; bufsize <= _MAX_PATH; bufsize *= 2) {
      
      // Allocate buffer.
      buf = new char[bufsize];
      
      // Make the system call, break if we got the current path.
      if (getcwd(buf.get(), bufsize)) {
        break;
      }
      
      // Throw an error if we got any error but the buffer being too small.
      if (errno != ERANGE) {
        s_throwSysCallError(errno, "getcwd");
      }
      
    }
    
    // Return the current working directory.
    return buf.get();
    
  }
  
  /*
   * Update the cache.
   */
  void POSIXFile::_update_cache () const {
    
    // Call fstat().
    if (fstat(_fd, &_stat)) {
      s_throwSysCallError(errno, "stat", _filename);
    }
    
    // Cache has been successfully updated.
    _cache_valid = true;
    
  }
  
  /*
   * Read from the file.
   */
  ssize_t POSIXFile::_read (void* const buf, off_t const bufsize) {
    
    // Work area.
    ssize_t retval = 0;
    
    // Make sure we are not attempting a read at end of file.
    if (_eof) {
      throw Errors::EoF().Operation("read");
    }
    
    // Read the file.
    if ((retval = ::read(_fd, buf, bufsize)) == -1) {
      s_throwSysCallError(errno, "read", _filename);
    }
    
    // Update the current position.
    _curpos += retval;
    
    // Check for end of file.
    _eof = _curpos >= size();
    
    // Return the number of bytes read.
    return retval;
    
  }
  
  /*
   * Seek to a particular offset.
   */
  void POSIXFile::_seek (off_t const offset, SeekMode const whence) {
    
    // Do the seek.
    if (lseek(_fd, offset, whence) == -1) {
      s_throwSysCallError(errno, "lseek", _filename);
    }
    
    // Update the current position.
    switch (whence) {
      case SM_SET: _curpos  = offset         ; break;
      case SM_CUR: _curpos += offset         ; break;
      case SM_END: _curpos  = size() + offset; break;
    }
    
    // Update end of file status.
    _eof = _curpos >= size();
    
  }
  
  /***************************************************************************/
  // Static function members.
  
  /*
   * Change file owner.
   */
  void POSIXFile::chmod (string const& filename, mode_t const new_mode) {
    
    // Update the mode of the file.
    if (chmod(filename.c_str(), new_mode & _PERM_MASK)) {
      s_throwSysCallError(errno, "chmod");
    }
    
  }
  
  /*
   * Change the file owner.
   */
  void POSIXFile::chown (string const& filename, uid_t const owner, gid_t const group) {
    
    // Don't work if we don't have to.
    if (owner == UID_NOCHANGE && group == GID_NOCHANGE) {
      return;
    }
    
    // Call chown().
    if (chown(filename.c_str(), owner, group)) {
      s_throwSysCallError(errno, "chown");
    }
    
  }
  
  /*
   * stat().
   */
  struct stat* POSIXFile::s_stat (string const& filename, struct stat* const buf) {
    
    // Call stat().
    if (stat(filename.c_str(), buf)) {
      s_throwSysCallError(errno, "stat");
    }
    
    // Done, return.
    return buf;
    
  }
  struct stat* POSIXFile::s_try_stat (string const& filename, struct stat* const buf) {
    
    // Init.
    struct stat* retval = buf;
    
    // Call stat(). Will not follow symlinks.
    if (stat(filename.c_str(), retval)) {
      retval = 0;
    }
    
    // Done, return.
    return retval;
    
  }
  
  /*
   * Handle and throw a system call error.
   */
  void POSIXFile::s_throwSysCallError (int const errnum, std::string const& syscall, std::string const& filename) {
    
    // Throw the appropriate error.
    switch (errnum) {
      case EACCES      : throw dynamic_cast<Errors::AccessDenied &>(Errors::AccessDenied ().Errno(errnum).SysCall(syscall).Filename(filename));
      case EAGAIN      : throw dynamic_cast<Errors::TryAgain     &>(Errors::TryAgain     ().Errno(errnum).SysCall(syscall).Filename(filename));
      case EBADF       : throw dynamic_cast<Errors::BadDescriptor&>(Errors::BadDescriptor().Errno(errnum).SysCall(syscall).Filename(filename));
      case EDEADLK     : throw dynamic_cast<Errors::Deadlock     &>(Errors::Deadlock     ().Errno(errnum).SysCall(syscall).Filename(filename));
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
      case ENOLCK      : throw dynamic_cast<Errors::LockTableFull&>(Errors::LockTableFull().Errno(errnum).SysCall(syscall).Filename(filename));
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
  
  /*
   * Compress redundant directory separators.
   */
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

